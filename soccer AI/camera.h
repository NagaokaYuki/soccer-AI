/*=======================================================================

	モデル表示処理 [camera.h]

												AT-13C-245
												長岡佑樹

=========================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/////////////////////////////////
//
//		定数定義
//
/////////////////////////////////
#define CAMERA_ANGLE		(3.0f)							// 画角
#define ANGLE (D3DX_PI / CAMERA_ANGLE)

/////////////////////////////////////////
//
//	3Dオブジェクトクラス
//
/////////////////////////////////////////
class CCamera
{
public:
	CCamera();						// カメラコンストラクタ
	~CCamera();						// カメラデストラクタ
	static CCamera *Create(void);			// オブジェクト生成処理
	void Init(void);				// カメラ初期化処理
	void Uninit(void);				// カメラ終了処理
	void Set(void);					// カメラセット処理
	void Update(void);				// カメラ更新処理
	void SetCamera(D3DXVECTOR3 pos);	
	D3DXMATRIX ViewCamera(void);
	D3DXVECTOR3 GetCamera(void);
	void SetScreenState(int width, int height)
	{
		m_nScreenWidth = width;
		m_nScreenHeight = height;
	}
	D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,int Sx, int Sy, float fZ, int Screen_w,int Screen_h);
	D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy);
private:							 
	D3DXMATRIX m_mtxView;			// ビュー座標
	D3DXMATRIX m_mtxProj;			// プロジェクション変数
	D3DXVECTOR3 m_posAt;			// 注視点
	D3DXVECTOR3 m_posEye;			// カメラ座標
	D3DXVECTOR3 m_vecUp;			// 上方向ベクトル
	D3DXVECTOR3 m_Rot;				// カメラ回転座標
	D3DXVECTOR3 m_Vertical;			// ベクトル
	D3DXVECTOR3 m_Length;			// 距離
	float m_fLength;				// 注視点からの距離
	float fLength;
	float fRadius;
	float m_fYRot;
	float m_fXRot;
	int m_nScreenWidth;
	int m_nScreenHeight;

};

/*------------------------------------------終了---------------------------------------------------*/
#endif