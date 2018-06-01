/*=======================================================================

	�S�[���L�[�p�[�X�e�[�g [goalKeeperStates.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "goalKeeperStates.h"
#include "debugConsole.h"
#include "soccerPitch.h"
#include "playerBase.h"
#include "goalKeeper.h"
#include "steeringBehaviors.h"
#include "soccerTeam.h"
#include "goal.h"
#include "geometry.h"
#include "fieldPlayer.h"
#include "paramLoader.h"
#include "telegram.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////
//#define GOALY_STATE_INFO_ON

/////////////////////////////////////////////////////////
//
//	GlobalKeeperState��Instance�֐�
//
/////////////////////////////////////////////////////////
GlobalKeeperState* GlobalKeeperState::Instance()
{
  static GlobalKeeperState instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	GlobalKeeperState��OnMessage�֐�
//
/////////////////////////////////////////////////////////
bool GlobalKeeperState::OnMessage(GoalKeeper* keeper, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
    case Msg_GoHome:
    {
      keeper->SetDefaultHomeRegion();
      
      keeper->GetFSM()->ChangeState(ReturnHome::Instance());
    }

    break;

    case Msg_ReceiveBall:
      {
        keeper->GetFSM()->ChangeState(InterceptBall::Instance());
      }

      break;

  }
  return false;
}

/////////////////////////////////////////////////////////////////////////
//
//	GlobalKeeperState��TendGoal�֐�
//
//	�{�[���ƃl�b�g�̌��Ɏ������g��u�����߂́u����v���c�s�����g�p���āA
//	�S�[���}�E�X�̍�����E�ֈړ�����
//
//	�{�[�����u�S�[���L�[�p�[�͈̔́v���ɓ������ꍇ�A
//	�{�[���̓S�[���}�E�X���痣��āA������C���^�[�Z�v�g���悤�Ƃ���
//
/////////////////////////////////////////////////////////////////////////
TendGoal* TendGoal::Instance()
{
  static TendGoal instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoal��Enter�֐�
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Enter(GoalKeeper* keeper)
{
  // ������I���ɂ���
  keeper->Steering()->InterposeOn(Prm.GoalKeeperTendingDistance);

  // �{�[���̈ʒu�ƃS�[���̌��ɉ����Ĉʒu����ڕW�ʒu�Ƃ̊ԂɃG�[�W�F���g��z�u����
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoal��Execute�֐�
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Execute(GoalKeeper* keeper)
{
  // �{�[���̈ʒu���ω�����ƁA����̃C���^�[�^�[�Q�b�g���ω����邽�߁A�e�X�V�X�e�b�v���X�V����
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

  // �{�[�����͈͓��ɓ������ꍇ�A�L�[�p�[�͂����h���A
  // ��Ԃ�ς��ă{�[�����v���C���ɖ߂�
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();

    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());

    return;
  }

  // �{�[�����\�ߒ�߂�ꂽ�������ɂ���ꍇ�A
  // �L�[�p�[�͎��s����ʒu����O�ɏo�Ă�����Z�[�u����
  if (keeper->BallWithinRangeForIntercept() && !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(InterceptBall::Instance());
  }

  // �S�[���L�[�p���S�[�����C�����痣��Ă��āA�ΐ푊�肪�����Ă�����߂�
  if (keeper->TooFarFromGoalMouth() && keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(ReturnHome::Instance());

    return;
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	TendGoal��Exit�֐�
//
/////////////////////////////////////////////////////////////////////////
void TendGoal::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->InterposeOff();
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHome��Instance�֐�
//
//	�S�[���L�[�p�[�̓S�[�����S�ɖ߂�A�X�e�[�g��TendGoal�ɖ߂�
//
/////////////////////////////////////////////////////////////////////////
ReturnHome* ReturnHome::Instance()
{
  static ReturnHome instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHome��Enter�֐�
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOn();
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHome��Execute�֐�
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Execute(GoalKeeper* keeper)
{
  keeper->Steering()->SetTarget(keeper->HomeRegion()->Center());

  // �{�[�����R���g���[�����Ă���ꍇ�A
  // �X�e�[�g��ς��ăS�[���Ɍ����킹��
  if (keeper->InHomeRegion() || !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(TendGoal::Instance());
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	ReturnHome��Exit�֐�
//
/////////////////////////////////////////////////////////////////////////
void ReturnHome::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOff();
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBall��Instance�֐�
//
//	�Ǐ]�s�������ă{�[�����C���^�[�Z�v�g���悤�Ƃ���
//
/////////////////////////////////////////////////////////////////////////
InterceptBall* InterceptBall::Instance()
{
  static InterceptBall instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBall��Enter�֐�
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOn();  

    #ifdef GOALY_STATE_INFO_ON
    debug_con << "Goaly " << keeper->ID() << " enters InterceptBall" <<  "";
    #endif
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBall��Execute�֐�
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Execute(GoalKeeper* keeper)
{ 
  // �S�[���L�[�p���S�[�����牓���Ɉړ������ꍇ�A
  // �{�[���ɋ߂��ɂȂ����莩�G���A�ɖ߂�
  // �{�[���ɋ߂��ꍇ�̓C���^�[�Z�v�g����
  if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
  {
    keeper->GetFSM()->ChangeState(ReturnHome::Instance());

    return;
  }
  
  // �{�[�����S�[���L�[�p�[�̎�͈̔͂ɓ���ƁA
  // �{�[�����Z�[�u���ăv���[���ɖ߂�
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();
    
    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());

    return;
  }
}

/////////////////////////////////////////////////////////////////////////
//
//	InterceptBall��Exit�֐�
//
/////////////////////////////////////////////////////////////////////////
void InterceptBall::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOff();
}


/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlay��Instance�֐�
//
/////////////////////////////////////////////////////////////////////////
PutBallBackInPlay* PutBallBackInPlay::Instance()
{
  static PutBallBackInPlay instance;

  return &instance;
}

/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlay��Enter�֐�
//
/////////////////////////////////////////////////////////////////////////
void PutBallBackInPlay::Enter(GoalKeeper* keeper)
{
  // �L�[�p�[�������Ă��邱�Ƃ��`�[���ɒm�点��
  keeper->Team()->SetControllingPlayer(keeper);

  // ���ׂĂ̑I������G���A�֖߂�
  keeper->Team()->Opponents()->ReturnAllFieldPlayersToHome();
  keeper->Team()->ReturnAllFieldPlayersToHome();
}

/////////////////////////////////////////////////////////////////////////
//
//	PutBallBackInPlay��Execute�֐�
//
/////////////////////////////////////////////////////////////////////////
void PutBallBackInPlay::Execute(GoalKeeper* keeper)
{
  PlayerBase*  receiver = NULL;
  Vector2D     BallTarget;
    
  // �p�X�ł���v���C���[��T���A����������
  // �p�X������
  if (keeper->Team()->FindPass(keeper,
                              receiver,
                              BallTarget,
                              Prm.MaxPassingForce,
                              Prm.GoalkeeperMinPassDist))
  {     
    // �p�X������
    keeper->Ball()->Kick(Vec2DNormalize(BallTarget - keeper->Ball()->Pos()),
                         Prm.MaxPassingForce);

    // �S�[���L�[�p�[���{�[���������Ă��Ȃ�
    keeper->Pitch()->SetGoalKeeperHasBall(false);

    // �{�[�������邱�Ƃ�m�点��
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                          keeper->ID(),
                          receiver->ID(),
                          Msg_ReceiveBall,
                          &BallTarget);
    
    // �S�[���ɖ߂� 
    keeper->GetFSM()->ChangeState(TendGoal::Instance());

    return;
  }  

  // ���x���Z�b�g
  keeper->SetVelocity(Vector2D());
}

