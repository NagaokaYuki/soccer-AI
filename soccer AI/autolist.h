/*=======================================================================

	オートリスト [autolist.h]

												長岡佑樹

	このクラスから継承して、類似したオブジェクトのリストを自動的に作成し、
	オブジェクトが作成されるたびに、自動的にリストに追加する
	破壊されるたびに自動的に除去する        

=========================================================================*/
#ifndef AUTOLIST_H
#define AUTOLIST_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <list>

/////////////////////////////////////////////////////////
//
//	AutoListクラス
//
/////////////////////////////////////////////////////////
template <class T>
class AutoList
{
public:

  typedef std::list<T*> ObjectList;
  
private:

  static ObjectList m_Members;

protected:

  AutoList()
  {
    // このオブジェクトをT *型にキャストし、それをリストに追加する
    m_Members.push_back(static_cast<T*>(this));
  }

  ~AutoList()
  {
    m_Members.remove(static_cast<T*>(this));    
  }

public:


  static ObjectList& GetAllMembers(){return m_Members;}
};


template <class T>
std::list<T*> AutoList<T>::m_Members;



#endif