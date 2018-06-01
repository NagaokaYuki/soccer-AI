
//=============================================================================
//
// スコア処理 [score.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

///////////////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////////////////
#include "scene.h"


class CNumber;
///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define SCORE_MAX			(1)

///////////////////////////////////////////////
//
//		構造体定義
//
////////////////////////////////////////////////
class CScore : public CScene
{
public:
	CScore(int Priority);						// コンストラクタ
	~CScore();						// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理	
	static CScore *Create(void);	// オブジェクト生成処理
	void AddScore(int nValue);		// スコア加算処理  
	int GetScore(void);				// スコア取得処理  

private:
	CNumber *m_apNumber[SCORE_MAX];	// 
	D3DXVECTOR3 m_pos;				// 表示座標
	D3DXVECTOR3 size;				// 幅、高さ
	int m_nScore;					// スコア変数

};

#endif