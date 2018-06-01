/*=======================================================================

	�G���e�B�e�B�}�l�[�W���[ [entityManager.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "entityManager.h"
#include "baseGameEntity.h"

/////////////////////////////////////////////////////////
//
//	Instance�֐�
//
/////////////////////////////////////////////////////////
EntityManager* EntityManager::Instance()
{
  static EntityManager instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GetEntityFromID�֐�
//
/////////////////////////////////////////////////////////
BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
  // �G���e�B�e�B��������
  EntityMap::const_iterator ent = m_EntityMap.find(id);

  assert ( (ent !=  m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

  return ent->second;
}

/////////////////////////////////////////////////////////
//
//	RemoveEntity�֐�
//
/////////////////////////////////////////////////////////
void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{    
  m_EntityMap.erase(m_EntityMap.find(pEntity->ID()));
} 

/////////////////////////////////////////////////////////
//
//	RegisterEntity�֐�
//
/////////////////////////////////////////////////////////
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
  m_EntityMap.insert(std::make_pair(NewEntity->ID(), NewEntity));
}
