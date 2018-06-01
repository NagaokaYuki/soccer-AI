//=============================================================================
//
// �i���o�[���� [number.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

///////////////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////////////////
#include "main.h"

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
class CNumber
{
public:
	CNumber();								// �R���X�g���N�^
	~CNumber();								// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��
	static CNumber *Create(void);			// �I�u�W�F�N�g��������
	static HRESULT Load(void);				// ���[�h����  
	static void Unload(void);				// �A�����[�h����
	void SetNumber(int nNumber);			// �Z�b�g����
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
											
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`�����L
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;
	int m_nNumber;
};

#endif