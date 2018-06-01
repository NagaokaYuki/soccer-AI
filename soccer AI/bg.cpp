//=============================================================================
//
// プレイヤー処理 [bg.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "bg.h"
#include "renderer.h"
#include "scene.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define BG_TITLETEXTURE "data/TEXTURE/shoot.jpg"
#define BG_GAMETEXTURE	"data/TEXTURE/soccercoat.png"
#define BG_RESULTTEXTURE "data/TEXTURE/result000.jpg"
#define BG_POS_X		(0.0f)						// ポリゴン2D表示座標X
#define BG_POS_Y		(0.0f)						// ポリゴン2D表示座標Y
#define BG_POS_Z		(0.0f)						// ポリゴン2D表示座標Z
#define BG_WIDTH		(SCREEN_WIDTH)				// ポリゴン2Dの幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				// ポリゴン2Dの高さ
#define BG_TEXTURE_X	(0.0f)						// ポリゴンテクスチャ座標U
#define BG_TEXTURE_Y	(0.0f)						// ポリゴンテクスチャ座標V

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

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CBg *CBg::Create(BGTYPE BgType)
{
	CBg *pBg;
	pBg = new CBg(8);
	pBg->Init();

	switch(BgType)
	{
	case BG_TYPE_TITLE:
		pBg->LoadTexture(BG_TITLETEXTURE);
		break;

	case BG_TYPE_GAME:
		pBg->LoadTexture(BG_GAMETEXTURE);
		break;

	case BG_TYPE_RESULT:
		pBg->LoadTexture(BG_RESULTTEXTURE);
		break;
	}
	return pBg;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CBg::CBg(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CBg::~CBg()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CBg::Init(void)
{
	m_pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, BG_POS_Z);
	m_size = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);
	m_color = 255;
	m_tex= D3DXVECTOR2(BG_TEXTURE_X, BG_TEXTURE_Y);

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点フォーマットの設定
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			// 作成したい頂点バッファのサイズ,最速で書く、メモリ管理方法(デバイスにおまかせ)
	{
		MessageBox(NULL,"頂点の作成に失敗しました。","エラー", MB_OK);
	}

	// 頂点情報を設定
	VERTEX_2D* pVtx;				// 仮想アドレス用ポインタ

	// 頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(2D座標・右周り)
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	// rhwの設定(必ず1.0f)
	pVtx[1].rhw = 1.0f;
	pVtx[0].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[1].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[2].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[3].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);

	// UVの座標設定
	pVtx[0].tex = D3DXVECTOR2(m_tex.x, m_tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_tex.x, m_tex.y + TEXTURE_V);
	pVtx[3].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y + TEXTURE_V);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CBg::Uninit(void)
{
	if(m_pTexture != NULL && m_bLoadTexture == true)
	{
		m_pTexture ->Release();
		m_pTexture = NULL;
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff ->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CBg::Update(void)
{
	
}
///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CBg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点バッファをデータストリームに設定
	// デバイスの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if(pDevice != NULL)
	{
		pDevice->SetTexture(0, m_pTexture);
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,			// プリミティブの種類 
							0,								// オフセット(どこから描きはじめるか)
							NUM_POLYGON						// プリミティブの数
							);
}

///////////////////////////////////////////////
//
//		テクスチャーバインド処理
//
////////////////////////////////////////////////
void CBg::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		テクスチャーロード処理
//
////////////////////////////////////////////////
void CBg::LoadTexture(LPCSTR FileName)
{
	if(m_pTexture == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		FileName, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"テクスチャの読み込みに失敗しました。","エラー", MB_OK);
		}
		m_bLoadTexture = true;
	}
}