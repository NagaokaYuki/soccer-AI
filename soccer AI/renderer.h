//=============================================================================
//
// レンダラー処理 [renderer.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

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
class CRenderer
{
public:
	CRenderer();							//コンストラクタ
	~CRenderer();							//デストラクタ
	
	static HRESULT Init(HWND hWnd, bool bWindow);	//初期化処理
	static void Uninit(void);						//終了処理
	static void Update(void);						//更新処理
	static void BeginDraw(void);					//描画処理
	static void EndDraw(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);		//デバイス取得処理

private:
	static LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
	LPD3DXFONT m_pFont;					// フォントへのポインタ
#endif
};
#endif