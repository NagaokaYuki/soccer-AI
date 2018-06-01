//=============================================================================
//
// �V�[��2D���� [scene3D.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "manager.h"
#include "scene3D.h"
#include "renderer.h"
#include "input.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define TEXTURE3D_U				(1.0f)
#define TEXTURE3D_V				(1.0f)
							
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
//		CScene3D�̃R���X�g���N�^
//
////////////////////////////////////////////////
CScene3D *CScene3D::Create(void)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D(3);
	pScene3D->Init();

	return pScene3D;
}

///////////////////////////////////////////////
//
//		CScene3D�̃R���X�g���N�^
//
////////////////////////////////////////////////
CScene3D::CScene3D(int Priority):CScene(Priority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_bLoadTexture = false;
}

///////////////////////////////////////////////
//
//		CScene3D�̃f�X�g���N�^
//
////////////////////////////////////////////////
CScene3D::~CScene3D()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CScene3D::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL)))			//�쐬���������_�o�b�t�@�̃T�C�Y,�ő��ŏ����A�������Ǘ����@(�f�o�C�X�ɂ��܂���)
	{
		MessageBox(NULL,"���_�̍쐬�Ɏ��s���܂����B","�G���[", MB_OK);
	}

	// ���_����ݒ�
	VERTEX_3D* pVtx;				//���z�A�h���X�p�|�C���^

	// ���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(3D���W�E�E���)
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - m_Size.x, m_Pos.y + m_Size.y, m_Pos.z + m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y + m_Size.y, m_Pos.z + m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x - m_Size.x, m_Pos.y - m_Size.y, m_Pos.z - m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x, m_Pos.y - m_Size.y, m_Pos.z - m_Size.z);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y);
	pVtx[1].tex = D3DXVECTOR2(m_Tex.x + TEXTURE3D_U, m_Tex.y);
	pVtx[2].tex = D3DXVECTOR2(m_Tex.x, m_Tex.y + TEXTURE3D_V);
	pVtx[3].tex = D3DXVECTOR2(m_Tex.x + TEXTURE3D_U, m_Tex.y + TEXTURE3D_V);

	// �x�N�g��
	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CScene3D::Uninit(void)
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
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CScene3D::Update(void)
{
	// ���_����ݒ�
	VERTEX_3D* pVtx;				// ���z�A�h���X�p�|�C���^
	// ���_�o�b�t�@�����b�N���ĉ��z�A�h���X���擾����
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(3D���W�E�E���)
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x -m_Size.x,   m_Pos.y, m_Pos.z + m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - m_Size.x,  m_Pos.y + m_Size.y,  m_Pos.z + m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + m_Size.x,  m_Pos.y, m_Pos.z - m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + m_Size.x,  m_Pos.y + m_Size.y, m_Pos.z - m_Size.z);

	m_pVtxBuff->Unlock();				// ���_�o�b�t�@�̃��b�N������
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CScene3D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	/*----------------------------���[���h���W�ϊ�--------------------------------*/
	D3DXMatrixIdentity(&m_mtxWorld);													// �P�ʍs������֐�
																						   
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);				// ���[���h�}�g���b�N�X = ��]�s��
																						   
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);							// �|���Z�̎d��
																						   
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);									// �f�o�C�X�ɐݒ�

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	// �f�o�C�X�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	
	// �|���S���̕`��
	// �v���~�e�B�u
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);		//(�v���~�e�B�u�̎��, �I�t�Z�b�g(���_��), �v���~�e�B�u��)
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[�o�C���h����
//
////////////////////////////////////////////////
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[���[�h����
//
////////////////////////////////////////////////
void CScene3D::LoadTexture(LPCSTR FileName)
{
	if(m_pTexture == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
		if(FAILED( D3DXCreateTextureFromFile( pDevice, 
		FileName, 
		&m_pTexture ) ) ) 
		{		
			MessageBox(NULL,"�e�N�X�`���̏������Ɏ��s���܂����B","�G���[", MB_OK);
		}
		m_bLoadTexture = true;
	}
}

