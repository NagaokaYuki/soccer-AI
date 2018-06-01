//=============================================================================
//
// �V�[��2D���� [scene2D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�v���g�^�C�v�錾
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�O���[�o���ϐ�
//
///////////////////////////////////////////////

///////////////////////////////////////////////
//
//		CScene2D�̃R���X�g���N�^
//
////////////////////////////////////////////////
CScene2D *CScene2D::Create(void)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D(4);
	pScene2D->Init();

	return pScene2D;
}

///////////////////////////////////////////////
//
//		CScene2D�̃R���X�g���N�^
//
////////////////////////////////////////////////
CScene2D::CScene2D(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}

///////////////////////////////////////////////
//
//		CScene2D�̃f�X�g���N�^
//
////////////////////////////////////////////////
CScene2D::~CScene2D()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CScene2D::Init(void)
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
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

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
void CScene2D::Uninit(void)
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
void CScene2D::Update(void)
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

	// UV�̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_tex.x, m_tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_tex.x, m_tex.y + TEXTURE_V);
	pVtx[3].tex = D3DXVECTOR2(m_tex.x + TEXTURE_U, m_tex.y + TEXTURE_V);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CScene2D::Draw(void)
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
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[���[�h����
//
////////////////////////////////////////////////
void CScene2D::LoadTexture(LPCSTR FileName)
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

///////////////////////////////////////////////
//
//		�e�N�X�`���[�A�����[�h����
//
////////////////////////////////////////////////
void CScene2D::UnLoadTexture(void)
{

}
///////////////////////////////////////////////
//
//		�v���C���[�̍��W���擾����
//
////////////////////////////////////////////////
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

///////////////////////////////////////////////
//
//		�J���[�̐ݒ�		
//
////////////////////////////////////////////////
void CScene2D::SetColor(D3DCOLOR color)
{
	// ���_����ݒ�
	VERTEX_2D* pVtx;				// ���z�A�h���X�p�|�C���^
	// ���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[(0�`255�̐����l)
	pVtx[0].color = color;
	pVtx[1].color = color;
	pVtx[2].color = color;
	pVtx[3].color = color;

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�A�j���[�V�����̐ݒ�		
//
////////////////////////////////////////////////
void CScene2D :: SetAnimation( D3DXVECTOR2 pos, D3DXVECTOR2 size )
{
	// ���_����ݒ�
	VERTEX_2D* pVtx = NULL;			// ���z�A�h���X�p�|�C���^

	// ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuff->Lock( 0, 0, ( void** )&pVtx, 0 );

	// ���_�f�[�^��UV�f�[�^�̒ǉ�
	pVtx[0].tex = D3DXVECTOR2(pos.x, pos.y );							// UV�l����
	pVtx[1].tex = D3DXVECTOR2(pos.x + size.x, pos.y );					// UV�l�E��
	pVtx[2].tex = D3DXVECTOR2(pos.x, pos.y + size.y );					// UV�l����
	pVtx[3].tex = D3DXVECTOR2(pos.x + size.x, pos.y + size.y );			// UV�l�E��

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock( );
}

