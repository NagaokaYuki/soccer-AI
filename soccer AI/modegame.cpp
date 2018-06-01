//=============================================================================
//
// マネージャーの処理 [modegame.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "modegame.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "light.h"
#include "camera.h"
#include "bg.h"
#include "input.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "billboard.h"
#include "debug.h"
#include "moderesult.h"
#include "number.h"
#include "score.h"
#include "time.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		静的メンバ変数
//
////////////////////////////////////////////////
CCamera *CModeGame::m_pCamera = NULL;						// カメラの静的メンバ変数
CLight *CModeGame::m_pLight = NULL;							// ライトクラスの静的メンバ変数
CPolygon2D *CModeGame::m_pPolygon2D = NULL;					// ポリゴン２Dの静的メンバ変数
bool CModeGame::m_bUse = NULL;								// フラグの静的メンバ変数
CScore *CModeGame::m_pScore = NULL;							// スコアの静的メンバ変数
CBg *CModeGame::m_pBg = NULL;								// 背景クラスの静的メンバ変数
CTime *CModeGame::m_pTime = NULL;							// タイムクラスの静的メンバ変数

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CModeGame::Init(void)
{
	// デバイス取得
	CInputKeyboard *pInput = CManager::GetKeyboard();

	// ライトの生成
	m_pLight = new CLight;
	m_pLight->Init();

	// カメラの生成
	m_pCamera = new CCamera;
	m_pCamera->Init();

	// 背景の生成
	m_pBg = CBg::Create(CBg::BG_TYPE_GAME);

	// スコアの生成
	m_pScore = CScore::Create();

	// タイムの生成
	m_pTime = CTime::Create();

	// スコアのロード
	CNumber::Load();

}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CModeGame::Uninit(void)
{
	// オブジェクトの破棄
	CScene::ReleaseAll();
			
	// ライトオブジェクトの破棄
	if(m_pLight != NULL)
	{
		m_pLight->Uninit();
		m_pLight = NULL;
	}

	// カメラオブジェクトの破棄
	if(m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		m_pCamera = NULL;
	}

	// ナンバーアンロード
	CNumber::Unload();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CModeGame::Update(void)
{
	// デバイス取得
	CInputKeyboard *pInput = CManager::GetKeyboard();

	// カメラ更新処理
	m_pCamera->Update();					
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CModeGame::Draw(void)
{
	m_pCamera->Set();						// カメラセット処理
	CScene::DrawAll();
	//m_pBg->Draw();
}

//////////////////////////////////////////////
//
//		カメラ取得処理
//
//////////////////////////////////////////////
CCamera *CModeGame::GetCamera(void)
{
	return m_pCamera;
}

//////////////////////////////////////////////
//
//		球体と球体の当たり判定
//
//////////////////////////////////////////////
bool CModeGame::SphereCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	/*--------------------------------------------------式------------------------------------------*/
	//		球1 ： 中心点の座標P1(x1, y1, z1), 半径r1
	//		球2 ： 中心点の座標P2(x2, y2, z2), 半径r2
	//		(x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2 <= (r1+r2)^2ならば衝突を起こしている
	if((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (radius1 + radius2) * (radius1 + radius2))
	{
		return true;
	}
	return false;
}
