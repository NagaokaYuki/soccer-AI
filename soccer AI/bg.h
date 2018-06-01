//=============================================================================
//
// �v���C���[���� [polygon2D.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_

///////////////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////////////////
#include "scene2D.h"

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
class CBg : public CScene
{
public:
	// �w�i�^�C�v
	typedef enum
	{
		BG_TYPE_NONE = 0,
		BG_TYPE_TITLE,
		BG_TYPE_GAME,
		BG_TYPE_RESULT
	}BGTYPE;

	CBg(int Priority);										// �R���X�g���N�^
	~CBg();													// �f�X�g���N�^
	static CBg *Create(BGTYPE BgType);						// �I�u�W�F�N�g��������
	void Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);			// �o�C���h�e�N�X�`���[����
	void LoadTexture(LPCSTR FileName);						// �e�N�X�`�����[�h����

private:
	LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	bool m_bLoadTexture;									// �e�N�X�`�����ǂݍ��܂ꂽ���ǂ���
	D3DXVECTOR3 m_pos;										// �\�����W
	D3DXVECTOR2 m_size;										// ��,����
	D3DXVECTOR2 m_tex;										// �e�N�X�`�����W
	D3DCOLOR m_color;										
};

#endif