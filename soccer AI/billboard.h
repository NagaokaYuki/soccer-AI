//=============================================================================
//
// �v���C���[���� [billboard.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

///////////////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////////////////
#include "scene.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////


///////////////////////////////////////////////
//
//		�\���̒�`
//
////////////////////////////////////////////////
class CBillboard : public CScene
{
public:
	CBillboard(int Priority);								// �R���X�g���N�^
	~CBillboard();											// �f�X�g���N�^
	static CBillboard *Create(void);						// �I�u�W�F�N�g��������
	void Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// �o�C���h�e�N�X�`���[����
	void LoadTexture(LPCSTR FileName);						// �e�N�X�`�����[�h����
	D3DXVECTOR3 GetPos(void){return m_Pos;}					// �v���C���[�̍��W���擾
private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	bool m_bLoadTexture;									// �e�N�X�`�����ǂݍ��܂ꂽ���ǂ���

protected:
	D3DXMATRIX m_mtxWorld;									// ���[���h�ϐ�
	D3DXMATRIX m_mtxRot;									// ��]
	D3DXMATRIX m_mtxTranslation;							// ���s
	D3DXMATRIX m_mtxScall;									// �g��
	D3DXVECTOR3 m_Pos;										// ���s�ړ��x�N�g��
	D3DXVECTOR2 m_Tex;										// �e�N�X�`�����W
	D3DXVECTOR3 m_Rot;										// ��]�x�N�g��
	D3DXVECTOR3 m_Scl;										// �g��x�N�g��
	D3DXVECTOR3 m_Size;										// 2D�I�u�W�F�N�̕��A����
	
	D3DCOLOR m_color;
	D3DXVECTOR3 m_Move;
};

#endif