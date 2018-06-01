/*=======================================================================

	プレシジョンタイマー [precisionTimer.h]

												長岡佑樹

=========================================================================*/
#ifndef PRECISION_TIMER_H
#define PRECISION_TIMER_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	PrecisionTimerクラス
//
/////////////////////////////////////////////////////////
class PrecisionTimer
{

private:
	LONGLONG  m_CurrentTime,
            m_LastTime,
            m_LastTimeInTimeElapsed,
            m_NextTime,
            m_StartTime,
            m_FrameTime,
            m_PerfCountFreq;

	double  m_TimeElapsed,
            m_LastTimeElapsed,
            m_TimeScale;

	double  m_NormalFPS;
	double  m_SlowFPS;
	bool    m_bStarted;

	// trueの場合、TimeElapsed（）の呼び出しは,
	// 現在の経過時間が前回よりもはるかに小さい場合は
	// 0を返します。メニューを使用して
	// ユーザに関連する問題に対処する
	///ウィンドウのサイズ変更/移動など
	bool      m_bSmoothUpdates;


public:

	PrecisionTimer();
	PrecisionTimer(double fps);
	
	void    Start();
	
	// 次のフレームに移動するのに十分な時間が経過したかどうかを判断する
	inline bool    ReadyForNextFrame();

	// 上記の呼び出しの後でこれを使用
	// double GetTimeElapsed（）{return m_TimeElapsed;}
	
	inline double  TimeElapsed();
	
	double  CurrentTime()
	{ 
	  QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	
	  return (m_CurrentTime - m_StartTime) * m_TimeScale;
	}
	
	bool    Started()const{return m_bStarted;}

	void    SmoothUpdatesOn(){m_bSmoothUpdates = true;}
	void    SmoothUpdatesOff(){m_bSmoothUpdates = false;}

};

/////////////////////////////////////////////////////////
//
//	ReadyForNextFrame関数
//
//  次のフレームステップに移動するときにtrueを返す 
//  FPSが設定されている場合に使用される
//
/////////////////////////////////////////////////////////
inline bool PrecisionTimer::ReadyForNextFrame()
{
  assert(m_NormalFPS && "PrecisionTimer::ReadyForNextFrame<No FPS set in timer>");
  
  QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);

  if (m_CurrentTime > m_NextTime)
  {

    m_TimeElapsed = (m_CurrentTime - m_LastTime) * m_TimeScale;
    m_LastTime    = m_CurrentTime;

    // 次のフレームをレンダリングするための更新時間
    m_NextTime = m_CurrentTime + m_FrameTime;

    return true;
  }

  return false;
}

/////////////////////////////////////////////////////////
//
//	ReadyForNextFrame関数
//
//	この関数の最後の呼び出しから経過した時間を返す
//
/////////////////////////////////////////////////////////
inline double PrecisionTimer::TimeElapsed()
{
  m_LastTimeElapsed = m_TimeElapsed;

  QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
  
  m_TimeElapsed = (m_CurrentTime - m_LastTimeInTimeElapsed) * m_TimeScale;
  
  m_LastTimeInTimeElapsed    = m_CurrentTime;

  const double Smoothness = 5.0;

  if (m_bSmoothUpdates)
  {
    if (m_TimeElapsed < (m_LastTimeElapsed * Smoothness))
    {
      return m_TimeElapsed;
    }

    else
    {
      return 0.0;
    }
  }
  
  else
  {
    return m_TimeElapsed;
  }
    
}

#endif

  
