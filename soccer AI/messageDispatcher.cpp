/*=======================================================================

	メッセージディスパッチャー [messageDispatcher.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "MessageDispatcher.h"
#include "baseGameEntity.h"
#include "frameCounter.h"
#include "entityManager.h"
#include "debugConsole.h"

using std::set;

// デバッグウィンドウにメッセージ情報を送信する
//#define SHOW_MESSAGING_INFO

/////////////////////////////////////////////////////////
//
//	Instance関数
//
/////////////////////////////////////////////////////////
MessageDispatcher* MessageDispatcher::Instance()
{
  static MessageDispatcher instance; 
  
  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Discharge関数
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
//	DispatchMsg関数
//
//	メッセージ、受信者、送信者、および任意の時間遅延が
//	与えられた場合、この関数はメッセージを正しいエージェント
//	（遅延がない場合）にルーティングするか、
//	メッセージキューに格納して正しい時刻に送出する
//
/////////////////////////////////////////////////////////
void MessageDispatcher::DispatchMsg(double       delay,
                                    int          sender,
                                    int          receiver,
                                    int          msg,
                                    void*        AdditionalInfo = NULL)
{

  // 受信機へのポインタを得る
  BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

  // 受信機が有効であることを確認する
  if (pReceiver == NULL)
  {
    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nWarning! No Receiver with ID of " << receiver << " found" << "";
    #endif

    return;
  }
  
  // テレグラムを作成する
  Telegram telegram(0, sender, receiver, msg, AdditionalInfo);
  
  // 遅れがなければ、直ちに電文を送る                   
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

  // テレグラムが送信されるべき時間を計算する
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
//	DispatchDelayedMessages関数
//
//	有効期限が切れたタイムスタンプのテレグラムを送出する
//	ディスパッチされたすべてのテレグラムが
//	キューから削除される
//
/////////////////////////////////////////////////////////
void MessageDispatcher::DispatchDelayedMessages()
{ 
  double CurrentTime = TickCounter->GetCurrentFrame(); 

  // キューの前面からすべてのテレグラムを削除する
  while( !PriorityQ.empty() &&
	     (PriorityQ.begin()->DispatchTime < CurrentTime) && 
         (PriorityQ.begin()->DispatchTime > 0) )
  {
    // キューの前面からテレグラムを読む
    const Telegram& telegram = *PriorityQ.begin();

    // 受信者を見つける
    BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(telegram.Receiver);

    #ifdef SHOW_MESSAGING_INFO
    debug_con << "\nQueued telegram ready for dispatch: Sent to " 
         << pReceiver->ID() << ". Msg is "<< telegram.Msg << "";
    #endif

    // テレグラムを受信者に送信する
    Discharge(pReceiver, telegram);

	// キューから削除する
    PriorityQ.erase(PriorityQ.begin());
  }
}



