/*=======================================================================

	メッセージディスパッチャー [messageDispatcher.cpp]

												
												長岡佑樹

=========================================================================*/
#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <set>
#include <string>
#include "Telegram.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
#define Dispatcher MessageDispatcher::Instance()		// MessageDispatcherのInstanceの定義

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class BaseGameEntity;


const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;


class MessageDispatcher
{
private:  
  
  // 自動ソートと重複の回避の利点のため、
  // 遅延メッセージのコンテナとしてstd :: setが使用される
  // メッセージはディスパッチ時間でソートされる
  std::set<Telegram> PriorityQ;

  // 受信エンティティのメッセージ処理メンバー関数pReceiverを
  // 新しく作成されたテレグラムで呼び出す
  void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

  MessageDispatcher(){}

  // コピーctorと割り当てはプライベート
  MessageDispatcher(const MessageDispatcher&);
  MessageDispatcher& operator=(const MessageDispatcher&);

public:

  static MessageDispatcher* Instance();

  // 別のエージェントにメッセージを送信(受信はIDで参照)
  void DispatchMsg(double      delay,
                   int         sender,
                   int         receiver,
                   int         msg,
                   void*       ExtraInfo);

  // すべての遅延メッセージを送信
  // メインゲームループを通して毎回呼び出される
  void DispatchDelayedMessages();
};



#endif