/*=======================================================================

	エンティティのテンプレート [entityFunctionTemplates.h]

												長岡佑樹

=========================================================================*/
#ifndef GAME_ENTITY_FUNCTION_TEMPLATES
#define GAME_ENTITY_FUNCTION_TEMPLATES

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "baseGameEntity.h"
#include "geometry.h"

/////////////////////////////////////////////////////////
//
//	Overlapped関数
//
//	エンティティが標準コンテナに格納されている
//	いくつかのエンティティのいずれかと重複しているか
//	どうかを調べる
//
/////////////////////////////////////////////////////////
template <class T, class conT>
bool Overlapped(const T* ob, const conT& conOb, double MinDistBetweenObstacles = 40.0)
{
  typename conT::const_iterator it;

  for (it=conOb.begin(); it != conOb.end(); ++it)
  {
    if (TwoCirclesOverlapped(ob->Pos(),
                             ob->BRadius()+MinDistBetweenObstacles,                             
                             (*it)->Pos(),
                             (*it)->BRadius()))
    {
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////
//
//	TagNeighbors関数
//
//	単一のエンティティパラメータの半径内にあるstd
//	コンテナに含まれる任意のエンティティをタグ付け
//
/////////////////////////////////////////////////////////
template <class T, class conT>
void TagNeighbors(T* entity, conT& others, const double radius)
{
  typename conT::iterator it;

  // 範囲をチェックするすべてのエンティティを反復処理する
  for (it=others.begin(); it != others.end(); ++it)
  {
    // 現在のタグをすべて消去
    (*it)->UnTag();

    // sqrtsを避けるために距離平方で作業
    Vector2D to = (*it)->Pos() - entity->Pos();

    // 範囲に加えることによって、他方の境界半径を考慮に入れる
    double range = radius + (*it)->BRadius();

    // 範囲内のエンティティの場合、検討するためのタグ
    if ( ((*it) != entity) && (to.LengthSq() < range*range))
    {
      (*it)->Tag();
    } 
  }
}

/////////////////////////////////////////////////////////
//
//	EnforceNonPenetrationContraint関数
//
//	エンティティ間の重複があるかどうかをチェックし、
//	存在する場合、エンティティは互いに離れて移動する
//
/////////////////////////////////////////////////////////
template <class T, class conT>
void EnforceNonPenetrationContraint(T entity, const conT& others)
{
  typename conT::const_iterator it;

  // 境界の半径のオーバーラップをチェックする
  // すべてのエンティティを反復処理する
  for (it=others.begin(); it != others.end(); ++it)
  {
    if (*it == entity) continue;

    // エンティティの位置間の距離を計算する
    Vector2D ToEntity = entity->Pos() - (*it)->Pos();

    double DistFromEachOther = ToEntity.Length();

    // この距離が半径の合計よりも小さい場合、
    // このエンティティはToEntityベクトルと
    // 平行な方向に移動する
    double AmountOfOverLap = (*it)->BRadius() + entity->BRadius() -
                             DistFromEachOther;

    if (AmountOfOverLap >= 0)
    {
      // エンティティをオーバーラップの量に相当する距離だけ移動させる
      entity->SetPos(entity->Pos() + (ToEntity/DistFromEachOther) *
                     AmountOfOverLap);
    }
  }
}

#endif