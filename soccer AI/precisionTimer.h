/*=======================================================================

	�v���V�W�����^�C�}�[ [precisionTimer.h]

												�����C��

=========================================================================*/
#ifndef PRECISION_TIMER_H
#define PRECISION_TIMER_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	PrecisionTimer�N���X
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

	// true�̏ꍇ�ATimeElapsed�i�j�̌Ăяo����,
	// ���݂̌o�ߎ��Ԃ��O������͂邩�ɏ������ꍇ��
	// 0��Ԃ��܂��B���j���[���g�p����
	// ���[�U�Ɋ֘A������ɑΏ�����
	///�E�B���h�E�̃T�C�Y�ύX/�ړ��Ȃ�
	bool      m_bSmoothUpdates;


public:

	PrecisionTimer();
	PrecisionTimer(double fps);
	
	void    Start();
	
	// ���̃t���[���Ɉړ�����̂ɏ\���Ȏ��Ԃ��o�߂������ǂ����𔻒f����
	inline bool    ReadyForNextFrame();

	// ��L�̌Ăяo���̌�ł�����g�p
	// double GetTimeElapsed�i�j{return m_TimeElapsed;}
	
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
//	ReadyForNextFrame�֐�
//
//  ���̃t���[���X�e�b�v�Ɉړ�����Ƃ���true��Ԃ� 
//  FPS���ݒ肳��Ă���ꍇ�Ɏg�p�����
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

    // ���̃t���[���������_�����O���邽�߂̍X�V����
    m_NextTime = m_CurrentTime + m_FrameTime;

    return true;
  }

  return false;
}

/////////////////////////////////////////////////////////
//
//	ReadyForNextFrame�֐�
//
//	���̊֐��̍Ō�̌Ăяo������o�߂������Ԃ�Ԃ�
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

  
