/*=======================================================================

	�E�B���h�E�Y���[�e�B���e�B[windowUtils.h]

												
												�����C��

=========================================================================*/
#ifndef WINDOW_UTILS_H
#define WINDOW_UTILS_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <string>

struct Vector2D;

// ������Ăяo���ăN���C�A���g�E�B���h�E�����t���b�V������
inline void RedrawWindow(HWND hwnd, bool RedrawBackGround = true)
{
  InvalidateRect(hwnd, NULL, RedrawBackGround);
  UpdateWindow(hwnd);
}

// ������Ăяo���ăN���C�A���g�E�B���h�E�����t���b�V������
inline void RedrawWindowRect(HWND hwnd, bool RedrawBackGround, RECT& RedrawArea)
{
  InvalidateRect(hwnd, &RedrawArea, RedrawBackGround);
  UpdateWindow(hwnd);
}

// ���ڎ��ʎq�A�ړI�̏�ԁA����у��j���[���L�҂�HWND���^����ꂽ�Ƃ��ɁA
// ���j���[���ڂ̏�Ԃ�ύX���܂��B
void ChangeMenuState(HWND hwnd, UINT MenuItem, UINT state);

// b���^�̏ꍇ��MenuItem���`�F�b�N����A�����łȂ��ꍇ�̓`�F�b�N����Ȃ�
void CheckMenuItemAppropriately(HWND hwnd, UINT MenuItem, bool b);


// �v���b�g�t�H�[��SDK�ɂ���
// StringCchLength�֐��̑���
// �c�[���o�[�̕�������`�F�b�N���邽�߂ɂ̂ݎg�p
bool CheckBufferLength(char* buff, int MaxLength, unsigned int& BufferLength);

void ErrorBox(std::string& msg);
void ErrorBox(char* msg);

// �A�N�e�B�u�ȃE�B���h�E����ɂ����J�[�\���̍��W���擾
Vector2D GetClientCursorPosition();

// �A�N�e�B�u�ȃE�B���h�E����ɂ����J�[�\���̍��W���擾
Vector2D GetClientCursorPosition(HWND hwnd);


// Mr Petzold��2�֗̕��ȋ@�\�B
// �t�@�C�������擾���邽�߂̋��ʂ̃_�C�A���O�{�b�N�X���J��
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

// �w�肳�ꂽ�E�B���h�E�̃T�C�Y���w�肳�ꂽ�T�C�Y�ɕύX����ɂ́A������Ăяo��
void ResizeWindow(HWND hwnd, int cx, int cy);

int  GetWindowHeight(HWND hwnd);
int  GetWindowWidth(HWND hwnd);

#endif