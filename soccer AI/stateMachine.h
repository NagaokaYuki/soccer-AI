/*=======================================================================

	ステートマシーン [stateMachine.h]

												長岡佑樹

	このクラスを継承し、エージェントにFSM機能を
	提供するいくつかの状態を作成する

=========================================================================*/
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <cassert>
#include <string>
#include "state.h"
#include "telegram.h"

/////////////////////////////////////////////////////////
//
//	StateMachineクラス
//
/////////////////////////////////////////////////////////
template <class entity_type>
class StateMachine
{
private:
	// インスタンスを所有するエージェントへのポインタ
	entity_type*          m_pOwner;
	
	State<entity_type>*   m_pCurrentState;
	
	// 最後の状態
	State<entity_type>*   m_pPreviousState;
	
	// FSMが更新されるたびに呼び出される
	State<entity_type>*   m_pGlobalState;

public:

	StateMachine(entity_type* owner):m_pOwner(owner),
	                                 m_pCurrentState(NULL),
	                                 m_pPreviousState(NULL),
	                                 m_pGlobalState(NULL)
	{}
	
	virtual ~StateMachine(){}
	
	// これらのメソッドを使用してFSMを初期化する
	void SetCurrentState(State<entity_type>* s){m_pCurrentState = s;}
	void SetGlobalState(State<entity_type>* s) {m_pGlobalState = s;}
	void SetPreviousState(State<entity_type>* s){m_pPreviousState = s;}
	
	// FSMを更新する
	void  Update()const
	{
		// グローバル状態が存在する場合は、そのexecuteメソッドを呼び出し、それ以外の場合は何もしない
		if(m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);
		
		// 現在の状態と同じ
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}
	
	bool  HandleMessage(const Telegram& msg)const
	{
		// まず、現在の状態が有効で、メッセージを処理できるかどうかを確認
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
		  return true;
		}
		
		// グローバル状態が実装されている場合には、
		// メッセージをグローバル状態に送信
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
		  return true;
		}
		
		return false;
	}

	// 新しい状態に変更する
	void  ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState>:trying to assign null state to current");
		
		// 前の状態の記録を保持する
		m_pPreviousState = m_pCurrentState;
		
		// 既存の状態の終了メソッドを呼び出す
		m_pCurrentState->Exit(m_pOwner);
		
		// 状態を新しい状態に変更する
		m_pCurrentState = pNewState;
		
		// 新しい状態のエントリメソッドを呼び出す
		m_pCurrentState->Enter(m_pOwner);
	}
	
	// 状態を以前の状態に戻す
	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}
	
	// 現在の状態の型がパラメータとして渡された
	// クラスの型と等しい場合はtrueを返す
	bool  isInState(const State<entity_type>& st)const
	{
		if (typeid(*m_pCurrentState) == typeid(st)) return true;
		return false;
	}
	
	State<entity_type>*  CurrentState()  const{return m_pCurrentState;}
	State<entity_type>*  GlobalState()   const{return m_pGlobalState;}
	State<entity_type>*  PreviousState() const{return m_pPreviousState;}
	
	// 現在の状態の名前を取得するためにデバッグ中にのみ使用
	std::string         GetNameOfCurrentState()const
	{
		std::string s(typeid(*m_pCurrentState).name());
		
		// 文字列の先頭から 'class'部分を削除する
		if (s.size() > 5)
		{
		  s.erase(0, 6);
		}
		
		return s;
	}
};

#endif


