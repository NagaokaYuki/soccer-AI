/*=======================================================================

	サッカーピッチ [sccerPitch.cpp]

												長岡佑樹

	2つのサッカーチーム、2つのゴール、
	プレーエリア、ボールなどのインスタンスを所有

=========================================================================*/
#ifndef SOCCERPITCH_H
#define SOCCERPITCH_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <vector>
#include <cassert>

#include "wall2D.h"
#include "vector2D.h"

class Region;
class Goal;
class SoccerTeam;
class SoccerBall;
class SoccerTeam;
class PlayerBase;


class SoccerPitch
{ 
public:

  SoccerBall*          m_pBall;

  SoccerTeam*          m_pRedTeam;
  SoccerTeam*          m_pBlueTeam;

  Goal*                m_pRedGoal;
  Goal*                m_pBlueGoal;
   
  // Wall2Dクラスのメンバ変数
  std::vector<Wall2D>  m_vecWalls;

  // Regionクラスのメンバ変数
  Region*              m_pPlayingArea;

  std::vector<Region*> m_Regions;

  // ゴールキーパーのフラグ
  bool                 m_bGoalKeeperHasBall;

  // ゲーム開始時のフラグ
  bool                 m_bGameOn;

  // ポーズフラグ
  bool                 m_bPaused;

  // クライアントウィンドウのサイズのローカルコピー
  int                  m_cxClient,
                       m_cyClient;  
  
  // プレイヤーが自分自身を位置づけるために利用する
  // 領域をインスタンス化します
  void CreateRegions(double width, double height);


public:

  SoccerPitch(int cxClient, int cyClient);

  ~SoccerPitch();

  void  Update();

  bool  Render();

  void  TogglePause(){m_bPaused = !m_bPaused;}
  bool  Paused()const{return m_bPaused;}

  int   cxClient()const{return m_cxClient;}
  int   cyClient()const{return m_cyClient;}

  bool  GoalKeeperHasBall()const{return m_bGoalKeeperHasBall;}
  void  SetGoalKeeperHasBall(bool b){m_bGoalKeeperHasBall = b;}

  const Region*const         PlayingArea()const{return m_pPlayingArea;}
  const std::vector<Wall2D>& Walls(){return m_vecWalls;}                      
  SoccerBall*const           Ball()const{return m_pBall;}

  const Region* const GetRegionFromIndex(int idx)                                
  {
    assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

    return m_Regions[idx];
  }

  bool  GameOn()const{return m_bGameOn;}
  void  SetGameOn(){m_bGameOn = true;}
  void  SetGameOff(){m_bGameOn = false;}

};

#endif