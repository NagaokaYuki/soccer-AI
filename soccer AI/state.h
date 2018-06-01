/*=======================================================================

	�X�e�[�g���� [state.h]

												�����C��

	��Ԃ̃C���^�t�F�[�X���`���钊�ۊ�{�N���X

=========================================================================*/
#ifndef STATE_H
#define STATE_H

struct Telegram;

/////////////////////////////////////////////////////////
//
//	�e���v���[�g
//
/////////////////////////////////////////////////////////
template <class entity_type>

/////////////////////////////////////////////////////////
//
//	State�N���X
//
/////////////////////////////////////////////////////////
class State
{
public:
	
	virtual ~State(){}

	// ��Ԃɓ���Ǝ��s
	virtual void Enter(entity_type*)=0;
	
	// ��Ԃ̒ʏ�̍X�V�֐�
	virtual void Execute(entity_type*)=0;
	
	// ��Ԃ��I������Ǝ��s
	virtual void Exit(entity_type*)=0;
	
	// �G�[�W�F���g�����b�Z�[�W�f�B�X�p�b�`���[���烁�b�Z�[�W���󂯎��������s
	virtual bool OnMessage(entity_type*, const Telegram&)=0;

};

#endif