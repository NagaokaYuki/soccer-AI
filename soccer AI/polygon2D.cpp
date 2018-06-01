//=============================================================================
//
// プレイヤー処理 [polygon2D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "polygon2D.h"
#include "renderer.h"
#include "scene.h"
#include "billboard.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define LIFEGAGE_TEXTURE "data/TEXTURE/lifegage.png"
#define POLYGON2D_POS_X		(0.0f)						// ポリゴン2D表示座標X
#define POLYGON2D_POS_Y		(0.0f)						// ポリゴン2D表示座標Y
#define POLYGON2D_POS_Z		(0.0f)						// ポリゴン2D表示座標Z
#define POLYGON2D_WIDTH		(250.0f)						// ポリゴン2Dの幅
#define POLYGON2D_HEIGHT	(50.0f)						// ポリゴン2Dの高さ
#define POLYGON2D_TEXTURE_X	(0.0f)						// ポリゴンテクスチャ座標U
#define POLYGON2D_TEXTURE_Y	(0.0f)						// ポリゴンテクスチャ座標V

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
CPolygon2D *CPolygon2D::Create(void)
{
	CPolygon2D *pPolygon2D;
	pPolygon2D = new CPolygon2D(16);
	pPolygon2D->Init();
	pPolygon2D->LoadTexture(LIFEGAGE_TEXTURE);
	return pPolygon2D;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CPolygon2D::CPolygon2D(int Priority):CScene2D(Priority)
{
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CPolygon2D::~CPolygon2D()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CPolygon2D::Init(void)
{
	m_pos = D3DXVECTOR3(POLYGON2D_POS_X, POLYGON2D_POS_Y, POLYGON2D_POS_Z);
	m_size = D3DXVECTOR2(POLYGON2D_WIDTH, POLYGON2D_HEIGHT);
	m_tex= D3DXVECTOR2(POLYGON2D_TEXTURE_X, POLYGON2D_TEXTURE_Y);
	CScene2D::Init();
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CPolygon2D::Uninit(void)
{
	CScene2D::Uninit();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CPolygon2D::Update(void)
{
	CScene2D::Update();

}
///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CPolygon2D::Draw(void)
{
	CScene2D::Draw();
}

