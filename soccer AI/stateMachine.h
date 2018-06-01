/*=======================================================================

	�X�e�[�g�}�V�[�� [stateMachine.h]

												�����C��

	���̃N���X���p�����A�G�[�W�F���g��FSM�@�\��
	�񋟂��邢�����̏�Ԃ��쐬����

=========================================================================*/
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <cassert>
#include <string>
#include "state.h"
#include "telegram.h"

/////////////////////////////////////////////////////////
//
//	StateMachine�N���X
//
/////////////////////////////////////////////////////////
template <class entity_type>
class StateMachine
{
private:
	// �C���X�^���X�����L����G�[�W�F���g�ւ̃|�C���^
	entity_type*          m_pOwner;
	
	State<entity_type>*   m_pCurrentState;
	
	// �Ō�̏��
	State<entity_type>*   m_pPreviousState;
	
	// FSM���X�V����邽�тɌĂяo�����
	State<entity_type>*   m_pGlobalState;

public:

	StateMachine(entity_type* owner):m_pOwner(owner),
	                                 m_pCurrentState(NULL),
	                                 m_pPreviousState(NULL),
	                                 m_pGlobalState(NULL)
	{}
	
	virtual ~StateMachine(){}
	
	// �����̃��\�b�h���g�p����FSM������������
	void SetCurrentState(State<entity_type>* s){m_pCurrentState = s;}
	void SetGlobalState(State<entity_type>* s) {m_pGlobalState = s;}
	void SetPreviousState(State<entity_type>* s){m_pPreviousState = s;}
	
	// FSM���X�V����
	void  Update()const
	{
		// �O���[�o����Ԃ����݂���ꍇ�́A����execute���\�b�h���Ăяo���A����ȊO�̏ꍇ�͉������Ȃ�
		if(m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);
		
		// ���݂̏�ԂƓ���
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}
	
	bool  HandleMessage(const Telegram& msg)const
	{
		// �܂��A���݂̏�Ԃ��L���ŁA���b�Z�[�W�������ł��邩�ǂ������m�F
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
		  return true;
		}
		
		// �O���[�o����Ԃ���������Ă���ꍇ�ɂ́A
		// ���b�Z�[�W���O���[�o����Ԃɑ��M
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
		  return true;
		}
		
		return false;
	}

	// �V������ԂɕύX����
	void  ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine::ChangeState>:trying to assign null state to current");
		
		// �O�̏�Ԃ̋L�^��ێ�����
		m_pPreviousState = m_pCurrentState;
		
		// �����̏�Ԃ̏I�����\�b�h���Ăяo��
		m_pCurrentState->Exit(m_pOwner);
		
		// ��Ԃ�V������ԂɕύX����
		m_pCurrentState = pNewState;
		
		// �V������Ԃ̃G���g�����\�b�h���Ăяo��
		m_pCurrentState->Enter(m_pOwner);
	}
	
	// ��Ԃ��ȑO�̏�Ԃɖ߂�
	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}
	
	// ���݂̏�Ԃ̌^���p�����[�^�Ƃ��ēn���ꂽ
	// �N���X�̌^�Ɠ������ꍇ��true��Ԃ�
	bool  isInState(const State<entity_type>& st)const
	{
		if (typeid(*m_pCurrentState) == typeid(st)) return true;
		return false;
	}
	
	State<entity_type>*  CurrentState()  const{return m_pCurrentState;}
	State<entity_type>*  GlobalState()   const{return m_pGlobalState;}
	State<entity_type>*  PreviousState() const{return m_pPreviousState;}
	
	// ���݂̏�Ԃ̖��O���擾���邽�߂Ƀf�o�b�O���ɂ̂ݎg�p
	std::string         GetNameOfCurrentState()const
	{
		std::string s(typeid(*m_pCurrentState).name());
		
		// ������̐擪���� 'class'�������폜����
		if (s.size() > 5)
		{
		  s.erase(0, 6);
		}
		
		return s;
	}
};

#endif


