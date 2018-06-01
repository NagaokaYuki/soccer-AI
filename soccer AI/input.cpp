//=============================================================================
//
// ���͏��� [input.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "input.h"

///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define	COUNT_WAIT_REPEAT	(20)	// ���s�[�g�҂�����

///////////////////////////////////////////////
//
//		�ÓI�����o�ϐ�
//
////////////////////////////////////////////////
LPDIRECTINPUT8	CInput::m_pDInput = NULL;	// DirectInput�I�u�W�F�N�g

///////////////////////////////////////////////
//
//		CInput�̃R���X�g���N�^
//
////////////////////////////////////////////////
CInput::CInput()
{
	m_pDIDevice = NULL;
}

///////////////////////////////////////////////
//
//		CInput�̃f�X�g���N�^
//
////////////////////////////////////////////////
CInput::~CInput()
{
}

///////////////////////////////////////////////
//
//		���͏����̏���������
//
////////////////////////////////////////////////
HRESULT CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̍쐬
	if(!m_pDInput)
	{
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&m_pDInput, NULL);
	}

	return hr;
}

///////////////////////////////////////////////
//
//		���͏����̍X�V����
//
////////////////////////////////////////////////
#if 0	// ---> �������z�֐���
HRESULT CInput::Update(void)
{
	return S_OK;
}
#endif

///////////////////////////////////////////////
//
//		���͏����̏I������
//
////////////////////////////////////////////////
void CInput::Uninit(void)
{
	if(m_pDIDevice)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		m_pDIDevice->Unacquire();

		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}

	if(m_pDInput)
	{// DirectInput�I�u�W�F�N�g�̊J��
		m_pDInput->Release();
		m_pDInput = NULL;
	}
}

///////////////////////////////////////////////
//
//		CInputKeyboard�̃R���X�g���N�^
//
////////////////////////////////////////////////
CInputKeyboard::CInputKeyboard()
{
	// �e���[�N�̃N���A
	ZeroMemory(m_aKeyState, sizeof m_aKeyState);
	ZeroMemory(m_aKeyStateTrigger, sizeof m_aKeyStateTrigger);
	ZeroMemory(m_aKeyStateRelease, sizeof m_aKeyStateRelease);
	ZeroMemory(m_aKeyStateRepeat, sizeof m_aKeyStateRepeat);
	ZeroMemory(m_aKeyStateRepeatCnt, sizeof m_aKeyStateRepeatCnt);
}

