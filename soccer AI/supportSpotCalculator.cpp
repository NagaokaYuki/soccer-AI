/*=======================================================================

	サポートスポットカルキュレータ [supportSpotCalculator.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
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
//	コンストラクタ
//
/////////////////////////////////////////////////////////
SupportSpotCalculator::SupportSpotCalculator(int           numX,
                                             int           numY,
                                             SoccerTeam*   team):m_pBestSupportingSpot(NULL),
                                                                  m_pTeam(team)
{
  const Region* PlayingField = team->Pitch()->PlayingArea();

  // 各スウィートスポットの位置を計算し、それらを作成してm_Spotsに格納
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
  
  // レギュレーター生成
  m_pRegulator = new Regulator(Prm.SupportSpotUpdateFreq);
}

/////////////////////////////////////////////////////////
//
//	デストラクタ
//
/////////////////////////////////////////////////////////
SupportSpotCalculator::~SupportSpotCalculator()
{
  delete m_pRegulator;
}

/////////////////////////////////////////////////////////
//
//	DetermineBestSupportingPosition関数
//
/////////////////////////////////////////////////////////
Vector2D SupportSpotCalculator::DetermineBestSupportingPosition()
{
  // 数フレームごとにスポットを更新                             
  if (!m_pRegulator->isReady() && m_pBestSupportingSpot)
  {
    return m_pBestSupportingSpot->m_vPos;
  }

  // ベストサポートスポットをリセットする
  m_pBestSupportingSpot = NULL;
 
  double BestScoreSoFar = 0.0;

  std::vector<SupportSpot>::iterator curSpot;

  for (curSpot = m_Spots.begin(); curSpot != m_Spots.end(); ++curSpot)
  {
    // 最初に以前のスコアを削除する
    // エイドをオンにしている場合、
    // すべてのスポットの位置を見ることができるようにスコアが1に設定される
    curSpot->m_dScore = 1.0;

    // ボールの位置からこの位置への安全なパスを作ることは可能な場合
    if(m_pTeam->isPassSafeFromAllOpponents(m_pTeam->ControllingPlayer()->Pos(),
                                           curSpot->m_vPos,
                                           NULL,
                                           Prm.MaxPassingForce))
    {
      curSpot->m_dScore += Prm.Spot_PassSafeScore;
    }
      
   
    // このポジションから目標を採点できるかどうかを判断
    if( m_pTeam->CanShoot(curSpot->m_vPos,            
                          Prm.MaxShootingForce))
    {
      curSpot->m_dScore += Prm.Spot_CanScoreFromPositionScore;
    }   

    
    // このスポットがコントロールプレイヤーからどのくらい離れているかを計算し、
    // 離れているほどスコアは高くなる
    // OptimalDistanceピクセルより離れた距離はスコアを受け取らない
    if (m_pTeam->SupportingPlayer())
    {
      const double OptimalDistance = 200.0;
        
      double dist = Vec2DDistance(m_pTeam->ControllingPlayer()->Pos(),
                                 curSpot->m_vPos);
      
      double temp = fabs(OptimalDistance - dist);

      if (temp < OptimalDistance)
      {

        // 距離を正規化してスコアに追加
        curSpot->m_dScore += Prm.Spot_DistFromControllingPlayerScore *
                             (OptimalDistance-temp)/OptimalDistance;  
      }
    }
    
    // この点がこれまでのところ最高得点であるかどうかを確認
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
//	GetBestSupportingSpot関数
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
//	Render関数
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