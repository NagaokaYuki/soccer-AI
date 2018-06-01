//=============================================================================
//
// �}�l�[�W���[�̏��� [manager.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CModeTitle;
class CMode;
class CDebug;
class CSound;
///////////////////////////////////////////////
//
//		�萔��`
//
////////////////////////////////////////////////
#define POLYGON_WIDTH			(100.0f)

///////////////////////////////////////////////
//
//		�}�l�[�W���[�N���X
//
////////////////////////////////////////////////
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);				// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��																		   
	static CInputKeyboard *GetKeyboard(void);								// �L�[�{�[�h�̏��擾����
	static CInputMouse *GetMouse(void){return m_pInputMouse;}				// �}�E�X�̏��擾����
	static void SetMode(CMode *mode);
private:																	
	static CInputKeyboard *m_pInputKeyboard;								// �L�[�{�[�h�ւ̃|�C���^ 
	static CInputMouse *m_pInputMouse;										// �}�E�X�ւ̃|�C���^ 
	static CMode* m_pMode;
	static CModeTitle *m_pModeTitle;
	static CSound* m_pSound;
	//static CDebug *m_pDebug;
	D3DXVECTOR3 m_pos;
};

#endif
