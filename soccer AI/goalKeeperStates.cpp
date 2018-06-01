/*=======================================================================

	ゴールキーパーステート [goalKeeperStates.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "goalKeeperStates.h"
#include "debugConsole.h"
#include "soccerPitch.h"
#include "playerBase.h"
#include "goalKeeper.h"
#include "steeringBehaviors.h"
#include "soccerTeam.h"
#include "goal.h"
#include "geometry.h"
#include "fieldPlayer.h"
#include "paramLoader.h"
#include "telegram.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
//#define GOALY_STATE_INFO_ON

/////////////////////////////////////////////////////////
//
//	GlobalKeeperStateのInstance関数
//
/////////////////////////////////////////////////////////
GlobalKeeperState* GlobalKeeperState::Instance()
{
  static GlobalKeeperState instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GlobalKeeperStateのOnMessage関数
//
/////////////////////////////////////////////////////////
bool GlobalKeeperState::OnMessage(GoalKeeper* keeper, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
    case Msg_GoHome:
    {
      keeper->SetDefaultHomeRegion();
      
      keeper->GetFSM()->ChangeState(ReturnHome::Instance());
    }

    break;

    case Msg_ReceiveBall:
      {
        keeper->GetFSM()->ChangeState(InterceptBall::Instance());
      }

      break;

  }
  return false;
}

/////////////////////////////////////////////////////////////////////////
//
//	GlobalKeeperStateのTendGoal関数
//
//	ボールとネットの後ろに自分自身を置くための「介入」操縦行動を使用して、
//	ゴールマウスの左から右へ移動する
//
//	ボールが「ゴールキーパーの範囲」内に入った場合、
//	ボールはゴールマウスから離れて、それをインターセプトしようとする
//
/////////////////////////////////////////////////////////////////////////
TendGoal* TendGoal::Instance()
{
  static TendGoal instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoalのEnter関数
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Enter(GoalKeeper* keeper)
{
  // 介入をオンにする
  keeper->Steering()->InterposeOn(Prm.GoalKeeperTendingDistance);

  // ボールの位置とゴールの口に沿って位置する目標位置との間にエージェントを配置する
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoalのExecute関数
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Execute(GoalKeeper* keeper)
{
  // ボールの位置が変化すると、後方のインターターゲットが変化するため、各更新ステップを更新する
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

  // ボールが範囲内に入った場合、キーパーはそれを防ぎ、
  // 状態を変えてボールをプレイ中に戻す
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();

    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());

    return;
  }

  // ボールが予め定められた距離内にある場合、
  // キーパーは試行する位置から外に出てそれをセーブする
  if (keeper->BallWithinRangeForIntercept() && !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(InterceptBall::Instance());
  }

  // ゴールキーパがゴールラインから離れていて、対戦相手が迫ってきたら戻る
  if (keeper->TooFarFromGoalMouth() && keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(ReturnHome::Instance());

    return;
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoalのExit関数
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->InterposeOff();
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHomeのInstance関数
//
//	ゴールキーパーはゴール中心に戻り、ステートをTendGoalに戻す
//
/////////////////////////////////////////////////////////////////////////
ReturnHome* ReturnHome::Instance()
{
  static ReturnHome instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHomeのEnter関数
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOn();
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHomeのExecute関数
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Execute(GoalKeeper* keeper)
{
  keeper->Steering()->SetTarget(keeper->HomeRegion()->Center());

  // ボールをコントロールしている場合、
  // ステートを変えてゴールに向かわせる
  if (keeper->InHomeRegion() || !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(TendGoal::Instance());
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHomeのExit関数
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBallのInstance関数
//
//	追従行動をしてボールをインターセプトしようとする
//
/////////////////////////////////////////////////////////////////////////
InterceptBall* InterceptBall::Instance()
{
  static InterceptBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBallのEnter関数
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOn();  

    #ifdef GOALY_STATE_INFO_ON
    debug_con << "Goaly " << keeper->ID() << " enters InterceptBall" <<  "";
    #endif
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBallのExecute関数
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Execute(GoalKeeper* keeper)
{ 
  // ゴールキーパがゴールから遠くに移動した場合、
  // ボールに近くにない限り自エリアに戻り
  // ボールに近い場合はインターセプトする
  if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
  {
    keeper->GetFSM()->ChangeState(ReturnHome::Instance());

    return;
  }
  
  // ボールがゴールキーパーの手の範囲に入ると、
  // ボールをセーブしてプレー中に戻す
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();
    
    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());

    return;
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBallのExit関数
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOff();
}


/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlayのInstance関数
//
/////////////////////////////////////////////////////////////////////////
PutBallBackInPlay* PutBallBackInPlay::Instance()
{
  static PutBallBackInPlay instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlayのEnter関数
//
/////////////////////////////////////////////////////////////////////////
void PutBallBackInPlay::Enter(GoalKeeper* keeper)
{
  // キーパーが持っていることをチームに知らせる
  keeper->Team()->SetControllingPlayer(keeper);

  // すべての選手を自エリアへ戻す
  keeper->Team()->Opponents()->ReturnAllFieldPlayersToHome();
  keeper->Team()->ReturnAllFieldPlayersToHome();
}

/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlayのExecute関数
//
/////////////////////////////////////////////////////////////////////////
void PutBallBackInPlay::Execute(GoalKeeper* keeper)
{
  PlayerBase*  receiver = NULL;
  Vector2D     BallTarget;
    
  // パスできるプレイヤーを探し、見つかったら
  // パスをする
  if (keeper->Team()->FindPass(keeper,
                              receiver,
                              BallTarget,
                              Prm.MaxPassingForce,
                              Prm.GoalkeeperMinPassDist))
  {     
    // パスをする
    keeper->Ball()->Kick(Vec2DNormalize(BallTarget - keeper->Ball()->Pos()),
                         Prm.MaxPassingForce);

    // ゴールキーパーがボールを持っていない
    keeper->Pitch()->SetGoalKeeperHasBall(false);

    // ボールが来ることを知らせる
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                          keeper->ID(),
                          receiver->ID(),
                          Msg_ReceiveBall,
                          &BallTarget);
    
    // ゴールに戻る 
    keeper->GetFSM()->ChangeState(TendGoal::Instance());

    return;
  }  

  // 速度をセット
  keeper->SetVelocity(Vector2D());
}

