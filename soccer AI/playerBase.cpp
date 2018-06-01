/*=======================================================================

	�v���C���[�x�[�X [playerBase.cpp]

												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "playerBase.h"
#include "steeringBehaviors.h"
#include "transformations.h"
#include "geometry.h"
#include "cgdi.h"
#include "C2DMatrix.h"
#include "region.h"
#include "paramLoader.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"
#include "soccerTeam.h"
#include "paramLoader.h"
#include "goal.h"
#include "soccerBall.h"
#include "soccerPitch.h"
#include "debugConsole.h"

using std::vector;

/////////////////////////////////////////////////////////
//
//	�R���X�g���N�^
//
/////////////////////////////////////////////////////////
PlayerBase::PlayerBase(SoccerTeam* home_team,
                       int   home_region,
                       Vector2D  heading,
                       Vector2D velocity,
                       double    mass,
                       double    max_force,
                       double    max_speed,
                       double    max_turn_rate,
                       double    scale,
                       player_role role):    

    MovingEntity(home_team->Pitch()->GetRegionFromIndex(home_region)->Center(),
                 scale*10.0,
                 velocity,
                 max_speed,
                 heading,
                 mass,
                 Vector2D(scale,scale),
                 max_turn_rate,
                 max_force),
   m_pTeam(home_team),
   m_dDistSqToBall(MaxFloat),
   m_iHomeRegion(home_region),
   m_iDefaultRegion(home_region),
   m_PlayerRole(role)
{
  
  // ���_�o�b�t�@��ݒ肵�A���E���a���v�Z
  const int NumPlayerVerts = 4;
  const Vector2D player[NumPlayerVerts] = {Vector2D(-3, 8),
                                            Vector2D(3,10),
                                            Vector2D(3,-10),
                                            Vector2D(-3,-8)};

  for (int vtx=0; vtx<NumPlayerVerts; ++vtx)
  {
    m_vecPlayerVB.push_back(player[vtx]);

    // ���E�̔��a���ő�̒����ɐݒ�
    if (abs(player[vtx].x) > m_dBoundingRadius)
    {
      m_dBoundingRadius = abs(player[vtx].x);
    }

    if (abs(player[vtx].y) > m_dBoundingRadius)
    {
      m_dBoundingRadius = abs(player[vtx].y);
    }
  }

  // �X�e�A�����O����N���X��ݒ�
  m_pSteering = new SteeringBehaviors(this,
                                      m_pTeam->Pitch(),
                                      Ball());  
  
  // �J�n�ʒu
  m_pSteering->SetTarget(home_team->Pitch()->GetRegionFromIndex(home_region)->Center());
}

/////////////////////////////////////////////////////////
//
//	�f�X�g���N�^
//
/////////////////////////////////////////////////////////
PlayerBase::~PlayerBase()
{
  delete m_pSteering;
}

/////////////////////////////////////////////////////////
//
//	TrackBall�֐�
//
/////////////////////////////////////////////////////////
void PlayerBase::TrackBall()
{
  RotateHeadingToFacePosition(Ball()->Pos());  
}

/////////////////////////////////////////////////////////
//
//	TrackTarget�֐�
//
/////////////////////////////////////////////////////////
void PlayerBase::TrackTarget()
{
  SetHeading(Vec2DNormalize(Steering()->Target() - Pos()));
}

/////////////////////////////////////////////////////////
//
//	SortByDistanceToOpponentsGoal�֐�
//
/////////////////////////////////////////////////////////
bool  SortByDistanceToOpponentsGoal(const PlayerBase*const p1,
                                    const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() < p2->DistToOppGoal());
}

/////////////////////////////////////////////////////////
//
//	SortByReversedDistanceToOpponentsGoal�֐�
//
/////////////////////////////////////////////////////////
bool  SortByReversedDistanceToOpponentsGoal(const PlayerBase*const p1,
                                            const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() > p2->DistToOppGoal());
}

/////////////////////////////////////////////////////////
//
//	WithinFieldOfView�֐�
//
//	subject�����̃v���C���[�̎�����ɂ���ꍇ��true��Ԃ�
//
/////////////////////////////////////////////////////////
bool PlayerBase::PositionInFrontOfPlayer(Vector2D position)const
{
  Vector2D ToSubject = position - Pos();

  if (ToSubject.Dot(Heading()) > 0) 
    
    return true;

  else

    return false;
}

/////////////////////////////////////////////////////////
//
//	IsThreatened�֐�
//
//	subject�����̃v���C���[�̎�����ɂ���ꍇ��true��Ԃ�
//
/////////////////////////////////////////////////////////
bool PlayerBase::isThreatened()const
{
  // ���ׂĂ̑ΐ푊����`�F�b�N���āA�v���C���[�̃]�[�����ɂ��Ȃ����Ƃ��m�F
  std::vector<PlayerBase*>::const_iterator curOpp;  
  curOpp = Team()->Opponents()->Members().begin();
 
  for (curOpp; curOpp != Team()->Opponents()->Members().end(); ++curOpp)
  {
    // �v���C���[�܂ł̋������v�Z
    // dist���]�[�������������A���肪�v���C���[�̑O�ɂ���ꍇ��true��Ԃ�
    if (PositionInFrontOfPlayer((*curOpp)->Pos()) &&
       (Vec2DDistanceSq(Pos(), (*curOpp)->Pos()) < Prm.PlayerComfortZoneSq))
    {        
      return true;
    }
   
  }

  return false;
}

/////////////////////////////////////////////////////////
//
//	FindSupport�֐�
//
//	SupportSpot�ɍł��߂��v���C���[�����肵�A
//	�X�e�[�g��SupportAttacker�ɕύX����
//
/////////////////////////////////////////////////////////
void PlayerBase::FindSupport()const
{    
  // �T�|�[�g���Ȃ��ꍇ�́A�K�؂ȃv���C���[��T��
  if (Team()->SupportingPlayer() == NULL)
  {
    PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();

    Team()->SetSupportingPlayer(BestSupportPly);

    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            ID(),
                            Team()->SupportingPlayer()->ID(),
                            Msg_SupportAttacker,
                            NULL);
  }
    
  PlayerBase* BestSupportPly = Team()->DetermineBestSupportingAttacker();
    
  // �U���҂��T�|�[�g���邽�߂ɗ��p�\�ȍőP�̃v���C���[��
  // �ύX���ꂽ�ꍇ�A�|�C���^���X�V���Ċ֘A����v���C���[�̃X�e�[�g���X�V����
  if (BestSupportPly && (BestSupportPly != Team()->SupportingPlayer()))
  {
    
    if (Team()->SupportingPlayer())
    {
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              ID(),
                              Team()->SupportingPlayer()->ID(),
                              Msg_GoHome,
                              NULL);
    }
    
    
    
    Team()->SetSupportingPlayer(BestSupportPly);

    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            ID(),
                            Team()->SupportingPlayer()->ID(),
                            Msg_SupportAttacker,
                            NULL);
  }
}

/////////////////////////////////////////////////////////
//
//	DistToOppGoal�֐�
//
/////////////////////////////////////////////////////////
double PlayerBase::DistToOppGoal()const
{
  return fabs(Pos().x - Team()->OpponentsGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	DistToHomeGoal�֐�
//
/////////////////////////////////////////////////////////
double PlayerBase::DistToHomeGoal()const
{
	return fabs(Pos().x - Team()->HomeGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	isControllingPlayer�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::isControllingPlayer()const
{
	return Team()->ControllingPlayer()==this;
}

/////////////////////////////////////////////////////////
//
//	BallWithinKeeperRange�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinKeeperRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < Prm.KeeperInBallRangeSq);
}

/////////////////////////////////////////////////////////
//
//	BallWithinReceivingRange�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinReceivingRange()const
{
  return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < Prm.BallWithinReceivingRangeSq);
}

/////////////////////////////////////////////////////////
//
//	BallWithinKickingRange�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinKickingRange()const
{
	return (Vec2DDistanceSq(Ball()->Pos(), Pos()) < Prm.PlayerKickingDistanceSq);
}

/////////////////////////////////////////////////////////
//
//	InHomeRegion�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::InHomeRegion()const
{
	if (m_PlayerRole == goal_keeper)
	{
	  return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::normal);
	}
	else
	{
	  return Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::halfsize);
	}
}

/////////////////////////////////////////////////////////
//
//	AtTarget�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::AtTarget()const
{
	return (Vec2DDistanceSq(Pos(), Steering()->Target()) < Prm.PlayerInTargetRangeSq);
}

/////////////////////////////////////////////////////////
//
//	isClosestTeamMemberToBall�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::isClosestTeamMemberToBall()const
{
	return Team()->PlayerClosestToBall() == this;
}

/////////////////////////////////////////////////////////
//
//	isClosestPlayerOnPitchToBall�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::isClosestPlayerOnPitchToBall()const
{
	return isClosestTeamMemberToBall() && (DistSqToBall() < 
			Team()->Opponents()->ClosestDistToBallSq());
}

/////////////////////////////////////////////////////////
//
//	InHotRegion�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::InHotRegion()const
{
	return fabs(Pos().y - Team()->OpponentsGoal()->Center().y ) < 
		Pitch()->PlayingArea()->Length()/3.0;
}

/////////////////////////////////////////////////////////
//
//	isAheadOfAttacker�֐�
//
/////////////////////////////////////////////////////////
bool PlayerBase::isAheadOfAttacker()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x) <
			fabs(Team()->ControllingPlayer()->Pos().x - Team()->OpponentsGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	Ball�֐�
//
/////////////////////////////////////////////////////////
SoccerBall* const PlayerBase::Ball()const
{
	return Team()->Pitch()->Ball();
}

/////////////////////////////////////////////////////////
//
//	Pitch�֐�
//
/////////////////////////////////////////////////////////
SoccerPitch* const PlayerBase::Pitch()const
{
	return Team()->Pitch();
}

/////////////////////////////////////////////////////////
//
//	HomeRegion�֐�
//
/////////////////////////////////////////////////////////
const Region* const PlayerBase::HomeRegion()const
{
	return Pitch()->GetRegionFromIndex(m_iHomeRegion);
}