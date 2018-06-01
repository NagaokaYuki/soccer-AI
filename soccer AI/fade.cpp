//=============================================================================
//
// �v���C���[���� [fade.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define FADE_TEXTURE "data/TEXTURE/fade.png"
#define FADE_POS_X		(0.0f)						// �t�F�[�h�̕\���ʒuX
#define FADE_POS_Y		(0.0f)						// �t�F�[�h�̕\���ʒuY
#define FADE_POS_Z		(0.0f)						// �t�F�[�h�̕\���ʒuZ
#define FADE_WIDTH		(SCREEN_WIDTH)				// �t�F�[�h�̕�
#define FADE_HEIGHT		(SCREEN_HEIGHT)				// �t�F�[�h�̍���
#define FADE_RATE		(0.01f)						// �t�F�[�h�W���A�t�F�[�h�̑��x�ɂ������
#define FADE_TEXTURE_X	(0.0f)						// �t�F�[�h�e�N�X�`�����WU
#define FADE_TEXTURE_Y	(0.0f)						// �t�F�[�h�e�N�X�`�����WV

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
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;

///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CFade *CFade::Create(void)
{
	CFade *pFade;
	pFade = new CFade;
	pFade->Init();
	pFade->BindTexture(m_pTexture);
	return pFade;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CFade::CFade()
{
	m_fade = FADE_NONE;
}
///////////////////////////////////////////////
//
//		�f�X�g���N�^
//
////////////////////////////////////////////////
CFade::~CFade()
{
}

///////////////////////////////////////////////
//
//		���[�h����
//
////////////////////////////////////////////////
void CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if(m_pTexture == NULL)
	{
		if(FAILED(D3DXCreateTextureFromFile(pDevice,
		FADE_TEXTURE,
		&m_pTexture)))
		{		
			MessageBox(NULL,"�e�N�X�`���̏������Ɏ��s���܂����B","�G���[", MB_OK);
		}
	}
}

///////////////////////////////////////////////
//
//		�A�����[�h����
//
////////////////////////////////////////////////
void CFade::UnLoad(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CFade::Init(void)
{
	m_pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, FADE_POS_Z);
	m_size = D3DXVECTOR2(FADE_WIDTH, FADE_HEIGHT);
	m_tex= D3DXVECTOR2(FADE_TEXTURE_X, FADE_TEXTURE_Y);
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_fade = FADE_IN;
	CScene2D::BindTexture(m_pTexture);
	CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	CScene2D::Init();
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CFade::Uninit(void)
{
	CScene2D::Uninit();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CFade::Update(void)
{
	// NONE�Ȃ�X�V���Ȃ�
	if(m_fade == FADE_NONE)
	{
		return;
	}

	// IN�Ȃ�i�X�������Ă���
	if(m_fade == FADE_IN)
	{
		m_colorFade.a -= FADE_RATE;
		if(m_colorFade.a <= 0.0f)
		{
			m_colorFade = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	// OUT�Ȃ�i�X�Z�����Ă���
	else if(m_fade == FADE_OUT)
	{
		m_colorFade.a += FADE_RATE;
		if(m_colorFade.a >= 1.0f)
		{
			m_colorFade.a = 1.0f;
			m_fade = FADE_IN;
			//SetMode(g_modeNext);
		}
	}

	CScene2D::SetColor(m_colorFade);

	CScene2D::Update();

}
///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CFade::Draw(void)
{
	CScene2D::Draw();
}

////////////////////////////////////
//
//		�t�F�[�h�Z�b�g����
//
////////////////////////////////////
void CFade:: SetFade(FADE fade/*, MODE modeNext*/)
{
	m_fade = fade;
	/*m_modeNext = modeNext;*/
}

////////////////////////////////////
//
//		�t�F�[�h���擾����
//
////////////////////////////////////
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}