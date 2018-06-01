/*=======================================================================

	�S�[���L�[�p�[ [goalKeeper.h]

												�����C��

	�S�[���L�[�p�[����������N���X

=========================================================================*/
#ifndef GOALY_H
#define GOALY_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "vector2D.h"
#include "playerBase.h"
#include "stateMachine.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class PlayerBase;

/////////////////////////////////////////////////////////
//
//	GoalKeeper�N���X
//
/////////////////////////////////////////////////////////
class GoalKeeper : public PlayerBase
{
private:
	
	// �X�e�[�g�}�V���̃C���X�^���X
	StateMachine<GoalKeeper>*  m_pStateMachine;
	
	// �{�[���Ɍ����Ďw�������x�N�g��
	Vector2D   m_vLookAt;
	
public:
	GoalKeeper(SoccerTeam*        home_team,
	           int                home_region,
	           State<GoalKeeper>* start_state,
	           Vector2D           heading,
	           Vector2D           velocity,
	           double              mass,
	           double              max_force,
	           double              max_speed,
	           double              max_turn_rate,
	           double              scale);
	
	~GoalKeeper(){delete m_pStateMachine;}
	
	void        Update();
	void        Render();
	bool        HandleMessage(const Telegram& msg);
	
	
	// �S�[���L�[�p�[���{�[�����C���^�[�Z�v�g����Ƃ���
	// �\���߂Â����ꍇ�ɐ^��Ԃ�
	bool        BallWithinRangeForIntercept()const;
	
	// �S�[���L�[�p�[���S�[�����牓������ꍇ
	bool        TooFarFromGoalMouth()const;
	
	// �C���^�[�Z�v�g�X�e�[�g�������ꍇ�ɌĂяo��
	// �^�[�Q�b�g��1�Ƃ��ăS�[���ɉ������X�|�b�g�����肷��
	// �L�[�p�[���J�o�[���悤�Ƃ��Ă���S�[�����C������
	// �{�[�����t�B�[���h��̂ǂ��ɂ��邩�ɂ���Ĉړ�����
	Vector2D    GetRearInterposeTarget()const;
	
	StateMachine<GoalKeeper>* GetFSM()const{return m_pStateMachine;}
	
	
	Vector2D    LookAt()const{return m_vLookAt;}
	void        SetLookAt(Vector2D v){m_vLookAt=v;}
};



#endif