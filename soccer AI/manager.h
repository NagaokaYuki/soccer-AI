//=============================================================================
//
// マネージャーの処理 [manager.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CModeTitle;
class CMode;
class CDebug;
class CSound;
///////////////////////////////////////////////
//
//		定数定義
//
////////////////////////////////////////////////
#define POLYGON_WIDTH			(100.0f)

///////////////////////////////////////////////
//
//		マネージャークラス
//
////////////////////////////////////////////////
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);				// 初期化処理
	void Uninit(void);														// 終了処理
	void Update(void);														// 更新処理
	void Draw(void);														// 描画処理																		   
	static CInputKeyboard *GetKeyboard(void);								// キーボードの情報取得処理
	static CInputMouse *GetMouse(void){return m_pInputMouse;}				// マウスの情報取得処理
	static void SetMode(CMode *mode);
private:																	
	static CInputKeyboard *m_pInputKeyboard;								// キーボードへのポインタ 
	static CInputMouse *m_pInputMouse;										// マウスへのポインタ 
	static CMode* m_pMode;
	static CModeTitle *m_pModeTitle;
	static CSound* m_pSound;
	//static CDebug *m_pDebug;
	D3DXVECTOR3 m_pos;
};

#endif
