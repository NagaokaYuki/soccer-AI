//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "polygon3D.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define POLYGON3D_TEXTURENAME "data/TEXTURE/kobeni3.jpg"	// jpg,tga,bmp,dds	ddsは圧縮した状態でも使用可
#define POLYGON3D_POS_X		(0.0f)							// ポリゴン3D表示座標X
#define POLYGON3D_POS_Y		(1.0f)							// ポリゴン3D表示座標Y
#define POLYGON3D_POS_Z		(0.0f)							// ポリゴン3D表示座標Z

#define POLYGON3D_WIDTH		(15.0f)							// ポリゴン3Dの幅
#define POLYGON3D_HEIGHT	(15.0f)							// ポリゴン3Dの高さ

#define POLYGON3D_TEXTURE_X	(0.0f)							// ポリゴンテクスチャ座標U
#define POLYGON3D_TEXTURE_Y	(0.0f)							// ポリゴンテクスチャ座標V

#define POLYGON_ROT_X		(0.0f)							// ポリゴン回転座標X
#define POLYGON_ROT_Y		(0.0f)							// ポリゴン回転座標Y
#define POLYGON_ROT_Z		(0.0f)							// ポリゴン回転座標Z

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
CPolygon3D *CPolygon3D::Create(void)
{
	CPolygon3D *pPolygon3D;
	pPolygon3D = new CPolygon3D(17);
	pPolygon3D->Init();
	pPolygon3D->LoadTexture(POLYGON3D_TEXTURENAME);
	return pPolygon3D;
}
///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CPolygon3D::CPolygon3D(int Priority):CScene3D(Priority)
{
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CPolygon3D::~CPolygon3D()
{
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CPolygon3D::Init(void)
{
	m_Pos = D3DXVECTOR3(POLYGON3D_POS_X, POLYGON3D_POS_Y, POLYGON3D_POS_Z);
	m_Size = D3DXVECTOR3(POLYGON3D_WIDTH, 0.0f, POLYGON3D_HEIGHT);
	m_Tex= D3DXVECTOR2(POLYGON3D_TEXTURE_X, POLYGON3D_TEXTURE_Y);
	m_Rot = D3DXVECTOR3(POLYGON_ROT_X, POLYGON_ROT_Y, POLYGON_ROT_Z);
	CScene3D::Init();
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CPolygon3D::Uninit(void)
{
	CScene3D::Uninit();
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CPolygon3D::Update(void)
{
	CScene3D::Update();

}
///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CPolygon3D::Draw(void)
{
	CScene3D::Draw();
}