///////////////////////////////////////////////
//
//		CInputKeyboard�̃f�X�g���N�^
//
////////////////////////////////////////////////
CInputKeyboard::~CInputKeyboard()
{
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�̏���������
//
////////////////////////////////////////////////
HRESULT CInputKeyboard::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init(hInst, hWnd);

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if(FAILED(hr) || m_pDIDevice == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// m_pDIDevice�A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h���͏����̊J��
//
////////////////////////////////////////////////
void CInputKeyboard::Uninit(void)
{
	// ���͏����̊J��
	CInput::Uninit();
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�̍X�V����
//
////////////////////////////////////////////////
HRESULT CInputKeyboard::Update(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	if(!m_pDIDevice)
	{
		return S_FALSE;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState(sizeof(aKeyState), aKeyState);
	if(SUCCEEDED(hr))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey] = m_aKeyStateTrigger[nCntKey];

			if(aKeyState[nCntKey])
			{
				m_aKeyStateRepeatCnt[nCntKey]++;
				if(m_aKeyStateRepeatCnt[nCntKey] >= COUNT_WAIT_REPEAT)
				{
					m_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCnt[nCntKey] = 0;
				m_aKeyStateRepeat[nCntKey] = 0;
			}

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�f�[�^�擾(�v���X)
//
////////////////////////////////////////////////
BOOL CInputKeyboard::GetKeyPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? TRUE: FALSE;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�f�[�^�擾(�g���K�[)
//
////////////////////////////////////////////////
BOOL CInputKeyboard::GetKeyTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? TRUE: FALSE;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�f�[�^�擾(�����[�X)
//
////////////////////////////////////////////////
BOOL CInputKeyboard::GetKeyRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? TRUE: FALSE;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�f�[�^�擾(���s�[�g)
//
////////////////////////////////////////////////
BOOL CInputKeyboard::GetKeyRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? TRUE: FALSE;
}

///////////////////////////////////////////////
//
//		�L�[�{�[�h�f�[�^�폜(�g���K�[)
//
////////////////////////////////////////////////
void CInputKeyboard::FlushKeyTrigger(int nKey)
{
	m_aKeyStateTrigger[nKey] = 0;
}

//////////////////////////////////
//
// �}�E�X�̃R���X�g���N�^
//
//////////////////////////////////
CInputMouse::CInputMouse()
{
	ZeroMemory( &m_mouseState, sizeof( m_mouseState ) );
	ZeroMemory( &m_mouseStateTrigger, sizeof( m_mouseStateTrigger ) );
}

//////////////////////////////////
//
// �}�E�X�̃f�X�g���N�^
//
//////////////////////////////////
CInputMouse::~CInputMouse()
{
}

//////////////////////////////////
//
// �}�E�X�̏���������
//
//////////////////////////////////
HRESULT CInputMouse::Init( HINSTANCE hInst, HWND hWnd )
{
	HRESULT hr;

	// ���͏����̏�����
	CInput::Init( hInst, hWnd );

	// �f�o�C�X�I�u�W�F�N�g�̍쐬
	hr = m_pDInput->CreateDevice( GUID_SysMouse, &m_pDIDevice, NULL );

	if( FAILED( hr ) || m_pDIDevice == NULL )
	{
		MessageBox( hWnd, "�}�E�X���Ȃ��I", "�x��", MB_ICONHAND );
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat( &c_dfDIMouse2 );

	if( FAILED(hr) )
	{
		MessageBox( hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���B", "�x��", MB_ICONHAND );
		return hr;
	}

	// �������[�h��ݒ�( �t�H�A�O�����h&��r�����[�h)
	hr = m_pDIDevice->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

	if( FAILED(hr) )
	{
		MessageBox( hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���B", "�x��", MB_ICONHAND );
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof( dipdw );
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;

	hr = m_pDIDevice->SetProperty( DIPROP_AXISMODE, &dipdw.diph );
	
	// �}�E�X�ւ̃A�N�Z�X�����l��( ���͐���J�n)
	m_pDIDevice->Acquire();

	return hr;
}

//////////////////////////////////
//
// �}�E�X�̏I������
//
//////////////////////////////////
void CInputMouse::Uninit( void )
{
	// ���͏����̊J��
	CInput::Uninit();
}

//////////////////////////////////
//
// �}�E�X�̍X�V����
//
//////////////////////////////////
HRESULT CInputMouse::Update( void )
{
	HRESULT hr;
	DIMOUSESTATE2 mouseState;

	if( !m_pDIDevice )
	{
		return S_FALSE;
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_pDIDevice->GetDeviceState( sizeof( mouseState ), &mouseState );

	if( SUCCEEDED(hr) )
	{
		// �g���K�[
		m_mouseStateTrigger.lX = ( ( m_mouseState.lX ^ mouseState.lX ) &mouseState.lX );
		m_mouseStateTrigger.lY = ( ( m_mouseState.lY ^ mouseState.lY ) &mouseState.lY );
		m_mouseStateTrigger.lZ = ( ( m_mouseState.lZ ^ mouseState.lZ ) &mouseState.lZ );

		// �����[�X
		m_mouseStateRelese.lX = ( ( m_mouseState.lX ^ mouseState.lX ) &m_mouseState.lX );
		m_mouseStateRelese.lY = ( ( m_mouseState.lY ^ mouseState.lY ) &m_mouseState.lY );
		m_mouseStateRelese.lZ = ( ( m_mouseState.lZ ^ mouseState.lZ ) &m_mouseState.lZ );

		for( int nCntKey = 0; nCntKey < 8; nCntKey++ )
		{
			m_mouseStateTrigger.rgbButtons[nCntKey] = ( ( m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey] )&mouseState.rgbButtons[nCntKey] );
			m_mouseStateRelese.rgbButtons[nCntKey] = ( ( m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey] )&m_mouseState.rgbButtons[nCntKey] );	
		}

		m_mouseState = mouseState;

		// �X�N���[�����W���擾
		GetCursorPos( &m_posMouseWorld );
	}
	else
	{
		m_pDIDevice->Acquire();
	}

	return hr;
}

//////////////////////////////////
//
// �}�E�X�����̓g���K�[
//
//////////////////////////////////
BOOL CInputMouse::GetLeftTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[ 0 ] & 0x80 ) ? true:false;
}

//////////////////////////////////
//
// �}�E�X�E���̓g���K�[
//
//////////////////////////////////
BOOL CInputMouse::GetRightTrigger( void )
{
	return ( m_mouseStateTrigger.rgbButtons[ 1 ] & 0x80 ) ? true:false;
}

//////////////////////////////////
//
// �}�E�X�����̓v���X
//
//////////////////////////////////
BOOL CInputMouse::GetLeftPress( void )
{
	return ( m_mouseState.rgbButtons[ 0 ] & 0x80 ) ? true:false;
}

//////////////////////////////////
//
// �}�E�X�E���̓v���X
//
//////////////////////////////////
BOOL CInputMouse::GetRightPress( void )
{
	return ( m_mouseState.rgbButtons[ 1 ] & 0x80 ) ? true:false;
}

LONG CInputMouse::GetAxisX( void )
{
	return m_mouseState.lX;
}

LONG CInputMouse::GetAxisY( void )
{
	return m_mouseState.lY;
}

LONG CInputMouse::GetAxisZ( void )
{
	return m_mouseState.lZ;
}

