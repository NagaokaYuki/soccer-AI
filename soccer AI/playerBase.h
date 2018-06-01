/*=======================================================================

	�v���C���[�x�[�X [playerBase.h]

												�����C��


	�v���C���[�̊�{�N���X�B
	�v���C���[�̓I�[�g���X�g�N���X���p�����邽�߁A
	�쐬���ꂽ�v���C���[�͑��̃Q�[���I�u�W�F�N�g��
	�ȒP�ɃA�N�Z�X�ł���悤�Ɏ����I�ɒǉ������

=========================================================================*/
#pragma warning (disable:4786)
#ifndef PLAYERBASE_H
#define PLAYERBASE_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <cassert>
#include "autolist.h"
#include "vector2D.h"
#include "movingEntity.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;
class Region;

/////////////////////////////////////////////////////////
//
//	PlayerBase�N���X
//
/////////////////////////////////////////////////////////
class PlayerBase : public MovingEntity,
                   public AutoList<PlayerBase>
{
public:
	enum player_role{goal_keeper, attacker, defender};

protected:
	
	// �v���C���[�̃`�[���ɂ��������
	player_role             m_PlayerRole;
	
	// SoccerTeam�̃|�C���^
	SoccerTeam*             m_pTeam;
	
	// SteeringBehaviors�̃|�C���^
	SteeringBehaviors*      m_pSteering;
	
	// �v���C���[�̎��G���A
	int                     m_iHomeRegion;
	
	// �v���C���[���L�b�N�I�t����O�̗̈�
	int                     m_iDefaultRegion;
	
	// �{�[���܂ł̋���(�l�p�`�̋��)
	// �e���ԃX�e�b�v���Ƃ�1��v�Z����A����ɕۑ�����
	double                   m_dDistSqToBall;
	
	
	// ���_�o�b�t�@
	std::vector<Vector2D>   m_vecPlayerVB;

	// �ϊ����ꂽ���_�o�b�t�@
	std::vector<Vector2D>   m_vecPlayerVBTrans;

public:
	PlayerBase(SoccerTeam*    home_team,
			   int            home_region,
			   Vector2D       heading,
			   Vector2D       velocity,
			   double          mass,
			   double          max_force,
			   double          max_speed,
			   double          max_turn_rate,
			   double          scale,
			   player_role    role);
	
	virtual ~PlayerBase();
	
	// �v���C���[�̃]�[�����ɑΐ푊�肪����ꍇ��true��Ԃ�
	bool        isThreatened()const;
	
	// �v���C���[���{�[���܂��̓v���C���[�̌��݂̃^�[�Q�b�g�Ɍ�����悤�ɉ�]������
	void        TrackBall();
	void        TrackTarget();
	
	// �T�|�[�g���Ă���ꏊ�ɍł��߂��v���C���[�̏�Ԃ�ύX����
	// �U�����v���C���[���T�|�[�g����悤�Ɏw������
	void        FindSupport()const;
	
	// �S�[���L�[�p�[���{�[�������ނ��Ƃ��ł���ꍇ,true��Ԃ�
	bool        BallWithinKeeperRange()const;
	
	// �{�[�����L�b�N�͈͓��ɂ���ꍇ��true��Ԃ�
	bool        BallWithinKickingRange()const;
	
	// �{�[������M�@�͈͓̔��ɓ������ꍇ��true��Ԃ�
	bool        BallWithinReceivingRange()const;
	
	// �v���C���[�����G���A�̋��E���Ɉʒu����ꍇ��true��Ԃ�
	bool        InHomeRegion()const;
	
	// �v���C���[���U���҂����O�ɂ���ꍇ��true��Ԃ�
	bool        isAheadOfAttacker()const;
	
	// �v���C���[���w�肳�ꂽ�T�|�[�g�X�|�b�g�ɂ���ꍇ��true��Ԃ�
	bool        AtSupportSpot()const;
	
	// �v���C���[���X�e�A�����O�^�[�Q�b�g�ɂ���ꍇ��true��Ԃ�
	bool        AtTarget()const;
	
	// �v���C���[���{�[���̃`�[�����ōł��߂��v���C���[�̏ꍇ��true��Ԃ�
	bool        isClosestTeamMemberToBall()const;
	
	// position�Ŏw�肳�ꂽ�|�C���g���v���C���[�̑O�ɂ���ꍇ��true��Ԃ�
	bool        PositionInFrontOfPlayer(Vector2D position)const;
	
	// �v���C���[���{�[���̃s�b�`��ōł��߂��v���C���[�̏ꍇ��true��Ԃ�
	bool        isClosestPlayerOnPitchToBall()const;
	
	// �v���[���[���R���g���[���v���[���[�ł���ꍇ��true��Ԃ�
	bool        isControllingPlayer()const;
	
	// �v���C���[���w�肳�ꂽ�z�b�g�G���A�ɂ���ꍇ��true��Ԃ�
	// ����̃S�[���ɋ߂��G���A
	bool        InHotRegion()const;
	
	player_role Role()const{return m_PlayerRole;}
	
	double       DistSqToBall()const{return m_dDistSqToBall;}
	void        SetDistSqToBall(double val){m_dDistSqToBall = val;}
	
	// ����G���A/���G���A�܂ł̋������v�Z����
	// �p�X������x�Ɏg�p����
	double       DistToOppGoal()const;
	double       DistToHomeGoal()const;
	
	void        SetDefaultHomeRegion(){m_iHomeRegion = m_iDefaultRegion;}
	
	SoccerBall* const        Ball()const;
	SoccerPitch* const       Pitch()const;
	SteeringBehaviors*const  Steering()const{return m_pSteering;}
	const Region* const      HomeRegion()const;
	void                     SetHomeRegion(int NewRegion){m_iHomeRegion = NewRegion;}
	SoccerTeam*const         Team()const{return m_pTeam;}
	
};

#endif