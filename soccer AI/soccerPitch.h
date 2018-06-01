/*=======================================================================

	�T�b�J�[�s�b�` [sccerPitch.cpp]

												�����C��

	2�̃T�b�J�[�`�[���A2�̃S�[���A
	�v���[�G���A�A�{�[���Ȃǂ̃C���X�^���X�����L

=========================================================================*/
#ifndef SOCCERPITCH_H
#define SOCCERPITCH_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <windows.h>
#include <vector>
#include <cassert>

#include "wall2D.h"
#include "vector2D.h"

class Region;
class Goal;
class SoccerTeam;
class SoccerBall;
class SoccerTeam;
class PlayerBase;


class SoccerPitch
{ 
public:

  SoccerBall*          m_pBall;

  SoccerTeam*          m_pRedTeam;
  SoccerTeam*          m_pBlueTeam;

  Goal*                m_pRedGoal;
  Goal*                m_pBlueGoal;
   
  // Wall2D�N���X�̃����o�ϐ�
  std::vector<Wall2D>  m_vecWalls;

  // Region�N���X�̃����o�ϐ�
  Region*              m_pPlayingArea;

  std::vector<Region*> m_Regions;

  // �S�[���L�[�p�[�̃t���O
  bool                 m_bGoalKeeperHasBall;

  // �Q�[���J�n���̃t���O
  bool                 m_bGameOn;

  // �|�[�Y�t���O
  bool                 m_bPaused;

  // �N���C�A���g�E�B���h�E�̃T�C�Y�̃��[�J���R�s�[
  int                  m_cxClient,
                       m_cyClient;  
  
  // �v���C���[���������g���ʒu�Â��邽�߂ɗ��p����
  // �̈���C���X�^���X�����܂�
  void CreateRegions(double width, double height);


public:

  SoccerPitch(int cxClient, int cyClient);

  ~SoccerPitch();

  void  Update();

  bool  Render();

  void  TogglePause(){m_bPaused = !m_bPaused;}
  bool  Paused()const{return m_bPaused;}

  int   cxClient()const{return m_cxClient;}
  int   cyClient()const{return m_cyClient;}

  bool  GoalKeeperHasBall()const{return m_bGoalKeeperHasBall;}
  void  SetGoalKeeperHasBall(bool b){m_bGoalKeeperHasBall = b;}

  const Region*const         PlayingArea()const{return m_pPlayingArea;}
  const std::vector<Wall2D>& Walls(){return m_vecWalls;}                      
  SoccerBall*const           Ball()const{return m_pBall;}

  const Region* const GetRegionFromIndex(int idx)                                
  {
    assert ( (idx >= 0) && (idx < (int)m_Regions.size()) );

    return m_Regions[idx];
  }

  bool  GameOn()const{return m_bGameOn;}
  void  SetGameOn(){m_bGameOn = true;}
  void  SetGameOff(){m_bGameOn = false;}

};

#endif