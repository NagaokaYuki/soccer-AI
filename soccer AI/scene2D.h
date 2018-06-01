//=============================================================================
//
// �I�u�W�F�N�g2D�̏��� [scene2D.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

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
class CScene2D : public CScene
{
public:
	CScene2D(int Priority);												// �R���X�g���N�^
	~CScene2D();											// �f�X�g���N�^
	static CScene2D *Create(void);							// �I�u�W�F�N�g��������
	void Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// �o�C���h�e�N�X�`���[����
	void LoadTexture(LPCSTR FileName);						// �e�N�X�`�����[�h����
	void UnLoadTexture(void);
	D3DXVECTOR3 GetPos(void);								// �v���C���[�̍��W���擾
	void SetAnimation( D3DXVECTOR2 pos, D3DXVECTOR2 size );	// 
	void SetColor(D3DCOLOR color);							// 

private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	bool m_bLoadTexture;									// �e�N�X�`�����ǂݍ��܂ꂽ���ǂ���

protected:
	D3DXVECTOR3 m_pos;										// �\�����W
	D3DXVECTOR2 m_size;										// ��,����
	D3DXVECTOR2 m_tex;										// �e�N�X�`�����W
	D3DCOLOR m_color;										
};

#endif