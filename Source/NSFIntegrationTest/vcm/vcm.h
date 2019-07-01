#pragma once

//
// Versatile Configuration Management Library
//
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <list>
#include <set>
#include <Engine.h>

//#include <windows.h> // for thread safety
#include <stdexcept>

namespace vcm
{
  // 値
  class Value
  {
  public:
    // 属性
    std::string data;    // 値…どんな型でも常に文字列として保存
    bool update;         // アップデートされたならtrue

    // 操作
    Value();
    Value(const char *);
    Value(const std::string &);
    Value(int);

    operator int () const ;
    int GetInt() const ;
    operator std::string () const ;
    operator const char *() const ;
    std::string GetStr() const ;

    Value &operator = ( int );
    void SetInt( int );
    Value &operator = ( const std::string & );
    void SetStr( const std::string & );
    Value &operator = ( const char * );
    void SetStr( const char * );
  };

  // コンフィグレーション本体の定義
  class ObserverI
  {
  public:
    virtual void Notify(int)=0;
  };

  class Observable
  {
  protected:
    std::set<ObserverI *>oblist;
  public:
    virtual ~Observable() {}

    virtual void AttachObserver(ObserverI *p)
    {
      oblist.insert(p);
    }

    virtual void DetachObserver(ObserverI *p)
    {
      oblist.erase(p);
    }

    virtual int GetObserverNum()
    {
      return (int)oblist.size();
    }

    virtual ObserverI *GetObserver()
    {
      if(oblist.size()!=0)
        return *oblist.begin();
      else
        return NULL;
    }

    void Notify(int id)
    {
      std::set<ObserverI *>::iterator it;
      for(it=oblist.begin();it!=oblist.end();it++)
        (*it)->Notify(id);
    }
  }; 


  class Configuration : public Observable
  {
  protected:

    std::map < std::string, Value > data;

    // thread safety
    static FCriticalSection mutex;

  public:

	virtual ~Configuration() {}

    // 値を読む．無ければエラー．
    Value &operator[]( const std::string id )
    {
	  FScopeLock ScopeLock(&mutex);
      std::map< std::string, Value >::iterator it;
      if(( it = data.find( id ) )==data.end() )
        throw std::out_of_range( id );
      else
        return it->second;
    }
    // 値を作る 重複して作ろうとすると false が帰って失敗
    bool CreateValue( const std::string id, const Value &value )
    {
	  FScopeLock ScopeLock(&mutex);
      std::map< std::string, Value >::iterator it;
      if(( it = data.find( id ) )!=data.end() )
        return false;
      else
      {
        data[id] = value;
        return true;
      }
    }
    // 値をセットする．無ければ作る．
    inline void SetValue( const std::string id, const Value &value )
    {
	  FScopeLock ScopeLock(&mutex);
      data[id] = value;
    }
    // 値を読む．無ければ自動的に作成される．
    inline Value &GetValue( const std::string id )
    {
	  FScopeLock ScopeLock(&mutex);
      return data[id];
    }
    inline void Clear()
    { 
	  FScopeLock ScopeLock(&mutex);
      data.clear(); 
    }
    void Read( Configuration &src )
    {
	  FScopeLock ScopeLock(&mutex);
      std::map<std::string, Value>::iterator it;
      for(it=data.begin();it!=data.end();it++)
        it->second = src[it->first];
    }
    void Write( Configuration &src )
    {
	  FScopeLock ScopeLock(&mutex);
      std::map<std::string, Value>::iterator it;
      for(it=data.begin();it!=data.end();it++)
        src[it->first]=it->second;
    }
  };

} // namespace vcm

#include "vcm_ui.h"
