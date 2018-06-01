/*=======================================================================

	テレグラム [telegram.h]

												
												長岡佑樹

=========================================================================*/
#ifndef TELEGRAM_H
#define TELEGRAM_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>

/////////////////////////////////////////////////////////
//
//	構造体 or 列挙型
//
/////////////////////////////////////////////////////////
struct Telegram
{
  // このTelegramを送ったエンティティ
  int          Sender;

  // このTelegramを受け取るエンティティ
  int          Receiver;

  // メッセージそのもの。
  //"MessageTypes.h"に列挙されているものすべて
  int          Msg;

  // メッセージは直ちに送信されるか、特定の時間だけ遅らせることができる
  // 遅らせる必要がある場合は、このフィールドはメッセ―ジが送信される時間が書かれている
  double       DispatchTime;

  // メッセージに添えられる追加情報
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


// これらのテレグラムはプライオリティキューに格納
// PQが時間優先順位でソートできるように、
// >演算子をオーバーロードする必要がある
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

// テレグラムのExtraInfoフィールドを必要なタイプにデリファレンスするための便利なヘルパー機能
template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}


#endif