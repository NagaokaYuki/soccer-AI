/*=======================================================================

	�E�B���h�E�\������ [scene.cpp]

												AT-13C-245
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "scene.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		�ÓI�����o�ϐ�
//
////////////////////////////////////////////////
int CScene :: m_nNumScene = 0;
CScene *CScene :: m_apScene[MAX_SCENE] = {};
CScene *CScene :: m_Top[PRIORITY] = {};

/////////////////////////////////////////////////////////
//
//
//	�I�u�W�F�N�g�̃R���X�g���N�^
//	
//
/////////////////////////////////////////////////////////
CScene::CScene(int Priority)
{
	if(m_Top[Priority] == NULL)
	{
		m_Top[Priority] = this;
	}
	else
	{
		CScene *pScene = m_Top[Priority];

		while(pScene->m_Next != NULL)
		{
			pScene = pScene->m_Next;
		}
		pScene->m_Next = this;
	}

	this->m_Next = NULL;
	m_Delete = false;
}

/////////////////////////////////////////////////////////
//
//
//	�I�u�W�F�N�g�̃f�X�g���N�^
//
//
/////////////////////////////////////////////////////////
CScene::~CScene()
{
}

///////////////////////////////////////////////
//
//		�I�u�W�F�N�g�̍X�V����
//
////////////////////////////////////////////////
void CScene::UpdateAll(void)
{
	for(int i = 0; i < PRIORITY; i++)
	{
		CScene *pScene = m_Top[i];
		CScene *pSceneNext;
		CScene *pScenePrev;

		while(pScene != NULL)
		{
			pScene->Update();
			pScene = pScene->m_Next;
		}

		pScene = m_Top[i];
		pScenePrev = m_Top[i];
	
		while(pScene != NULL)
		{
			pSceneNext = pScene->m_Next;

			if(pScene->m_Delete == true)
			{
				if(pScene == m_Top[i])
				{	m_Top[i] = pScene->m_Next;
					pScene->Uninit();
					delete pScene;
				}
				else
				{
					pScenePrev->m_Next = pScene->m_Next;
					pScene->Uninit();
					delete pScene;
				}
			}
			else
			{
				pScenePrev = pScene;
			}
			pScene = pSceneNext;
		}
	
	}
}

///////////////////////////////////////////////
//
//		�I�u�W�F�N�g�̕`�揈��
//
////////////////////////////////////////////////
void CScene::DrawAll(void)
{
	for(int i = 0; i< PRIORITY; i++)
	{
		CScene *pScene = m_Top[i];

		while(pScene != NULL)
		{
			pScene->Draw();
			pScene = pScene->m_Next;
		}
	}
}

///////////////////////////////////////////////
//
//		�I�u�W�F�N�g��S�Ĕj��
//
////////////////////////////////////////////////
void CScene::ReleaseAll(void)
{
	for(int i = 0; i < PRIORITY; i++)
	{
		CScene *pScene = m_Top[i];
		CScene *pSceneNext;

		while(pScene != NULL)
		{
			pSceneNext = pScene->m_Next;
			pScene->Uninit();
			delete pScene;
			pScene = pSceneNext;
		}

		m_Top[i] = NULL;
	}
}

///////////////////////////////////////////////
//
//		�I�u�W�F�N�g��j��
//
////////////////////////////////////////////////
void CScene::Release(void)
{
	m_Delete = true;		// �폜�t���O
}

////////////////////////////////////////////
//
//	�I�u�W�F�N�g�^�C�v�̃C���f�b�N�X�擾����
//
///////////////////////////////////////////
CScene *CScene::GetScene(int nIdxScene)
{
	return m_apScene[nIdxScene];
}

/////////////////////////////////////////////////////////
//
//
//	2D�I�u�W�F�N�̃f�X�g���N�^
//
//
/////////////////////////////////////////////////////////
D3DXVECTOR3 CScene::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

////////////////////////////////////////////
//
//		�I�u�W�F�N�g�^�C�v�̃Z�b�g����
//
///////////////////////////////////////////
void CScene::SetObjType(CScene::OBJTYPE objType)
{
	m_objType = objType;
}

////////////////////////////////////////////
//
//	�I�u�W�F�N�g�^�C�v�̃C���f�b�N�X�擾����
//
///////////////////////////////////////////
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

CScene *CScene::GetList(int Priority)
{
	return m_Top[Priority];
}