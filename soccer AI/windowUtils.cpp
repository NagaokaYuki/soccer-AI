/*=======================================================================

	ウィンドウユーティリティ [windowUtils.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "windowUtils.h"
#include <windows.h>
#include "vector2D.h"
#include "utils.h"
#include "stream_Utility_functions.h"

/////////////////////////////////////////////////////////
//
//	ChangeMenuState関数
//	
//	項目識別子、目的の状態、およびメニュー所有者のHWNDが
//	与えられたときに、メニュー項目の状態を変更
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
//	CheckMenuItemAppropriately関数
//	
//	bが真の場合はMenuItemがチェックされ、
//	そうでない場合はチェックされない
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
//	CheckBufferLength関数
//	
//	StringCchLength関数の代わりに使用されます。
//	ツールバーの文字列をチェックするためにのみ使用されます
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

// アクティブウィンドウを基準にしたカーソルの座標を取得
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
//	クライアントがファイルダイアログの共通コントロールを使用
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
     ofn.lpstrFile         = NULL ;          // オープンおよびクローズ機能で設定
     ofn.nMaxFile          = MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;          // オープンおよびクローズ機能で設定
     ofn.nMaxFileTitle     = MAX_PATH ;
     ofn.lpstrInitialDir   = NULL ;
     ofn.lpstrTitle        = NULL ;
     ofn.Flags             = 0 ;             // オープンおよびクローズ機能で設定
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
//	ResizeWindow関数
//  アクティブウィンドウのサイズを
//	指定されたサイズに変更するには、これを呼び出す
//  
/////////////////////////////////////////////////////////
void ResizeWindow(HWND hwnd, int cx, int cy)
{
  //does this window have a menu. If so set a flag to true
  HMENU hwndMenu = GetMenu(hwnd);
  bool bMenu = false;
  if (hwndMenu) bMenu = true;

  // 希望のウィンドウサイズの矩形を作成
  RECT DesiredSize;
  DesiredSize.left = 0;
  DesiredSize.top  = 0;
  DesiredSize.right = cx;
  DesiredSize.bottom = cy;

  // ウィンドウが望ましいクライアント領域に与えられるべきサイズを決定
  AdjustWindowRectEx(&DesiredSize,
                     WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                     bMenu,
                     NULL);

  // フィットするようにウィンドウのサイズを変更
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
//	GetWindowHeight関数
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
//	GetWindowWidth関数
//  
/////////////////////////////////////////////////////////
int  GetWindowWidth(HWND hwnd)
{
  if (hwnd == 0) return 0;

  RECT windowRect;

  GetWindowRect(hwnd, &windowRect);

  return windowRect.right - windowRect.left;
}