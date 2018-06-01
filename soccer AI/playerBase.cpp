/*=======================================================================

	プレイヤーベース [playerBase.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
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
//	コンストラクタ
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
  
  // 頂点バッファを設定し、境界半径を計算
  const int NumPlayerVerts = 4;
  const Vector2D player[NumPlayerVerts] = {Vector2D(-3, 8),
                                            Vector2D(3,10),
                                            Vector2D(3,-10),
                                            Vector2D(-3,-8)};

  for (int vtx=0; vtx<NumPlayerVerts; ++vtx)
  {
    m_vecPlayerVB.push_back(player[vtx]);

    // 境界の半径を最大の長さに設定
    if (abs(player[vtx].x) > m_dBoundingRadius)
    {
      m_dBoundingRadius = abs(player[vtx].x);
    }

    if (abs(player[vtx].y) > m_dBoundingRadius)
    {
      m_dBoundingRadius = abs(player[vtx].y);
    }
  }

  // ステアリング動作クラスを設定
  m_pSteering = new SteeringBehaviors(this,
                                      m_pTeam->Pitch(),
                                      Ball());  
  
  // 開始位置
  m_pSteering->SetTarget(home_team->Pitch()->GetRegionFromIndex(home_region)->Center());
}

/////////////////////////////////////////////////////////
//
//	デストラクタ
//
/////////////////////////////////////////////////////////
PlayerBase::~PlayerBase()
{
  delete m_pSteering;
}

/////////////////////////////////////////////////////////
//
//	TrackBall関数
//
/////////////////////////////////////////////////////////
void PlayerBase::TrackBall()
{
  RotateHeadingToFacePosition(Ball()->Pos());  
}

/////////////////////////////////////////////////////////
//
//	TrackTarget関数
//
/////////////////////////////////////////////////////////
void PlayerBase::TrackTarget()
{
  SetHeading(Vec2DNormalize(Steering()->Target() - Pos()));
}

/////////////////////////////////////////////////////////
//
//	SortByDistanceToOpponentsGoal関数
//
/////////////////////////////////////////////////////////
bool  SortByDistanceToOpponentsGoal(const PlayerBase*const p1,
                                    const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() < p2->DistToOppGoal());
}

/////////////////////////////////////////////////////////
//
//	SortByReversedDistanceToOpponentsGoal関数
//
/////////////////////////////////////////////////////////
bool  SortByReversedDistanceToOpponentsGoal(const PlayerBase*const p1,
                                            const PlayerBase*const p2)
{
  return (p1->DistToOppGoal() > p2->DistToOppGoal());
}

/////////////////////////////////////////////////////////
//
//	WithinFieldOfView関数
//
//	subjectがこのプレイヤーの視野内にある場合はtrueを返す
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
//	IsThreatened関数
//
//	subjectがこのプレイヤーの視野内にある場合はtrueを返す
//
/////////////////////////////////////////////////////////
bool PlayerBase::isThreatened()const
{
  // すべての対戦相手をチェックして、プレイヤーのゾーン内にいないことを確認
  std::vector<PlayerBase*>::const_iterator curOpp;  
  curOpp = Team()->Opponents()->Members().begin();
 
  for (curOpp; curOpp != Team()->Opponents()->Members().end(); ++curOpp)
  {
    // プレイヤーまでの距離を計算
    // distがゾーンよりも小さく、相手がプレイヤーの前にいる場合はtrueを返す
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
//	FindSupport関数
//
//	SupportSpotに最も近いプレイヤーを決定し、
//	ステートをSupportAttackerに変更する
//
/////////////////////////////////////////////////////////
void PlayerBase::FindSupport()const
{    
  // サポートがない場合は、適切なプレイヤーを探す
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
    
  // 攻撃者をサポートするために利用可能な最善のプレイヤーが
  // 変更された場合、ポインタを更新して関連するプレイヤーのステートを更新する
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
//	DistToOppGoal関数
//
/////////////////////////////////////////////////////////
double PlayerBase::DistToOppGoal()const
{
  return fabs(Pos().x - Team()->OpponentsGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	DistToHomeGoal関数
//
/////////////////////////////////////////////////////////
double PlayerBase::DistToHomeGoal()const
{
	return fabs(Pos().x - Team()->HomeGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	isControllingPlayer関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::isControllingPlayer()const
{
	return Team()->ControllingPlayer()==this;
}

/////////////////////////////////////////////////////////
//
//	BallWithinKeeperRange関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinKeeperRange()const
{
	return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < Prm.KeeperInBallRangeSq);
}

/////////////////////////////////////////////////////////
//
//	BallWithinReceivingRange関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinReceivingRange()const
{
  return (Vec2DDistanceSq(Pos(), Ball()->Pos()) < Prm.BallWithinReceivingRangeSq);
}

/////////////////////////////////////////////////////////
//
//	BallWithinKickingRange関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::BallWithinKickingRange()const
{
	return (Vec2DDistanceSq(Ball()->Pos(), Pos()) < Prm.PlayerKickingDistanceSq);
}

/////////////////////////////////////////////////////////
//
//	InHomeRegion関数
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
//	AtTarget関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::AtTarget()const
{
	return (Vec2DDistanceSq(Pos(), Steering()->Target()) < Prm.PlayerInTargetRangeSq);
}

/////////////////////////////////////////////////////////
//
//	isClosestTeamMemberToBall関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::isClosestTeamMemberToBall()const
{
	return Team()->PlayerClosestToBall() == this;
}

/////////////////////////////////////////////////////////
//
//	isClosestPlayerOnPitchToBall関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::isClosestPlayerOnPitchToBall()const
{
	return isClosestTeamMemberToBall() && (DistSqToBall() < 
			Team()->Opponents()->ClosestDistToBallSq());
}

/////////////////////////////////////////////////////////
//
//	InHotRegion関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::InHotRegion()const
{
	return fabs(Pos().y - Team()->OpponentsGoal()->Center().y ) < 
		Pitch()->PlayingArea()->Length()/3.0;
}

/////////////////////////////////////////////////////////
//
//	isAheadOfAttacker関数
//
/////////////////////////////////////////////////////////
bool PlayerBase::isAheadOfAttacker()const
{
	return fabs(Pos().x - Team()->OpponentsGoal()->Center().x) <
			fabs(Team()->ControllingPlayer()->Pos().x - Team()->OpponentsGoal()->Center().x);
}

/////////////////////////////////////////////////////////
//
//	Ball関数
//
/////////////////////////////////////////////////////////
SoccerBall* const PlayerBase::Ball()const
{
	return Team()->Pitch()->Ball();
}

/////////////////////////////////////////////////////////
//
//	Pitch関数
//
/////////////////////////////////////////////////////////
SoccerPitch* const PlayerBase::Pitch()const
{
	return Team()->Pitch();
}

/////////////////////////////////////////////////////////
//
//	HomeRegion関数
//
/////////////////////////////////////////////////////////
const Region* const PlayerBase::HomeRegion()const
{
	return Pitch()->GetRegionFromIndex(m_iHomeRegion);
}