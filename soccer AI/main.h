/*=======================================================================

	�E�B���h�E�\������ [main.h]

												
												�����C��

=========================================================================*/
//////////////////////////////////
//
//�w�b�_�t�@�C��
//
//////////////////////////////////
#ifndef _MAIN_H_
#define _MAIN_H_

///////////////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////////////////
#include "windows.h"
#include "d3dx9.h"
#define DIRECTINPUT_VERSION				(0x0800)
#include <dinput.h>
#include <XAudio2.h>
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////
//
//		���C�u�����t�@�C��
//		[�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
////////////////////////////////////////////////
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
																	   
////////////////////////////////////
//
//�萔��`
//
////////////////////////////////////
// ���_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g
// ���_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH		(700)											// �E�B���h�E�̕�
#define SCREEN_HEIGHT		(400)											// �E�B���h�E�̍���
																			   
#define NUM_VERTEX			(4)												// ���_��
#define NUM_POLYGON			(2)												// �|���S����
//#define SAFE_RELEASE(x){if(x)	{(x)->Release();	(x) = NULL}	}
const int TeamSize = 5;

/////////////////////////////////////////////////////////
//
//
//	���_���\����
//	��:(��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��)
//
/////////////////////////////////////////////////////////
//	��:(��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��)
typedef struct
{
	D3DXVECTOR3 pos;						// �\�����W
	D3DXVECTOR3 normal;						// �@��
	D3DCOLOR color;							// �F
	D3DXVECTOR2 tex;						// �e�N�X�`��
}VERTEX_3D;									// �^��

/////////////////////////////////////////////////////////
//
//
//	���_���\����
//	��:(��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��)
//
/////////////////////////////////////////////////////////
typedef struct
{
	D3DXVECTOR3 pos;											// X,Y,Z
	float rhw;													// ���W�ϊ��ςݒ��_���W 
	D3DCOLOR color;												// ���_�J���[
	D3DXVECTOR2 tex;											// �e�N�X�`��
}VERTEX_2D;														// �^��

//////////////////////////////////
//
//�v���g�^�C�v�錾
//
//////////////////////////////////
LPDIRECT3DDEVICE9 GetDevice(void);		//Direct3DDevice�̎擾
int GetFPS(void);
/*------------------------------------------�I��---------------------------------------------------*/
#endif