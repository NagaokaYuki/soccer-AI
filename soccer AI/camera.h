/*=======================================================================

	���f���\������ [camera.h]

												AT-13C-245
												�����C��

=========================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

/////////////////////////////////
//
//		�萔��`
//
/////////////////////////////////
#define CAMERA_ANGLE		(3.0f)							// ��p
#define ANGLE (D3DX_PI / CAMERA_ANGLE)

/////////////////////////////////////////
//
//	3D�I�u�W�F�N�g�N���X
//
/////////////////////////////////////////
class CCamera
{
public:
	CCamera();						// �J�����R���X�g���N�^
	~CCamera();						// �J�����f�X�g���N�^
	static CCamera *Create(void);			// �I�u�W�F�N�g��������
	void Init(void);				// �J��������������
	void Uninit(void);				// �J�����I������
	void Set(void);					// �J�����Z�b�g����
	void Update(void);				// �J�����X�V����
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
	D3DXMATRIX m_mtxView;			// �r���[���W
	D3DXMATRIX m_mtxProj;			// �v���W�F�N�V�����ϐ�
	D3DXVECTOR3 m_posAt;			// �����_
	D3DXVECTOR3 m_posEye;			// �J�������W
	D3DXVECTOR3 m_vecUp;			// ������x�N�g��
	D3DXVECTOR3 m_Rot;				// �J������]���W
	D3DXVECTOR3 m_Vertical;			// �x�N�g��
	D3DXVECTOR3 m_Length;			// ����
	float m_fLength;				// �����_����̋���
	float fLength;
	float fRadius;
	float m_fYRot;
	float m_fXRot;
	int m_nScreenWidth;
	int m_nScreenHeight;

};

/*------------------------------------------�I��---------------------------------------------------*/
#endif