//=============================================================================
//
// �}�l�[�W���[�̏��� [modegame.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "modegame.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "light.h"
#include "camera.h"
#include "bg.h"
#include "input.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "billboard.h"
#include "debug.h"
#include "moderesult.h"
#include "number.h"
#include "score.h"
#include "time.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�ÓI�����o�ϐ�
//
////////////////////////////////////////////////
CCamera *CModeGame::m_pCamera = NULL;						// �J�����̐ÓI�����o�ϐ�
CLight *CModeGame::m_pLight = NULL;							// ���C�g�N���X�̐ÓI�����o�ϐ�
CPolygon2D *CModeGame::m_pPolygon2D = NULL;					// �|���S���QD�̐ÓI�����o�ϐ�
bool CModeGame::m_bUse = NULL;								// �t���O�̐ÓI�����o�ϐ�
CScore *CModeGame::m_pScore = NULL;							// �X�R�A�̐ÓI�����o�ϐ�
CBg *CModeGame::m_pBg = NULL;								// �w�i�N���X�̐ÓI�����o�ϐ�
CTime *CModeGame::m_pTime = NULL;							// �^�C���N���X�̐ÓI�����o�ϐ�

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CModeGame::Init(void)
{
	// �f�o�C�X�擾
	CInputKeyboard *pInput = CManager::GetKeyboard();

	// ���C�g�̐���
	m_pLight = new CLight;
	m_pLight->Init();

	// �J�����̐���
	m_pCamera = new CCamera;
	m_pCamera->Init();

	// �w�i�̐���
	m_pBg = CBg::Create(CBg::BG_TYPE_GAME);

	// �X�R�A�̐���
	m_pScore = CScore::Create();

	// �^�C���̐���
	m_pTime = CTime::Create();

	// �X�R�A�̃��[�h
	CNumber::Load();

}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CModeGame::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();
			
	// ���C�g�I�u�W�F�N�g�̔j��
	if(m_pLight != NULL)
	{
		m_pLight->Uninit();
		m_pLight = NULL;
	}

	// �J�����I�u�W�F�N�g�̔j��
	if(m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		m_pCamera = NULL;
	}

	// �i���o�[�A�����[�h
	CNumber::Unload();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CModeGame::Update(void)
{
	// �f�o�C�X�擾
	CInputKeyboard *pInput = CManager::GetKeyboard();

	// �J�����X�V����
	m_pCamera->Update();					
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CModeGame::Draw(void)
{
	m_pCamera->Set();						// �J�����Z�b�g����
	CScene::DrawAll();
	//m_pBg->Draw();
}

//////////////////////////////////////////////
//
//		�J�����擾����
//
//////////////////////////////////////////////
CCamera *CModeGame::GetCamera(void)
{
	return m_pCamera;
}

//////////////////////////////////////////////
//
//		���̂Ƌ��̂̓����蔻��
//
//////////////////////////////////////////////
bool CModeGame::SphereCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2)
{
	/*--------------------------------------------------��------------------------------------------*/
	//		��1 �F ���S�_�̍��WP1(x1, y1, z1), ���ar1
	//		��2 �F ���S�_�̍��WP2(x2, y2, z2), ���ar2
	//		(x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2 <= (r1+r2)^2�Ȃ�ΏՓ˂��N�����Ă���
	if((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (radius1 + radius2) * (radius1 + radius2))
	{
		return true;
	}
	return false;
}
