/*=======================================================================

	�e���O���� [telegram.h]

												
												�����C��

=========================================================================*/
#ifndef TELEGRAM_H
#define TELEGRAM_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>

/////////////////////////////////////////////////////////
//
//	�\���� or �񋓌^
//
/////////////////////////////////////////////////////////
struct Telegram
{
  // ����Telegram�𑗂����G���e�B�e�B
  int          Sender;

  // ����Telegram���󂯎��G���e�B�e�B
  int          Receiver;

  // ���b�Z�[�W���̂��́B
  //"MessageTypes.h"�ɗ񋓂���Ă�����̂��ׂ�
  int          Msg;

  // ���b�Z�[�W�͒����ɑ��M����邩�A����̎��Ԃ����x�点�邱�Ƃ��ł���
  // �x�点��K�v������ꍇ�́A���̃t�B�[���h�̓��b�Z�\�W�����M����鎞�Ԃ�������Ă���
  double       DispatchTime;

  // ���b�Z�[�W�ɓY������ǉ����
  void*        ExtraInfo;


  Telegram():DispatchTime(-1),
                  Sender(-1),
                  Receiver(-1),
                  Msg(-1)
  {}


  Telegram(double time,
           int    sender,
           int    receiver,
           int    msg,
           void*  info = NULL): DispatchTime(time),
                               Sender(sender),
                               Receiver(receiver),
                               Msg(msg),
                               ExtraInfo(info)
  {}
 
};


// �����̃e���O�����̓v���C�I���e�B�L���[�Ɋi�[
// PQ�����ԗD�揇�ʂŃ\�[�g�ł���悤�ɁA
// >���Z�q���I�[�o�[���[�h����K�v������
const double SmallestDelay = 0.25;


inline bool operator==(const Telegram& t1, const Telegram& t2)
{
  return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
          (t1.Sender == t2.Sender)        &&
          (t1.Receiver == t2.Receiver)    &&
          (t1.Msg == t2.Msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
  if (t1 == t2)
  {
    return false;
  }

  else
  {
    return  (t1.DispatchTime < t2.DispatchTime);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t)
{
  os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
     << "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;

  return os;
}

// �e���O������ExtraInfo�t�B�[���h��K�v�ȃ^�C�v�Ƀf���t�@�����X���邽�߂֗̕��ȃw���p�[�@�\
template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}


#endif