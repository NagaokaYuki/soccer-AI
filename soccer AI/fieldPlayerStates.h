/*=======================================================================

	フィールドプレイヤーのステート [fieldPlayerStates.h]

												長岡佑樹
	
	フィールドプレイヤーのためのステート

=========================================================================*/
#ifndef FIELDPLAYERSTATES_H
#define FIELDPLAYERSTATES_H

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
class FieldPlayer;
class SoccerPitch;

/////////////////////////////////////////////////////////
//
//	GlobalPlayerStateクラス
//
/////////////////////////////////////////////////////////
class GlobalPlayerState : public State<FieldPlayer>
{
private:
	
	GlobalPlayerState(){}

public:

	// シングルトン
	static GlobalPlayerState* Instance();
	
	void Enter(FieldPlayer* player){}
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&);
};

/////////////////////////////////////////////////////////
//
//	ChaseBallクラス
//
/////////////////////////////////////////////////////////
class ChaseBall : public State<FieldPlayer>
{
private:
	
	ChaseBall(){}

public:

	// シングルトン
	static ChaseBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Dribbleクラス
//
/////////////////////////////////////////////////////////
class Dribble : public State<FieldPlayer>
{
private:
	
	Dribble(){}

public:

	// シングルトン
	static Dribble* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegionクラス
//
/////////////////////////////////////////////////////////
class ReturnToHomeRegion: public State<FieldPlayer>
{
private:
	
	ReturnToHomeRegion(){}

public:

	// シングルトン
	static ReturnToHomeRegion* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Waitクラス
//
/////////////////////////////////////////////////////////
class Wait: public State<FieldPlayer>
{
private:
	Wait(){}

public:

	// シングルトン
	static Wait* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	KickBallクラス
//
/////////////////////////////////////////////////////////
class KickBall: public State<FieldPlayer>
{
private:
	KickBall(){}

public:
	// シングルトン
	static KickBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReceiveBallクラス
//
/////////////////////////////////////////////////////////
class ReceiveBall: public State<FieldPlayer>
{
private:
	ReceiveBall(){}

public:

	// シングルトン
	static ReceiveBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	SupportAttackerクラス
//
/////////////////////////////////////////////////////////
class SupportAttacker: public State<FieldPlayer>
{
private:
	SupportAttacker(){}

public:

	// シングルトン
	static SupportAttacker* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

#endif