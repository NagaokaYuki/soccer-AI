/*=======================================================================

	�I�[�g���X�g [autolist.h]

												�����C��

	���̃N���X����p�����āA�ގ������I�u�W�F�N�g�̃��X�g�������I�ɍ쐬���A
	�I�u�W�F�N�g���쐬����邽�тɁA�����I�Ƀ��X�g�ɒǉ�����
	�j�󂳂�邽�тɎ����I�ɏ�������        

=========================================================================*/
#ifndef AUTOLIST_H
#define AUTOLIST_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <list>

/////////////////////////////////////////////////////////
//
//	AutoList�N���X
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
    // ���̃I�u�W�F�N�g��T *�^�ɃL���X�g���A��������X�g�ɒǉ�����
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