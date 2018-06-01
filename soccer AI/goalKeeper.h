/*=======================================================================

	ゴールキーパー [goalKeeper.h]

												長岡佑樹

	ゴールキーパーを実装するクラス

=========================================================================*/
#ifndef GOALY_H
#define GOALY_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "vector2D.h"
#include "playerBase.h"
#include "stateMachine.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class PlayerBase;

/////////////////////////////////////////////////////////
//
//	GoalKeeperクラス
//
/////////////////////////////////////////////////////////
class GoalKeeper : public PlayerBase
{
private:
	
	// ステートマシンのインスタンス
	StateMachine<GoalKeeper>*  m_pStateMachine;
	
	// ボールに向けて指し示すベクトル
	Vector2D   m_vLookAt;
	
public:
	GoalKeeper(SoccerTeam*        home_team,
	           int                home_region,
	           State<GoalKeeper>* start_state,
	           Vector2D           heading,
	           Vector2D           velocity,
	           double              mass,
	           double              max_force,
	           double              max_speed,
	           double              max_turn_rate,
	           double              scale);
	
	~GoalKeeper(){delete m_pStateMachine;}
	
	void        Update();
	void        Render();
	bool        HandleMessage(const Telegram& msg);
	
	
	// ゴールキーパーがボールをインターセプトするときに
	// 十分近づいた場合に真を返す
	bool        BallWithinRangeForIntercept()const;
	
	// ゴールキーパーがゴールから遠すぎる場合
	bool        TooFarFromGoalMouth()const;
	
	// インターセプトステートだった場合に呼び出す
	// ターゲットの1つとしてゴールに沿ったスポットを決定する
	// キーパーがカバーしようとしているゴールラインから
	// ボールがフィールド上のどこにあるかによって移動する
	Vector2D    GetRearInterposeTarget()const;
	
	StateMachine<GoalKeeper>* GetFSM()const{return m_pStateMachine;}
	
	
	Vector2D    LookAt()const{return m_vLookAt;}
	void        SetLookAt(Vector2D v){m_vLookAt=v;}
};



#endif