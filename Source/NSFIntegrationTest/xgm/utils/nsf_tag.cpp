#include "nsf_tag.h"
#include "CoreMinimal.h"
using namespace xgm;

NSF_TAG::NSF_TAG() : sdat(NULL)
{
  memset(m_sect,0,sizeof(m_sect));
  memset(m_tagf,0,sizeof(m_tagf));
}

NSF_TAG::NSF_TAG(NSF *nsf)
{
  SetNSF(nsf);
}

NSF_TAG::~NSF_TAG(){}

void NSF_TAG::SetNSF(NSF *nsf)
{
  sdat = nsf;
  Sync();
}

void NSF_TAG::Sync()
{
  _splitpath(sdat->filename, drv, dir, fname, ext );
  if(dir[0]=='\0') strcpy(dir,".\\");
  _makepath(m_tagf,drv,dir,"taginfo","tag");
  _makepath(m_tagl,drv,dir,fname,"tag");
  _makepath(m_sect,"","",fname,ext);
}

int NSF_TAG::WriteTagItem(int song, const char *title, int time, int loop, int fade)
{
	return 0 * (song + *title + time + loop + fade); // we don't care about tags

  if(IsExistSection()&&IsWriteEnable())
  {
    char key[16], val[512];
    char *p = val;
    sprintf(key,"@Song%d",song+1);

    const char *t = title; 
    while (*t != '\0')
    {
      if (*t == '\\' || *t == ',')
        *(p++) = '\\';
      *(p++) = *(t++);
    }
    (*p++)=',';

    if(time>=0) p+=sprintf(p,"%d", time/1000);
    p+=sprintf(p,",");
    if(loop>=0) p+=sprintf(p,"%d", loop/1000);
    p+=sprintf(p,",");
    if(fade>=0) p+=sprintf(p,"%d", fade/1000);
#if false
	// XXX this is a Windows-only function 
    return WritePrivateProfileString(m_sect,key,val,m_tagf);
#endif
  }
  else
  {
    DEBUG_OUT("存在しないセクションへの書込み\n");
    return 0;
  }
}

int NSF_TAG::InitTagItem(int song, const char *title_format)
{
  if(IsExistSection()&&IsWriteEnable())
  {
    return WriteTagItem(song, sdat->GetTitleString(title_format,song), -1,-1,-1);
  }
  else
  {
    DEBUG_OUT("存在しないセクションへの書込み\n");
    return 0;
  }
}

static bool IsFileExist(char *file)
{
  FILE *fp = fopen(file, "r");
  if(fp==NULL)
  {
    DEBUG_OUT("NOT EXIST\n");
    return false;
  }
  fclose(fp);
  return true;
}

bool NSF_TAG::IsExistSection(bool local)
{
	return false;
#if false
  static char temp[8];

  if(!IsFileExist(local?m_tagl:m_tagf))
    return false;

  if(GetPrivateProfileSection(m_sect,temp,8,local?m_tagl:m_tagf))
    return true;
  else
    return false;
#endif
}

int NSF_TAG::CreateTag(const char *title_format)
{
	return true;
#if false
  int i;
  char temp[8], *p;

  WritePrivateProfileString(m_sect,"Title",sdat->title,m_tagf);
  WritePrivateProfileString(m_sect,"Artist",sdat->artist,m_tagf);
  WritePrivateProfileString(m_sect,"Comment",sdat->copyright,m_tagf);

  // 年号の取り出し
  for(i=0,p=sdat->copyright;*p!='\0';p++)
  {
    if('0'<=(*p)&&(*p)<='9')
    {
      temp[i++]=*p;
      if(i==4)
      {
        temp[4]='\0';
        WritePrivateProfileString(m_sect,"Date",temp,m_tagf);
        break;
      }
    }
    else i=0;
  }

  WriteEnable(true);

  // 全曲分の書き込み
  for(i=sdat->start-1;i<sdat->songs;i++)
    InitTagItem(i,title_format);

  return true;
#endif
}

