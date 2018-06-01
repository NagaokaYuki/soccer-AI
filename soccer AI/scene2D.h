//=============================================================================
//
// オブジェクト2Dの処理 [scene2D.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

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
class CScene2D : public CScene
{
public:
	CScene2D(int Priority);												// コンストラクタ
	~CScene2D();											// デストラクタ
	static CScene2D *Create(void);							// オブジェクト生成処理
	void Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// バインドテクスチャー処理
	void LoadTexture(LPCSTR FileName);						// テクスチャロード処理
	void UnLoadTexture(void);
	D3DXVECTOR3 GetPos(void);								// プレイヤーの座標を取得
	void SetAnimation( D3DXVECTOR2 pos, D3DXVECTOR2 size );	// 
	void SetColor(D3DCOLOR color);							// 

private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	bool m_bLoadTexture;									// テクスチャが読み込まれたかどうか

protected:
	D3DXVECTOR3 m_pos;										// 表示座標
	D3DXVECTOR2 m_size;										// 幅,高さ
	D3DXVECTOR2 m_tex;										// テクスチャ座標
	D3DCOLOR m_color;										
};

#endif