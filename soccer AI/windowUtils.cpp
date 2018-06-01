/*=======================================================================

	�E�B���h�E���[�e�B���e�B [windowUtils.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "windowUtils.h"
#include <windows.h>
#include "vector2D.h"
#include "utils.h"
#include "stream_Utility_functions.h"

/////////////////////////////////////////////////////////
//
//	ChangeMenuState�֐�
//	
//	���ڎ��ʎq�A�ړI�̏�ԁA����у��j���[���L�҂�HWND��
//	�^����ꂽ�Ƃ��ɁA���j���[���ڂ̏�Ԃ�ύX
//
/////////////////////////////////////////////////////////
void ChangeMenuState(HWND hwnd, UINT MenuItem, UINT state)
{
  MENUITEMINFO mi;

  mi.cbSize = sizeof(MENUITEMINFO);
  mi.fMask  = MIIM_STATE;
  mi.fState = state;

  SetMenuItemInfo(GetMenu(hwnd), MenuItem, false, &mi);
  DrawMenuBar(hwnd);
}

/////////////////////////////////////////////////////////
//
//	CheckMenuItemAppropriately�֐�
//	
//	b���^�̏ꍇ��MenuItem���`�F�b�N����A
//	�����łȂ��ꍇ�̓`�F�b�N����Ȃ�
//
/////////////////////////////////////////////////////////
void CheckMenuItemAppropriately(HWND hwnd, UINT MenuItem, bool b)
{
  if (b)
  {
    ChangeMenuState(hwnd, MenuItem, MFS_CHECKED);
  }
  else
  {
    ChangeMenuState(hwnd, MenuItem, MFS_UNCHECKED);
  }
}

/////////////////////////////////////////////////////////
//
//	CheckBufferLength�֐�
//	
//	StringCchLength�֐��̑���Ɏg�p����܂��B
//	�c�[���o�[�̕�������`�F�b�N���邽�߂ɂ̂ݎg�p����܂�
//
/////////////////////////////////////////////////////////
bool CheckBufferLength(char* buff, int MaxLength, int& BufferLength)
{
  std::string s = ttos(buff);

  BufferLength = s.length();

  if (BufferLength > MaxLength)
  {
    BufferLength = 0; return false;
  }  

  return true;
}

void ErrorBox(std::string& msg)
{
  MessageBox(NULL, msg.c_str(), "Error", MB_OK);
}

void ErrorBox(char* msg)
{
  MessageBox(NULL, msg, "Error", MB_OK);
}

// �A�N�e�B�u�E�B���h�E����ɂ����J�[�\���̍��W���擾
Vector2D GetClientCursorPosition()
{
  POINT MousePos;

  GetCursorPos(&MousePos);

  ScreenToClient(GetActiveWindow(), &MousePos);

  return POINTtoVector(MousePos);
}

 
Vector2D GetClientCursorPosition(HWND hwnd)
{
  POINT MousePos;

  GetCursorPos(&MousePos);

  ScreenToClient(hwnd, &MousePos);

  return POINTtoVector(MousePos);
}


/////////////////////////////////////////////////////////
//	
//	�N���C�A���g���t�@�C���_�C�A���O�̋��ʃR���g���[�����g�p
//  
//  
/////////////////////////////////////////////////////////
void FileInitialize (HWND hwnd,
                     OPENFILENAME& ofn,
                     const std::string& defaultFileTypeDescription,
                     const std::string& defaultFileExtension)
{
  std::string filter = defaultFileTypeDescription + '\0' + "*." + defaultFileExtension + '\0' +
                       "All Files (*.*)" + '\0' + "*.*" + '\0' + '\0';
  
   static TCHAR szFilter[255];

   for (unsigned int i=0; i<filter.size(); ++i)
   {
     szFilter[i] = filter.at(i);
   }
     
     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hwnd ;
     ofn.hInstance         = NULL ;
     ofn.lpstrFilter       = szFilter ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = NULL ;          // �I�[�v������уN���[�Y�@�\�Őݒ�
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // �I�[�v������уN���[�Y�@�\�Őݒ�
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle        = NULL ;
     ofn.Flags             = 0 ;             // �I�[�v������уN���[�Y�@�\�Őݒ�
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt       = defaultFileExtension.c_str() ;
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;

}



BOOL FileOpenDlg (HWND               hwnd,
                  PTSTR              pstrFileName,
                  PTSTR              pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension)
{
     OPENFILENAME ofn;

     FileInitialize(hwnd, ofn, defaultFileTypeDescription, defaultFileExtension);
  
     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
     
     return GetOpenFileName (&ofn) ;
}

BOOL FileSaveDlg (HWND               hwnd,
                  PTSTR              pstrFileName,
                  PTSTR              pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension)
{
     OPENFILENAME ofn; FileInitialize(hwnd, ofn, defaultFileTypeDescription, defaultFileExtension);

     ofn.hwndOwner         = hwnd ;
     ofn.lpstrFile         = pstrFileName ;
     ofn.lpstrFileTitle    = pstrTitleName ;
     ofn.Flags             = OFN_OVERWRITEPROMPT ;
     
     return GetSaveFileName (&ofn) ;
}

/////////////////////////////////////////////////////////
//	
//	ResizeWindow�֐�
//  �A�N�e�B�u�E�B���h�E�̃T�C�Y��
//	�w�肳�ꂽ�T�C�Y�ɕύX����ɂ́A������Ăяo��
//  
/////////////////////////////////////////////////////////
void ResizeWindow(HWND hwnd, int cx, int cy)
{
  //does this window have a menu. If so set a flag to true
  HMENU hwndMenu = GetMenu(hwnd);
  bool bMenu = false;
  if (hwndMenu) bMenu = true;

  // ��]�̃E�B���h�E�T�C�Y�̋�`���쐬
  RECT DesiredSize;
  DesiredSize.left = 0;
  DesiredSize.top  = 0;
  DesiredSize.right = cx;
  DesiredSize.bottom = cy;

  // �E�B���h�E���]�܂����N���C�A���g�̈�ɗ^������ׂ��T�C�Y������
  AdjustWindowRectEx(&DesiredSize,
                     WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                     bMenu,
                     NULL);

  // �t�B�b�g����悤�ɃE�B���h�E�̃T�C�Y��ύX
  SetWindowPos(hwnd,
               NULL,
               GetSystemMetrics(SM_CXSCREEN)/2 - cx/2,
               GetSystemMetrics(SM_CYSCREEN)/2 - cy/2,
               DesiredSize.right,
               DesiredSize.bottom,
               SWP_NOZORDER);
}

/////////////////////////////////////////////////////////
//	
//	GetWindowHeight�֐�
//  
/////////////////////////////////////////////////////////
int  GetWindowHeight(HWND hwnd)
{
  if (hwnd == 0) return 0;

  RECT windowRect;

  GetWindowRect(hwnd, &windowRect);

  return windowRect.bottom - windowRect.top;
}

/////////////////////////////////////////////////////////
//	
//	GetWindowWidth�֐�
//  
/////////////////////////////////////////////////////////
int  GetWindowWidth(HWND hwnd)
{
  if (hwnd == 0) return 0;

  RECT windowRect;

  GetWindowRect(hwnd, &windowRect);

  return windowRect.right - windowRect.left;
}