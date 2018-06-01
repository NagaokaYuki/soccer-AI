/*=======================================================================

	�E�B���h�E�\������ [camera.cpp]

												AT-13C-245
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�萔��`
//
/////////////////////////////////////////////////////////
#define POS_AT_X			(0.0f)							// �����_X
#define POS_AT_Y			(50.0f)							// �����_Y
#define POS_AT_Z			(0.0f)							// �����_Z

#define POS_EYE_X			(0.0f)							// �J�������WX
#define POS_EYE_Y			(50.0f)							// �J�������WY
#define POS_EYE_Z			(-50.0f)						// �J�������WZ

#define VEC_UP_X			(0.0f)							// ������x�N�g��X
#define VEC_UP_Y			(1.0f)							// ������x�N�g��Y
#define VEC_UP_Z			(0.0f)							// ������x�N�g��Z


#define CAMERA_NEAR			(1.0f)							// near	0.0f < near 0�̓_���I�I
#define CAMERA_FAR			(1000.0f)						// far

#define CAMERA_ROT_X		(0.0f)							// �J������]���WX
#define CAMERA_ROT_Y		(0.0f)							// �J������]���WY
#define CAMERA_ROT_Z		(0.0f)							// �J������]���WZ


/////////////////////////////////////////////////////////
//
//	�O���[�o���ϐ�
//
/////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�I�u�W�F�N�g��������
//
////////////////////////////////////////////////
CCamera *CCamera::Create(void)
{
	CCamera *pCamera;					// ���b�V���E�H�[���N���X�̃|�C���^�^
	pCamera = new CCamera;				// ���b�V���E�H�[������
	pCamera->Init();					// ���b�V���E�H�[������������
	return pCamera;
}

/////////////////////////////////////////////////////////
//
//
//	�R���X�g���N�^
//
//
/////////////////////////////////////////////////////////
CCamera::CCamera()
{
}

/////////////////////////////////////////////////////////
//
//
//	�f�X�g���N�^
//
//
/////////////////////////////////////////////////////////
CCamera::~CCamera()
{
}

/////////////////////////////////////////////////////////
//
//
//	����������
//
//
/////////////////////////////////////////////////////////
void CCamera::Init(void)
{
	m_posAt =D3DXVECTOR3(POS_AT_X, POS_AT_Y, POS_AT_Z);				// �����_
	m_posEye = D3DXVECTOR3(POS_EYE_X, POS_EYE_Y, POS_EYE_Z);		// �J�������W
	m_vecUp = D3DXVECTOR3(VEC_UP_X, VEC_UP_Y, VEC_UP_Z);			// ������x�N�g��
	fLength = 25.0f;												// ���f���Ƃ̋���
	m_fLength = 0.0f;												// �}�E�X�̋���
	fRadius = 0.0f;													// ���a
	m_fYRot = 0.0f;													// ��]X
	m_fXRot = 0.0f;													// ��]Y
	m_nScreenHeight = SCREEN_HEIGHT;								// ��ʃT�C�Y
	m_nScreenWidth  = SCREEN_WIDTH;
}

/////////////////////////////////////////////////////////
//
//
//	�I������
//
//
/////////////////////////////////////////////////////////
void CCamera::Uninit(void)
{

}

/////////////////////////////////////////////////////////
//
//
//	�X�V����
//
//
/////////////////////////////////////////////////////////
void CCamera::Update(void)
{	
	
}

/////////////////////////////////////////////////////////
//
//
//	�Z�b�g����
//
//
/////////////////////////////////////////////////////////
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();	
	
	/*----------------------------�r���[���W�ϊ�--------------------------------*/		    
	D3DXMatrixLookAtLH(&m_mtxView, &m_posEye, &m_posAt, &m_vecUp);						// �����̏�񂩂�r���[�s������֐�
	
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);					// �f�o�C�X�ɐݒ�
	
	/*----------------------------�v���W�F�N�V�����ϊ�--------------------------------*/
	D3DXMatrixPerspectiveFovLH(&m_mtxProj, 
	ANGLE,															// ��p(����p)
	(float)SCREEN_WIDTH / SCREEN_HEIGHT,							// �A�X�y�N�g��
	CAMERA_NEAR,													// near
	CAMERA_FAR);													// far

	// �f�o�C�X�ɐݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

/////////////////////////////////////////////////////////
//
//
//	�J�����Z�b�g����
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
//	�J�������W�擾����
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
//	�J�����r���[�擾����
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
// �X�N���[�����W�����[���h���W�ɕϊ�
//
//
/////////////////////////////////////////////////////////
D3DXVECTOR3* CCamera::CalcScreenToWorld(D3DXVECTOR3* pout,int Sx, int Sy, float fZ, int Screen_w,int Screen_h)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse( &InvView, NULL, &m_mtxView);
	D3DXMatrixInverse( &InvPrj, NULL, &m_mtxProj );
	D3DXMatrixIdentity( &VP );
	VP._11 = Screen_w/2.0f; VP._22 = -Screen_h/2.0f;
	VP._41 = Screen_w/2.0f; VP._42 = Screen_h/2.0f;
	D3DXMatrixInverse( &InvViewport, NULL, &VP );
	
	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord( pout, &D3DXVECTOR3(Sx,Sy,fZ), &tmp );
	
	return pout;
}

/////////////////////////////////////////////////////////
//
//
// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
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
//	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
//	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
//	if( ray.y <= 0 ) 
//	{
//		// ����_
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