//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "mode.h"
#include "input.h"
#include "modetitle.h"
#include "debug.h"
#include "sound.h"

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
CInputKeyboard *CManager::m_pInputKeyboard = NULL;			// �L�[�{�[�h�̐ÓI�����o�ϐ�
CInputMouse *CManager::m_pInputMouse = NULL;				// �L�[�{�[�h�̐ÓI�����o�ϐ�
CMode *CManager::m_pMode = NULL;
CModeTitle *CManager::m_pModeTitle = NULL;
CSound *CManager::m_pSound = NULL;

///////////////////////////////////////////////
//
//		CManager�̃R���X�g���N�^
//
////////////////////////////////////////////////
CManager::CManager()
{
}

///////////////////////////////////////////////
//
//		CManager�̃f�X�g���N�^
//
////////////////////////////////////////////////
CManager::~CManager()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	CRenderer::Init(hWnd, TRUE);

	CDebug::Init();

	// �L�[�{�[�h�̐���
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);

	// �L�[�{�[�h�̐���
	m_pInputMouse = new CInputMouse;
	m_pInputMouse->Init(hInstance, hWnd);

	m_pSound = new CSound;
	m_pSound->InitSound(hWnd);

	m_pMode = new CModeTitle;
	m_pMode->Init();
	
	return S_OK;
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CManager::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();
	
	CRenderer::Uninit();
	
	// �L�[�{�[�h�̔j��
	if(m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		m_pInputKeyboard = NULL;
	}
	// �L�[�{�[�h�̔j��
	if(m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		m_pInputMouse = NULL;
	}

	if(m_pMode != NULL)
	{
		m_pMode->Uninit();
		m_pMode = NULL;
	}
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CManager::Update(void)
{
	CScene::UpdateAll();
	CDebug::ClearText();
	m_pInputKeyboard->Update();				// �L�[�{�[�h�X�V����
	m_pInputMouse->Update();
	m_pMode->Update();
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CManager::Draw(void)
{
	CRenderer::BeginDraw();
	m_pMode->Draw();
	CRenderer::EndDraw();
}

///////////////////////////////////////////////
//
//		�Z�b�g���[�h����
//
////////////////////////////////////////////////
void CManager::SetMode(CMode *mode)
{
	if(m_pMode != NULL)
	{
		m_pMode->Uninit();
		delete m_pMode;
	}
	m_pMode = mode;

	if(m_pMode != NULL)
	{
		m_pMode->Init();
	}
}


//////////////////////////////////////////////
//
//		�L�[�{�[�h�擾����
//
//////////////////////////////////////////////
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

