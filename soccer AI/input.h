//=============================================================================
//
// 入力処理 [input.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define	NUM_KEY_MAX		(256)	// キー最大数

///////////////////////////////////////////////
//
//		入力クラス
//
////////////////////////////////////////////////
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;		// ---> 純粋仮想関数化

protected:
	static LPDIRECTINPUT8 m_pDInput;	// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8 m_pDIDevice;	// Deviceオブジェクト(入力に必要)
};

///////////////////////////////////////////////
//
//		キーボード入力クラス
//
////////////////////////////////////////////////
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);

private:
	BYTE	m_aKeyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
	BYTE	m_aKeyStateTrigger[NUM_KEY_MAX];	// トリガーワーク
	BYTE	m_aKeyStateRelease[NUM_KEY_MAX];	// リリースワーク
	BYTE	m_aKeyStateRepeat[NUM_KEY_MAX];		// リピートワーク
	int		m_aKeyStateRepeatCnt[NUM_KEY_MAX];	// リピートカウンタ
};

//////////////////////////////////
//
// CInputMouseクラス
//
//////////////////////////////////
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init( HINSTANCE hInst, HWND hWnd );
	void Uninit( void );
	HRESULT Update( void );

	BOOL GetLeftPress( void );
	BOOL GetLeftTrigger( void );
	BOOL GetRightPress( void );
	BOOL GetRightTrigger( void );
	LONG GetAxisX( void );
	LONG GetAxisY( void );
	LONG GetAxisZ( void );
	//POINT *GetPosWorld( void ){ return &m_posMouseWorld; }

private:
	DIMOUSESTATE2 m_mouseState;
	DIMOUSESTATE2 m_mouseStateTrigger;
	DIMOUSESTATE2 m_mouseStateRelese;

	POINT m_posMouseWorld;
};
#endif