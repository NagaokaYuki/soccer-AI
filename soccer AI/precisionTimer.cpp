/*=======================================================================

	プレシジョンタイマー [precisionTimer.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "PrecisionTimer.h"


/////////////////////////////////////////////////////////
//
//	デフォルトコンストラクタ
//
/////////////////////////////////////////////////////////
PrecisionTimer::PrecisionTimer(): m_NormalFPS(0.0),
                  m_SlowFPS(1.0),
                  m_TimeElapsed(0.0),
                  m_FrameTime(0),
                  m_LastTime(0),
                  m_LastTimeInTimeElapsed(0),
                  m_PerfCountFreq(0),
                  m_bStarted(false),
                  m_StartTime(0),
                  m_LastTimeElapsed(0.0),
                  m_bSmoothUpdates(false)
{
  // 1秒間にどれくらい多くのティックが得られるか
  QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
  
  m_TimeScale = 1.0/m_PerfCountFreq;
}

/////////////////////////////////////////////////////////
//
//	コンストラクタ
//	FPSを指定するために使用する
//
/////////////////////////////////////////////////////////
PrecisionTimer::PrecisionTimer(double fps): m_NormalFPS(fps),
                  m_SlowFPS(1.0),
                  m_TimeElapsed(0.0),
                  m_FrameTime(0),
                  m_LastTime(0),
                  m_LastTimeInTimeElapsed(0),
                  m_PerfCountFreq(0),
                  m_bStarted(false),
                  m_StartTime(0),
                  m_LastTimeElapsed(0.0),
                  m_bSmoothUpdates(false)
{

  // 1秒間にどれくらい多くのティックが得られるか
  QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);

  m_TimeScale = 1.0/m_PerfCountFreq;

  // フレームあたりのティックを計算
  m_FrameTime = (LONGLONG)(m_PerfCountFreq / m_NormalFPS);
}



/////////////////////////////////////////////////////////
//
//	Start関数
//	ゲームループの直前にこれを呼び出して、タイマーを開始
//
/////////////////////////////////////////////////////////
void PrecisionTimer::Start()
{
  m_bStarted = true;
  
  m_TimeElapsed = 0.0;

  // 時間取得
  QueryPerformanceCounter( (LARGE_INTEGER*) &m_LastTime);

  // タイマーが開始されたときの記録を保持
  m_StartTime = m_LastTimeInTimeElapsed = m_LastTime;

  // 次のフレームをレンダリングするための更新時間
  m_NextTime = m_LastTime + m_FrameTime;

  return;
}

