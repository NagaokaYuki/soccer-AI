/*=======================================================================

	チームステート [teamStates.h]

												長岡佑樹

	
	サッカーチームのステートプロトタイプ

=========================================================================*/
#ifndef TEAMSTATES_H
#define TEAMSTATES_H

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
class SoccerTeam;

/////////////////////////////////////////////////////////
//
//	Attackingクラス
//
/////////////////////////////////////////////////////////
class Attacking : public State<SoccerTeam>
{ 
private:
  
  Attacking(){}

public:

  // シングルトン
  static Attacking* Instance();

  void Enter(SoccerTeam* team);

  void Execute(SoccerTeam* team);

  void Exit(SoccerTeam* team);

  bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Defendingクラス
//
/////////////////////////////////////////////////////////
class Defending : public State<SoccerTeam>
{ 
private:
	
	Defending(){}

public:
	
	// シングルトン
	static Defending* Instance();
	
	void Enter(SoccerTeam* team);
	
	void Execute(SoccerTeam* team);
	
	void Exit(SoccerTeam* team);
	
	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	PrepareForKickOffクラス
//
/////////////////////////////////////////////////////////
class PrepareForKickOff : public State<SoccerTeam>
{ 
private:
	
	PrepareForKickOff(){}

public:

    // シングルトン
	static PrepareForKickOff* Instance();
	
	void Enter(SoccerTeam* team);
	
	void Execute(SoccerTeam* team);
	
	void Exit(SoccerTeam* team);
	
	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

#endif