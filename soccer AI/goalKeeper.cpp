/*=======================================================================

	�S�[���L�[�p�[ [goalKeeper.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�R���X�g���N�^
//
/////////////////////////////////////////////////////////
GoalKeeper::GoalKeeper(SoccerTeam*        home_team,									// ���`�[��	
                       int                home_region,									// ���G���A
                       State<GoalKeeper>* start_state,									// �X�e�[�g
                       Vector2D           heading,										// ����
                       Vector2D           velocity,										// ���x
                       double              mass,										// ����
                       double              max_force,									// �ő��
                       double              max_speed,									// �ő呬�x
                       double              max_turn_rate,								// �ő��]��
                       double              scale): PlayerBase(home_team,				// �g��k���F ���`�[��
                                                             home_region,				// ���G���A
                                                             heading,					// ����
                                                             velocity,					// ���x
                                                             mass,						// ����
                                                             max_force,					// ��
                                                             max_speed,					// �ő呬�x
                                                             max_turn_rate,				// �ő��]��
                                                             scale,						// �g��k��
                                                             PlayerBase::goal_keeper )	// �v���C���[���
                                         
                                        
{   
	// �X�e�[�g�}�V���ݒ�
	m_pStateMachine = new StateMachine<GoalKeeper>(this);
	
	m_pStateMachine->SetCurrentState(start_state);
	m_pStateMachine->SetPreviousState(start_state);
	m_pStateMachine->SetGlobalState(GlobalKeeperState::Instance());
	
	m_pStateMachine->CurrentState()->Enter(this);        
}

/////////////////////////////////////////////////////////
//
//	Update�֐�
//
/////////////////////////////////////////////////////////
void GoalKeeper::Update()
{ 
	// ���݂̃X�e�[�g�����s
	m_pStateMachine->Update();
	
	// �e���Ǎs������̌����͂��v�Z����
	Vector2D SteeringForce = m_pSteering->Calculate();
	
	// ����=��/����
	Vector2D Acceleration = SteeringForce / m_dMass;
	
	// ���x���X�V
	m_vVelocity += Acceleration;
	
	// �v���[���[���ō����x�𒴂��Ȃ��悤�ɂ���
	m_vVelocity.Truncate(m_dMaxSpeed);
	
	// �ʒu���X�V
	m_vPosition += m_vVelocity;
	
	
	// �K�v�ɉ����ċK������
	if(Prm.bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, AutoList<PlayerBase>::GetAllMembers());
	}
	
	// �v���C���[�̑��x���[���łȂ��ꍇ�͌������X�V����
	if ( !m_vVelocity.isZero())
	{    
		m_vHeading = Vec2DNormalize(m_vVelocity);
		
		m_vSide = m_vHeading.Perp();
	}
	
	// m_vLookAt�̃x�N�g���̓{�[���Ɍ���
	if (!Pitch()->GoalKeeperHasBall())
	{
		m_vLookAt = Vec2DNormalize(Ball()->Pos() - Pos());
	}
}

/////////////////////////////////////////////////////////
//
//	BallWithinRangeForIntercept�֐�
//
/////////////////////////////////////////////////////////
bool GoalKeeper::BallWithinRangeForIntercept()const
{
	return (Vec2DDistanceSq(Team()->HomeGoal()->Center(), Ball()->Pos()) <=
			Prm.GoalKeeperInterceptRangeSq);
}

/////////////////////////////////////////////////////////
//
//	TooFarFromGoalMouth�֐�
//
/////////////////////////////////////////////////////////
bool GoalKeeper::TooFarFromGoalMouth()const
{
	return (Vec2DDistanceSq(Pos(), GetRearInterposeTarget()) >
			Prm.GoalKeeperInterceptRangeSq);
}

/////////////////////////////////////////////////////////
//
//	GetRearInterposeTarget�֐�
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
//	HandleMessage�֐�
//
/////////////////////////////////////////////////////////
bool GoalKeeper::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

/////////////////////////////////////////////////////////
//
//	Render�֐�
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
	
	// ���ɕ`��
	gdi->BrownBrush();
	gdi->Circle(Pos(), 6);
	
	// ID��`��
	if (Prm.bIDs)
	{
		gdi->TextColor(0, 170, 0);;
		gdi->TextAtPos(Pos().x-20, Pos().y-20, ttos(ID()));
	}
	
	// �X�e�[�g��`��
	if (Prm.bStates)
	{ 
		gdi->TextColor(0, 170, 0); 
		gdi->TransparentText(); 
		gdi->TextAtPos(m_vPosition.x, m_vPosition.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
	}
}