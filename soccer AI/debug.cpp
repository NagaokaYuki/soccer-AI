//=============================================================================
//
// �v���C���[���� [debug.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "debug.h"
#include "renderer.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�ÓI�����o�ϐ�
//
////////////////////////////////////////////////
char CDebug::m_Text[1024] = {};
LPD3DXFONT CDebug::m_pFont = NULL;

///////////////////////////////////////////////
//
//		����������
//
////////////////////////////////////////////////
void CDebug::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	D3DXCreateFont(pDevice,
			18,
			0,
			0,
			0,
			FALSE,
			SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			"Terminal",
			&m_pFont
			);
}

///////////////////////////////////////////////
//
//		�e�L�X�g�N���A
//
///////////////////////////////////////////////
void CDebug::ClearText(void)
{
	strcpy_s(m_Text, "");
}

///////////////////////////////////////////////
//
//		�I������
//
////////////////////////////////////////////////
void CDebug::AddText(char *Text)
{
	strcat_s(m_Text, Text);
}

///////////////////////////////////////////////
//
//		�`�揈��
//
////////////////////////////////////////////////
void CDebug::Draw(void)
{
	RECT rc = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	m_pFont->DrawText(NULL, m_Text, -1, &rc, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}

