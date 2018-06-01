/*=======================================================================

	ゴールキーパのステート [goalKeeperStates.h]

												長岡佑樹

	ゴールキーパーが使用しているすべてのステートの宣言


=========================================================================*/
#ifndef KEEPERSTATES_H
#define KEEPERSTATES_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <string>
#include "state.h"
#include "telegram.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class GoalKeeper;
class SoccerPitch;

/////////////////////////////////////////////////////////
//
//	GlobalKeeperStateクラス
//
/////////////////////////////////////////////////////////
class GlobalKeeperState: public State<GoalKeeper>
{
private:
	
	GlobalKeeperState(){}

public:

	// シングルトン
	static GlobalKeeperState* Instance();
	
	void Enter(GoalKeeper* keeper){}
	
	void Execute(GoalKeeper* keeper){}
	
	void Exit(GoalKeeper* keeper){}
	
	bool OnMessage(GoalKeeper*, const Telegram&);
};

/////////////////////////////////////////////////////////
//
//	TendGoalクラス
//
/////////////////////////////////////////////////////////
class TendGoal: public State<GoalKeeper>
{
private:
	
	TendGoal(){}

public:

	// シングルトン
	static TendGoal* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	InterceptBallクラス
//
/////////////////////////////////////////////////////////
class InterceptBall: public State<GoalKeeper>
{
private:
	
	InterceptBall(){}

public:

	// シングルトン
	static InterceptBall* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReturnHomeクラス
//
/////////////////////////////////////////////////////////
class ReturnHome: public State<GoalKeeper>
{
private:
	
	ReturnHome(){}

public:

	// シングルトン
	static ReturnHome* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	PutBallBackInPlayクラス
//
/////////////////////////////////////////////////////////
class PutBallBackInPlay: public State<GoalKeeper>
{
private:
	
	PutBallBackInPlay(){}

public:

	// シングルトン
	static PutBallBackInPlay* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper){}
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

#endif