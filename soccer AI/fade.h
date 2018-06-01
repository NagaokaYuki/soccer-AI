//=============================================================================
//
// �v���C���[���� [fade.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

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
class CFade : public CScene2D
{
public:
	typedef enum
	{
		FADE_NONE = 0,							// �Ȃ�
		FADE_IN,								// �t�F�[�h�C��
		FADE_OUT,								// �t�F�[�h�A�E�g
		FADE_MAX								// �t�F�[�h�̍ő吔
	}FADE;										// �^��

	CFade();									// �R���X�g���N�^
	~CFade();									// �f�X�g���N�^
	static void Load(void);
	static void UnLoad(void);
	static CFade *Create(void);					// �I�u�W�F�N�g��������
	void Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��
	void SetFade(FADE fade/*, MODE modeNext*/);	// �t�F�[�h�ݒ�֐�
	FADE GetFade(void);							// �t�F�[�h�擾�֐�

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`���̃|�C���^
	FADE m_fade;								// �񋓌^�̕ϐ�
	D3DXCOLOR m_colorFade;						// �J���[�̕ϐ�
};

#endif