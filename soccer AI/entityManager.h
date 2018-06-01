/*=======================================================================

	�G���e�B�e�B�}�l�[�W���[ [entityManager.h]

												�����C��

	�G���e�B�e�B�̊Ǘ��������V���O���g���N���X

=========================================================================*/
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <map>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class BaseGameEntity;

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////
#define EntityMgr EntityManager::Instance()

/////////////////////////////////////////////////////////
//
//	EntityManager�N���X
//
/////////////////////////////////////////////////////////
class EntityManager
{
private:

  typedef std::map<int, BaseGameEntity*> EntityMap;

private:

  // �G���e�B�e�B�ւ̃|�C���^�����̎��ʔԍ��ɂ���đ��ݎQ�Ƃ����
  EntityMap m_EntityMap;

  EntityManager(){}

  EntityManager(const EntityManager&);
  EntityManager& operator=(const EntityManager&);

public:

  static EntityManager* Instance();

  // �G���e�B�e�B��ID�Ŏw�肳�ꂽ�C���f�b�N�X�ʒu��
  // std :: vector m_Entities�ɃG���e�B�e�B�ւ̃|�C���^���i�[����
  void            RegisterEntity(BaseGameEntity* NewEntity);

  // �p�����[�^�Ƃ��ė^����ꂽID�����G���e�B�e�B�ւ̃|�C���^��Ԃ�
  BaseGameEntity* GetEntityFromID(int id)const;

  // ���X�g����G���e�B�e�B���폜����
  void            RemoveEntity(BaseGameEntity* pEntity);

  // �}�b�v���炷�ׂẴG���e�B�e�B���N���A����
  void            Reset(){m_EntityMap.clear();}
};

#endif