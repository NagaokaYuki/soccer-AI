//=============================================================================
//
// �i���o�[���� [number.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define NUMBER_TEXTURENAME "data/TEXTURE/number000.png"		// jpg,tga,bmp,dds	dds�͈��k������Ԃł��g�p��
#define TEXTURE_WIDTH		(0.1f)							// �e�N�X�`���̕�
#define TEXTURE_HEIGHT		(1.0f)							// �e�N�X�`���̍���

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
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;
int g_Score;

///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CNumber *CNumber::Create(void)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	pNumber->Init();
	return pNumber;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}
///////////////////////////////////////////////
//
//		�f�X�g���N�^
//
////////////////////////////////////////////////
CNumber::~CNumber()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
HRESULT CNumber::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			// �쐬���������_�o�b�t�@�̃T�C�Y,�ő��ŏ����A�������Ǘ����@(�f�o�C�X�ɂ��܂���)
	{
		MessageBox(NULL,"���_�̍쐬�Ɏ��s���܂����B","�G���[", MB_OK);
		return E_FAIL;
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	// UV�̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
	return S_OK;
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CNumber::Uninit(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture ->Release();
		m_pTexture = NULL;
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff ->Release();
		m_pVtxBuff = NULL;
	}
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CNumber::Update(void)
{
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
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������

}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	// �f�o�C�X�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, m_pTexture);
	
	
	// �|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,			// �v���~�e�B�u�̎�� 
								0,								// �I�t�Z�b�g(�ǂ�����`���͂��߂邩)
								NUM_POLYGON						// �v���~�e�B�u�̐�
								);
}

///////////////////////////////////////////////
//
//		���[�h����
//
////////////////////////////////////////////////
HRESULT CNumber::Load(void)
{
	if(m_pTexture == NULL)
	{
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		NUMBER_TEXTURENAME, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"�e�N�X�`���̏������Ɏ��s���܂����B","�G���[", MB_OK);
		}
	}

	return S_OK;
}

///////////////////////////////////////////////
//
//		�A�����[�h����
//
////////////////////////////////////////////////
void CNumber::Unload(void)
{
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

///////////////////////////////////////////////
//
//		�Z�b�g����
//
////////////////////////////////////////////////
void CNumber::SetNumber(int nNumber)
{
	// ���_����ݒ�
	VERTEX_2D* pVtx;				// ���z�A�h���X�p�|�C���^

	// ���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// UV�̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH + TEXTURE_WIDTH, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH, TEXTURE_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(nNumber * TEXTURE_WIDTH + TEXTURE_WIDTH, TEXTURE_HEIGHT);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�Z�b�g����
//
////////////////////////////////////////////////
void CNumber::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

///////////////////////////////////////////////
//
//		�Z�b�g����
//
////////////////////////////////////////////////
void CNumber::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
}
