/*=======================================================================

	�t�B�[���h�v���C���[�X�e�[�g [fieldPlayerStates.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�萔��`
//
/////////////////////////////////////////////////////////
#define PLAYER_STATE_INFO_ON

/////////////////////////////////////////////////////////
//
//	GlobalPlayerState��Instance�֐�
//
/////////////////////////////////////////////////////////
GlobalPlayerState* GlobalPlayerState::Instance()
{
  static GlobalPlayerState instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GlobalPlayerState��Execute�֐�
//
/////////////////////////////////////////////////////////
void GlobalPlayerState::Execute(FieldPlayer* player)                                     
{
  // �v���[���[���{�[���������Ă���ꍇ�A�{�[���ɋ߂Â��ƁA�ō����x��������
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
//	GlobalPlayerState��OnMessage�֐�
//
/////////////////////////////////////////////////////////
bool GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
  case Msg_ReceiveBall:
    {
      // �^�[�Q�b�g�Z�b�g
      player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

      // �X�e�[�g�ύX 
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
      
      // �œK��SupportPotision�ɖڕW��ݒ肷��
      player->Steering()->SetTarget(player->Team()->GetSupportSpot());

      // �X�e�[�g�ύX
      player->GetFSM()->ChangeState(SupportAttacker::Instance());

      return true;
    }

    break;

 case Msg_Wait:
    {
      // ��Ԃ�ύX����
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
      
      // �p�X��v�����Ă���v���[���[�̈ʒu���擾
      FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

      #ifdef PLAYER_STATE_INFO_ON
      debug_con << "Player " << player->ID() << " received request from " <<
                    receiver->ID() << " to make pass" << "";
      #endif

      // �{�[�����R�荞�ݔ͈͓��ɂȂ����A�܂��̓{�[�������łɎ󂯎�葤�̃v���[���[�ł���ꍇ�A
      // ���̃v���[���[�̓{�[����v�������v���[���[�ɓn�����Ƃ��ł��Ȃ�
      if (player->Team()->Receiver() != NULL ||
         !player->BallWithinKickingRange() )
      {
        #ifdef PLAYER_STATE_INFO_ON
        debug_con << "Player " << player->ID() << " cannot make requested pass <cannot kick ball>" << "";
        #endif

        return true;
      }
      
      // �p�X  
      player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
                           Prm.MaxPassingForce);

          
     #ifdef PLAYER_STATE_INFO_ON
     debug_con << "Player " << player->ID() << " Passed ball to requesting player" << "";
     #endif
        
      // �p�X���󂯎��v���C���[�ɒm�点��
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              player->ID(),
                              receiver->ID(),
                              Msg_ReceiveBall,
                              &receiver->Pos());

   

      // �X�e�[�g�ύX   
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
//	ChaseBall��Instance�֐�
//
/////////////////////////////////////////////////////////
ChaseBall* ChaseBall::Instance()
{
  static ChaseBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ChaseBall��Enter�֐�
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
//	ChaseBall��Execute�֐�
//
/////////////////////////////////////////////////////////
void ChaseBall::Execute(FieldPlayer* player)                                     
{
  // �{�[�����R�荞�ݔ͈͓��ɂ���ꍇ�A�v���C���[�͏�Ԃ�KickBall�ɕύX
  if (player->BallWithinKickingRange())
  {
    player->GetFSM()->ChangeState(KickBall::Instance());
    
    return;
  }
                                                                              
  // �v���C���[���{�[���ɍł��߂��v���C���[�ł���΁A
  // �v���C���[�����ǂ������Ȃ���΂Ȃ�Ȃ�
  if (player->isClosestTeamMemberToBall())
  {
    player->Steering()->SetTarget(player->Ball()->Pos());

    return;
  }
  
  // �v���C���[���{�[���ɋ߂��Ȃ��ꍇ�A���G���A�ɖ߂�A�ʂ̋@���҂�
  player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());
}

/////////////////////////////////////////////////////////
//
//	ChaseBall��Exit�֐�
//
/////////////////////////////////////////////////////////
void ChaseBall::Exit(FieldPlayer* player)
{
  player->Steering()->SeekOff();
}

/////////////////////////////////////////////////////////
//
//	SupportAttacker��Instance�֐�
//
/////////////////////////////////////////////////////////
SupportAttacker* SupportAttacker::Instance()
{
  static SupportAttacker instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	SupportAttacker��Enter�֐�
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
//	SupportAttacker��Execute�֐�
//
/////////////////////////////////////////////////////////
void SupportAttacker::Execute(FieldPlayer* player)                                     
{
  // �`�[�����R���g���[�����������玩�G���A�ɖ߂�
  if (!player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance()); return;
  } 

  // SupportPosition���ω������ꍇ�A���ǖڕW��ύX
  if (player->Team()->GetSupportSpot() != player->Steering()->Target())
  {    
    player->Steering()->SetTarget(player->Team()->GetSupportSpot());

    player->Steering()->ArriveOn();
  }

  // �v���C���[���ڕW�ɃV���[�g�ł��A���̃v���C���[�Ƀ{�[�����p�X�ł���΁A
  // ���̃v���C���[�͂��̃v���C���[�Ƀ{�[����n��
  if( player->Team()->CanShoot(player->Pos(),
                               Prm.MaxShootingForce))
  {
    player->Team()->RequestPass(player);
  }


  // �v���C���[���T�|�[�g�X�|�b�g�Ɉʒu���A
  // �`�[�����܂��{�[����ێ����Ă���ꍇ�A
  // ���̃v���C���[�͂܂��c���āA�{�[���ɒ��ʂ���
  if (player->AtTarget())
  {
    player->Steering()->ArriveOff();
        
    // �v���C���[�̓{�[���ɒ��ʂ���
    player->TrackBall();

    player->SetVelocity(Vector2D(0,0));

    // ���̃v���C���[���}�[�N���Ă��Ȃ��ꍇ�A�p�X��v��
    if (!player->isThreatened())
    {
      player->Team()->RequestPass(player);
    }
  }
}

/////////////////////////////////////////////////////////
//
//	SupportAttacker��Exit�֐�
//
/////////////////////////////////////////////////////////
void SupportAttacker::Exit(FieldPlayer* player)
{
  // �T�|�[�g����v���[���[��NULL�ɐݒ�
  player->Team()->SetSupportingPlayer(NULL);

  player->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegion��Instance�֐�
//
/////////////////////////////////////////////////////////
ReturnToHomeRegion* ReturnToHomeRegion::Instance()
{
  static ReturnToHomeRegion instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegion��Enter�֐�
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
//	ReturnToHomeRegion��Execute�֐�
//
/////////////////////////////////////////////////////////
void ReturnToHomeRegion::Execute(FieldPlayer* player)
{
  if (player->Pitch()->GameOn())
  {
    // �{�[�������̃`�[�������o�[�������̃v���[���[�ɋ߂��ꍇ����
    // ���蓖�Ă�ꂽ���V�[�o�[���Ȃ�����
    // �S�[���L�[�p�[���{�[����^���Ȃ������ꍇ�́A�����ǂ�������
    if ( player->isClosestTeamMemberToBall() &&
         (player->Team()->Receiver() == NULL) &&
         !player->Pitch()->GoalKeeperHasBall())
    {
      player->GetFSM()->ChangeState(ChaseBall::Instance());

      return;
    }
  }

  // �Q�[�����I���ŁA���G���A�ɏ\���߂��ꍇ�A
  // �X�e�[�g��ύX���đҋ@�����A
  // �v���C���[�̖ڕW�����݂̈ʒu�ɖ߂�
  if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
                                                             Region::halfsize))
  {
    player->Steering()->SetTarget(player->Pos());
    player->GetFSM()->ChangeState(Wait::Instance());
  }
  // �Q�[�����v���C����Ă��Ȃ��ꍇ�A
  // �v���C���[�͎��G���A�̒��S�ɋ߂Â�
  else if(!player->Pitch()->GameOn() && player->AtTarget())
  {
    player->GetFSM()->ChangeState(Wait::Instance());
  }
}

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegion��Exit�֐�
//
/////////////////////////////////////////////////////////
void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
  player->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////
//
//	Wait��Instance�֐�
//
/////////////////////////////////////////////////////////
Wait* Wait::Instance()
{
  static Wait instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Wait��Enter�֐�
//
/////////////////////////////////////////////////////////
void Wait::Enter(FieldPlayer* player)
{
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters wait state" << "";
  #endif

  // �Q�[�����I���łȂ��ꍇ�́A�^�[�Q�b�g���v���C���[�̃G���A�̒��S�ł��邱�Ƃ��m�F
  // �L�b�N�I�t�̏ꍇ�̐������ʒu�ɂ���
  if (!player->Pitch()->GameOn())
  {
    player->Steering()->SetTarget(player->HomeRegion()->Center());
  }
}

/////////////////////////////////////////////////////////
//
//	Wait��Execute�֐�
//
/////////////////////////////////////////////////////////
void Wait::Execute(FieldPlayer* player)
{    
  // �v���C���[���O�ꂽ��ԂɂȂ�����A���̈ʒu�ɖ߂�
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

  // ���̃v���C���[�̃`�[�����{�[�����R���g���[�����Ă��āA
  // ���̃v���C���[���{�[���������Ă��Ȃ�����
  // ���̃{�[����ێ����Ă���v���C���[������ɂ���ꍇ�A�p�X��v������
  if ( player->Team()->InControl()    &&
     (!player->isControllingPlayer()) &&
       player->isAheadOfAttacker() )
  {
    player->Team()->RequestPass(player);

    return;
  }

  if (player->Pitch()->GameOn())
  {
   // �{�[�������̃`�[�������o�[�������̃v���[���[�ɋ߂�
   // ���V�[�o�[�����蓖�Ă��Ă��Ȃ����S�[���L�[�p�[��
   // �{�[�����Ȃ��ꍇ�́A�����ǂ�������
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
//	Wait��Exit�֐�
//
/////////////////////////////////////////////////////////
void Wait::Exit(FieldPlayer* player)
{
}

/////////////////////////////////////////////////////////
//
//	KickBall��Instance�֐�
//
/////////////////////////////////////////////////////////
KickBall* KickBall::Instance()
{
  static KickBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	KickBall��Enter�֐�
//
/////////////////////////////////////////////////////////
void KickBall::Enter(FieldPlayer* player)
{
  // �`�[���ɂ��̃v���[���[�����䂵�Ă��邱�Ƃ�m�点��
   player->Team()->SetControllingPlayer(player);
   
   // �v���C���[�͖��b�����̃L�b�N�����݂�
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
//	KickBall��Execute�֐�
//
/////////////////////////////////////////////////////////
void KickBall::Execute(FieldPlayer* player)
{ 
  // �{�[�����w���x�N�g���ƃv���[���[�̕����̓��ς��v�Z
  Vector2D ToBall = player->Ball()->Pos() - player->Pos();
  double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

  // �S�[���L�[�p�[���������Ă���ꍇ�A
  // �܂��̓v���[���[�̌��ɂ���ꍇ�A
  // �܂��͊��Ɋ��蓖�Ă�ꂽ���V�[�o�[������ꍇ��
  // �{�[�����R�邱�Ƃ��ł��Ȃ�
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
  
  // �v���C���[���ڎw���ׂ�����̃S�[�����C���ɉ������ʒu��ێ�����
  Vector2D    BallTarget;

  // �Ō��͂𒲐����邽�߂Ƀh�b�g�v���_�N�g���g�p
  // �{�[�����O���ɂ���قǋ����L�b�N������
  double power = Prm.MaxShootingForce * dot;

  // �v���[���[�����̃|�W�V��������S�[�������߂�ꂽ�Ɣ��f���ꂽ�ꍇ�A
  // �܂��͂Ƃɂ����{�[�����L�b�N����K�v������ꍇ�A�v���[���[�̓V���[�g����
  if (player->Team()->CanShoot(player->Ball()->Pos(),
                               power,
                               BallTarget)                   || 
     (RandFloat() < Prm.ChancePlayerAttemptsPotShot))
  {
   #ifdef PLAYER_STATE_INFO_ON
   debug_con << "Player " << player->ID() << " attempts a shot at " << BallTarget << "";
   #endif

   // �L�b�N�Ƀm�C�Y��������
   // �m�C�Y�̗ʂ́APrm.PlayerKickingAccuracy��ύX���邱�ƂŒ�������
   BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

   // �{�[�����R�������
   Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
   player->Ball()->Kick(KickDirection, power);
    
   // �X�e�[�g�ύX  
   player->GetFSM()->ChangeState(Wait::Instance());
   
   player->FindSupport();
  
   return;
 }

  PlayerBase* receiver = NULL;

  power = Prm.MaxPassingForce * dot;
  
  // �p�X���󂯎����҂����邩�ǂ������m�F����
  if (player->isThreatened()  &&
      player->Team()->FindPass(player,
                              receiver,
                              BallTarget,
                              power,
                              Prm.MinPassDist))
  {     
    // �L�b�N�Ƀm�C�Y��������
    BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

    Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
    player->Ball()->Kick(KickDirection, power);

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " passes the ball with force " << power << "  to player " 
              << receiver->ID() << "  Target is " << BallTarget << "";
    #endif

    
    // �p�X��m�点��
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            player->ID(),
                            receiver->ID(),
                            Msg_ReceiveBall,
                            &BallTarget);                            
   

    // �w������Ă��Ȃ��v���C���[��
    // ���̈ʒu�ŗ��܂�
    player->GetFSM()->ChangeState(Wait::Instance());

    player->FindSupport();

    return;
  }

  // �V���[�g���邱�Ƃ��p�X���邱�Ƃ��ł��Ȃ��ꍇ
  // �h���u������
  else
  {   
    player->FindSupport();

    player->GetFSM()->ChangeState(Dribble::Instance());
  }   
}

/////////////////////////////////////////////////////////
//
//	Dribble��Instance�֐�
//
/////////////////////////////////////////////////////////
Dribble* Dribble::Instance()
{
  static Dribble instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Dribble��Enter�֐�
//
/////////////////////////////////////////////////////////
void Dribble::Enter(FieldPlayer* player)
{
  // �`�[���Ƀv���[���[�����䂵�Ă��邱�Ƃ�m�点��
  player->Team()->SetControllingPlayer(player);

#ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters dribble state" << "";
  #endif
}

/////////////////////////////////////////////////////////
//
//	Dribble��Execute�֐�
//
/////////////////////////////////////////////////////////
void Dribble::Execute(FieldPlayer* player)
{
  double dot = player->Team()->HomeGoal()->Facing().Dot(player->Heading());

  // �{�[�����v���C���[�ƃz�[���S�[���̊Ԃɂ���ꍇ�A
  // �v���C���[�������������������܂ŁA������̃L�b�N�Ɖ�]���s��
  if (dot < 0)
  {
    // �v���C���[�̌��������ʉ�]�����A
    // �{�[�������̕����ɏR��
    Vector2D direction = player->Heading();

    // �v���C���[�������������ɉ�]����悤��
    // �v���C���[�̌����ƖڕW�̌����Ƃ̊Ԃ̊p�x��
    // �������v�Z����
    double angle = QuarterPi * -1 *
                 player->Team()->HomeGoal()->Facing().Sign(player->Heading());

    Vec2DRotateAroundOrigin(direction, angle);

    // �v���C���[���{�[�����R���g���[�����ē����ɉ�]����Ƃ��Ɏg�p
    const double KickingForce = 0.8;

    player->Ball()->Kick(direction, KickingForce);
  }
  // �{�[�����R�� 
  else
  {
    player->Ball()->Kick(player->Team()->HomeGoal()->Facing(),
                         Prm.MaxDribbleForce);  
  }

  // �{�[�����R������X�e�[�g��ύX
  player->GetFSM()->ChangeState(ChaseBall::Instance());
    
  return;  
}

/////////////////////////////////////////////////////////
//
//	ReceiveBall��Instance�֐�
//
/////////////////////////////////////////////////////////
ReceiveBall* ReceiveBall::Instance()
{
  static ReceiveBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	ReceiveBall��Enter�֐�
//
/////////////////////////////////////////////////////////
void ReceiveBall::Enter(FieldPlayer* player)
{
  // ���̃v���C���[���{�[����ێ����Ă��邱�Ƃ��`�[���ɒm�点��
  player->Team()->SetReceiver(player);
  
  // �R���g���[���v���C���[
  player->Team()->SetControllingPlayer(player);

  // ��M�����2�̃p�^�[��
  // 1.���b�Z�[�W���󂯂Ƃ����v���C���[�����M�����v���C���[�̕���������
  // 2.�{�[����ǂ����߂ɒǐՍs�����g�p����

  // ����v���C���[����M�����v���C���[�ɋ߂����ǂ����A
  // ��M���v���C���[�����葤�̃z�b�g���[�W�����ɂ��邩�ǂ������ׂ�
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
//	ReceiveBall��Execute�֐�
//
/////////////////////////////////////////////////////////
void ReceiveBall::Execute(FieldPlayer* player)
{
  // �{�[�����v���C���[�ɏ\���ɐڋ߂����ꍇ�A�܂���
  // �`�[�����{�[���R���g���[�����������ꍇ�X�e�[�g��ύX����
  if (player->BallWithinReceivingRange() || !player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(ChaseBall::Instance());

    return;
  }  

  if (player->Steering()->PursuitIsOn())
  {
    player->Steering()->SetTarget(player->Ball()->Pos());
  }

  // �X�e�A�����O�^�[�Q�b�g�ɂȂ����ꍇ�A
  // �{�[����҂��đΖʂ���
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
//	ReceiveBall��Exit�֐�
//
/////////////////////////////////////////////////////////
void ReceiveBall::Exit(FieldPlayer* player)
{
  player->Steering()->ArriveOff();
  player->Steering()->PursuitOff();

  player->Team()->SetReceiver(NULL);
}