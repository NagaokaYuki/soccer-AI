/*=======================================================================

	�E�B���h�E�\������ [scene.h]

												AT-13C-245
												�����C��

=========================================================================*/

/////////////////////////////////////////
//
//
//	�C���N���[�h�K�[�h
//
//
/////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
/////////////////////////////////////////
//
//
//		�}�N����`
//
//
/////////////////////////////////////////
#define	MAX_SCENE				(100)				// �I�u�W�F�N�g�̍ő吔
#define TEXTURE_U				(1.0f)
#define TEXTURE_V				(1.0f)
#define PRIORITY				(100)

/////////////////////////////////////////
//
//
//	�V�[���N���X
//
//
/////////////////////////////////////////
class CScene
{
public:
	// ��ނ̗񋓌^
	typedef enum
	{
		OBJTYPE_NONE = 0,					// ��ނȂ�
		OBJTYPE_PLAYER,						// �v���C���[
		OBJTYPE_ENEMY,						// �G
		OBJTYPE_BULLET,						// �e
		OBJTYPE_ITEM,						// ����
		OBJTYPE_RETICLE,					// ���e�B�N��
		OBJTYPE_PARTICLE,					// �p�[�e�B�N��
		OBJTYPE_MAX							// �ő吔
	}OBJTYPE;

	CScene(int Priority);								// �V�[���R���X�g���N�^
	~CScene();								// �V�[���f�X�g���N�^
											   
	virtual void Init(void) = 0;			// �V�[������������
	virtual void Uninit(void) = 0;			// �V�[���I������
	virtual void Update(void) = 0;			// �V�[���X�V����
	virtual void Draw(void) = 0;			// �V�[���`�揈��
	D3DXVECTOR3 GetPos(void);				// ���W�擾����
	static void UpdateAll(void);			// �I�u�W�F�N�g�̍X�V����
	static void DrawAll(void);				// �I�u�W�F�N�g�̕`�揈��
	static void ReleaseAll(void);			// �I�u�W�F�N�g��S�Ĕj��
	static CScene *GetScene(int nIdxScene);	// �V�[���擾����
	void SetObjType(OBJTYPE objType);		// �I�u�W�F�N�g�^�C�v�Z�b�g����
	OBJTYPE GetObjType(void);				// �I�u�W�F�N�g�^�C�v�擾����
	static CScene* GetList(int Priority);
	CScene* GetNext(CScene* pNext){return pNext->m_Next;}
protected:									   
	void Release(void);						// �I�u�W�F�N�g��j��
	
private:
	static CScene *m_Top[PRIORITY];
	CScene *m_Next;
	static CScene *m_apScene[MAX_SCENE];	// �I�u�W�F�N�g�ւ̃|�C���^
	static int m_nNumScene;					// ���I�u�W�F�N�g��
	bool m_Delete;
	int m_nID;								// �ŗLID
	OBJTYPE m_objType;						// �I�u�W�F�^�C�v�̃����o�ϐ�
};


/*-----------------------------------------------------------*/
#endif