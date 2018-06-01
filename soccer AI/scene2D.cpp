//=============================================================================
//
// シーン2D処理 [scene2D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////

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
//		CScene2Dのコンストラクタ
//
////////////////////////////////////////////////
CScene2D *CScene2D::Create(void)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D(4);
	pScene2D->Init();

	return pScene2D;
}

///////////////////////////////////////////////
//
//		CScene2Dのコンストラクタ
//
////////////////////////////////////////////////
CScene2D::CScene2D(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}

///////////////////////////////////////////////
//
//		CScene2Dのデストラクタ
//
////////////////////////////////////////////////
CScene2D::~CScene2D()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CScene2D::Init(void)
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
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void CScene2D::Uninit(void)
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
void CScene2D::Update(void)
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

	// UVの座標設定
	pVtx[0].tex = D3DXVECTOR2(m_tex.x, m_tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_tex.x, m_tex.y + TEXTURE_V);
	pVtx[3].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y + TEXTURE_V);

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CScene2D::Draw(void)
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
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		テクスチャーロード処理
//
////////////////////////////////////////////////
void CScene2D::LoadTexture(LPCSTR FileName)
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

///////////////////////////////////////////////
//
//		テクスチャーアンロード処理
//
////////////////////////////////////////////////
void CScene2D::UnLoadTexture(void)
{

}
///////////////////////////////////////////////
//
//		プレイヤーの座標を取得処理
//
////////////////////////////////////////////////
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

///////////////////////////////////////////////
//
//		カラーの設定		
//
////////////////////////////////////////////////
void CScene2D::SetColor(D3DCOLOR color)
{
	// 頂点情報を設定
	VERTEX_2D* pVtx;				// 仮想アドレス用ポインタ
	// 頂点バッファをロックして仮想アドレスを取得する
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー(0～255の整数値)
	pVtx[0].color = color;
	pVtx[1].color = color;
	pVtx[2].color = color;
	pVtx[3].color = color;

	m_pVtxBuff->Unlock();				// 頂点バッファのロックを解除
}

///////////////////////////////////////////////
//
//		アニメーションの設定		
//
////////////////////////////////////////////////
void CScene2D :: SetAnimation( D3DXVECTOR2 pos, D3DXVECTOR2 size )
{
	// 頂点情報を設定
	VERTEX_2D* pVtx = NULL;			// 仮想アドレス用ポインタ

	// 頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0, 0, ( void** )&pVtx, 0 );

	// 頂点データにUVデータの追加
	pVtx[0].tex = D3DXVECTOR2(pos.x, pos.y );							// UV値左上
	pVtx[1].tex = D3DXVECTOR2(pos.x + size.x, pos.y );					// UV値右上
	pVtx[2].tex = D3DXVECTOR2(pos.x, pos.y + size.y );					// UV値左下
	pVtx[3].tex = D3DXVECTOR2(pos.x + size.x, pos.y + size.y );			// UV値右下

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock( );
}

