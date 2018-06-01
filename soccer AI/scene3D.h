//=============================================================================
//
// オブジェクト2Dの処理 [scene3D.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

///////////////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////////////////
#include "scene.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		構造体定義
//
////////////////////////////////////////////////
class CScene3D : public CScene
{
public:
	CScene3D(int Priority);												// コンストラクタ
	~CScene3D();											// デストラクタ
	static CScene3D *Create(void);							// オブジェクト生成処理
	void Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// バインドテクスチャー処理
	void LoadTexture(LPCSTR FileName);						// テクスチャロード処理
	D3DXVECTOR3 GetPos(void);								// プレイヤーの座標を取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	bool m_bLoadTexture;									// テクスチャが読み込まれたかどうか

protected:
	D3DXMATRIX m_mtxWorld;									// ワールド変数
	D3DXMATRIX m_mtxRot;									// 回転
	D3DXMATRIX m_mtxTranslation;							// 平行
	D3DXMATRIX m_mtxScall;									// 拡大
	D3DXVECTOR3 m_Pos;										// 平行移動ベクトル
	D3DXVECTOR2 m_Tex;										// テクスチャ座標
	D3DXVECTOR3 m_Rot;										// 回転ベクトル
	D3DXVECTOR3 m_Scl;										// 拡大ベクトル
	D3DXVECTOR3 m_Size;										// 2Dオブジェクの幅、高さ
	
	D3DCOLOR m_color;
	D3DXVECTOR3 m_Move;
};

#endif