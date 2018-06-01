/*=======================================================================

	�T�|�[�g�X�|�b�g�J���L�����[�^ [supportSpotCalculator.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "supportSpotCalculator.h"
#include "playerBase.h"
#include "goal.h"
#include "soccerBall.h"
#include "regulator.h"
#include "soccerTeam.h"
#include "paramLoader.h"
#include "soccerPitch.h"
#include "debugConsole.h"

/////////////////////////////////////////////////////////
//
//	�R���X�g���N�^
//
/////////////////////////////////////////////////////////
SupportSpotCalculator::SupportSpotCalculator(int           numX,
                                             int           numY,
                                             SoccerTeam*   team):m_pBestSupportingSpot(NULL),
                                                                  m_pTeam(team)
{
  const Region* PlayingField = team->Pitch()->PlayingArea();

  // �e�X�E�B�[�g�X�|�b�g�̈ʒu���v�Z���A�������쐬����m_Spots�Ɋi�[
  double HeightOfSSRegion = PlayingField->Height() * 0.8;
  double WidthOfSSRegion  = PlayingField->Width() * 0.9;
  double SliceX = WidthOfSSRegion / numX ;
  double SliceY = HeightOfSSRegion / numY;

  double left  = PlayingField->Left() + (PlayingField->Width()-WidthOfSSRegion)/2.0 + SliceX/2.0;
  double right = PlayingField->Right() - (PlayingField->Width()-WidthOfSSRegion)/2.0 - SliceX/2.0;
  double top   = PlayingField->Top() + (PlayingField->Height()-HeightOfSSRegion)/2.0 + SliceY/2.0;

  for (int x=0; x<(numX/2)-1; ++x)
  {
    for (int y=0; y<numY; ++y)
    {      
      if (m_pTeam->Color() == SoccerTeam::blue)
      {
        m_Spots.push_back(SupportSpot(Vector2D(left+x*SliceX, top+y*SliceY), 0.0));
      }

      else
      {
        m_Spots.push_back(SupportSpot(Vector2D(right-x*SliceX, top+y*SliceY), 0.0));
      }
    }
  }
  
  // ���M�����[�^�[����
  m_pRegulator = new Regulator(Prm.SupportSpotUpdateFreq);
}

/////////////////////////////////////////////////////////
//
//	�f�X�g���N�^
//
/////////////////////////////////////////////////////////
SupportSpotCalculator::~SupportSpotCalculator()
{
  delete m_pRegulator;
}

/////////////////////////////////////////////////////////
//
//	DetermineBestSupportingPosition�֐�
//
/////////////////////////////////////////////////////////
Vector2D SupportSpotCalculator::DetermineBestSupportingPosition()
{
  // ���t���[�����ƂɃX�|�b�g���X�V                             
  if (!m_pRegulator->isReady() && m_pBestSupportingSpot)
  {
    return m_pBestSupportingSpot->m_vPos;
  }

  // �x�X�g�T�|�[�g�X�|�b�g�����Z�b�g����
  m_pBestSupportingSpot = NULL;
 
  double BestScoreSoFar = 0.0;

  std::vector<SupportSpot>::iterator curSpot;

  for (curSpot = m_Spots.begin(); curSpot != m_Spots.end(); ++curSpot)
  {
    // �ŏ��ɈȑO�̃X�R�A���폜����
    // �G�C�h���I���ɂ��Ă���ꍇ�A
    // ���ׂẴX�|�b�g�̈ʒu�����邱�Ƃ��ł���悤�ɃX�R�A��1�ɐݒ肳���
    curSpot->m_dScore = 1.0;

    // �{�[���̈ʒu���炱�̈ʒu�ւ̈��S�ȃp�X����邱�Ƃ͉\�ȏꍇ
    if(m_pTeam->isPassSafeFromAllOpponents(m_pTeam->ControllingPlayer()->Pos(),
                                           curSpot->m_vPos,
                                           NULL,
                                           Prm.MaxPassingForce))
    {
      curSpot->m_dScore += Prm.Spot_PassSafeScore;
    }
      
   
    // ���̃|�W�V��������ڕW���̓_�ł��邩�ǂ����𔻒f
    if( m_pTeam->CanShoot(curSpot->m_vPos,            
                          Prm.MaxShootingForce))
    {
      curSpot->m_dScore += Prm.Spot_CanScoreFromPositionScore;
    }   

    
    // ���̃X�|�b�g���R���g���[���v���C���[����ǂ̂��炢����Ă��邩���v�Z���A
    // ����Ă���قǃX�R�A�͍����Ȃ�
    // OptimalDistance�s�N�Z����藣�ꂽ�����̓X�R�A���󂯎��Ȃ�
    if (m_pTeam->SupportingPlayer())
    {
      const double OptimalDistance = 200.0;
        
      double dist = Vec2DDistance(m_pTeam->ControllingPlayer()->Pos(),
                                 curSpot->m_vPos);
      
      double temp = fabs(OptimalDistance - dist);

      if (temp < OptimalDistance)
      {

        // �����𐳋K�����ăX�R�A�ɒǉ�
        curSpot->m_dScore += Prm.Spot_DistFromControllingPlayerScore *
                             (OptimalDistance-temp)/OptimalDistance;  
      }
    }
    
    // ���̓_������܂ł̂Ƃ���ō����_�ł��邩�ǂ������m�F
    if (curSpot->m_dScore > BestScoreSoFar)
    {
      BestScoreSoFar = curSpot->m_dScore;

      m_pBestSupportingSpot = &(*curSpot);
    }    
    
  }

  return m_pBestSupportingSpot->m_vPos;
}

/////////////////////////////////////////////////////////
//
//	GetBestSupportingSpot�֐�
//
/////////////////////////////////////////////////////////
Vector2D SupportSpotCalculator::GetBestSupportingSpot()
{
  if (m_pBestSupportingSpot)
  {
    return m_pBestSupportingSpot->m_vPos;
  }
    
  else
  { 
    return DetermineBestSupportingPosition();
  }
}

/////////////////////////////////////////////////////////
//
//	Render�֐�
//
/////////////////////////////////////////////////////////
void SupportSpotCalculator::Render()const
{
    gdi->HollowBrush();
    gdi->GreyPen();

    for (unsigned int spt=0; spt<m_Spots.size(); ++spt)
    {
      gdi->Circle(m_Spots[spt].m_vPos, m_Spots[spt].m_dScore);
    }

    if (m_pBestSupportingSpot)
    {
      gdi->GreenPen();
      gdi->Circle(m_pBestSupportingSpot->m_vPos, m_pBestSupportingSpot->m_dScore);
    }
}