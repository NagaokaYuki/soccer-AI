/*=======================================================================

	ウィンドウ表示処理 [scene.h]

												AT-13C-245
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////
//
//
//	インクルードガード
//
//
/////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
/////////////////////////////////////////
//
//
//		マクロ定義
//
//
/////////////////////////////////////////
#define	MAX_SCENE				(100)				// オブジェクトの最大数
#define TEXTURE_U				(1.0f)
#define TEXTURE_V				(1.0f)
#define PRIORITY				(100)

/////////////////////////////////////////
//
//
//	シーンクラス
//
//
/////////////////////////////////////////
class CScene
{
public:
	// 種類の列挙型
	typedef enum
	{
		OBJTYPE_NONE = 0,					// 種類なし
		OBJTYPE_PLAYER,						// プレイヤー
		OBJTYPE_ENEMY,						// 敵
		OBJTYPE_BULLET,						// 弾
		OBJTYPE_ITEM,						// 爆発
		OBJTYPE_RETICLE,					// レティクル
		OBJTYPE_PARTICLE,					// パーティクル
		OBJTYPE_MAX							// 最大数
	}OBJTYPE;

	CScene(int Priority);								// シーンコンストラクタ
	~CScene();								// シーンデストラクタ
											   
	virtual void Init(void) = 0;			// シーン初期化処理
	virtual void Uninit(void) = 0;			// シーン終了処理
	virtual void Update(void) = 0;			// シーン更新処理
	virtual void Draw(void) = 0;			// シーン描画処理
	D3DXVECTOR3 GetPos(void);				// 座標取得処理
	static void UpdateAll(void);			// オブジェクトの更新処理
	static void DrawAll(void);				// オブジェクトの描画処理
	static void ReleaseAll(void);			// オブジェクトを全て破棄
	static CScene *GetScene(int nIdxScene);	// シーン取得処理
	void SetObjType(OBJTYPE objType);		// オブジェクトタイプセット処理
	OBJTYPE GetObjType(void);				// オブジェクトタイプ取得処理
	static CScene* GetList(int Priority);
	CScene* GetNext(CScene* pNext){return pNext->m_Next;}
protected:									   
	void Release(void);						// オブジェクトを破棄
	
private:
	static CScene *m_Top[PRIORITY];
	CScene *m_Next;
	static CScene *m_apScene[MAX_SCENE];	// オブジェクトへのポインタ
	static int m_nNumScene;					// 総オブジェクト数
	bool m_Delete;
	int m_nID;								// 固有ID
	OBJTYPE m_objType;						// オブジェタイプのメンバ変数
};


/*-----------------------------------------------------------*/
#endif