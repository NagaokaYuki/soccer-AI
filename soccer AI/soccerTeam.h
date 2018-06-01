/*=======================================================================

	�T�b�J�[�`�[�� [soccerTeam.h]

												�����C��

  �T�b�J�[������G�[�W�F���g�̃`�[�����`����N���X
  SoccerTeam�ɂ͕����̃t�B�[���h�v���[���[��1�l�̃S�[���L�[�p�[���܂܂�
  SoccerTeam�͗L����ԃ}�V���Ƃ��Ď�������A�U���A�h��A�����KickOff�̏�Ԃ�����

=========================================================================*/
#ifndef SOCCERTEAM_H
#define SOCCERTEAM_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>

#include "Region.h"
#include "SupportSpotCalculator.h"
#include "StateMachine.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;

/////////////////////////////////////////////////////////
//
//	SoccerTeam�N���X
//
/////////////////////////////////////////////////////////                
class SoccerTeam 
{
public:
  
  enum team_color {blue, red};

private:

   // ��ԃ}�V���N���X�̃C���X�^���X
  StateMachine<SoccerTeam>*  m_pStateMachine;

  // �`�[���J���[
  team_color                m_Color;

  // �`�[�������o�[�ւ̃|�C���^
  std::vector<PlayerBase*>  m_Players;

  // �T�b�J�[�̃s�b�`�ւ̃|�C���^
  SoccerPitch*              m_pPitch;

  // �S�[���ւ̃|�C���^
  Goal*                     m_pOpponentsGoal;
  Goal*                     m_pHomeGoal;
  
  // ����`�[���ւ̃|�C���^
  SoccerTeam*               m_pOpponents;
   
  // �L�[�v���C���[�ւ̃|�C���^
  PlayerBase*               m_pControllingPlayer;
  PlayerBase*               m_pSupportingPlayer;
  PlayerBase*               m_pReceivingPlayer;
  PlayerBase*               m_pPlayerClosestToBall;

  // �ł��߂��v���[���[���{�[������̋����̎��拗��
  double                     m_dDistSqToBallOfClosestPlayer;

  // �v���C���[�͂�����g�p���ċ��Z��̐헪�I�|�W�V����������
  SupportSpotCalculator*    m_pSupportSpotCalc;


  // �`�[���̂��ׂĂ̑I����쐬
  void CreatePlayers();

  // �{�[���ɍł��߂��v���[���[���w���悤��m_pClosestPlayerToBall��ݒ�
  void CalculateClosestPlayerToBall();


public:

  SoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~SoccerTeam();

  void        Render()const;
  void        Update();

  // ���ׂẴt�B�[���h�v���[���[�̏�Ԃ�
  // ReturnToHomeRegion�̏�ԂɕύX�����
  // �S�[���L�[�p�[���������Ă���ꍇ�Ɏ�Ɏg�p�����
  void        ReturnAllFieldPlayersToHome()const;

  // �v���C���[���S�[���ɃV���[�g���鎞�A
  // ���K�����ꂽ�x�N�g���ɐݒ肷��ꍇ��true��Ԃ�
  // false�̓[���x�N�g���ɐݒ�
  bool        CanShoot(Vector2D  BallPos,
                       double     power, 
                       Vector2D& ShotTarget = Vector2D())const;

  // �őP�̃p�X��������֐�
  bool        FindPass(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                  power,
                      double                  MinPassingDistance)const;

  // 3�̐��ݓI�ȃp�X���v�Z�����B 1�͎�M�@�̌��݂̈ʒu�ɒ��ڌ����Ă���A
  // 2�̓{�[���ʒu�����M�@���甼�a�u�͈́v�̉~�܂ł̐ڐ��ł���B
  // �����̃p�X�́A���肪�T�󂵂ăv���[�G���A���ŏI�����邱�Ƃ��m�F���邽�߂Ƀe�X�g
  // �������ׂẴp�X�������������ꍇ�A�֐���false��Ԃ�
  // ����ȊO�̏ꍇ�A�֐��͑���̃S�[���G���A�ɍł��߂��{�[�����󂯎��p�X��Ԃ�
  bool        GetBestPassToReceiver(const PlayerBase* const passer,
                                    const PlayerBase* const receiver,
                                    Vector2D& PassTarget,
                                    const double power)const;

  // 'from'���� 'target'�܂ł̃p�X�������I�ɏR��ꂽ���ǂ������e�X�g����
  bool        isPassSafeFromOpponent(Vector2D    from,
                                     Vector2D    target,
                                     const PlayerBase* const receiver,
                                     const PlayerBase* const opp,
                                     double       PassingForce)const;

  // 'from'�̈ʒu���� 'target'�̈ʒu�܂ł̃p�X��
  // �e�����o�[�ɑ΂��ăe�X�g
  // �C���^�[�Z�v�g����Ȃ��ꍇtrue��Ԃ�
  bool        isPassSafeFromAllOpponents(Vector2D from,
                                         Vector2D target,
                                         const PlayerBase* const receiver,
                                         double     PassingForce)const;

  // �ʒu�̔��a���ɑΐ푊�肪����ꍇ��true��Ԃ�
  bool        isOpponentWithinRadius(Vector2D pos, double rad);

  // ���N�G�X�^�ƃR���g���[���v���[���̊ԂŃp�X���\���ǂ������e�X�g���� 
  void        RequestPass(FieldPlayer* requester)const;

  // �ł��x�����₷���ʒu���v�Z���A
  // �ł��K�؂ȍU���҂����̒n�_�Ɉړ�����̂�������
  PlayerBase* DetermineBestSupportingAttacker();
  

  const std::vector<PlayerBase*>& Members()const{return m_Players;}  

  StateMachine<SoccerTeam>* GetFSM()const{return m_pStateMachine;}
  
  Goal*const           HomeGoal()const{return m_pHomeGoal;}
  Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}

  SoccerPitch*const    Pitch()const{return m_pPitch;}           

  SoccerTeam*const     Opponents()const{return m_pOpponents;}
  void                 SetOpponents(SoccerTeam* opps){m_pOpponents = opps;}

  team_color           Color()const{return m_Color;}

  void                 SetPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}
  PlayerBase*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
  
  double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

  Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

  PlayerBase*          SupportingPlayer()const{return m_pSupportingPlayer;}
  void                 SetSupportingPlayer(PlayerBase* plyr){m_pSupportingPlayer = plyr;}

  PlayerBase*          Receiver()const{return m_pReceivingPlayer;}
  void                 SetReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

  PlayerBase*          ControllingPlayer()const{return m_pControllingPlayer;}
  void                 SetControllingPlayer(PlayerBase* plyr)
  {
    m_pControllingPlayer = plyr;
    Opponents()->LostControl();
  }


  bool  InControl()const{if(m_pControllingPlayer)return true; else return false;}
  void  LostControl(){m_pControllingPlayer = NULL;}

  PlayerBase*  GetPlayerFromID(int id)const;
  

  void SetPlayerHomeRegion(int plyr, int region)const;

  void DetermineBestSupportingPosition()const{m_pSupportSpotCalc->DetermineBestSupportingPosition();}

  void UpdateTargetsOfWaitingPlayers()const;

  // �����ꂩ�̃`�[�����Ɉʒu���Ă��Ȃ��ꍇ��false��Ԃ�
  bool AllPlayersAtHome()const;

  std::string Name()const{if (m_Color == blue) return "Blue"; return "Red";}

};

#endif