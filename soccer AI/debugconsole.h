/*=======================================================================

	�f�o�b�O�R���\�[�� [debugconsole.h]

												�����C��

	�f�o�b�O�����L�^���ĕ\�����邽�߂̃T�C�Y�ύX�\��
	�R���\�[���E�B���h�E���쐬����
	<<���Z�q�istd :: cout�Ɠ����悤�Ɂj���g�p���āA
	�R���\�[���E�B���h�E�Ƀe�L�X�g�ƌ^�𑗐M����debug_con�}�N�����g�p����B
	�܂��̓t���b�V���}�N�����g�p���ăo�b�t�@���t���b�V������
	
=========================================================================*/
#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include <windows.h>
#include <iosfwd>
#include <fstream>
#include "utils.h"
#include "WindowUtils.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////
const int UM_SETSCROLL = WM_USER + 32;

//�o�b�t�@���t���b�V�������O�ɃR���\�[���ɕ\�������ő�s���t�@�C��
const int MaxBufferSize = 500;

// �R���\�[���E�B���h�E�̏����l
const int DEBUG_WINDOW_WIDTH  = 400;
const int DEBUG_WINDOW_HEIGHT = 400;

// ���ׂẴf�o�b�O���b�Z�[�W���n�C�p�[�X�y�[�X�ɑ���DEBUG�̒�`����������
// #define DEBUG
#ifdef DEBUG
#define debug_con *(DebugConsole::Instance())
#else
#define debug_con *(CSink::Instance())
#endif

// �R���\�[���ւ̏o�͂��I��/�I�t�ɐ؂�ւ�
#define debug_on  DebugConsole::On();
#define debug_off DebugConsole::Off();

/////////////////////////////////////////////////////////
//
//	CSink�N���X
//
// ���̏����ȃN���X�͂ǂ�ȓ��͂ɑ΂��Ă��V���N�Ƃ��ċ@�\��,
// �R���\�[�����K�v�Ȃ��Ƃ���DebugConsole�N���X�̑���Ɏg�p
//
/////////////////////////////////////////////////////////
class CSink
{
private:

  CSink(){};

  //�R�s�[ctor�Ɗ��蓖�Ă̓v���C�x�[�g
  CSink(const CSink&);
  CSink& operator=(const CSink&);
  
public:

  static CSink* Instance(){static CSink instance; return &instance;}
  
  template<class T>
  CSink& operator<<(const T&)
  {
	  return *this;
  }
};

/////////////////////////////////////////////////////////
//
//	DebugConsole�N���X
//
/////////////////////////////////////////////////////////
class DebugConsole
{
private:

  static HWND	         m_hwnd;
  
  // ������o�b�t�@�f�o�b�O�X�g���[���ւ̂��ׂĂ̓��͂������Ɋi�[�����
  static std::vector<std::string> m_Buffer;
  
  //true�̏ꍇ�A���̓��͂��o�b�t�@�Ƀv�b�V����,false�̏ꍇ�ǉ�
  static bool          m_bFlushed;  
  
  // �f�o�b�O�E�B���h�E�̈ʒu
  static int           m_iPosTop;
  static int           m_iPosLeft;

  // �E�B���h�E���j�����ꂽ�ꍇ��true�ɐݒ�
  static bool          m_bDestroyed;

  // false�̏ꍇ�A�R���\�[���͓��͂𖳎�
  static bool          m_bActive;

  // �f�t�H���g�̃��M���O�t�@�C��
  static std::ofstream m_LogOut;



  // �f�o�b�O�E�B���h�E���b�Z�[�W�n���h��
  static LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

  // �E�B���h�E�N���X��o�^���A�E�B���h�E���쐬�ictor�ɂ���ČĂяo�����j
  static bool             Create();

  static void             DrawWindow(){InvalidateRect(m_hwnd, NULL, TRUE); UpdateWindow(m_hwnd);}

private:

  DebugConsole(){}
 
  // �R�s�[ctor�Ɗ��蓖�Ă̓v���C�x�[�g
  DebugConsole(const DebugConsole&);
  DebugConsole& operator=(const DebugConsole&);

public:

  ~DebugConsole(){WriteAndResetBuffer(); }

  static DebugConsole* Instance();

             
  void ClearBuffer(){m_Buffer.clear(); flush();}


  static void flush()
  {
    if (!m_bDestroyed)
    {
      m_bFlushed = true; SendMessage(m_hwnd, UM_SETSCROLL, NULL, NULL);
    }
  }

  // �o�b�t�@�̓��e���t�@�C�� "debug_log.txt"�ɏ������݁A
  // �o�b�t�@���N���A���ēK�؂ȃX�N���[���������Z�b�g����
  void WriteAndResetBuffer();

  // ��������L���ɂ��邽�߂Ɏg�p
  static void  Off(){m_bActive = false;}
  static void  On()  {m_bActive = true;}

  bool Destroyed()const{return m_bDestroyed;}
 

  // �C�ӂ̌^���󂯓���邽�߂�<<���I�[�o�[���[�h����
  template <class T>
  DebugConsole& operator<<(const T& t)
  {
    if (!m_bActive || m_bDestroyed) return *this;
   
    //�I�[�o�[�t���[�����ꍇ�́A�o�b�t�@�[�ƃX�N���[���������Z�b�g
    //�]�����t�@�C���ɏ���
    if (m_Buffer.size() > MaxBufferSize)
    {
       WriteAndResetBuffer();
    }
    
    std::ostringstream ss; ss << t;

    if (ss.str() == ""){flush(); return *this;}
    
    if (!m_bFlushed)
      {m_Buffer.back() += ss.str();}
    else
      {m_Buffer.push_back(ss.str());m_bFlushed = false;}

    return *this;
  }
};

#endif