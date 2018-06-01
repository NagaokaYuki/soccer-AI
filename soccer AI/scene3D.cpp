//=============================================================================
//
// シーン2D処理 [scene3D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "scene3D.h"
#include "renderer.h"
#include "input.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define TEXTURE3D_U				(1.0f)
#define TEXTURE3D_V				(1.0f)
							
///////////////////////////////////////////////
//
//		プロトタイプ宣言
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		グローバル変数
//
///////////////////////////////////////////////

///////////////////////////////////////////////
//
//		CScene3Dのコンストラクタ
//
////////////////////////////////////////////////
CScene3D *CScene3D::Create(void)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D(3);
	pScene3D->Init();

	return pScene3D;
}

///////////////////////////////////////////////
//
//		CScene3Dのコンストラクタ
//
////////////////////////////////////////////////
CScene3D::CScene3D(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}

///////////////////////////////////////////////
//
//		CScene3Dのデストラクタ
//
////////////////////////////////////////////////
CScene3D::~CScene3D()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CScene3D::Init(void)
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
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Size.x, m_Pos.y + m_Size.y, m_Pos.z + m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, m_Pos.z + m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_Size.x, m_Pos.y - m_Size.y, m_Pos.z - m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y - m_Size.y, m_Pos.z - m_Size.z);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_Tex.x + TEXTURE3D_U, m_Tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + TEXTURE3D_V);
	pVtx[3].tex = D3DXVECTOR2(m_Tex.x + TEXTURE3D_U, m_Tex.y + TEXTURE3D_V);

	// ベクトル
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
void CScene3D::Uninit(void)
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
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CScene3D::Update(void)
{
	// 頂点情報を設定
	VERTEX_3D* pVtx;				// 仮想アドレス用ポインタ
	// 頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定(3D座標・右回り)
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x -m_Size.x,   m_Pos.y, m_Pos.z + m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Size.x,  m_Pos.y + m_Size.y,  m_Pos.z + m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + m_Size.x,  m_Pos.y, m_Pos.z - m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x,  m_Pos.y + m_Size.y, m_Pos.z - m_Size.z);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CScene3D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	/*----------------------------ワールド座標変換--------------------------------*/
	D3DXMatrixIdentity(&m_mtxWorld);													// 単位行列を作る関数
																						   
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);				// ワールドマトリックス = 回転行列
																						   
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);							// 掛け算の仕方
																						   
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);									// デバイスに設定

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
}

///////////////////////////////////////////////
//
//		テクスチャーバインド処理
//
////////////////////////////////////////////////
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		テクスチャーロード処理
//
////////////////////////////////////////////////
void CScene3D::LoadTexture(LPCSTR FileName)
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

