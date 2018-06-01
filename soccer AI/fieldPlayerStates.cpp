/*=======================================================================

	フィールドプレイヤーステート [fieldPlayerStates.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "fieldPlayerStates.h"
#include "debugConsole.h"
#include "soccerPitch.h"
#include "fieldPlayer.h"
#include "steeringBehaviors.h"
#include "soccerTeam.h"
#include "goal.h"
#include "geometry.h"
#include "soccerBall.h"
#include "paramLoader.h"
#include "telegram.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"
#include "regulator.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
#define PLAYER_STATE_INFO_ON

/////////////////////////////////////////////////////////
//
//	GlobalPlayerStateのInstance関数
//
/////////////////////////////////////////////////////////
GlobalPlayerState* GlobalPlayerState::Instance()
{
  static GlobalPlayerState instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GlobalPlayerStateのExecute関数
//
/////////////////////////////////////////////////////////
void GlobalPlayerState::Execute(FieldPlayer* player)                                     
{
  // プレーヤーがボールを持っている場合、ボールに近づくと、最高速度が下がる
  if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
  {
    player->SetMaxSpeed(Prm.PlayerMaxSpeedWithBall);
  }

  else
  {
     player->SetMaxSpeed(Prm.PlayerMaxSpeedWithoutBall);
  }
    
}

/////////////////////////////////////////////////////////
//
//	GlobalPlayerStateのOnMessage関数
//
/////////////////////////////////////////////////////////
bool GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
  case Msg_ReceiveBall:
    {
      // ターゲットセット
      player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

      // ステート変更 
      player->GetFSM()->ChangeState(ReceiveBall::Instance());

      return true;
    }

    break;

  case Msg_SupportAttacker:
    {
      if (player->GetFSM()->isInState(*SupportAttacker::Instance()))
      {
        return true;
      }
      
      // 最適なSupportPotisionに目標を設定する
      player->Steering()->SetTarget(player->Team()->GetSupportSpot());

      // ステート変更
      player->GetFSM()->ChangeState(SupportAttacker::Instance());

      return true;
    }

    break;

 case Msg_Wait:
    {
      // 状態を変更する
      player->GetFSM()->ChangeState(Wait::Instance());

      return true;
    }

    break;

  case Msg_GoHome:
    {
      player->SetDefaultHomeRegion();
      
      player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());

      return true;
    }

    break;

  case Msg_PassToMe:
    {  
      
      // パスを要求しているプレーヤーの位置を取得
      FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

      #ifdef PLAYER_STATE_INFO_ON
      debug_con << "Player " << player->ID() << " received request from " <<
                    receiver->ID() << " to make pass" << "";
      #endif

      // ボールが蹴り込み範囲内にないか、またはボールがすでに受け取り側のプレーヤーである場合、
      // このプレーヤーはボールを要求したプレーヤーに渡すことができない
      if (player->Team()->Receiver() != NULL ||
         !player->BallWithinKickingRange() )
      {
        #ifdef PLAYER_STATE_INFO_ON
        debug_con << "Player " << player->ID() << " cannot make requested pass <cannot kick ball>" << "";
        #endif

        return true;
      }
      
      // パス  
      player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
                           Prm.MaxPassingForce);

          
     #ifdef PLAYER_STATE_INFO_ON
     debug_con << "Player " << player->ID() << " Passed ball to requesting player" << "";
     #endif
        
      // パスを受け取るプレイヤーに知らせる
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              player->ID(),
                              receiver->ID(),
                              Msg_ReceiveBall,
                              &receiver->Pos());

   

      // ステート変更   
      player->GetFSM()->ChangeState(Wait::Instance());

      player->FindSupport();

      return true;
    }

    break;
  }

  return false;
}
                                
/////////////////////////////////////////////////////////
//
//	ChaseBallのInstance関数
//
/////////////////////////////////////////////////////////
ChaseBall* ChaseBall::Instance()
{
  static ChaseBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ChaseBallのEnter関数
//
/////////////////////////////////////////////////////////
void ChaseBall::Enter(FieldPlayer* player)
{
  player->Steering()->SeekOn();

  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters chase state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	ChaseBallのExecute関数
//
/////////////////////////////////////////////////////////
void ChaseBall::Execute(FieldPlayer* player)                                     
{
  // ボールが蹴り込み範囲内にある場合、プレイヤーは状態をKickBallに変更
  if (player->BallWithinKickingRange())
  {
    player->GetFSM()->ChangeState(KickBall::Instance());
    
    return;
  }
                                                                              
  // プレイヤーがボールに最も近いプレイヤーであれば、
  // プレイヤーそれを追い続けなければならない
  if (player->isClosestTeamMemberToBall())
  {
    player->Steering()->SetTarget(player->Ball()->Pos());

    return;
  }
  
  // プレイヤーがボールに近くない場合、自エリアに戻り、別の機会を待つ
  player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());
}

/////////////////////////////////////////////////////////
//
//	ChaseBallのExit関数
//
/////////////////////////////////////////////////////////
void ChaseBall::Exit(FieldPlayer* player)
{
  player->Steering()->SeekOff();
}

/////////////////////////////////////////////////////////
//
//	SupportAttackerのInstance関数
//
/////////////////////////////////////////////////////////
SupportAttacker* SupportAttacker::Instance()
{
  static SupportAttacker instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	SupportAttackerのEnter関数
//
/////////////////////////////////////////////////////////
void SupportAttacker::Enter(FieldPlayer* player)
{
  player->Steering()->ArriveOn();

  player->Steering()->SetTarget(player->Team()->GetSupportSpot());
  
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters support state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	SupportAttackerのExecute関数
//
/////////////////////////////////////////////////////////
void SupportAttacker::Execute(FieldPlayer* player)                                     
{
  // チームがコントロールを失ったら自エリアに戻る
  if (!player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance()); return;
  } 

  // SupportPositionが変化した場合、操舵目標を変更
  if (player->Team()->GetSupportSpot() != player->Steering()->Target())
  {    
    player->Steering()->SetTarget(player->Team()->GetSupportSpot());

    player->Steering()->ArriveOn();
  }

  // プレイヤーが目標にシュートでき、そのプレイヤーにボールをパスできれば、
  // 他のプレイヤーはこのプレイヤーにボールを渡す
  if( player->Team()->CanShoot(player->Pos(),
                               Prm.MaxShootingForce))
  {
    player->Team()->RequestPass(player);
  }


  // プレイヤーがサポートスポットに位置し、
  // チームがまだボールを保持している場合、
  // そのプレイヤーはまだ残って、ボールに直面する
  if (player->AtTarget())
  {
    player->Steering()->ArriveOff();
        
    // プレイヤーはボールに直面する
    player->TrackBall();

    player->SetVelocity(Vector2D(0,0));

    // 他のプレイヤーがマークしていない場合、パスを要求
    if (!player->isThreatened())
    {
      player->Team()->RequestPass(player);
    }
  }
}

/////////////////////////////////////////////////////////
//
//	SupportAttackerのExit関数
//
/////////////////////////////////////////////////////////
void SupportAttacker::Exit(FieldPlayer* player)
{
  // サポートするプレーヤーをNULLに設定
  player->Team()->SetSupportingPlayer(NULL);

  player->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegionのInstance関数
//
/////////////////////////////////////////////////////////
ReturnToHomeRegion* ReturnToHomeRegion::Instance()
{
  static ReturnToHomeRegion instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegionのEnter関数
//
/////////////////////////////////////////////////////////
void ReturnToHomeRegion::Enter(FieldPlayer* player)
{
  player->Steering()->ArriveOn();

  if (!player->HomeRegion()->Inside(player->Steering()->Target(), Region::halfsize))
  {
    player->Steering()->SetTarget(player->HomeRegion()->Center());
  }

  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters ReturnToHome state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegionのExecute関数
//
/////////////////////////////////////////////////////////
void ReturnToHomeRegion::Execute(FieldPlayer* player)
{
  if (player->Pitch()->GameOn())
  {
    // ボールが他のチームメンバーよりもこのプレーヤーに近い場合かつ
    // 割り当てられたレシーバーがないかつ
    // ゴールキーパーがボールを与えなかった場合は、それを追いかける
    if ( player->isClosestTeamMemberToBall() &&
         (player->Team()->Receiver() == NULL) &&
         !player->Pitch()->GoalKeeperHasBall())
    {
      player->GetFSM()->ChangeState(ChaseBall::Instance());

      return;
    }
  }

  // ゲームがオンで、自エリアに十分近い場合、
  // ステートを変更して待機させ、
  // プレイヤーの目標を現在の位置に戻す
  if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
                                                             Region::halfsize))
  {
    player->Steering()->SetTarget(player->Pos());
    player->GetFSM()->ChangeState(Wait::Instance());
  }
  // ゲームがプレイされていない場合、
  // プレイヤーは自エリアの中心に近づく
  else if(!player->Pitch()->GameOn() && player->AtTarget())
  {
    player->GetFSM()->ChangeState(Wait::Instance());
  }
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegionのExit関数
//
/////////////////////////////////////////////////////////
void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
  player->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////
//
//	WaitのInstance関数
//
/////////////////////////////////////////////////////////
Wait* Wait::Instance()
{
  static Wait instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	WaitのEnter関数
//
/////////////////////////////////////////////////////////
void Wait::Enter(FieldPlayer* player)
{
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters wait state" << "";
  #endif

  // ゲームがオンでない場合は、ターゲットがプレイヤーのエリアの中心であることを確認
  // キックオフの場合の正しい位置にいる
  if (!player->Pitch()->GameOn())
  {
    player->Steering()->SetTarget(player->HomeRegion()->Center());
  }
}

/////////////////////////////////////////////////////////
//
//	WaitのExecute関数
//
/////////////////////////////////////////////////////////
void Wait::Execute(FieldPlayer* player)
{    
  // プレイヤーが外れた状態になったら、元の位置に戻る
  if (!player->AtTarget())
  {
    player->Steering()->ArriveOn();

    return;
  }
  else
  {
    player->Steering()->ArriveOff();

    player->SetVelocity(Vector2D(0,0));

    player->TrackBall();
  }

  // このプレイヤーのチームがボールをコントロールしていて、
  // このプレイヤーがボールを持っていないかつ
  // そのボールを保持しているプレイヤーよりも上にいる場合、パスを要求する
  if ( player->Team()->InControl()    &&
     (!player->isControllingPlayer()) &&
       player->isAheadOfAttacker() )
  {
    player->Team()->RequestPass(player);

    return;
  }

  if (player->Pitch()->GameOn())
  {
   // ボールが他のチームメンバーよりもこのプレーヤーに近く
   // レシーバーが割り当てられていないかつゴールキーパーに
   // ボールがない場合は、それを追いかける
   if (player->isClosestTeamMemberToBall() &&
       player->Team()->Receiver() == NULL  &&
       !player->Pitch()->GoalKeeperHasBall())
   {
     player->GetFSM()->ChangeState(ChaseBall::Instance());

     return;
   }
  } 
}

/////////////////////////////////////////////////////////
//
//	WaitのExit関数
//
/////////////////////////////////////////////////////////
void Wait::Exit(FieldPlayer* player)
{
}

/////////////////////////////////////////////////////////
//
//	KickBallのInstance関数
//
/////////////////////////////////////////////////////////
KickBall* KickBall::Instance()
{
  static KickBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	KickBallのEnter関数
//
/////////////////////////////////////////////////////////
void KickBall::Enter(FieldPlayer* player)
{
  // チームにこのプレーヤーが制御していることを知らせる
   player->Team()->SetControllingPlayer(player);
   
   // プレイヤーは毎秒多くのキックを試みる
   if (!player->isReadyForNextKick()) 
   {
     player->GetFSM()->ChangeState(ChaseBall::Instance());
   }

   
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters kick state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	KickBallのExecute関数
//
/////////////////////////////////////////////////////////
void KickBall::Execute(FieldPlayer* player)
{ 
  // ボールを指すベクトルとプレーヤーの方向の内積を計算
  Vector2D ToBall = player->Ball()->Pos() - player->Pos();
  double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

  // ゴールキーパーが所持している場合、
  // またはプレーヤーの後ろにいる場合、
  // または既に割り当てられたレシーバーがある場合は
  // ボールを蹴ることができない
  if (player->Team()->Receiver() != NULL   ||
      player->Pitch()->GoalKeeperHasBall() ||
      (dot < 0) ) 
  {
    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Goaly has ball / ball behind player" << "";
    #endif
    
    player->GetFSM()->ChangeState(ChaseBall::Instance());

    return;
  }
  
  // プレイヤーが目指すべき相手のゴールラインに沿った位置を保持する
  Vector2D    BallTarget;

  // 打撃力を調整するためにドットプロダクトを使用
  // ボールが前方にあるほど強いキックをする
  double power = Prm.MaxShootingForce * dot;

  // プレーヤーがこのポジションからゴールを決められたと判断された場合、
  // またはとにかくボールをキックする必要がある場合、プレーヤーはシュートする
  if (player->Team()->CanShoot(player->Ball()->Pos(),
                               power,
                               BallTarget)                   || 
     (RandFloat() < Prm.ChancePlayerAttemptsPotShot))
  {
   #ifdef PLAYER_STATE_INFO_ON
   debug_con << "Player " << player->ID() << " attempts a shot at " << BallTarget << "";
   #endif

   // キックにノイズを加える
   // ノイズの量は、Prm.PlayerKickingAccuracyを変更することで調整する
   BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

   // ボールが蹴られる方向
   Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
   player->Ball()->Kick(KickDirection, power);
    
   // ステート変更  
   player->GetFSM()->ChangeState(Wait::Instance());
   
   player->FindSupport();
  
   return;
 }

  PlayerBase* receiver = NULL;

  power = Prm.MaxPassingForce * dot;
  
  // パスを受け取る候補者がいるかどうかを確認する
  if (player->isThreatened()  &&
      player->Team()->FindPass(player,
                              receiver,
                              BallTarget,
                              power,
                              Prm.MinPassDist))
  {     
    // キックにノイズを加える
    BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

    Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
    player->Ball()->Kick(KickDirection, power);

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " passes the ball with force " << power << "  to player " 
              << receiver->ID() << "  Target is " << BallTarget << "";
    #endif

    
    // パスを知らせる
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            player->ID(),
                            receiver->ID(),
                            Msg_ReceiveBall,
                            &BallTarget);                            
   

    // 指示されていないプレイヤーは
    // その位置で留まる
    player->GetFSM()->ChangeState(Wait::Instance());

    player->FindSupport();

    return;
  }

  // シュートすることもパスすることもできない場合
  // ドリブルする
  else
  {   
    player->FindSupport();

    player->GetFSM()->ChangeState(Dribble::Instance());
  }   
}

/////////////////////////////////////////////////////////
//
//	DribbleのInstance関数
//
/////////////////////////////////////////////////////////
Dribble* Dribble::Instance()
{
  static Dribble instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	DribbleのEnter関数
//
/////////////////////////////////////////////////////////
void Dribble::Enter(FieldPlayer* player)
{
  // チームにプレーヤーが制御していることを知らせる
  player->Team()->SetControllingPlayer(player);

#ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters dribble state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	DribbleのExecute関数
//
/////////////////////////////////////////////////////////
void Dribble::Execute(FieldPlayer* player)
{
  double dot = player->Team()->HomeGoal()->Facing().Dot(player->Heading());

  // ボールがプレイヤーとホームゴールの間にある場合、
  // プレイヤーが正しい方向を向くまで、複数回のキックと回転を行う
  if (dot < 0)
  {
    // プレイヤーの向きを少量回転させ、
    // ボールをその方向に蹴る
    Vector2D direction = player->Heading();

    // プレイヤーが正しい方向に回転するように
    // プレイヤーの向きと目標の向きとの間の角度の
    // 符号を計算する
    double angle = QuarterPi * -1 *
                 player->Team()->HomeGoal()->Facing().Sign(player->Heading());

    Vec2DRotateAroundOrigin(direction, angle);

    // プレイヤーがボールをコントロールして同時に回転するときに使用
    const double KickingForce = 0.8;

    player->Ball()->Kick(direction, KickingForce);
  }
  // ボールを蹴る 
  else
  {
    player->Ball()->Kick(player->Team()->HomeGoal()->Facing(),
                         Prm.MaxDribbleForce);  
  }

  // ボールを蹴ったらステートを変更
  player->GetFSM()->ChangeState(ChaseBall::Instance());
    
  return;  
}

/////////////////////////////////////////////////////////
//
//	ReceiveBallのInstance関数
//
/////////////////////////////////////////////////////////
ReceiveBall* ReceiveBall::Instance()
{
  static ReceiveBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ReceiveBallのEnter関数
//
/////////////////////////////////////////////////////////
void ReceiveBall::Enter(FieldPlayer* player)
{
  // このプレイヤーがボールを保持していることをチームに知らせる
  player->Team()->SetReceiver(player);
  
  // コントロールプレイヤー
  player->Team()->SetControllingPlayer(player);

  // 受信動作の2つのパターン
  // 1.メッセージを受けとったプレイヤーが送信したプレイヤーの方向を向く
  // 2.ボールを追うために追跡行動を使用する

  // 相手プレイヤーが受信したプレイヤーに近いかどうか、
  // 受信側プレイヤーが相手側のホットリージョンにいるかどうか調べる
  const double PassThreatRadius = 70.0;

  if (( player->InHotRegion() ||
        RandFloat() < Prm.ChanceOfUsingArriveTypeReceiveBehavior) &&
     !player->Team()->isOpponentWithinRadius(player->Pos(), PassThreatRadius))
  {
    player->Steering()->ArriveOn();
    
    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " enters receive state (Using Arrive)" << "";
    #endif
  }
  else
  {
    player->Steering()->PursuitOn();

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " enters receive state (Using Pursuit)" << "";
    #endif
  }
}

/////////////////////////////////////////////////////////
//
//	ReceiveBallのExecute関数
//
/////////////////////////////////////////////////////////
void ReceiveBall::Execute(FieldPlayer* player)
{
  // ボールがプレイヤーに十分に接近した場合、または
  // チームがボールコントロールを失った場合ステートを変更する
  if (player->BallWithinReceivingRange() || !player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(ChaseBall::Instance());

    return;
  }  

  if (player->Steering()->PursuitIsOn())
  {
    player->Steering()->SetTarget(player->Ball()->Pos());
  }

  // ステアリングターゲットになった場合、
  // ボールを待って対面する
  if (player->AtTarget())
  {
    player->Steering()->ArriveOff();
    player->Steering()->PursuitOff();
    player->TrackBall();    
    player->SetVelocity(Vector2D(0,0));
  } 
}

/////////////////////////////////////////////////////////
//
//	ReceiveBallのExit関数
//
/////////////////////////////////////////////////////////
void ReceiveBall::Exit(FieldPlayer* player)
{
  player->Steering()->ArriveOff();
  player->Steering()->PursuitOff();

  player->Team()->SetReceiver(NULL);
}