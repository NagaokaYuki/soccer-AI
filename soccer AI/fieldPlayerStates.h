/*=======================================================================

	�t�B�[���h�v���C���[�̃X�e�[�g [fieldPlayerStates.h]

												�����C��
	
	�t�B�[���h�v���C���[�̂��߂̃X�e�[�g

=========================================================================*/
#ifndef FIELDPLAYERSTATES_H
#define FIELDPLAYERSTATES_H

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
class FieldPlayer;
class SoccerPitch;

/////////////////////////////////////////////////////////
//
//	GlobalPlayerState�N���X
//
/////////////////////////////////////////////////////////
class GlobalPlayerState : public State<FieldPlayer>
{
private:
	
	GlobalPlayerState(){}

public:

	// �V���O���g��
	static GlobalPlayerState* Instance();
	
	void Enter(FieldPlayer* player){}
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&);
};

/////////////////////////////////////////////////////////
//
//	ChaseBall�N���X
//
/////////////////////////////////////////////////////////
class ChaseBall : public State<FieldPlayer>
{
private:
	
	ChaseBall(){}

public:

	// �V���O���g��
	static ChaseBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Dribble�N���X
//
/////////////////////////////////////////////////////////
class Dribble : public State<FieldPlayer>
{
private:
	
	Dribble(){}

public:

	// �V���O���g��
	static Dribble* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReturnToHomeRegion�N���X
//
/////////////////////////////////////////////////////////
class ReturnToHomeRegion: public State<FieldPlayer>
{
private:
	
	ReturnToHomeRegion(){}

public:

	// �V���O���g��
	static ReturnToHomeRegion* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	Wait�N���X
//
/////////////////////////////////////////////////////////
class Wait: public State<FieldPlayer>
{
private:
	Wait(){}

public:

	// �V���O���g��
	static Wait* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	KickBall�N���X
//
/////////////////////////////////////////////////////////
class KickBall: public State<FieldPlayer>
{
private:
	KickBall(){}

public:
	// �V���O���g��
	static KickBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player){}
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	ReceiveBall�N���X
//
/////////////////////////////////////////////////////////
class ReceiveBall: public State<FieldPlayer>
{
private:
	ReceiveBall(){}

public:

	// �V���O���g��
	static ReceiveBall* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/////////////////////////////////////////////////////////
//
//	SupportAttacker�N���X
//
/////////////////////////////////////////////////////////
class SupportAttacker: public State<FieldPlayer>
{
private:
	SupportAttacker(){}

public:

	// �V���O���g��
	static SupportAttacker* Instance();
	
	void Enter(FieldPlayer* player);
	
	void Execute(FieldPlayer* player);
	
	void Exit(FieldPlayer* player);
	
	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

#endif