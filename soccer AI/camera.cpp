/*=======================================================================

	ウィンドウ表示処理 [camera.cpp]

												AT-13C-245
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "main.h"
#include "modegame.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////
#define POS_AT_X			(0.0f)							// 注視点X
#define POS_AT_Y			(50.0f)							// 注視点Y
#define POS_AT_Z			(0.0f)							// 注視点Z

#define POS_EYE_X			(0.0f)							// カメラ座標X
#define POS_EYE_Y			(50.0f)							// カメラ座標Y
#define POS_EYE_Z			(-50.0f)						// カメラ座標Z

#define VEC_UP_X			(0.0f)							// 上方向ベクトルX
#define VEC_UP_Y			(1.0f)							// 上方向ベクトルY
#define VEC_UP_Z			(0.0f)							// 上方向ベクトルZ


#define CAMERA_NEAR			(1.0f)							// near	0.0f < near 0はダメ！！
#define CAMERA_FAR			(1000.0f)						// far

#define CAMERA_ROT_X		(0.0f)							// カメラ回転座標X
#define CAMERA_ROT_Y		(0.0f)							// カメラ回転座標Y
#define CAMERA_ROT_Z		(0.0f)							// カメラ回転座標Z


/////////////////////////////////////////////////////////
//
//	グローバル変数
//
/////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		オブジェクト生成処理
//
////////////////////////////////////////////////
CCamera *CCamera::Create(void)
{
	CCamera *pCamera;					// メッシュウォールクラスのポインタ型
	pCamera = new CCamera;				// メッシュウォール生成
	pCamera->Init();					// メッシュウォール初期化処理
	return pCamera;
}

/////////////////////////////////////////////////////////
//
//
//	コンストラクタ
//
//
/////////////////////////////////////////////////////////
CCamera::CCamera()
{
}

/////////////////////////////////////////////////////////
//
//
//	デストラクタ
//
//
/////////////////////////////////////////////////////////
CCamera::~CCamera()
{
}

/////////////////////////////////////////////////////////
//
//
//	初期化処理
//
//
/////////////////////////////////////////////////////////
void CCamera::Init(void)
{
	m_posAt =D3DXVECTOR3(POS_AT_X, POS_AT_Y, POS_AT_Z);				// 注視点
	m_posEye = D3DXVECTOR3(POS_EYE_X, POS_EYE_Y, POS_EYE_Z);		// カメラ座標
	m_vecUp = D3DXVECTOR3(VEC_UP_X, VEC_UP_Y, VEC_UP_Z);			// 上方向ベクトル
	fLength = 25.0f;												// モデルとの距離
	m_fLength = 0.0f;												// マウスの距離
	fRadius = 0.0f;													// 半径
	m_fYRot = 0.0f;													// 回転X
	m_fXRot = 0.0f;													// 回転Y
	m_nScreenHeight = SCREEN_HEIGHT;								// 画面サイズ
	m_nScreenWidth  = SCREEN_WIDTH;
}

/////////////////////////////////////////////////////////
//
//
//	終了処理
//
//
/////////////////////////////////////////////////////////
void CCamera::Uninit(void)
{

}

/////////////////////////////////////////////////////////
//
//
//	更新処理
//
//
/////////////////////////////////////////////////////////
void CCamera::Update(void)
{	
	
}

/////////////////////////////////////////////////////////
//
//
//	セット処理
//
//
/////////////////////////////////////////////////////////
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();	
	
	/*----------------------------ビュー座標変換--------------------------------*/		    
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posAt, &m_vecUp);						// これらの情報からビュー行列を作る関数
	
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);					// デバイスに設定
	
	/*----------------------------プロジェクション変換--------------------------------*/
	D3DXMatrixPerspectiveFovLH(&m_mtxProj, 
	ANGLE,															// 画角(視野角)
	(float)SCREEN_WIDTH / SCREEN_HEIGHT,							// アスペクト比
	CAMERA_NEAR,													// near
	CAMERA_FAR);													// far

	// デバイスに設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

/////////////////////////////////////////////////////////
//
//
//	カメラセット処理
//
//
/////////////////////////////////////////////////////////
void CCamera::SetCamera(D3DXVECTOR3 pos)
{
	m_posEye += pos;
}

/////////////////////////////////////////////////////////
//
//
//	カメラ座標取得処理
//
//
/////////////////////////////////////////////////////////
D3DXVECTOR3 CCamera::GetCamera(void)
{
	return m_posEye;
}

/////////////////////////////////////////////////////////
//
//
//	カメラビュー取得処理
//
//
/////////////////////////////////////////////////////////
D3DXMATRIX CCamera::ViewCamera(void)
{
	return m_mtxView;
}

/////////////////////////////////////////////////////////
//
//
// スクリーン座標をワールド座標に変換
//
//
/////////////////////////////////////////////////////////
D3DXVECTOR3* CCamera::CalcScreenToWorld(D3DXVECTOR3* pout,int Sx, int Sy, float fZ, int Screen_w,int Screen_h)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse( &InvView, NULL, &m_mtxView);
	D3DXMatrixInverse( &InvPrj, NULL, &m_mtxProj );
	D3DXMatrixIdentity( &VP );
	VP._11 = Screen_w/2.0f; VP._22 = -Screen_h/2.0f;
	VP._41 = Screen_w/2.0f; VP._42 = Screen_h/2.0f;
	D3DXMatrixInverse( &InvViewport, NULL, &VP );
	
	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord( pout, &D3DXVECTOR3(Sx,Sy,fZ), &tmp );
	
	return pout;
}

/////////////////////////////////////////////////////////
//
//
// XZ平面とスクリーン座標の交点算出関数
//
//
/////////////////////////////////////////////////////////
//D3DXVECTOR3* CCamera::CalcScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy)
//{
//	D3DXVECTOR3 nearpos;
//	D3DXVECTOR3 farpos;
//	D3DXVECTOR3 ray;
//	CalcScreenToWorld( &nearpos, Sx, Sy, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT );
//	CalcScreenToWorld( &farpos, Sx, Sy, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT );
//	ray = farpos - nearpos;
//	D3DXVec3Normalize( &ray, &ray );
//
//	// 床との交差が起きている場合は交点を
//	// 起きていない場合は遠くの壁との交点を出力
//	if( ray.y <= 0 ) 
//	{
//		// 床交点
//		float Lray = D3DXVec3Dot( &ray, &D3DXVECTOR3(0, 1, 0) );
//		float LP0 = D3DXVec3Dot( &(-nearpos), &D3DXVECTOR3(0, 1, 0) );
//		*pout = nearpos + (LP0/Lray)*ray;
//	}
//	else
//	{
//		*pout = farpos;
//	}
//
//	return pout;
//}