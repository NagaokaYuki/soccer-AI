/*=======================================================================
//
//	�|���S���\������ [modegame.h]
//
//												AT-12D-249
//												�����C��
//
//=========================================================================*/

////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////
#ifndef _MODEGAME_H_
#define _MODEGAME_H_

#include "main.h"
#include "mode.h"

////////////////////////////////////
//
//	�O���錾
//
////////////////////////////////////
class CRenderer;				// �����_���[�N���X
class CInputKeyboard;			// �L�[�{�[�h�N���X
class CCamera;					// �J�����N���X
class CLight;					// ���C�g�N���X
class CDebug;					// �f�o�b�O�N���X
class CInputMouse;				// �}�E�X�N���X
class CPolygon2D;				// �|���S��2D�N���X
class CScore;					// �X�R�A�N���X
class CBg;						// �w�i�N���X
class CTime;					// �^�C���N���X

////////////////////////////////////
//
//		ModeGame�N���X
//
////////////////////////////////////
class CModeGame :public CMode
{	
public:
	void Init  (void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw  (void);		// �`�揈��
	static bool SphereCollision(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float radius1, float radius2);		// ���̂����蔻��
	static CCamera *GetCamera(void);																	// �J�����擾����
	static void SetUse(bool bUse){m_bUse = bUse;}														// �t���O�Z�b�g����
	static CScore *GetScore(void){return m_pScore;}														// �X�R�A�擾����
	static bool GetUse(void){return m_bUse;}
	
private:			
	static CLight *m_pLight;						// ���C�g�̐ÓI�����o�ϐ�
	static CCamera *m_pCamera;						// �J�����̐ÓI�����o�ϐ�
	static CPolygon2D *m_pPolygon2D;				// �|���S���QD�̐ÓI�����o�ϐ�
	static bool m_bUse;								// �t���O�̐ÓI�����o�ϐ�
	static CScore *m_pScore;						// �X�R�A�̐ÓI�����o�ϐ�
	static CBg* m_pBg;								// �w�i�N���X�̐ÓI�����o�ϐ�
	static CTime* m_pTime;							// �^�C���N���X�̐ÓI�����o�ϐ�
};

///*------------------------------------------�I��---------------------------------------------------*/
#endif