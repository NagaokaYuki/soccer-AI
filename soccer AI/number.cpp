//=============================================================================
//
// ナンバー処理 [number.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define NUMBER_TEXTURENAME "data/TEXTURE/number000.png"		// jpg,tga,bmp,dds	ddsは圧縮した状態でも使用可
#define TEXTURE_WIDTH		(0.1f)							// テクスチャの幅
#define TEXTURE_HEIGHT		(1.0f)							// テクスチャの高さ

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
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;
int g_Score;

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CNumber *CNumber::Create(void)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init();
	return pNumber;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CNumber::~CNumber()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
HRESULT CNumber::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点フォーマットの設定
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			// 作成したい頂点バッファのサイズ,最速で書く、メモリ管理方法(デバイスにおまかせ)
	{
		MessageBox(NULL,"頂点の作成に失敗しました。","エラー", MB_OK);
		return E_FAIL;
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	// UVの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
	return S_OK;
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CNumber::Uninit(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture ->Release();
		m_pTexture = NULL;
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff ->Release();
		m_pVtxBuff = NULL;
	}
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CNumber::Update(void)
{
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除

}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点バッファをデータストリームに設定
	// デバイスの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, m_pTexture);
	
	
	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,			// プリミティブの種類 
								0,								// オフセット(どこから描きはじめるか)
								NUM_POLYGON						// プリミティブの数
								);
}

///////////////////////////////////////////////
//
//		ロード処理
//
////////////////////////////////////////////////
HRESULT CNumber::Load(void)
{
	if(m_pTexture == NULL)
	{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		NUMBER_TEXTURENAME, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"テクスチャの初期化に失敗しました。","エラー", MB_OK);
		}
	}

	return S_OK;
}

///////////////////////////////////////////////
//
//		アンロード処理
//
////////////////////////////////////////////////
void CNumber::Unload(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

///////////////////////////////////////////////
//
//		セット処理
//
////////////////////////////////////////////////
void CNumber::SetNumber(int nNumber)
{
	// 頂点情報を設定
	VERTEX_2D* pVtx;				// 仮想アドレス用ポインタ

	// 頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// UVの座標設定
	pVtx[0].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH + TEXTURE_WIDTH, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH, TEXTURE_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH + TEXTURE_WIDTH, TEXTURE_HEIGHT);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		セット処理
//
////////////////////////////////////////////////
void CNumber::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

///////////////////////////////////////////////
//
//		セット処理
//
////////////////////////////////////////////////
void CNumber::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
}
