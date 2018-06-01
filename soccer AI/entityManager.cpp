/*=======================================================================

	エンティティマネージャー [entityManager.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "entityManager.h"
#include "baseGameEntity.h"

/////////////////////////////////////////////////////////
//
//	Instance関数
//
/////////////////////////////////////////////////////////
EntityManager* EntityManager::Instance()
{
  static EntityManager instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GetEntityFromID関数
//
/////////////////////////////////////////////////////////
BaseGameEntity* EntityManager::GetEntityFromID(int id)const
{
  // エンティティを見つける
  EntityMap::const_iterator ent = m_EntityMap.find(id);

  assert ( (ent !=  m_EntityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

  return ent->second;
}

/////////////////////////////////////////////////////////
//
//	RemoveEntity関数
//
/////////////////////////////////////////////////////////
void EntityManager::RemoveEntity(BaseGameEntity* pEntity)
{    
  m_EntityMap.erase(m_EntityMap.find(pEntity->ID()));
} 

/////////////////////////////////////////////////////////
//
//	RegisterEntity関数
//
/////////////////////////////////////////////////////////
void EntityManager::RegisterEntity(BaseGameEntity* NewEntity)
{
  m_EntityMap.insert(std::make_pair(NewEntity->ID(), NewEntity));
}
