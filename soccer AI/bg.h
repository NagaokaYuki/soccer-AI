//=============================================================================
//
// プレイヤー処理 [polygon2D.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

///////////////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////////////////
#include "scene2D.h"

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
class CBg : public CScene
{
public:
	// 背景タイプ
	typedef enum
	{
		BG_TYPE_NONE = 0,
		BG_TYPE_TITLE,
		BG_TYPE_GAME,
		BG_TYPE_RESULT
	}BGTYPE;

	CBg(int Priority);										// コンストラクタ
	~CBg();													// デストラクタ
	static CBg *Create(BGTYPE BgType);						// オブジェクト生成処理
	void Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// バインドテクスチャー処理
	void LoadTexture(LPCSTR FileName);						// テクスチャロード処理

private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	bool m_bLoadTexture;									// テクスチャが読み込まれたかどうか
	D3DXVECTOR3 m_pos;										// 表示座標
	D3DXVECTOR2 m_size;										// 幅,高さ
	D3DXVECTOR2 m_tex;										// テクスチャ座標
	D3DCOLOR m_color;										
};

#endif