static int time2int(char *time)
{
  char buf[128], *p;
  int ret = 0;

  if(time==NULL) return -1;
  for(p=time;*p==' '||*p=='\t';p++);
  if(*p=='\0') return -1;

  strncpy(buf,time,128);
  buf[127]='\0';

  p = strtok(buf,":");

  while(p)
  {
    ret *= 60;
    ret += atoi(p);
    p = strtok(NULL,":");
  }

  return ret * 1000;
}

#ifndef is_sjis_prefix
#define is_sjis_prefix(c) ((0x81<=c&&c<=0x9F)||(0xE0<=c&&c<=0xFC)) 
#endif

#if false
static bool is_sjis_prefix(int c)
{
  if((0x81<=c&&c<=0x9F)||(0xE0<=c&&c<=0xFC)) 
    return true ;
  else 
    return false ;
}
#endif

int NSF_TAG::ReadTagItem(int song)
{
	return 0 * song;
#if false
  char temp[16], buf[512], title[256], *p[3];
  int playtime, looptime, fadetime, i, j;
  char *tag;

  if(IsExistSection(true))
    tag = m_tagl;
  else if(IsExistSection(false))
    tag = m_tagf;
  else
  {
    sdat->time_in_ms = -1;
    sdat->loop_in_ms = -1;
    sdat->fade_in_ms = -1;
    return sdat->GetLength();
  }

  sprintf(temp,"@Song%d",song+1);
  GetPrivateProfileString(m_sect,temp,NULL,buf,512,tag);
  
  for(i=0,j=0;i<256&&j<512;title[i++]=buf[j++])
  {
    if(buf[j]=='\\')
      j++;
    else if(is_sjis_prefix(buf[j]))
      title[i++]=buf[j++];
    else if(buf[j]==','||buf[j]=='\0')
      break;
  }
  title[i]='\0';

  if(i==0)
  {
    if(GetPrivateProfileString(m_sect,"Title","",title,256,tag))
    {
      sdat->SetTitleString(title);
      sdat->title_unknown = false;
    }
  }
  else
  {
    sdat->SetTitleString(title);
    sdat->title_unknown = false;
  }

  if(buf[j]!='\0')
  {
    p[0] = strtok(buf+1+j,",");
    for(i=1;i<3;i++) p[i]=strtok(NULL,",");
    playtime = time2int(p[0]);
    looptime = time2int(p[1]);
    fadetime = time2int(p[2]);
    DEBUG_OUT("%d %d %d\n", playtime, looptime, fadetime);
    sdat->time_in_ms = (playtime>=0)?playtime:-1;
    sdat->loop_in_ms = (looptime>=0)?looptime:-1;
    sdat->fade_in_ms = (fadetime>=0)?fadetime:-1;
  }

  return sdat->GetLength();
#endif
}

int NSF_TAG::ClearTag()
{
	return 0;
#if false
  if(IsExistSection()&&IsWriteEnable())
    return WritePrivateProfileSection(m_sect,"",m_tagf);
  else
    return 0;
#endif
}

int NSF_TAG::WriteEnable(bool b)
{
	return 0;
#if false
  if(IsExistSection())
    return WritePrivateProfileString(m_sect,"NSFplugWriteEnable",b?"1":"0",m_tagf);
  else
    return 0;
#endif
}

void NSF_TAG::UpdateTagItem(int song, const char *title_format, int loopnum)
{
  if(IsExistSection()&&IsWriteEnable())
  {
    WriteTagItem(song, 
      sdat->GetTitleString(title_format),
      sdat->GetPlayTime(),
      sdat->GetLoopTime() * (loopnum>0?loopnum-1:0),
      sdat->GetFadeTime());
  }
}

bool NSF_TAG::IsWriteEnable()
{
	return false;
#if false
  if(IsExistSection())
    return GetPrivateProfileInt(m_sect,"NSFplugWriteEnable",0,m_tagf)?true:false;
  else
    return false;
#endif
}