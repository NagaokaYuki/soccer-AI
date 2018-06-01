/*=======================================================================

	フィールドプレイヤー [fieldPlayer.h]

												長岡佑樹

	PlayerBaseから派生したクラス
	サッカーのピッチ、蹴り、ドリブル、シュートのを動きをカプセル化

=========================================================================*/
#pragma warning (disable:4786)
#ifndef FIELDPLAYER_H
#define FIELDPLAYER_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include "FieldPlayerStates.h"
#include "vector2D.h"
#include "stateMachine.h"
#include "playerBase.h"
#include "stateMachine.h"
#include "regulator.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class CSteeringBehavior;
class SoccerTeam;
class SoccerPitch;
class Goal;
struct Telegram;

/////////////////////////////////////////////////////////
//
//	FieldPlayerクラス
//
/////////////////////////////////////////////////////////
class FieldPlayer : public PlayerBase
{
private:

   // ステートマシンクラスのインスタンス
  StateMachine<FieldPlayer>*  m_pStateMachine;
  
  // プレイヤーが1秒間に取ることができるキック数を制限
  Regulator*                  m_pKickLimiter;

  
public:

  FieldPlayer(SoccerTeam*    home_team,
             int        home_region,
             State<FieldPlayer>* start_state,
             Vector2D  heading,
             Vector2D      velocity,
             double         mass,
             double         max_force,
             double         max_speed,
             double         max_turn_rate,
             double         scale,
             player_role    role);   
  
  ~FieldPlayer();

  // これを呼び出してプレイヤーの位置と向きを更新
  void        Update();   

  void        Render();

  bool        HandleMessage(const Telegram& msg);

  StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}

  bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

         
};

#endif