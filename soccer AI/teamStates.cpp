/*=======================================================================

	�`�[���X�e�[�g [teamStates.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "teamStates.h"
#include "soccerTeam.h"
#include "playerBase.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"
#include "soccerPitch.h"
#include "main.h"
#include "debugConsole.h"

/////////////////////////////////////////////////////////
//
//	�萔��`
//
/////////////////////////////////////////////////////////
//#define DEBUG_TEAM_STATES

/////////////////////////////////////////////////////////
//
//	ChangePlayerHomeRegions�֐�
//
/////////////////////////////////////////////////////////
void ChangePlayerHomeRegions(SoccerTeam* team, const int NewRegions[TeamSize])
{
  for (int plyr=0; plyr<TeamSize; ++plyr)
  {
    team->SetPlayerHomeRegion(plyr, NewRegions[plyr]);
  }
}

/////////////////////////////////////////////////////////
//
//	Attacking��Instance�֐�
//
/////////////////////////////////////////////////////////
Attacking* Attacking::Instance()
{
  static Attacking instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Attacking��Enter�֐�
//
/////////////////////////////////////////////////////////
void Attacking::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Attacking state" << "";
#endif

  // �e�I��̃z�[�����`
  const int BlueRegions[TeamSize] = {1,12,14,6,4};
  const int RedRegions[TeamSize] = {16,3,5,9,13};

  // �v���C���[�̃z�[����ݒ�
  if (team->Color() == SoccerTeam::blue)
  {
    ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
    ChangePlayerHomeRegions(team, RedRegions);
  }

  // �v���C���[��Wait�܂���ReturnToHomeRegion��
  // �����ꂩ�̏�Ԃɂ���ꍇ�A�X�e�A�����O�^�[�Q�b�g��
  // �V�����z�[�����[�W�����̏�ԂɍX�V���āA�������ʒu�Ɉړ��ł���悤�ɂ���
  team->UpdateTargetsOfWaitingPlayers();
}

/////////////////////////////////////////////////////////
//
//	Attacking��Execute�֐�
//
/////////////////////////////////////////////////////////
void Attacking::Execute(SoccerTeam* team)
{
  // �R���g���[���`�F���W��ԂɂȂ��ꍇ
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(Defending::Instance()); return;
  }

  // �T�|�[�g����U���҂��ړ����邽�߂̍ŗǂ̈ʒu���v�Z����
  team->DetermineBestSupportingPosition();
}

/////////////////////////////////////////////////////////
//
//	Attacking��Exit�֐�
//
/////////////////////////////////////////////////////////
void Attacking::Exit(SoccerTeam* team)
{
  team->SetSupportingPlayer(NULL);
}

/////////////////////////////////////////////////////////
//
//	Defending��Instance�֐�
//
/////////////////////////////////////////////////////////
Defending* Defending::Instance()
{
  static Defending instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	Defending��Enter�֐�
//
/////////////////////////////////////////////////////////
void Defending::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Defending state" << "";
#endif

  // �e�I��̃z�[�����`
  const int BlueRegions[TeamSize] = {1,6,8,3,5};
  const int RedRegions[TeamSize] = {16,9,11,12,14};

  // �v���C���[�̃z�[����ݒ�
  if (team->Color() == SoccerTeam::blue)
  {
    ChangePlayerHomeRegions(team, BlueRegions);
  }
  else
  {
    ChangePlayerHomeRegions(team, RedRegions);
  }
  
  // �v���C���[��Wait�܂���ReturnToHomeRegion�̂����ꂩ�̏�Ԃɂ���ꍇ�A
  // ���̃X�e�A�����O�^�[�Q�b�g��V�����z�[�����[�W�����̂��̂ɍX�V����
  team->UpdateTargetsOfWaitingPlayers();
}

/////////////////////////////////////////////////////////
//
//	Defending��Execute�֐�
//
/////////////////////////////////////////////////////////
void Defending::Execute(SoccerTeam* team)
{
  // ����ω���Ԃɂ���ꍇ
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(Attacking::Instance()); return;
  }
}

/////////////////////////////////////////////////////////
//
//	Defending��Exit�֐�
//
/////////////////////////////////////////////////////////
void Defending::Exit(SoccerTeam* team)
{
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOff��Instance�֐�
//
/////////////////////////////////////////////////////////
PrepareForKickOff* PrepareForKickOff::Instance()
{
  static PrepareForKickOff instance;

  return &instance;
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOff��Enter�֐�
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Enter(SoccerTeam* team)
{
  // ���Z�b�g�L�[�v���C���[�̃|�C���^
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  // �e�v���C���[��Msg_GoHome�𑗐M
  team->ReturnAllFieldPlayersToHome();
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOff��Execute�֐�
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Execute(SoccerTeam* team)
{
  // �����̃`�[�����|�W�V�����ɂ���ꍇ�́A�Q�[�����J�n
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome())
  {
    team->GetFSM()->ChangeState(Defending::Instance());
  }
}

/////////////////////////////////////////////////////////
//
//	PrepareForKickOff��Exit�֐�
//
/////////////////////////////////////////////////////////
void PrepareForKickOff::Exit(SoccerTeam* team)
{
  team->Pitch()->SetGameOn();
}


