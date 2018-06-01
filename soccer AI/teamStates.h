/*=======================================================================

	�`�[���X�e�[�g [teamStates.h]

												�����C��

	
	�T�b�J�[�`�[���̃X�e�[�g�v���g�^�C�v

=========================================================================*/
#ifndef TEAMSTATES_H
#define TEAMSTATES_H

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
class SoccerTeam;

/////////////////////////////////////////////////////////
//
//	Attacking�N���X
//
/////////////////////////////////////////////////////////
class Attacking : public State<SoccerTeam>
{ 
private:
  
  Attacking(){}

public:

  // �V���O���g��
  static Attacking* Instance();

  void Enter(SoccerTeam* team);

  void Execute(SoccerTeam* team);

  void Exit(SoccerTeam* team);

  bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Defending�N���X
//
/////////////////////////////////////////////////////////
class Defending : public State<SoccerTeam>
{ 
private:
	
	Defending(){}

public:
	
	// �V���O���g��
	static Defending* Instance();
	
	void Enter(SoccerTeam* team);
	
	void Execute(SoccerTeam* team);
	
	void Exit(SoccerTeam* team);
	
	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	PrepareForKickOff�N���X
//
/////////////////////////////////////////////////////////
class PrepareForKickOff : public State<SoccerTeam>
{ 
private:
	
	PrepareForKickOff(){}

public:

    // �V���O���g��
	static PrepareForKickOff* Instance();
	
	void Enter(SoccerTeam* team);
	
	void Execute(SoccerTeam* team);
	
	void Exit(SoccerTeam* team);
	
	bool OnMessage(SoccerTeam*, const Telegram&){return false;}
};

#endif