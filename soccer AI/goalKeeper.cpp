/*=======================================================================

	ゴールキーパー [goalKeeper.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "goalKeeper.h"
#include "cgdi.h"
#include "steeringBehaviors.h"
#include "soccerTeam.h"
#include "soccerPitch.h"
#include "transformations.h"
#include "goalKeeperStates.h"
#include "goal.h"
#include "entityFunctionTemplates.h"
#include "paramLoader.h"

/////////////////////////////////////////////////////////
//
//	コンストラクタ
//
/////////////////////////////////////////////////////////
GoalKeeper::GoalKeeper(SoccerTeam*        home_team,									// 自チーム	
                       int                home_region,									// 自エリア
                       State<GoalKeeper>* start_state,									// ステート
                       Vector2D           heading,										// 向き
                       Vector2D           velocity,										// 速度
                       double              mass,										// 質量
                       double              max_force,									// 最大力
                       double              max_speed,									// 最大速度
                       double              max_turn_rate,								// 最大回転率
                       double              scale): PlayerBase(home_team,				// 拡大縮小： 自チーム
                                                             home_region,				// 自エリア
                                                             heading,					// 向き
                                                             velocity,					// 速度
                                                             mass,						// 質量
                                                             max_force,					// 力
                                                             max_speed,					// 最大速度
                                                             max_turn_rate,				// 最大回転率
                                                             scale,						// 拡大縮小
                                                             PlayerBase::goal_keeper )	// プレイヤー情報
                                         
                                        
{   
	// ステートマシン設定
	m_pStateMachine = new StateMachine<GoalKeeper>(this);
	
	m_pStateMachine->SetCurrentState(start_state);
	m_pStateMachine->SetPreviousState(start_state);
	m_pStateMachine->SetGlobalState(GlobalKeeperState::Instance());
	
	m_pStateMachine->CurrentState()->Enter(this);        
}

/////////////////////////////////////////////////////////
//
//	Update関数
//
/////////////////////////////////////////////////////////
void GoalKeeper::Update()
{ 
	// 現在のステートを実行
	m_pStateMachine->Update();
	
	// 各操舵行動からの結合力を計算する
	Vector2D SteeringForce = m_pSteering->Calculate();
	
	// 加速=力/質量
	Vector2D Acceleration = SteeringForce / m_dMass;
	
	// 速度を更新
	m_vVelocity += Acceleration;
	
	// プレーヤーが最高速度を超えないようにする
	m_vVelocity.Truncate(m_dMaxSpeed);
	
	// 位置を更新
	m_vPosition += m_vVelocity;
	
	
	// 必要に応じて規制する
	if(Prm.bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, AutoList<PlayerBase>::GetAllMembers());
	}
	
	// プレイヤーの速度がゼロでない場合は向きを更新する
	if ( !m_vVelocity.isZero())
	{    
		m_vHeading = Vec2DNormalize(m_vVelocity);
		
		m_vSide = m_vHeading.Perp();
	}
	
	// m_vLookAtのベクトルはボールに向く
	if (!Pitch()->GoalKeeperHasBall())
	{
		m_vLookAt = Vec2DNormalize(Ball()->Pos() - Pos());
	}
}

/////////////////////////////////////////////////////////
//
//	BallWithinRangeForIntercept関数
//
/////////////////////////////////////////////////////////
bool GoalKeeper::BallWithinRangeForIntercept()const
{
	return (Vec2DDistanceSq(Team()->HomeGoal()->Center(), Ball()->Pos()) <=
			Prm.GoalKeeperInterceptRangeSq);
}

/////////////////////////////////////////////////////////
//
//	TooFarFromGoalMouth関数
//
/////////////////////////////////////////////////////////
bool GoalKeeper::TooFarFromGoalMouth()const
{
	return (Vec2DDistanceSq(Pos(), GetRearInterposeTarget()) >
			Prm.GoalKeeperInterceptRangeSq);
}

/////////////////////////////////////////////////////////
//
//	GetRearInterposeTarget関数
//
/////////////////////////////////////////////////////////
Vector2D GoalKeeper::GetRearInterposeTarget()const
{
	double xPosTarget = Team()->HomeGoal()->Center().x;
	
	double yPosTarget = Pitch()->PlayingArea()->Center().y - 
	                   Prm.GoalWidth*0.5 + (Ball()->Pos().y*Prm.GoalWidth) /
	                   Pitch()->PlayingArea()->Height();
	
	return Vector2D(xPosTarget, yPosTarget); 
}

/////////////////////////////////////////////////////////
//
//	HandleMessage関数
//
/////////////////////////////////////////////////////////
bool GoalKeeper::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

/////////////////////////////////////////////////////////
//
//	Render関数
//
/////////////////////////////////////////////////////////
void GoalKeeper::Render()                                         
{
	if (Team()->Color() == SoccerTeam::blue)
	{
		gdi->BluePen();
	}
	else
	{
		gdi->RedPen();
	}
	
	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
	                                     Pos(),
	                                     m_vLookAt,
	                                     m_vLookAt.Perp(),
	                                     Scale());
	
	gdi->ClosedShape(m_vecPlayerVBTrans);
	
	// 頭に描く
	gdi->BrownBrush();
	gdi->Circle(Pos(), 6);
	
	// IDを描く
	if (Prm.bIDs)
	{
		gdi->TextColor(0, 170, 0);;
		gdi->TextAtPos(Pos().x-20, Pos().y-20, ttos(ID()));
	}
	
	// ステートを描く
	if (Prm.bStates)
	{ 
		gdi->TextColor(0, 170, 0); 
		gdi->TransparentText(); 
		gdi->TextAtPos(m_vPosition.x, m_vPosition.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
	}
}