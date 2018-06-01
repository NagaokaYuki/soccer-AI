/*=======================================================================

	サポートスポットカルキュレータ [supportSpotCalculator.h]

												長岡佑樹

=========================================================================*/
#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include "region.h"
#include "vector2D.h"
#include "cgdi.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class PlayerBase;
class Goal;
class SoccerBall;
class SoccerTeam;
class Regulator;

/////////////////////////////////////////////////////////
//
//	SupportSpotCalculatorクラス
//
/////////////////////////////////////////////////////////
class SupportSpotCalculator
{
private:
  
  // 各スポットの値と位置を保持するデータ構造
  struct SupportSpot
  {
	  Vector2D  m_vPos;		// 座標位置変数
	  double    m_dScore;		// スコア変数
	  SupportSpot(Vector2D pos, double value):m_vPos(pos),
                                        m_dScore(value)
	  {}
  };

private:
	SoccerTeam*               m_pTeam;
	std::vector<SupportSpot>  m_Spots;
	
	// 最後の更新からの最高値点へのポインタ
	SupportSpot*              m_pBestSupportingSpot;

	// スポットが計算される頻度を調整
	Regulator*                m_pRegulator;

public:
	SupportSpotCalculator(int numX,
                        int numY,
                        SoccerTeam* team);
	~SupportSpotCalculator();

	// スコアが高いほど、サークルは大きくなる
	void       Render()const;

	// 各可能なスポットを反復し、そのスコアを計算する
	Vector2D  DetermineBestSupportingPosition();
	
	// 最高の支持点があればそれを返し、
	// 計算されていない場合は、DetermineBestSupportingPositionを呼び出し
	// 結果を返す
	Vector2D  GetBestSupportingSpot();
};


#endif