/*=======================================================================

	���b�Z�[�W�f�B�X�p�b�`���[ [messageDispatcher.cpp]

												
												�����C��

=========================================================================*/
#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <set>
#include <string>
#include "Telegram.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////
#define Dispatcher MessageDispatcher::Instance()		// MessageDispatcher��Instance�̒�`

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class BaseGameEntity;


const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;


class MessageDispatcher
{
private:  
  
  // �����\�[�g�Əd���̉���̗��_�̂��߁A
  // �x�����b�Z�[�W�̃R���e�i�Ƃ���std :: set���g�p�����
  // ���b�Z�[�W�̓f�B�X�p�b�`���ԂŃ\�[�g�����
  std::set<Telegram> PriorityQ;

  // ��M�G���e�B�e�B�̃��b�Z�[�W���������o�[�֐�pReceiver��
  // �V�����쐬���ꂽ�e���O�����ŌĂяo��
  void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

  MessageDispatcher(){}

  // �R�s�[ctor�Ɗ��蓖�Ă̓v���C�x�[�g
  MessageDispatcher(const MessageDispatcher&);
  MessageDispatcher& operator=(const MessageDispatcher&);

public:

  static MessageDispatcher* Instance();

  // �ʂ̃G�[�W�F���g�Ƀ��b�Z�[�W�𑗐M(��M��ID�ŎQ��)
  void DispatchMsg(double      delay,
                   int         sender,
                   int         receiver,
                   int         msg,
                   void*       ExtraInfo);

  // ���ׂĂ̒x�����b�Z�[�W�𑗐M
  // ���C���Q�[�����[�v��ʂ��Ė���Ăяo�����
  void DispatchDelayedMessages();
};



#endif