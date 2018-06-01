/*=======================================================================

	�E�B���h�E�\������ [light.cpp]

												AT-13C-245
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//
//	����������
//
//
/////////////////////////////////////////////////////////
void CLight::Init(void)
{
	D3DLIGHT9 light;
	// �f�o�C�X�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();

	//���C�g�̐ݒ�
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���C�g�̐F
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);			// ���s�����̐F
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, -1.0, 0.0f);			// �����̐F(���C�g�x�N�g���̕\��)
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	// ���s�����̌���

	// �f�o�C�X���C�g��ݒ�
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
/////////////////////////////////////////////////////////
//
//
//	�I������
//
//
/////////////////////////////////////////////////////////
void CLight::Uninit(void)
{
}
/////////////////////////////////////////////////////////
//
//
//	�X�V����
//
//
/////////////////////////////////////////////////////////
void CLight::Update(void)
{
	
}
