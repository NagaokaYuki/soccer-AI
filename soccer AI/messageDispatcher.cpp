/*=======================================================================

	���b�Z�[�W�f�B�X�p�b�`���[ [messageDispatcher.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "MessageDispatcher.h"
#include "baseGameEntity.h"
#include "frameCounter.h"
#include "entityManager.h"
#include "debugConsole.h"

using std::set;

// �f�o�b�O�E�B���h�E�Ƀ��b�Z�[�W���𑗐M����
//#define SHOW_MESSAGING_INFO

/////////////////////////////////////////////////////////
//
//	Instance�֐�
//
/////////////////////////////////////////////////////////
MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance; 
  
  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Discharge�֐�
//
/////////////////////////////////////////////////////////
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver, const Telegram& telegram)
{
  if (!pReceiver->HandleMessage(telegram))
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "Message not handled" << "";
    #endif
  }
}
/////////////////////////////////////////////////////////
//
//	DispatchMsg�֐�
//
//	���b�Z�[�W�A��M�ҁA���M�ҁA����єC�ӂ̎��Ԓx����
//	�^����ꂽ�ꍇ�A���̊֐��̓��b�Z�[�W�𐳂����G�[�W�F���g
//	�i�x�����Ȃ��ꍇ�j�Ƀ��[�e�B���O���邩�A
//	���b�Z�[�W�L���[�Ɋi�[���Đ����������ɑ��o����
//
/////////////////////////////////////////////////////////
void MessageDispatcher::DispatchMsg(double       delay,
                                    int          sender,
                                    int          receiver,
                                    int          msg,
                                    void*        AdditionalInfo = NULL)
{

  // ��M�@�ւ̃|�C���^�𓾂�
  BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

  // ��M�@���L���ł��邱�Ƃ��m�F����
  if (pReceiver == NULL)
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nWarning! No Receiver with ID of " << receiver << " found" << "";
    #endif

    return;
  }
  
  // �e���O�������쐬����
  Telegram telegram(0, sender, receiver, msg, AdditionalInfo);
  
  // �x�ꂪ�Ȃ���΁A�����ɓd���𑗂�                   
  if (delay <= 0.0)                                                        
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nTelegram dispatched at time: " << TickCounter->GetCurrentFrame()
         << " by " << sender << " for " << receiver 
         << ". Msg is " << msg << "";
    #endif

    //send the telegram to the recipient
    Discharge(pReceiver, telegram);
  }

  // �e���O���������M�����ׂ����Ԃ��v�Z����
  else
  {
    double CurrentTime = TickCounter->GetCurrentFrame(); 

    telegram.DispatchTime = CurrentTime + delay;

    //and put it in the queue
    PriorityQ.insert(telegram);   

    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nDelayed telegram from " << sender << " recorded at time " 
            << TickCounter->GetCurrentFrame() << " for " << receiver
            << ". Msg is " << msg << "";
    #endif
  }
}

/////////////////////////////////////////////////////////
//
//
//	DispatchDelayedMessages�֐�
//
//	�L���������؂ꂽ�^�C���X�^���v�̃e���O�����𑗏o����
//	�f�B�X�p�b�`���ꂽ���ׂẴe���O������
//	�L���[����폜�����
//
/////////////////////////////////////////////////////////
void MessageDispatcher::DispatchDelayedMessages()
{ 
  double CurrentTime = TickCounter->GetCurrentFrame(); 

  // �L���[�̑O�ʂ��炷�ׂẴe���O�������폜����
  while( !PriorityQ.empty() &&
	     (PriorityQ.begin()->DispatchTime < CurrentTime) && 
         (PriorityQ.begin()->DispatchTime > 0) )
  {
    // �L���[�̑O�ʂ���e���O������ǂ�
    const Telegram& telegram = *PriorityQ.begin();

    // ��M�҂�������
    BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nQueued telegram ready for dispatch: Sent to " 
         << pReceiver->ID() << ". Msg is "<< telegram.Msg << "";
    #endif

    // �e���O��������M�҂ɑ��M����
    Discharge(pReceiver, telegram);

	// �L���[����폜����
    PriorityQ.erase(PriorityQ.begin());
  }
}



