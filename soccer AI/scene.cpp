/*=======================================================================

	ウィンドウ表示処理 [scene.cpp]

												AT-13C-245
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "scene.h"

/////////////////////////////////////////////////////////
//
//	定数定義
//
/////////////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		静的メンバ変数
//
////////////////////////////////////////////////
int CScene :: m_nNumScene = 0;
CScene *CScene :: m_apScene[MAX_SCENE] = {};
CScene *CScene :: m_Top[PRIORITY] = {};

/////////////////////////////////////////////////////////
//
//
//	オブジェクトのコンストラクタ
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
//	オブジェクトのデストラクタ
//
//
/////////////////////////////////////////////////////////
CScene::~CScene()
{
}

///////////////////////////////////////////////
//
//		オブジェクトの更新処理
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
//		オブジェクトの描画処理
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
//		オブジェクトを全て破棄
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
//		オブジェクトを破棄
//
////////////////////////////////////////////////
void CScene::Release(void)
{
	m_Delete = true;		// 削除フラグ
}

////////////////////////////////////////////
//
//	オブジェクトタイプのインデックス取得処理
//
///////////////////////////////////////////
CScene *CScene::GetScene(int nIdxScene)
{
	return m_apScene[nIdxScene];
}

/////////////////////////////////////////////////////////
//
//
//	2Dオブジェクのデストラクタ
//
//
/////////////////////////////////////////////////////////
D3DXVECTOR3 CScene::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

////////////////////////////////////////////
//
//		オブジェクトタイプのセット処理
//
///////////////////////////////////////////
void CScene::SetObjType(CScene::OBJTYPE objType)
{
	m_objType = objType;
}

////////////////////////////////////////////
//
//	オブジェクトタイプのインデックス取得処理
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