/*=======================================================================

	�f�o�b�O�R���\�[�� [debugconsole.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "DebugConsole.h"
#include <iterator>
#pragma warning (disable : 4786)

/////////////////////////////////////////////////////////
//
//	�ÓI�����o�ϐ�
//
/////////////////////////////////////////////////////////
std::vector<std::string> DebugConsole::m_Buffer;
HWND                     DebugConsole::m_hwnd       = NULL;
bool                     DebugConsole::m_bFlushed   = true;
bool                     DebugConsole::m_bDestroyed = false;
bool                     DebugConsole::m_bActive   = true;
std::ofstream            DebugConsole::m_LogOut;
int                      DebugConsole::m_iPosLeft;
int                      DebugConsole::m_iPosTop;

/////////////////////////////////////////////////////////
//
//	DebugWindowProc�֐�
//
/////////////////////////////////////////////////////////
LRESULT CALLBACK DebugConsole::DebugWindowProc(HWND hwnd, 
						                             UINT msg, 
                                         WPARAM wparam, 
                                         LPARAM lparam)
{
	// �N���C�A���g�E�B���h�E�̈�̐��@��ێ�
	static int cxClient, cyClient;

	// �t�H���g�̐��@
	static int cxChar, cyChar, cxCaps, cyPage;
	
	int iVertPos;
	
	TEXTMETRIC  tm;
	SCROLLINFO  si;   
	
	// �N���C�A���g�E�B���h�E�̃T�C�Y���擾
	RECT rect;
	GetClientRect(hwnd, &rect);
	cxClient = rect.right;
	cyClient = rect.bottom;

	switch(msg)
	{	
		case WM_CREATE: 
		{
      // �t�H���g�����擾
      HDC hdc = GetDC(hwnd);

      GetTextMetrics(hdc, &tm);
      cxChar = tm.tmAveCharWidth;
      cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
      cyChar = tm.tmHeight + tm.tmExternalLeading;
     
      // �����X�N���[���o�[�͈̔͂ƃy�[�W�T�C�Y��ݒ�
      si.cbSize = sizeof (si) ;
      si.fMask  = SIF_RANGE | SIF_PAGE;
      si.nMin   = 0 ;
      si.nMax   = 10 ;
      si.nPage  = cyClient / cyChar;

      SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

      ReleaseDC(hwnd, hdc);

		} 
			
		break;

    case WM_KEYUP:
      {
        switch(wparam)
        {
           case VK_ESCAPE:
            {             
              SendMessage(hwnd, WM_DESTROY, NULL, NULL);
            }
          
            break;
        }
      }

      break;

    
    case WM_VSCROLL:
       
      // ���ׂĂ̐����X�N���[���o�[�����擾
      si.cbSize = sizeof (si);
      si.fMask  = SIF_ALL;
      GetScrollInfo (hwnd, SB_VERT, &si) ;

      // ��Ŕ�r�̂��߂Ɉʒu��ۑ�����
      iVertPos = si.nPos ;

      switch (LOWORD (wparam))
      {
        case SB_TOP:
             si.nPos = si.nMin ;
             break ;
               
        case SB_BOTTOM:
             si.nPos = si.nMax ;
             break ;
               
        case SB_LINEUP:
             si.nPos -= 1 ;
             break ;
               
        case SB_LINEDOWN:
             si.nPos += 1 ;
             break ;
               
        case SB_PAGEUP:
             si.nPos -= si.nPage ;
             break ;

        case SB_PAGEDOWN:
             si.nPos += si.nPage ;
             break ;
               
        case SB_THUMBTRACK:
             si.nPos = si.nTrackPos ;
             break ;
               
        default:
             break ;         
      }
          
      // �ʒu��ݒ肵�A���ɂ�����擾
      // Windows�ɂ�钲���̂��߁A�ݒ肳�ꂽ�l�ƈقȂ�ꍇ������
      si.fMask = SIF_POS ;
      SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
      GetScrollInfo (hwnd, SB_VERT, &si) ;

      // �ʒu���ύX����Ă���ꍇ�́A�E�B���h�E���X�N���[�����čX�V
      if (si.nPos != iVertPos)
      {                    
         ScrollWindow (hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL) ;
         UpdateWindow (hwnd) ;
         DrawWindow();
      }
      
      break ;
          

    case UM_SETSCROLL:
    {
      // ���ׂĂ̐����X�N���[���o�[�����擾
      si.cbSize = sizeof (si);
      si.fMask  = SIF_ALL;
      GetScrollInfo (hwnd, SB_VERT, &si);

      si.nMin = 0;
      si.nMax = m_Buffer.size();

      si.nPos += 1;

      si.fMask  = SIF_RANGE | SIF_POS;
      SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
                        
      ScrollWindow (hwnd, 0, cyChar * si.nPos, NULL, NULL);
      DrawWindow();     
    }  

    break;

		case WM_PAINT: 
		{
      PAINTSTRUCT ps;
      
		  BeginPaint(hwnd, &ps);

        SetBkMode(ps.hdc, TRANSPARENT);
        SetTextColor(ps.hdc, RGB(255,255,255));

        if (m_Buffer.size() > 1)
        {
     
          // �����X�N���[���o�[�̈ʒu���擾
          si.cbSize = sizeof (si) ;
          si.fMask  = SIF_POS ;
          GetScrollInfo (hwnd, SB_VERT, &si) ;
          iVertPos = si.nPos ;
     
          // ���̃y�[�W�Ɏ��܂�s��
          int PageSize = (int)(cyClient / cyChar) - 1;

          int StartIndex = 0;
      
          if (iVertPos > PageSize)
          {
            StartIndex = iVertPos - PageSize;
          }

          std::vector<std::string>::iterator beg = m_Buffer.begin() + StartIndex;
          std::vector<std::string>::iterator end = m_Buffer.begin() + StartIndex+PageSize+1;

          int line=0;

          for (beg; (beg !=end) && (beg != m_Buffer.end()); ++beg)
          {
            TextOut(ps.hdc, 0, cyChar*line++, (*beg).c_str(), (*beg).size()); 
          }
        }
    
			EndPaint(hwnd, &ps);
		} 
			
		break;

    case WM_SIZE:
      {
      }
      break;

		case WM_DESTROY: 
		{
      m_bDestroyed = true;  

			DestroyWindow(m_hwnd);
 		} 
			
		break;

		default:break;

	} // switch���I��

	if (!m_bDestroyed)
	{
	  return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	else
	{
	  return NULL;
	}

}


/////////////////////////////////////////////////////////
//
//	Create�֐�
//
/////////////////////////////////////////////////////////
bool DebugConsole::Create()
{
  m_hwnd       = NULL;
  m_iPosLeft   = 0;
  m_iPosTop    = 0;
  m_bFlushed   = true;

  // �I�[�v�����O�t�@�C��
  m_LogOut.open("DebugLog.txt");

  
  WNDCLASSEX wDebugConsole = {sizeof(WNDCLASSEX), 
                       CS_HREDRAW | CS_VREDRAW,
                       DebugWindowProc,
                       0,
                       0, 
                       GetModuleHandle(NULL),
						           NULL,
						           NULL,
						           (HBRUSH)(GetStockObject(GRAY_BRUSH)),
						           NULL,
						           "Debug",
						           NULL }; 


  // �E�B���h�E�N���X��o�^����
  if (!RegisterClassEx(&wDebugConsole))
  {
    MessageBox(NULL, "Registration of Debug Console Failed!", "Error", 0);

    // �A�v���P�[�V�������I������
    return false;
  }
	

  // �N���C�A���g�E�B���h�E�̃T�C�Y���擾
 // RECT rectActive;
 // GetClientRect(GetActiveWindow(), &rectActive);

	// ���E�B���h�E���쐬����
  m_hwnd = CreateWindow("Debug",
                            "Debug Console", 
									          WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU| WS_VSCROLL | WS_THICKFRAME,
                            0,
									          0,
                            DEBUG_WINDOW_WIDTH,
									          DEBUG_WINDOW_HEIGHT,
									          NULL,
									          NULL,
									          wDebugConsole.hInstance,
									          NULL );

    // �E�B���h�E�̍쐬�������������Ƃ��m�F
  if(!m_hwnd)
  {
    MessageBox(m_hwnd, "CreateWindowEx Failed!", "Error!", 0);

    return false;
  }
	
	// �E�B���h�E��\������
	UpdateWindow(m_hwnd);

  return true;

}

/////////////////////////////////////////////////////////
//
//	Instance�֐�
//
/////////////////////////////////////////////////////////
DebugConsole* DebugConsole::Instance()
{
   static DebugConsole instance;     
   static bool created = false;
   if ( !created)
   {Create();created = true;}
   
   return &instance;
}

/////////////////////////////////////////////////////////
//
//	WriteAndResetBuffer�֐�
//
/////////////////////////////////////////////////////////
void DebugConsole::WriteAndResetBuffer()
{
 
  m_iPosLeft   = 0;
  m_iPosTop    = 0;
  m_bFlushed   = true;
      
  // �o�b�t�@�̓��e���t�@�C���ɏ����o��
  std::vector<std::string>::iterator it = m_Buffer.begin();

  for (it; it != m_Buffer.end(); ++it)
  {
    m_LogOut << *it << std::endl;
  }

  m_Buffer.clear();

  SendMessage(m_hwnd, UM_SETSCROLL, NULL, NULL);
}