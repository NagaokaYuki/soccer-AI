/*=======================================================================

	�S�[���L�[�p�̃X�e�[�g [goalKeeperStates.h]

												�����C��

	�S�[���L�[�p�[���g�p���Ă��邷�ׂẴX�e�[�g�̐錾


=========================================================================*/
#ifndef KEEPERSTATES_H
#define KEEPERSTATES_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <string>
#include "state.h"
#include "telegram.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class GoalKeeper;
class SoccerPitch;

/////////////////////////////////////////////////////////
//
//	GlobalKeeperState�N���X
//
/////////////////////////////////////////////////////////
class GlobalKeeperState: public State<GoalKeeper>
{
private:
	
	GlobalKeeperState(){}

public:

	// �V���O���g��
	static GlobalKeeperState* Instance();
	
	void Enter(GoalKeeper* keeper){}
	
	void Execute(GoalKeeper* keeper){}
	
	void Exit(GoalKeeper* keeper){}
	
	bool OnMessage(GoalKeeper*, const Telegram&);
};

/////////////////////////////////////////////////////////
//
//	TendGoal�N���X
//
/////////////////////////////////////////////////////////
class TendGoal: public State<GoalKeeper>
{
private:
	
	TendGoal(){}

public:

	// �V���O���g��
	static TendGoal* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	InterceptBall�N���X
//
/////////////////////////////////////////////////////////
class InterceptBall: public State<GoalKeeper>
{
private:
	
	InterceptBall(){}

public:

	// �V���O���g��
	static InterceptBall* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReturnHome�N���X
//
/////////////////////////////////////////////////////////
class ReturnHome: public State<GoalKeeper>
{
private:
	
	ReturnHome(){}

public:

	// �V���O���g��
	static ReturnHome* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper);
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	PutBallBackInPlay�N���X
//
/////////////////////////////////////////////////////////
class PutBallBackInPlay: public State<GoalKeeper>
{
private:
	
	PutBallBackInPlay(){}

public:

	// �V���O���g��
	static PutBallBackInPlay* Instance();
	
	void Enter(GoalKeeper* keeper);
	
	void Execute(GoalKeeper* keeper);
	
	void Exit(GoalKeeper* keeper){}
	
	bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

#endif