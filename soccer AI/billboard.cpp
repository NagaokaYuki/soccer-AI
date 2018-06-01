//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "modegame.h"
#include "manager.h"
#include "billboard.h"
#include "renderer.h"
#include "camera.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define TEXTUREBILBOARD_U				(1.0f)
#define TEXTUREBILBOARD_V				(1.0f)

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
CBillboard *CBillboard::Create(void)
{
	CBillboard *pBillboard;
	pBillboard = new CBillboard(3);
	pBillboard->Init();
	return pBillboard;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CBillboard::CBillboard(int Priority):CScene(Priority)
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
CBillboard::~CBillboard()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CBillboard::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// 頂点フォーマットの設定
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			//作成したい頂点バッファのサイズ,最速で書く、メモリ管理方法(デバイスにおまかせ)
	{
		MessageBox(NULL,"頂点の作成に失敗しました。","エラー", MB_OK);
	}

	// 頂点情報を設定
	VERTEX_3D* pVtx;				//仮想アドレス用ポインタ

	// 頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(3D座標・右回り)
	pVtx[0].pos = D3DXVECTOR3(0.0f - m_Size.x * 0.5f, 0.0f + m_Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + m_Size.x * 0.5f, 0.0f + m_Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - m_Size.x * 0.5f, 0.0f - m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + m_Size.x * 0.5f, 0.0f - m_Size.y, 0.0f);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ベクトル
	pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CBillboard::Uninit(void)
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
	CScene::Release();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CBillboard::Update(void)
{
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CBillboard::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxView = CModeGame::GetCamera()->ViewCamera();
	D3DXMATRIX mtxViewInverse;

	//ライト無効
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );
	
	//逆行列を作る
	D3DXMatrixInverse(&mtxViewInverse, NULL, &mtxView);
	 
	mtxViewInverse._41 = 0.0f;
	mtxViewInverse._42 = 0.0f;
	mtxViewInverse._43 = 0.0f;

																																	   
	D3DXMatrixMultiply(&m_mtxWorld,  &mtxViewInverse, &m_mtxWorld);							// 掛け算の仕方

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxTranslation, &m_mtxWorld);						// 掛け算の仕方
																						   
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);										// デバイスに設定

	//αテスト有効
	pDevice -> SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );	
	pDevice -> SetRenderState( D3DRS_ALPHAREF , 1 );
	pDevice -> SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点バッファをデータストリームに設定
	// デバイスの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	
	// ポリゴンの描画
	// プリミティブ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);		//(プリミティブの種類, オフセット(頂点数), プリミティブ数)

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);										//ライトオフ
}

///////////////////////////////////////////////
//
//		テクスチャーバインド処理
//
////////////////////////////////////////////////
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		テクスチャーロード処理
//
////////////////////////////////////////////////
void CBillboard::LoadTexture(LPCSTR FileName)
{
	if(m_pTexture == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		FileName, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"テクスチャの初期化に失敗しました。","エラー", MB_OK);
		}
		m_bLoadTexture = true;
	}
}
