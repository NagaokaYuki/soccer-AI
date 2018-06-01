/*=======================================================================

	チームステート [teamStates.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "teamStates.h"
#include "soccerTeam.h"
#include "playerBase.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"
#include "soccerPitch.h"
#include "main.h"
#include "debugConsole.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
//#define DEBUG_TEAM_STATES

/////////////////////////////////////////////////////////
//
//	ChangePlayerHomeRegions関数
//
/////////////////////////////////////////////////////////
void ChangePlayerHomeRegions(SoccerTeam* team, const int NewRegions[TeamSize])
{
  for (int plyr=0; plyr<TeamSize; ++plyr)
  {
    team->SetPlayerHomeRegion(plyr, NewRegions[plyr]);
  }
}

/////////////////////////////////////////////////////////
//
//	AttackingのInstance関数
//
/////////////////////////////////////////////////////////
Attacking* Attacking::Instance()
{
  static Attacking instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	AttackingのEnter関数
//
/////////////////////////////////////////////////////////
void Attacking::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Attacking state" << "";
#endif

  // 各選手のホームを定義
  const int BlueRegions[TeamSize] = {1,12,14,6,4};
  const int RedRegions[TeamSize] = {16,3,5,9,13};

  // プレイヤーのホームを設定
  if (team->Color() == SoccerTeam::blue)
  {
    ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
    ChangePlayerHomeRegions(team, RedRegions);
  }

  // プレイヤーがWaitまたはReturnToHomeRegionの
  // いずれかの状態にある場合、ステアリングターゲットを
  // 新しいホームリージョンの状態に更新して、正しい位置に移動できるようにする
  team->UpdateTargetsOfWaitingPlayers();
}

/////////////////////////////////////////////////////////
//
//	AttackingのExecute関数
//
/////////////////////////////////////////////////////////
void Attacking::Execute(SoccerTeam* team)
{
  // コントロールチェンジ状態にない場合
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(Defending::Instance()); return;
  }

  // サポートする攻撃者が移動するための最良の位置を計算する
  team->DetermineBestSupportingPosition();
}

/////////////////////////////////////////////////////////
//
//	AttackingのExit関数
//
/////////////////////////////////////////////////////////
void Attacking::Exit(SoccerTeam* team)
{
  team->SetSupportingPlayer(NULL);
}

/////////////////////////////////////////////////////////
//
//	DefendingのInstance関数
//
/////////////////////////////////////////////////////////
Defending* Defending::Instance()
{
  static Defending instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	DefendingのEnter関数
//
/////////////////////////////////////////////////////////
void Defending::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Defending state" << "";
#endif

  // 各選手のホームを定義
  const int BlueRegions[TeamSize] = {1,6,8,3,5};
  const int RedRegions[TeamSize] = {16,9,11,12,14};

  // プレイヤーのホームを設定
  if (team->Color() == SoccerTeam::blue)
  {
    ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
    ChangePlayerHomeRegions(team, RedRegions);
  }
  
  // プレイヤーがWaitまたはReturnToHomeRegionのいずれかの状態にある場合、
  // そのステアリングターゲットを新しいホームリージョンのものに更新する
  team->UpdateTargetsOfWaitingPlayers();
}

/////////////////////////////////////////////////////////
//
//	DefendingのExecute関数
//
/////////////////////////////////////////////////////////
void Defending::Execute(SoccerTeam* team)
{
  // 制御変化状態にある場合
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(Attacking::Instance()); return;
  }
}

/////////////////////////////////////////////////////////
//
//	DefendingのExit関数
//
/////////////////////////////////////////////////////////
void Defending::Exit(SoccerTeam* team)
{
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOffのInstance関数
//
/////////////////////////////////////////////////////////
PrepareForKickOff* PrepareForKickOff::Instance()
{
  static PrepareForKickOff instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOffのEnter関数
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Enter(SoccerTeam* team)
{
  // リセットキープレイヤーのポインタ
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  // 各プレイヤーにMsg_GoHomeを送信
  team->ReturnAllFieldPlayersToHome();
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOffのExecute関数
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Execute(SoccerTeam* team)
{
  // 両方のチームがポジションにある場合は、ゲームを開始
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome())
  {
    team->GetFSM()->ChangeState(Defending::Instance());
  }
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOffのExit関数
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Exit(SoccerTeam* team)
{
  team->Pitch()->SetGameOn();
}


