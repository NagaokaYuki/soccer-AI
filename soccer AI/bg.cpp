//=============================================================================
//
// �v���C���[���� [bg.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "bg.h"
#include "renderer.h"
#include "scene.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define BG_TITLETEXTURE "data/TEXTURE/shoot.jpg"
#define BG_GAMETEXTURE	"data/TEXTURE/soccercoat.png"
#define BG_RESULTTEXTURE "data/TEXTURE/result000.jpg"
#define BG_POS_X		(0.0f)						// �|���S��2D�\�����WX
#define BG_POS_Y		(0.0f)						// �|���S��2D�\�����WY
#define BG_POS_Z		(0.0f)						// �|���S��2D�\�����WZ
#define BG_WIDTH		(SCREEN_WIDTH)				// �|���S��2D�̕�
#define BG_HEIGHT		(SCREEN_HEIGHT)				// �|���S��2D�̍���
#define BG_TEXTURE_X	(0.0f)						// �|���S���e�N�X�`�����WU
#define BG_TEXTURE_Y	(0.0f)						// �|���S���e�N�X�`�����WV

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
CBg *CBg::Create(BGTYPE BgType)
{
	CBg *pBg;
	pBg = new CBg(8);
	pBg->Init();

	switch(BgType)
	{
	case BG_TYPE_TITLE:
		pBg->LoadTexture(BG_TITLETEXTURE);
		break;

	case BG_TYPE_GAME:
		pBg->LoadTexture(BG_GAMETEXTURE);
		break;

	case BG_TYPE_RESULT:
		pBg->LoadTexture(BG_RESULTTEXTURE);
		break;
	}
	return pBg;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CBg::CBg(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}
///////////////////////////////////////////////
//
//		�f�X�g���N�^
//
////////////////////////////////////////////////
CBg::~CBg()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CBg::Init(void)
{
	m_pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, BG_POS_Z);
	m_size = D3DXVECTOR2(BG_WIDTH, BG_HEIGHT);
	m_color = 255;
	m_tex= D3DXVECTOR2(BG_TEXTURE_X, BG_TEXTURE_Y);

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			// �쐬���������_�o�b�t�@�̃T�C�Y,�ő��ŏ����A�������Ǘ����@(�f�o�C�X�ɂ��܂���)
	{
		MessageBox(NULL,"���_�̍쐬�Ɏ��s���܂����B","�G���[", MB_OK);
	}

	// ���_����ݒ�
	VERTEX_2D* pVtx;				// ���z�A�h���X�p�|�C���^

	// ���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(2D���W�E�E����)
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	// rhw�̐ݒ�(�K��1.0f)
	pVtx[1].rhw = 1.0f;
	pVtx[0].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[1].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[2].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);
	pVtx[3].color = D3DCOLOR_RGBA(m_color, m_color, m_color, 255);

	// UV�̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_tex.x, m_tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_tex.x, m_tex.y + TEXTURE_V);
	pVtx[3].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y + TEXTURE_V);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CBg::Uninit(void)
{
	if(m_pTexture != NULL && m_bLoadTexture == true)
	{
		m_pTexture ->Release();
		m_pTexture = NULL;
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff ->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CBg::Update(void)
{
	
}
///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CBg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	// �f�o�C�X�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if(pDevice != NULL)
	{
		pDevice->SetTexture(0, m_pTexture);
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,			// �v���~�e�B�u�̎�� 
							0,								// �I�t�Z�b�g(�ǂ�����`���͂��߂邩)
							NUM_POLYGON						// �v���~�e�B�u�̐�
							);
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[�o�C���h����
//
////////////////////////////////////////////////
void CBg::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[���[�h����
//
////////////////////////////////////////////////
void CBg::LoadTexture(LPCSTR FileName)
{
	if(m_pTexture == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		FileName, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B","�G���[", MB_OK);
		}
		m_bLoadTexture = true;
	}
}