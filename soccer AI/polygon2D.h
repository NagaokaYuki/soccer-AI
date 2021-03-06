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
class CPolygon2D : public CScene2D
{
public:
	CPolygon2D(int Priority);						// コンストラクタ
	~CPolygon2D();						// デストラクタ
	static CPolygon2D *Create(void);	// オブジェクト生成処理
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
private:
};

#endif