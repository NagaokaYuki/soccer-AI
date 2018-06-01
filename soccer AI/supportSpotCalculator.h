/*=======================================================================

	�T�|�[�g�X�|�b�g�J���L�����[�^ [supportSpotCalculator.h]

												�����C��

=========================================================================*/
#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include "region.h"
#include "vector2D.h"
#include "cgdi.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class PlayerBase;
class Goal;
class SoccerBall;
class SoccerTeam;
class Regulator;

/////////////////////////////////////////////////////////
//
//	SupportSpotCalculator�N���X
//
/////////////////////////////////////////////////////////
class SupportSpotCalculator
{
private:
  
  // �e�X�|�b�g�̒l�ƈʒu��ێ�����f�[�^�\��
  struct SupportSpot
  {
	  Vector2D  m_vPos;		// ���W�ʒu�ϐ�
	  double    m_dScore;		// �X�R�A�ϐ�
	  SupportSpot(Vector2D pos, double value):m_vPos(pos),
                                        m_dScore(value)
	  {}
  };

private:
	SoccerTeam*               m_pTeam;
	std::vector<SupportSpot>  m_Spots;
	
	// �Ō�̍X�V����̍ō��l�_�ւ̃|�C���^
	SupportSpot*              m_pBestSupportingSpot;

	// �X�|�b�g���v�Z�����p�x�𒲐�
	Regulator*                m_pRegulator;

public:
	SupportSpotCalculator(int numX,
                        int numY,
                        SoccerTeam* team);
	~SupportSpotCalculator();

	// �X�R�A�������قǁA�T�[�N���͑傫���Ȃ�
	void       Render()const;

	// �e�\�ȃX�|�b�g�𔽕����A���̃X�R�A���v�Z����
	Vector2D  DetermineBestSupportingPosition();
	
	// �ō��̎x���_������΂����Ԃ��A
	// �v�Z����Ă��Ȃ��ꍇ�́ADetermineBestSupportingPosition���Ăяo��
	// ���ʂ�Ԃ�
	Vector2D  GetBestSupportingSpot();
};


#endif