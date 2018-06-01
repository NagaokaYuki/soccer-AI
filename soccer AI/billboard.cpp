//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "modegame.h"
#include "manager.h"
#include "billboard.h"
#include "renderer.h"
#include "camera.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define TEXTUREBILBOARD_U				(1.0f)
#define TEXTUREBILBOARD_V				(1.0f)

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
CBillboard *CBillboard::Create(void)
{
	CBillboard *pBillboard;
	pBillboard = new CBillboard(3);
	pBillboard->Init();
	return pBillboard;
}
///////////////////////////////////////////////
//
//		�R���X�g���N�^
//
////////////////////////////////////////////////
CBillboard::CBillboard(int Priority):CScene(Priority)
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
CBillboard::~CBillboard()
{
}

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CBillboard::Init(void)
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
	pVtx[0].pos = D3DXVECTOR3(0.0f - m_Size.x * 0.5f, 0.0f + m_Size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + m_Size.x * 0.5f, 0.0f + m_Size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - m_Size.x * 0.5f, 0.0f - m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + m_Size.x * 0.5f, 0.0f - m_Size.y, 0.0f);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �x�N�g��
	pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
void CBillboard::Uninit(void)
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
	CScene::Release();
}

///////////////////////////////////////////////
//
//		�X�V����
//
////////////////////////////////////////////////
void CBillboard::Update(void)
{
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CBillboard::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	D3DXMATRIX mtxView = CModeGame::GetCamera()->ViewCamera();
	D3DXMATRIX mtxViewInverse;

	//���C�g����
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );
	
	//�t�s������
	D3DXMatrixInverse(&mtxViewInverse, NULL, &mtxView);
	 
	mtxViewInverse._41 = 0.0f;
	mtxViewInverse._42 = 0.0f;
	mtxViewInverse._43 = 0.0f;

																																	   
	D3DXMatrixMultiply(&m_mtxWorld,  &mtxViewInverse, &m_mtxWorld);							// �|���Z�̎d��

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxTranslation, &m_mtxWorld);						// �|���Z�̎d��
																						   
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);										// �f�o�C�X�ɐݒ�

	//���e�X�g�L��
	pDevice -> SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );	
	pDevice -> SetRenderState( D3DRS_ALPHAREF , 1 );
	pDevice -> SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

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

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);										//���C�g�I�t
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[�o�C���h����
//
////////////////////////////////////////////////
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

///////////////////////////////////////////////
//
//		�e�N�X�`���[���[�h����
//
////////////////////////////////////////////////
void CBillboard::LoadTexture(LPCSTR FileName)
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
