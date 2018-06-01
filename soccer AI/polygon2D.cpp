//=============================================================================
//
// �v���C���[���� [polygon2D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "polygon2D.h"
#include "renderer.h"
#include "scene.h"
#include "billboard.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define LIFEGAGE_TEXTURE "data/TEXTURE/lifegage.png"
#define POLYGON2D_POS_X		(0.0f)						// �|���S��2D�\�����WX
#define POLYGON2D_POS_Y		(0.0f)						// �|���S��2D�\�����WY
#define POLYGON2D_POS_Z		(0.0f)						// �|���S��2D�\�����WZ
#define POLYGON2D_WIDTH		(250.0f)						// �|���S��2D�̕�
#define POLYGON2D_HEIGHT	(50.0f)						// �|���S��2D�̍���
#define POLYGON2D_TEXTURE_X	(0.0f)						// �|���S���e�N�X�`�����WU
#define POLYGON2D_TEXTURE_Y	(0.0f)						// �|���S���e�N�X�`�����WV

///////////////////////////////////////////////
//
//		�v���g�^�C�v�錾
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�O���[�o���ϐ�
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CPolygon2D *CPolygon2D::Create(void)
{
	CPolygon2D *pPolygon2D;
	pPolygon2D = new CPolygon2D(16);
	pPolygon2D->Init();
	pPolygon2D->LoadTexture(LIFEGAGE_TEXTURE);
	return pPolygon2D;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CPolygon2D::CPolygon2D(int Priority):CScene2D(Priority)
{
}
///////////////////////////////////////////////
//
//		�f�X�g���N�^
//
////////////////////////////////////////////////
CPolygon2D::~CPolygon2D()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CPolygon2D::Init(void)
{
	m_pos = D3DXVECTOR3(POLYGON2D_POS_X, POLYGON2D_POS_Y, POLYGON2D_POS_Z);
	m_size = D3DXVECTOR2(POLYGON2D_WIDTH, POLYGON2D_HEIGHT);
	m_tex= D3DXVECTOR2(POLYGON2D_TEXTURE_X, POLYGON2D_TEXTURE_Y);
	CScene2D::Init();
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CPolygon2D::Uninit(void)
{
	CScene2D::Uninit();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CPolygon2D::Update(void)
{
	CScene2D::Update();

}
///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CPolygon2D::Draw(void)
{
	CScene2D::Draw();
}

