/*=======================================================================

	エンティティマネージャー [entityManager.h]

												長岡佑樹

	エンティティの管理を扱うシングルトンクラス

=========================================================================*/
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <map>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class BaseGameEntity;

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
#define EntityMgr EntityManager::Instance()

/////////////////////////////////////////////////////////
//
//	EntityManagerクラス
//
/////////////////////////////////////////////////////////
class EntityManager
{
private:

  typedef std::map<int, BaseGameEntity*> EntityMap;

private:

  // エンティティへのポインタがその識別番号によって相互参照される
  EntityMap m_EntityMap;

  EntityManager(){}

  EntityManager(const EntityManager&);
  EntityManager& operator=(const EntityManager&);

public:

  static EntityManager* Instance();

  // エンティティのIDで指定されたインデックス位置の
  // std :: vector m_Entitiesにエンティティへのポインタを格納する
  void            RegisterEntity(BaseGameEntity* NewEntity);

  // パラメータとして与えられたIDを持つエンティティへのポインタを返す
  BaseGameEntity* GetEntityFromID(int id)const;

  // リストからエンティティを削除する
  void            RemoveEntity(BaseGameEntity* pEntity);

  // マップからすべてのエンティティをクリアする
  void            Reset(){m_EntityMap.clear();}
};

#endif