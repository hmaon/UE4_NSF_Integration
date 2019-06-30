#ifndef _NSF_TAG_H_
#define _NSF_TAG_H_
#include "../player/nsf/nsf.h"

// missing declarations?
#define NSF_MAX_DRIVE 512
#define NSF_MAX_DIR 512
#define NSF_MAX_FNAME 512
#define NSF_MAX_EXT 512

class NSF_TAG
{
public:
  xgm::NSF *sdat;
  char m_tagf[NSF_MAX_PATH]; //タグファイル名
  char m_tagl[NSF_MAX_PATH]; //ローカルタグファイル名
  char m_sect[NSF_MAX_PATH]; //セクション名
  char drv[NSF_MAX_DRIVE];
  char dir[NSF_MAX_DIR];
  char fname[NSF_MAX_FNAME];
  char ext[NSF_MAX_EXT];

  NSF_TAG();
  NSF_TAG(xgm::NSF *nsf);
  ~NSF_TAG();
  void SetNSF(xgm::NSF *);
  void Sync();
  int WriteTagItem(int song, const char *title, int time, int loop, int fade);
  int InitTagItem(int song, const char *title_format);
  int CreateTag(const char *title_format);
  bool IsExistSection(bool local=false);
  int ClearTag();
  int WriteEnable(bool b);
  int ReadTagItem(int song);
  void UpdateTagItem(int song, const char *title_format, int loopnum);
  bool IsWriteEnable();
  bool Export(); // taginfo.tag -> ローカルタグ情報
  bool Import(); // ローカルタグ情報 -> taginfo.tag
};

#endif
