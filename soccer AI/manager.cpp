//=============================================================================
//
// マネージャーの処理 [manager.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "mode.h"
#include "input.h"
#include "modetitle.h"
#include "debug.h"
#include "sound.h"

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
CInputKeyboard *CManager::m_pInputKeyboard = NULL;			// キーボードの静的メンバ変数
CInputMouse *CManager::m_pInputMouse = NULL;				// キーボードの静的メンバ変数
CMode *CManager::m_pMode = NULL;
CModeTitle *CManager::m_pModeTitle = NULL;
CSound *CManager::m_pSound = NULL;

///////////////////////////////////////////////
//
//		CManagerのコンストラクタ
//
////////////////////////////////////////////////
CManager::CManager()
{
}

///////////////////////////////////////////////
//
//		CManagerのデストラクタ
//
////////////////////////////////////////////////
CManager::~CManager()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	CRenderer::Init(hWnd, TRUE);

	CDebug::Init();

	// キーボードの生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// キーボードの生成
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	m_pMode = new CModeTitle;
	m_pMode->Init();
	
	return S_OK;
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CManager::Uninit(void)
{
	// オブジェクトの破棄
	CScene::ReleaseAll();
	
	CRenderer::Uninit();
	
	// キーボードの破棄
	if(m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = NULL;
	}
	// キーボードの破棄
	if(m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		m_pInputMouse = NULL;
	}

	if(m_pMode != NULL)
	{
		m_pMode->Uninit();
		m_pMode = NULL;
	}
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CManager::Update(void)
{
	CScene::UpdateAll();
	CDebug::ClearText();
	m_pInputKeyboard->Update();				// キーボード更新処理
	m_pInputMouse->Update();
	m_pMode->Update();
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CManager::Draw(void)
{
	CRenderer::BeginDraw();
	m_pMode->Draw();
	CRenderer::EndDraw();
}

///////////////////////////////////////////////
//
//		セットモード処理
//
////////////////////////////////////////////////
void CManager::SetMode(CMode *mode)
{
	if(m_pMode != NULL)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}
	m_pMode = mode;

	if(m_pMode != NULL)
	{
		m_pMode->Init();
	}
}


//////////////////////////////////////////////
//
//		キーボード取得処理
//
//////////////////////////////////////////////
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

