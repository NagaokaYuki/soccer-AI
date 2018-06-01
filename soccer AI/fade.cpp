//=============================================================================
//
// プレイヤー処理 [fade.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define FADE_TEXTURE "data/TEXTURE/fade.png"
#define FADE_POS_X		(0.0f)						// フェードの表示位置X
#define FADE_POS_Y		(0.0f)						// フェードの表示位置Y
#define FADE_POS_Z		(0.0f)						// フェードの表示位置Z
#define FADE_WIDTH		(SCREEN_WIDTH)				// フェードの幅
#define FADE_HEIGHT		(SCREEN_HEIGHT)				// フェードの高さ
#define FADE_RATE		(0.01f)						// フェード係数、フェードの速度にかかわる
#define FADE_TEXTURE_X	(0.0f)						// フェードテクスチャ座標U
#define FADE_TEXTURE_Y	(0.0f)						// フェードテクスチャ座標V

///////////////////////////////////////////////
//
//		プロトタイプ宣言
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		グローバル変数
//
////////////////////////////////////////////////
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CFade *CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade;
	pFade->Init();
	pFade->BindTexture(m_pTexture);
	return pFade;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CFade::CFade()
{
	m_fade = FADE_NONE;
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CFade::~CFade()
{
}

///////////////////////////////////////////////
//
//		ロード処理
//
////////////////////////////////////////////////
void CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if(m_pTexture == NULL)
	{
		if(FAILED(D3DXCreateTextureFromFile(pDevice,
		FADE_TEXTURE,
		&m_pTexture)))
		{		
			MessageBox(NULL,"テクスチャの初期化に失敗しました。","エラー", MB_OK);
		}
	}
}

///////////////////////////////////////////////
//
//		アンロード処理
//
////////////////////////////////////////////////
void CFade::UnLoad(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CFade::Init(void)
{
	m_pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, FADE_POS_Z);
	m_size = D3DXVECTOR2(FADE_WIDTH, FADE_HEIGHT);
	m_tex= D3DXVECTOR2(FADE_TEXTURE_X, FADE_TEXTURE_Y);
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_fade = FADE_IN;
	CScene2D::BindTexture(m_pTexture);
	CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	CScene2D::Init();
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CFade::Uninit(void)
{
	CScene2D::Uninit();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CFade::Update(void)
{
	// NONEなら更新しない
	if(m_fade == FADE_NONE)
	{
		return;
	}

	// INなら段々薄くしていく
	if(m_fade == FADE_IN)
	{
		m_colorFade.a -= FADE_RATE;
		if(m_colorFade.a <= 0.0f)
		{
			m_colorFade = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	// OUTなら段々濃くしていく
	else if(m_fade == FADE_OUT)
	{
		m_colorFade.a += FADE_RATE;
		if(m_colorFade.a >= 1.0f)
		{
			m_colorFade.a = 1.0f;
			m_fade = FADE_IN;
			//SetMode(g_modeNext);
		}
	}

	CScene2D::SetColor(m_colorFade);

	CScene2D::Update();

}
///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CFade::Draw(void)
{
	CScene2D::Draw();
}

////////////////////////////////////
//
//		フェードセット処理
//
////////////////////////////////////
void CFade:: SetFade(FADE fade/*, MODE modeNext*/)
{
	m_fade = fade;
	/*m_modeNext = modeNext;*/
}

////////////////////////////////////
//
//		フェード情報取得処理
//
////////////////////////////////////
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}