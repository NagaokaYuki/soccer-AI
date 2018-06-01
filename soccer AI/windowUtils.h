/*=======================================================================

	ウィンドウズユーティリティ[windowUtils.h]

												
												長岡佑樹

=========================================================================*/
#ifndef WINDOW_UTILS_H
#define WINDOW_UTILS_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <string>

struct Vector2D;

// これを呼び出してクライアントウィンドウをリフレッシュする
inline void RedrawWindow(HWND hwnd, bool RedrawBackGround = true)
{
  InvalidateRect(hwnd, NULL, RedrawBackGround);
  UpdateWindow(hwnd);
}

// これを呼び出してクライアントウィンドウをリフレッシュする
inline void RedrawWindowRect(HWND hwnd, bool RedrawBackGround, RECT& RedrawArea)
{
  InvalidateRect(hwnd, &RedrawArea, RedrawBackGround);
  UpdateWindow(hwnd);
}

// 項目識別子、目的の状態、およびメニュー所有者のHWNDが与えられたときに、
// メニュー項目の状態を変更します。
void ChangeMenuState(HWND hwnd, UINT MenuItem, UINT state);

// bが真の場合はMenuItemがチェックされ、そうでない場合はチェックされない
void CheckMenuItemAppropriately(HWND hwnd, UINT MenuItem, bool b);


// プラットフォームSDKにある
// StringCchLength関数の代わり
// ツールバーの文字列をチェックするためにのみ使用
bool CheckBufferLength(char* buff, int MaxLength, unsigned int& BufferLength);

void ErrorBox(std::string& msg);
void ErrorBox(char* msg);

// アクティブなウィンドウを基準にしたカーソルの座標を取得
Vector2D GetClientCursorPosition();

// アクティブなウィンドウを基準にしたカーソルの座標を取得
Vector2D GetClientCursorPosition(HWND hwnd);


// Mr Petzoldの2つの便利な機能。
// ファイル名を取得するための共通のダイアログボックスを開く
void FileInitialize (HWND hwnd,
                     OPENFILENAME& ofn,
                     const std::string& defaultFileTypeDescription,
                     const std::string& defaultFileExtension);

BOOL FileOpenDlg (HWND               hwnd,
                  PTSTR              pstrFileName,
                  PTSTR              pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension);

BOOL FileSaveDlg (HWND hwnd,
                  PTSTR pstrFileName,
                  PTSTR pstrTitleName,
                  const std::string& defaultFileTypeDescription,
                  const std::string& defaultFileExtension);

// 指定されたウィンドウのサイズを指定されたサイズに変更するには、これを呼び出す
void ResizeWindow(HWND hwnd, int cx, int cy);

int  GetWindowHeight(HWND hwnd);
int  GetWindowWidth(HWND hwnd);

#endif