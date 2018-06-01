/*=======================================================================

	デバッグコンソール [debugconsole.h]

												長岡佑樹

	デバッグ情報を記録して表示するためのサイズ変更可能な
	コンソールウィンドウを作成する
	<<演算子（std :: coutと同じように）を使用して、
	コンソールウィンドウにテキストと型を送信するdebug_conマクロを使用する。
	またはフラッシュマクロを使用してバッファをフラッシュする
	
=========================================================================*/
#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
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
//	定数定義
//
/////////////////////////////////////////////////////////
const int UM_SETSCROLL = WM_USER + 32;

//バッファがフラッシュされる前にコンソールに表示される最大行数ファイル
const int MaxBufferSize = 500;

// コンソールウィンドウの初期値
const int DEBUG_WINDOW_WIDTH  = 400;
const int DEBUG_WINDOW_HEIGHT = 400;

// すべてのデバッグメッセージをハイパースペースに送るDEBUGの定義を解除する
// #define DEBUG
#ifdef DEBUG
#define debug_con *(DebugConsole::Instance())
#else
#define debug_con *(CSink::Instance())
#endif

// コンソールへの出力をオン/オフに切り替え
#define debug_on  DebugConsole::On();
#define debug_off DebugConsole::Off();

/////////////////////////////////////////////////////////
//
//	CSinkクラス
//
// この小さなクラスはどんな入力に対してもシンクとして機能し,
// コンソールが必要ないときにDebugConsoleクラスの代わりに使用
//
/////////////////////////////////////////////////////////
class CSink
{
private:

  CSink(){};

  //コピーctorと割り当てはプライベート
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
//	DebugConsoleクラス
//
/////////////////////////////////////////////////////////
class DebugConsole
{
private:

  static HWND	         m_hwnd;
  
  // 文字列バッファデバッグストリームへのすべての入力がここに格納される
  static std::vector<std::string> m_Buffer;
  
  //trueの場合、次の入力がバッファにプッシュし,falseの場合追加
  static bool          m_bFlushed;  
  
  // デバッグウィンドウの位置
  static int           m_iPosTop;
  static int           m_iPosLeft;

  // ウィンドウが破棄された場合はtrueに設定
  static bool          m_bDestroyed;

  // falseの場合、コンソールは入力を無視
  static bool          m_bActive;

  // デフォルトのロギングファイル
  static std::ofstream m_LogOut;



  // デバッグウィンドウメッセージハンドラ
  static LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

  // ウィンドウクラスを登録し、ウィンドウを作成（ctorによって呼び出される）
  static bool             Create();

  static void             DrawWindow(){InvalidateRect(m_hwnd, NULL, TRUE); UpdateWindow(m_hwnd);}

private:

  DebugConsole(){}
 
  // コピーctorと割り当てはプライベート
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

  // バッファの内容をファイル "debug_log.txt"に書き込み、
  // バッファをクリアして適切なスクロール情報をリセットする
  void WriteAndResetBuffer();

  // 無効化を有効にするために使用
  static void  Off(){m_bActive = false;}
  static void  On()  {m_bActive = true;}

  bool Destroyed()const{return m_bDestroyed;}
 

  // 任意の型を受け入れるために<<をオーバーロードする
  template <class T>
  DebugConsole& operator<<(const T& t)
  {
    if (!m_bActive || m_bDestroyed) return *this;
   
    //オーバーフローした場合は、バッファーとスクロール情報をリセット
    //余分をファイルに書く
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