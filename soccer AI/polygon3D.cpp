//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "polygon3D.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define POLYGON3D_TEXTURENAME "data/TEXTURE/kobeni3.jpg"	// jpg,tga,bmp,dds	dds�͈��k������Ԃł��g�p��
#define POLYGON3D_POS_X		(0.0f)							// �|���S��3D�\�����WX
#define POLYGON3D_POS_Y		(1.0f)							// �|���S��3D�\�����WY
#define POLYGON3D_POS_Z		(0.0f)							// �|���S��3D�\�����WZ

#define POLYGON3D_WIDTH		(15.0f)							// �|���S��3D�̕�
#define POLYGON3D_HEIGHT	(15.0f)							// �|���S��3D�̍���

#define POLYGON3D_TEXTURE_X	(0.0f)							// �|���S���e�N�X�`�����WU
#define POLYGON3D_TEXTURE_Y	(0.0f)							// �|���S���e�N�X�`�����WV

#define POLYGON_ROT_X		(0.0f)							// �|���S����]���WX
#define POLYGON_ROT_Y		(0.0f)							// �|���S����]���WY
#define POLYGON_ROT_Z		(0.0f)							// �|���S����]���WZ

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
CPolygon3D *CPolygon3D::Create(void)
{
	CPolygon3D *pPolygon3D;
	pPolygon3D = new CPolygon3D(17);
	pPolygon3D->Init();
	pPolygon3D->LoadTexture(POLYGON3D_TEXTURENAME);
	return pPolygon3D;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CPolygon3D::CPolygon3D(int Priority):CScene3D(Priority)
{
}
///////////////////////////////////////////////
//
//		�f�X�g���N�^
//
////////////////////////////////////////////////
CPolygon3D::~CPolygon3D()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CPolygon3D::Init(void)
{
	m_Pos = D3DXVECTOR3(POLYGON3D_POS_X, POLYGON3D_POS_Y, POLYGON3D_POS_Z);
	m_Size = D3DXVECTOR3(POLYGON3D_WIDTH, 0.0f, POLYGON3D_HEIGHT);
	m_Tex= D3DXVECTOR2(POLYGON3D_TEXTURE_X, POLYGON3D_TEXTURE_Y);
	m_Rot = D3DXVECTOR3(POLYGON_ROT_X, POLYGON_ROT_Y, POLYGON_ROT_Z);
	CScene3D::Init();
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CPolygon3D::Uninit(void)
{
	CScene3D::Uninit();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CPolygon3D::Update(void)
{
	CScene3D::Update();

}
///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CPolygon3D::Draw(void)
{
	CScene3D::Draw();
}

