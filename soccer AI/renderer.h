//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

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
class CRenderer
{
public:
	CRenderer();							//�R���X�g���N�^
	~CRenderer();							//�f�X�g���N�^
	
	static HRESULT Init(HWND hWnd, bool bWindow);	//����������
	static void Uninit(void);						//�I������
	static void Update(void);						//�X�V����
	static void BeginDraw(void);					//�`�揈��
	static void EndDraw(void);
	static LPDIRECT3DDEVICE9 GetDevice(void);		//�f�o�C�X�擾����

private:
	static LPDIRECT3D9				m_pD3D;			// Direct3D�I�u�W�F�N�g
	static LPDIRECT3DDEVICE9		m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
	LPD3DXFONT m_pFont;					// �t�H���g�ւ̃|�C���^
#endif
};
#endif