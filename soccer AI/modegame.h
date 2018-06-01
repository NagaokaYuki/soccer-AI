/*=======================================================================
//
//	ポリゴン表示処理 [modegame.h]
//
//												AT-12D-249
//												長岡佑樹
//
//=========================================================================*/

////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////
#ifndef _MODEGAME_H_
#define _MODEGAME_H_

#include "main.h"
#include "mode.h"

////////////////////////////////////
//
//	前方宣言
//
////////////////////////////////////
class CRenderer;				// レンダラークラス
class CInputKeyboard;			// キーボードクラス
class CCamera;					// カメラクラス
class CLight;					// ライトクラス
class CDebug;					// デバッグクラス
class CInputMouse;				// マウスクラス
class CPolygon2D;				// ポリゴン2Dクラス
class CScore;					// スコアクラス
class CBg;						// 背景クラス
class CTime;					// タイムクラス

////////////////////////////////////
//
//		ModeGameクラス
//
////////////////////////////////////
class CModeGame :public CMode
{	
public:
	void Init  (void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw  (void);		// 描画処理
	static bool SphereCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2);		// 球体あたり判定
	static CCamera *GetCamera(void);																	// カメラ取得処理
	static void SetUse(bool bUse){m_bUse = bUse;}														// フラグセット処理
	static CScore *GetScore(void){return m_pScore;}														// スコア取得処理
	static bool GetUse(void){return m_bUse;}
	
private:			
	static CLight *m_pLight;						// ライトの静的メンバ変数
	static CCamera *m_pCamera;						// カメラの静的メンバ変数
	static CPolygon2D *m_pPolygon2D;				// ポリゴン２Dの静的メンバ変数
	static bool m_bUse;								// フラグの静的メンバ変数
	static CScore *m_pScore;						// スコアの静的メンバ変数
	static CBg* m_pBg;								// 背景クラスの静的メンバ変数
	static CTime* m_pTime;							// タイムクラスの静的メンバ変数
};

///*------------------------------------------終了---------------------------------------------------*/
#endif