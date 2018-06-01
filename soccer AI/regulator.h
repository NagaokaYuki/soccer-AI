/*=======================================================================

	���M�����[�^�[ [regulator.h]

												�����C��

	���̃N���X���g�p���ăR�[�h�t���[�𒲐�����
	(�X�V�֐��̏ꍇ�Ȃǁj�R�[�h�Z�N�V�������t���[���������p�x��
	�N���X���C���X�^���X�����i���b10��Ȃǁj�A
	���ɋ�����Ready�i�j��true��Ԃ��ꍇ�́A�v���O�����t���[�𑱍s

=========================================================================*/
#ifndef REGULATOR
#define REGULATOR

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "main.h"
#include "mmsystem.h" 
#include "utils.h"

/////////////////////////////////////////////////////////
//
//	Regulator�N���X
//
/////////////////////////////////////////////////////////
class Regulator
{
private:
	// �X�V�Ԋu
	double m_dUpdatePeriod;
	
	// ���񃌃M�����[�^���R�[�h�t���[��������Ƃ�
	DWORD m_dwNextUpdateTime;

public:
	Regulator(double NumUpdatesPerSecondRqd)
	{
		m_dwNextUpdateTime = (DWORD)(timeGetTime()+RandFloat()*1000);
		
		if (NumUpdatesPerSecondRqd > 0)
		{
		  m_dUpdatePeriod = 1000.0 / NumUpdatesPerSecondRqd; 
		}
		
		else if (isEqual(0.0, NumUpdatesPerSecondRqd))
		{
		  m_dUpdatePeriod = 0.0;
		}
		
		else if (NumUpdatesPerSecondRqd < 0)
		{
		  m_dUpdatePeriod = -1;
		}
	}

	// ���݂̎�����m_dwNextUpdateTime�𒴂����ꍇ��true��Ԃ�
	bool isReady()
	{
		// ���M�����[�^���[�����g���ŃC���X�^���X�����ꂽ�ꍇ�A
		// �X�e���X���[�h�ɂȂ�܂��i�������܂���j
		if (isEqual(0.0, m_dUpdatePeriod)) return true;

		// ���M�����[�^�����̎��g���ŃC���X�^���X������Ă���ꍇ
		// �R�[�h������Ȃ�
		if (m_dUpdatePeriod < 0) return false;
		
		DWORD CurrentTime = timeGetTime();
		
		// �K�v�ȍX�V�X�e�b�v���ƂɍX�V���Ԃ��ς��\��������~���b��
		// ���̃N���X�̕����̃N���C�A���g���A
		// �X�V���ϓ��ɍL�����Ă��邱�Ƃ��m�F���邽��
		static const double UpdatePeriodVariator = 10.0;
		
		if (CurrentTime >= m_dwNextUpdateTime)
		{
		  m_dwNextUpdateTime = (DWORD)(CurrentTime + m_dUpdatePeriod + RandInRange(-UpdatePeriodVariator, UpdatePeriodVariator));
		
		  return true;
		}
		
		return false;
	}
};

#endif