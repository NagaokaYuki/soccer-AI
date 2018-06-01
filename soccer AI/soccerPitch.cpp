/*=======================================================================

	サッカーピッチ [soccerPitch.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "soccerPitch.h"
#include "soccerBall.h"
#include "goal.h"
#include "region.h"
#include "transformations.h"
#include "geometry.h"
#include "soccerTeam.h"
#include "debugConsole.h"
#include "entityManager.h"
#include "paramLoader.h"
#include "playerBase.h"
#include "teamStates.h"
#include "frameCounter.h"

/////////////////////////////////////////////////////////
//
//	グローバル変数
//
/////////////////////////////////////////////////////////
const int NumRegionsHorizontal = 6; 
const int NumRegionsVertical   = 3;

/////////////////////////////////////////////////////////
//
//	コンストラクタ(ctr)
//
/////////////////////////////////////////////////////////
SoccerPitch::SoccerPitch(int cx, int cy):m_cxClient(cx),
                                         m_cyClient(cy),
                                         m_bPaused(false),
                                         m_bGoalKeeperHasBall(false),
                                         m_Regions(NumRegionsHorizontal*NumRegionsVertical),
                                         m_bGameOn(true)
{
  // プレイエリアを定義
  m_pPlayingArea = new Region(20, 20, cx-20, cy-20);

  // コートを作成
  CreateRegions(PlayingArea()->Width() / (double)NumRegionsHorizontal,
                PlayingArea()->Height() / (double)NumRegionsVertical);
  
  // ゴール生成
  m_pRedGoal  = new Goal(Vector2D( m_pPlayingArea->Left(), (cy-Prm.GoalWidth)/2),
                          Vector2D(m_pPlayingArea->Left(), cy - (cy-Prm.GoalWidth)/2),
                          Vector2D(1,0));
  // 青いゴール生成
  m_pBlueGoal = new Goal( Vector2D( m_pPlayingArea->Right(), (cy-Prm.GoalWidth)/2),
                          Vector2D(m_pPlayingArea->Right(), cy - (cy-Prm.GoalWidth)/2),
                          Vector2D(-1,0));


  // サッカーボール生成
  m_pBall = new SoccerBall(Vector2D((double)m_cxClient/2.0, (double)m_cyClient/2.0),
                           Prm.BallSize,
                           Prm.BallMass,
                           m_vecWalls);

  
  // チーム生成 
  m_pRedTeam  = new SoccerTeam(m_pRedGoal, m_pBlueGoal, this, SoccerTeam::red);
  m_pBlueTeam = new SoccerTeam(m_pBlueGoal, m_pRedGoal, this, SoccerTeam::blue);

  // 各チームの相手を確認
  m_pRedTeam->SetOpponents(m_pBlueTeam);
  m_pBlueTeam->SetOpponents(m_pRedTeam); 

  // 壁を生成
  Vector2D TopLeft(m_pPlayingArea->Left(), m_pPlayingArea->Top());                                        
  Vector2D TopRight(m_pPlayingArea->Right(), m_pPlayingArea->Top());
  Vector2D BottomRight(m_pPlayingArea->Right(), m_pPlayingArea->Bottom());
  Vector2D BottomLeft(m_pPlayingArea->Left(), m_pPlayingArea->Bottom());
                                      
  m_vecWalls.push_back(Wall2D(BottomLeft, m_pRedGoal->RightPost()));
  m_vecWalls.push_back(Wall2D(m_pRedGoal->LeftPost(), TopLeft));
  m_vecWalls.push_back(Wall2D(TopLeft, TopRight));
  m_vecWalls.push_back(Wall2D(TopRight, m_pBlueGoal->LeftPost()));
  m_vecWalls.push_back(Wall2D(m_pBlueGoal->RightPost(), BottomRight));
  m_vecWalls.push_back(Wall2D(BottomRight, BottomLeft));

  ParamLoader* p = ParamLoader::Instance();
}

/////////////////////////////////////////////////////////
//
//	デストラクタ(dtr)
//
/////////////////////////////////////////////////////////
SoccerPitch::~SoccerPitch()
{
  delete m_pBall;

  delete m_pRedTeam;
  delete m_pBlueTeam;

  delete m_pRedGoal;
  delete m_pBlueGoal;

  delete m_pPlayingArea;

  for (unsigned int i=0; i<m_Regions.size(); ++i)
  {
    delete m_Regions[i];
  }
}

/////////////////////////////////////////////////////////
//
//	Update関数
//
/////////////////////////////////////////////////////////
void SoccerPitch::Update()
{
  if (m_bPaused) return;

  static int tick = 0;

  // サッカーボール更新処理
  m_pBall->Update();

  // チーム更新処理
  m_pRedTeam->Update();
  m_pBlueTeam->Update();

  // キックオフのためにピッチをリセット
  if (m_pBlueGoal->Scored(m_pBall) || m_pRedGoal->Scored(m_pBall))
  {
    m_bGameOn = false;
    
    // ボールをリセット                                                     
    m_pBall->PlaceAtPosition(Vector2D((double)m_cxClient/2.0, (double)m_cyClient/2.0));

    // キックオフのために準備
    m_pRedTeam->GetFSM()->ChangeState(PrepareForKickOff::Instance());
    m_pBlueTeam->GetFSM()->ChangeState(PrepareForKickOff::Instance());
  }
}

/////////////////////////////////////////////////////////
//
//	CreateRegions関数
//
/////////////////////////////////////////////////////////
void SoccerPitch::CreateRegions(double width, double height)
{  
  // ベクトルへのインデックス
  int idx = m_Regions.size()-1;
    
  for (int col=0; col<NumRegionsHorizontal; ++col)
  {
    for (int row=0; row<NumRegionsVertical; ++row)
    {
      m_Regions[idx--] = new Region(PlayingArea()->Left()+col*width,
                                   PlayingArea()->Top()+row*height,
                                   PlayingArea()->Left()+(col+1)*width,
                                   PlayingArea()->Top()+(row+1)*height,
                                   idx);
    }
  }
}


/////////////////////////////////////////////////////////
//
//	Render関数
//
/////////////////////////////////////////////////////////
bool SoccerPitch::Render()
{
  // 芝を描画
  gdi->DarkGreenPen();
  gdi->DarkGreenBrush();
  gdi->Rect(0,0,m_cxClient, m_cyClient);

  // ピッチを描画
  if (Prm.bRegions)
  {   
    for (unsigned int r=0; r<m_Regions.size(); ++r)
    {
      m_Regions[r]->Render(true);
    }
  }
  
  // ゴールを描画
  gdi->HollowBrush();
  gdi->RedPen();
  gdi->Rect(m_pPlayingArea->Left(), (m_cyClient-Prm.GoalWidth)/2, m_pPlayingArea->Left()+40, m_cyClient - (m_cyClient-Prm.GoalWidth)/2);

  gdi->BluePen();
  gdi->Rect(m_pPlayingArea->Right(), (m_cyClient-Prm.GoalWidth)/2, m_pPlayingArea->Right()-40, m_cyClient - (m_cyClient-Prm.GoalWidth)/2);
  
  // ラインを描画
  gdi->WhitePen();
  gdi->Circle(m_pPlayingArea->Center(), m_pPlayingArea->Width() * 0.125);
  gdi->Line(m_pPlayingArea->Center().x, m_pPlayingArea->Top(), m_pPlayingArea->Center().x, m_pPlayingArea->Bottom());
  gdi->WhiteBrush();
  gdi->Circle(m_pPlayingArea->Center(), 2.0);


  // ボールを描画
  gdi->WhitePen();
  gdi->WhiteBrush();
  m_pBall->Render();
  
  // チームを描画
  m_pRedTeam->Render();
  m_pBlueTeam->Render(); 

  // 壁を描画
  gdi->WhitePen();
  for (unsigned int w=0; w<m_vecWalls.size(); ++w)
  {
    m_vecWalls[w].Render();
  }

  gdi->TextColor(Cgdi::red);
  gdi->TextAtPos((m_cxClient/2)-50, m_cyClient-18, "Red: " + ttos(m_pBlueGoal->NumGoalsScored()));

  gdi->TextColor(Cgdi::blue);
  gdi->TextAtPos((m_cxClient/2)+10, m_cyClient-18, "Blue: " + ttos(m_pRedGoal->NumGoalsScored()));

  return true;  
}







