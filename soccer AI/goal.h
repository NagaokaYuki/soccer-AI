/*=======================================================================

	ゴール [goal.h]

												
												長岡佑樹
	クラスでサッカーのピッチの目標を定義し,目標は
	左右のポストを表す2つの2Dベクトルによって定義

	各タイムステップでは、ゴールが採点されたかどうかを
	判断するためにScoredメソッドを呼び出す

=========================================================================*/
#ifndef GOAL_H
#define GOAL_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "SoccerBall.h"
#include "Vector2D.h"
#include "geometry.h"

/////////////////////////////////////////////////////////
//
//	Goalクラス
//
/////////////////////////////////////////////////////////
class Goal 
{

private:

  Vector2D   m_vLeftPost;
  Vector2D   m_vRightPost;

  // 目標の向きを表すベクトル
  Vector2D   m_vFacing;

  // ゴールラインの中心の位置
  Vector2D   m_vCenter;

  // Scored（）が目標を検出するたびに、これがインクリメントされる
  int        m_iNumGoalsScored;

public:

  Goal(Vector2D left, Vector2D right, Vector2D facing):m_vLeftPost(left),
                                                       m_vRightPost(right),
                                                       m_vCenter((left+right)/2.0),
                                                       m_iNumGoalsScored(0),
                                                       m_vFacing(facing)
  {  }

  // 現在のボールの位置と以前のボールの位置が与えられた場合、
  // このメソッドは、ボールがゴールラインを超えた場合にtrueを返し、
  // m_iNumGoalsScoredをインクリメントする
  inline bool Scored(const SoccerBall*const ball);

  Vector2D Center()const{return m_vCenter;}
  Vector2D Facing()const{return m_vFacing;}
  Vector2D LeftPost()const{return m_vLeftPost;}
  Vector2D RightPost()const{return m_vRightPost;}

  int      NumGoalsScored()const{return m_iNumGoalsScored;}
  void     ResetGoalsScored(){m_iNumGoalsScored = 0;}
};

bool Goal::Scored(const SoccerBall*const ball)
{
  if (LineIntersection2D(ball->Pos(), ball->OldPos(), m_vLeftPost, m_vRightPost))
  {
    ++m_iNumGoalsScored;

    return true;
  }

  return false;
}


#endif