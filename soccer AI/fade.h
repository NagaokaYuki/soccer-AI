//=============================================================================
//
// プレイヤー処理 [fade.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

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
class CFade : public CScene2D
{
public:
	typedef enum
	{
		FADE_NONE = 0,							// なし
		FADE_IN,								// フェードイン
		FADE_OUT,								// フェードアウト
		FADE_MAX								// フェードの最大数
	}FADE;										// 型名

	CFade();									// コンストラクタ
	~CFade();									// デストラクタ
	static void Load(void);
	static void UnLoad(void);
	static CFade *Create(void);					// オブジェクト生成処理
	void Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理
	void SetFade(FADE fade/*, MODE modeNext*/);	// フェード設定関数
	FADE GetFade(void);							// フェード取得関数

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャのポインタ
	FADE m_fade;								// 列挙型の変数
	D3DXCOLOR m_colorFade;						// カラーの変数
};

#endif