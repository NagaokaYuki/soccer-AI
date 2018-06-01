/*=======================================================================

	�v���V�W�����^�C�}�[ [precisionTimer.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "PrecisionTimer.h"


/////////////////////////////////////////////////////////
//
//	�f�t�H���g�R���X�g���N�^
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
  // 1�b�Ԃɂǂꂭ�炢�����̃e�B�b�N�������邩
  QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
  
  m_TimeScale = 1.0/m_PerfCountFreq;
}

/////////////////////////////////////////////////////////
//
//	�R���X�g���N�^
//	FPS���w�肷�邽�߂Ɏg�p����
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

  // 1�b�Ԃɂǂꂭ�炢�����̃e�B�b�N�������邩
  QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);

  m_TimeScale = 1.0/m_PerfCountFreq;

  // �t���[��������̃e�B�b�N���v�Z
  m_FrameTime = (LONGLONG)(m_PerfCountFreq / m_NormalFPS);
}



/////////////////////////////////////////////////////////
//
//	Start�֐�
//	�Q�[�����[�v�̒��O�ɂ�����Ăяo���āA�^�C�}�[���J�n
//
/////////////////////////////////////////////////////////
void PrecisionTimer::Start()
{
  m_bStarted = true;
  
  m_TimeElapsed = 0.0;

  // ���Ԏ擾
  QueryPerformanceCounter( (LARGE_INTEGER*) &m_LastTime);

  // �^�C�}�[���J�n���ꂽ�Ƃ��̋L�^��ێ�
  m_StartTime = m_LastTimeInTimeElapsed = m_LastTime;

  // ���̃t���[���������_�����O���邽�߂̍X�V����
  m_NextTime = m_LastTime + m_FrameTime;

  return;
}

