//=============================================================================
//
// ナンバー処理 [number.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

///////////////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////////////////
#include "main.h"

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
class CNumber
{
public:
	CNumber();								// コンストラクタ
	~CNumber();								// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理
	static CNumber *Create(void);			// オブジェクト生成処理
	static HRESULT Load(void);				// ロード処理  
	static void Unload(void);				// アンロード処理
	void SetNumber(int nNumber);			// セット処理
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
											
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ共有
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;
	int m_nNumber;
};

#endif