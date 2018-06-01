/*=======================================================================

	�t�B�[���h�v���C���[ [fieldPlayer.h]

												�����C��

	PlayerBase����h�������N���X
	�T�b�J�[�̃s�b�`�A�R��A�h���u���A�V���[�g�̂𓮂����J�v�Z����

=========================================================================*/
#pragma warning (disable:4786)
#ifndef FIELDPLAYER_H
#define FIELDPLAYER_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�O���錾
//
/////////////////////////////////////////////////////////
class CSteeringBehavior;
class SoccerTeam;
class SoccerPitch;
class Goal;
struct Telegram;

/////////////////////////////////////////////////////////
//
//	FieldPlayer�N���X
//
/////////////////////////////////////////////////////////
class FieldPlayer : public PlayerBase
{
private:

   // �X�e�[�g�}�V���N���X�̃C���X�^���X
  StateMachine<FieldPlayer>*  m_pStateMachine;
  
  // �v���C���[��1�b�ԂɎ�邱�Ƃ��ł���L�b�N���𐧌�
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

  // ������Ăяo���ăv���C���[�̈ʒu�ƌ������X�V
  void        Update();   

  void        Render();

  bool        HandleMessage(const Telegram& msg);

  StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}

  bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

         
};

#endif