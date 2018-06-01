/*=======================================================================

	ウィンドウ表示処理 [main.h]

												
												長岡佑樹

=========================================================================*/
//////////////////////////////////
//
//ヘッダファイル
//
//////////////////////////////////
#ifndef _MAIN_H_
#define _MAIN_H_

///////////////////////////////////////////////
//
//		ヘッダファイル
//
////////////////////////////////////////////////
#include "windows.h"
#include "d3dx9.h"
#define DIRECTINPUT_VERSION				(0x0800)
#include <dinput.h>
#include <XAudio2.h>
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////
//
//		ライブラリファイル
//		[構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
////////////////////////////////////////////////
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
																	   
////////////////////////////////////
//
//定数定義
//
////////////////////////////////////
// 頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// 頂点フォーマット
// 頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH		(700)											// ウィンドウの幅
#define SCREEN_HEIGHT		(400)											// ウィンドウの高さ
																			   
#define NUM_VERTEX			(4)												// 頂点数
#define NUM_POLYGON			(2)												// ポリゴン数
//#define SAFE_RELEASE(x){if(x)	{(x)->Release();	(x) = NULL}	}
const int TeamSize = 5;

/////////////////////////////////////////////////////////
//
//
//	頂点情報構造体
//	注:(上の頂点フォーマットと構造を合わせること)
//
/////////////////////////////////////////////////////////
//	注:(上の頂点フォーマットと構造を合わせること)
typedef struct
{
	D3DXVECTOR3 pos;						// 表示座標
	D3DXVECTOR3 normal;						// 法線
	D3DCOLOR color;							// 色
	D3DXVECTOR2 tex;						// テクスチャ
}VERTEX_3D;									// 型名

/////////////////////////////////////////////////////////
//
//
//	頂点情報構造体
//	注:(上の頂点フォーマットと構造を合わせること)
//
/////////////////////////////////////////////////////////
typedef struct
{
	D3DXVECTOR3 pos;											// X,Y,Z
	float rhw;													// 座標変換済み頂点座標 
	D3DCOLOR color;												// 頂点カラー
	D3DXVECTOR2 tex;											// テクスチャ
}VERTEX_2D;														// 型名

//////////////////////////////////
//
//プロトタイプ宣言
//
//////////////////////////////////
LPDIRECT3DDEVICE9 GetDevice(void);		//Direct3DDeviceの取得
int GetFPS(void);
/*------------------------------------------終了---------------------------------------------------*/
#endif