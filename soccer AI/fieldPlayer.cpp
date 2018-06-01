/*=======================================================================

	フィールドプレイヤー [fieldPlayer.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "fieldPlayer.h"
#include "playerBase.h"
#include "steeringBehaviors.h"
#include "transformations.h"
#include "geometry.h"
#include "cgdi.h"
#include "C2DMatrix.h"
#include "goal.h"
#include "region.h"
#include "entityFunctionTemplates.h"
#include "paramLoader.h"
#include "soccerTeam.h"
#include "regulator.h"
#include "debugConsole.h"
#include <limits>

using std::vector;

/////////////////////////////////////////////////////////
//
//	コンストラクタ
//
/////////////////////////////////////////////////////////
FieldPlayer::FieldPlayer(SoccerTeam* home_team,
                      int   home_region,
                      State<FieldPlayer>* start_state,
                      Vector2D  heading,
                      Vector2D velocity,
                      double    mass,
                      double    max_force,
                      double    max_speed,
                      double    max_turn_rate,
                      double    scale,
                      player_role role): PlayerBase(home_team,
                                                    home_region,
                                                    heading,
                                                    velocity,
                                                    mass,
                                                    max_force,
                                                    max_speed,
                                                    max_turn_rate,
                                                    scale,
                                                    role)                                    
{
  // ステートマシン設定
  m_pStateMachine =  new StateMachine<FieldPlayer>(this);

  if (start_state)
  {    
    m_pStateMachine->SetCurrentState(start_state);
    m_pStateMachine->SetPreviousState(start_state);
    m_pStateMachine->SetGlobalState(GlobalPlayerState::Instance());

    m_pStateMachine->CurrentState()->Enter(this);
  }    

  m_pSteering->SeparationOn();

  // レギュレーターを生成する
  m_pKickLimiter = new Regulator(Prm.PlayerKickFrequency);
}

/////////////////////////////////////////////////////////
//
//	デストラクタ
//
/////////////////////////////////////////////////////////
FieldPlayer::~FieldPlayer()
{
  delete m_pKickLimiter;
  delete m_pStateMachine;
}

/////////////////////////////////////////////////////////
//
//	Update関数
//
/////////////////////////////////////////////////////////
void FieldPlayer::Update()
{ 
  // 現在のステートを実行
  m_pStateMachine->Update();

  // 操舵力を計算する
  m_pSteering->Calculate();

  // 制御を加えて減速させる
  if (m_pSteering->Force().isZero())
  {
    const double BrakingRate = 0.8; 

    m_vVelocity = m_vVelocity * BrakingRate;                                     
  }
  
  // ラジアンだけを回転できるように制御する
  double TurningForce =   m_pSteering->SideComponent();

  Clamp(TurningForce, -Prm.PlayerMaxTurnRate, Prm.PlayerMaxTurnRate);

  // ベクトルを回転する
  Vec2DRotateAroundOrigin(m_vHeading, TurningForce);

  m_vVelocity = m_vHeading * m_vVelocity.Length();

  // m_vSideを再生成する
  m_vSide = m_vHeading.Perp();


  // プレイヤーの方向によって加えられる加速度を計算する
  Vector2D accel = m_vHeading * m_pSteering->ForwardComponent() / m_dMass;

  m_vVelocity += accel;

  // 最高速度を超えないようにする
  m_vVelocity.Truncate(m_dMaxSpeed);

  // 位置を更新する
  m_vPosition += m_vVelocity;

  // 規制する
  if(Prm.bNonPenetrationConstraint)
  {
    EnforceNonPenetrationContraint(this, AutoList<PlayerBase>::GetAllMembers());
  }
}

/////////////////////////////////////////////////////////
//
//	HandleMessage関数
//
/////////////////////////////////////////////////////////
bool FieldPlayer::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

/////////////////////////////////////////////////////////
//
//	Render関数
//
/////////////////////////////////////////////////////////
void FieldPlayer::Render()                                         
{
  gdi->TransparentText();
  gdi->TextColor(Cgdi::grey);

  // カラー設定
  if (Team()->Color() == SoccerTeam::blue){gdi->BluePen();}
  else{gdi->RedPen();}

  
  m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
                                         Pos(),
                                         Heading(),
                                         Side(),
                                         Scale());
  gdi->ClosedShape(m_vecPlayerVBTrans);  
  
  gdi->BrownBrush();
  if (Prm.bHighlightIfThreatened && (Team()->ControllingPlayer() == this) && isThreatened()) gdi->YellowBrush();
  gdi->Circle(Pos(), 6);

    
  if (Prm.bStates)
  {  
    gdi->TextColor(0, 170, 0);
    gdi->TextAtPos(m_vPosition.x, m_vPosition.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
  }

  if (Prm.bIDs)
  {
    gdi->TextColor(0, 170, 0);
    gdi->TextAtPos(Pos().x-20, Pos().y-20, ttos(ID()));
  }

  if (Prm.bViewTargets)
  {
    gdi->RedBrush();
    gdi->Circle(Steering()->Target(), 3);
    gdi->TextAtPos(Steering()->Target(), ttos(ID()));
  }   
}



