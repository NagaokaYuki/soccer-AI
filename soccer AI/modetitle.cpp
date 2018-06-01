//=============================================================================
//
// マネージャーの処理 [modetitle.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "mode.h"
#include "modetitle.h"
#include "manager.h"
#include "renderer.h"
#include "modegame.h"
#include "input.h"
#include "bg.h"

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
CBg *CModeTitle::m_pBg = NULL;

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CModeTitle::CModeTitle()
{
	
}
///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
CModeTitle::~CModeTitle()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CModeTitle::Init(void)
{
	m_pBg = CBg::Create(CBg::BG_TYPE_TITLE);
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CModeTitle::Uninit(void)
{
	// オブジェクトの破棄
	CScene::ReleaseAll();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CModeTitle::Update(void)
{
	// デバイス取得
	CInputKeyboard *pInput = CManager::GetKeyboard();

	if(pInput->GetKeyTrigger(DIK_RETURN))
	{
		CManager::SetMode(new CModeGame);
	}
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CModeTitle::Draw(void)
{
	m_pBg->Draw();
}
