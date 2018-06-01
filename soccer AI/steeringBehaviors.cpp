/*=======================================================================

	ステアリングビヘイバー [steeringBehaviors.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "steeringBehaviors.h"
#include "playerBase.h"
#include "transformations.h"
#include "utils.h"
#include "soccerTeam.h"
#include "autolist.h"
#include "paramLoader.h"
#include "soccerBall.h"

using std::string;
using std::vector;

/////////////////////////////////////////////////////////
//
//	コンストラクタ
//
/////////////////////////////////////////////////////////
SteeringBehaviors::SteeringBehaviors(PlayerBase*  agent,
                                     SoccerPitch* world,
                                     SoccerBall*  ball):
                                  
             m_pPlayer(agent),
             m_iFlags(0),
             m_dMultSeparation(Prm.SeparationCoefficient),
             m_bTagged(false),
             m_dViewDistance(Prm.ViewDistance),
             m_pBall(ball),
             m_dInterposeDist(0.0),
             m_Antenna(5,Vector2D())
{
}

/////////////////////////////////////////////////////////
//
//	AccumulateForce関数
//
//	車両が適用したままの
//	最大ステアリング力の量を計算し、
//	その量を加算するために適用する
//
/////////////////////////////////////////////////////////
bool SteeringBehaviors::AccumulateForce(Vector2D &sf, Vector2D ForceToAdd)
{
  // 最初にどれだけの操舵力を使うべきかを計算
  double MagnitudeSoFar = sf.Length();

  double magnitudeRemaining = m_pPlayer->MaxForce() - MagnitudeSoFar;

  // 使用する余力が残っていない場合はfalseを返す
  if (magnitudeRemaining <= 0.0) return false;

  // 追加したい力の大きさを計算
  double MagnitudeToAdd = ForceToAdd.Length();
  
  // 実際に追加できる力の量を計算
  if (MagnitudeToAdd > magnitudeRemaining)
  {
    MagnitudeToAdd = magnitudeRemaining;
  }

  // 操舵力に加える
  sf += (Vec2DNormalize(ForceToAdd) * MagnitudeToAdd); 
  
  return true;
}

/////////////////////////////////////////////////////////
//
//	Calculate関数
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Calculate()
{                                                                         
  // 力をリセットする
  m_vSteeringForce.Zero();

  // 個々の操舵力の値を保持
  m_vSteeringForce = SumForces();

  // 力が最大許容車を超えないようにする
  m_vSteeringForce.Truncate(m_pPlayer->MaxForce());

  return m_vSteeringForce;
}

/////////////////////////////////////////////////////////
//
//	SumForces関数
//
//	最大操舵力の大きさに達するまでそれらの力を
//	アキュムレートし、その時点で、
//	その点に蓄積された操舵力が戻される
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::SumForces()
{
   Vector2D force;
  
  // 常に隣人にタグを付ける
   FindNeighbours();

  if (On(separation))
  {
    force += Separation() * m_dMultSeparation;

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
  }    

  if (On(seek))
  {
    force += Seek(m_vTarget);

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
  }

  if (On(arrive))
  {
    force += Arrive(m_vTarget, fast);

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
  }

  if (On(pursuit))
  {
    force += Pursuit(m_pBall);

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
  }

  if (On(interpose))
  {
    force += Interpose(m_pBall, m_vTarget, m_dInterposeDist);

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
  }

  return m_vSteeringForce;
}

/////////////////////////////////////////////////////////
//
//	ForwardComponent関数
//
/////////////////////////////////////////////////////////
double SteeringBehaviors::ForwardComponent()
{
  return m_pPlayer->Heading().Dot(m_vSteeringForce);
}

/////////////////////////////////////////////////////////
//
//	SideComponent関数
//
/////////////////////////////////////////////////////////
double SteeringBehaviors::SideComponent()
{
  return m_pPlayer->Side().Dot(m_vSteeringForce) * m_pPlayer->MaxTurnRate();
}

/////////////////////////////////////////////////////////
//
//	Seek関数
//	
//	目標が与えられた場合、この挙動は、
//	エージェントをターゲットと整列させ、エージェントを
//	所望の方向に移動させる操舵力を戻す
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Seek(Vector2D target)
{
 
  Vector2D DesiredVelocity = Vec2DNormalize(target - m_pPlayer->Pos())
                            * m_pPlayer->MaxSpeed();

  return (DesiredVelocity - m_pPlayer->Velocity());
}

/////////////////////////////////////////////////////////
//
//	Arrive関数
//	
//	ターゲットにゼロ速度で到達する
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Arrive(Vector2D    target,
                                   Deceleration deceleration)
{
	Vector2D ToTarget = target - m_pPlayer->Pos();

	// ターゲットまでの距離を計算
	double dist = ToTarget.Length();
	
	if (dist > 0)
	{
		// Decelerationはintとして列挙されるため、
		// この値は減速の微調整に必要
		const double DecelerationTweaker = 0.3;
		
		// 所望の減速度が与えられたときに目標に到達するのに必要な速度を計算
		double speed =  dist / ((double)deceleration * DecelerationTweaker);                    
		
		// 速度が最大値を超えないようにする
		speed = min(speed, m_pPlayer->MaxSpeed());
		
		// ToTargetベクトルを正規化する
		Vector2D DesiredVelocity =  ToTarget * speed / dist;
		
		return (DesiredVelocity - m_pPlayer->Velocity());
	}
	
	return Vector2D(0,0);
}

/////////////////////////////////////////////////////////
//
//	Pursuit関数
//	
//	この動作はエージェントをボールの方に向ける力を作り出す
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Pursuit(const SoccerBall* ball)
{
  Vector2D ToBall = ball->Pos() - m_pPlayer->Pos();
 
  // 先読み時間は、ボールと追跡者との間の距離に比例する
  double LookAheadTime = 0.0;

  if (ball->Speed() != 0.0)
  {
    LookAheadTime = ToBall.Length() / ball->Speed();
  }

  // 未来のボールが現時点でどこになるかを計算
  m_vTarget = ball->FuturePosition(LookAheadTime);

  // ボールの未来の予想位置を追求
  return Arrive(m_vTarget, fast);
}

/////////////////////////////////////////////////////////
//
//	FindNeighbours関数
//	
//	予め定義された半径内の任意の物にタグを付ける
//
/////////////////////////////////////////////////////////
void SteeringBehaviors::FindNeighbours()
{
	std::list<PlayerBase*>& AllPlayers = AutoList<PlayerBase>::GetAllMembers();
	std::list<PlayerBase*>::iterator curPlyr;
	
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
		// 現在のタグをすべて消去
		(*curPlyr)->Steering()->UnTag();
		
		// sqrtsを避けるために距離平方で作業
		Vector2D to = (*curPlyr)->Pos() - m_pPlayer->Pos();
		
		if (to.LengthSq() < (m_dViewDistance * m_dViewDistance))
		{
			(*curPlyr)->Steering()->Tag();
		}
	}
}

/////////////////////////////////////////////////////////
//
//	Separation関数
//	
//	隣から反発する力を計算
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Separation()
{  
  Vector2D SteeringForce;
  
  std::list<PlayerBase*>& AllPlayers = AutoList<PlayerBase>::GetAllMembers();
  std::list<PlayerBase*>::iterator curPlyr;
  for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
  {
    if((*curPlyr != m_pPlayer) && (*curPlyr)->Steering()->Tagged())
    {
      Vector2D ToAgent = m_pPlayer->Pos() - (*curPlyr)->Pos();

      // エージェントの距離に反比例する力をスケーリングする
      SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
    }
  }

  return SteeringForce;
}

/////////////////////////////////////////////////////////
//
//	Interpose関数
//	
//	対戦相手とオブジェクトの位置が与えられた場合、
//	このメソッドはエージェント間にエージェントを
//	強制的に配置しようとするのを返す
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Interpose(const SoccerBall* ball,
                                      Vector2D  target,
                                      double     DistFromTarget)
{
  return Arrive(target + Vec2DNormalize(ball->Pos() - target) * 
                DistFromTarget, normal);
}

/////////////////////////////////////////////////////////
//
//	RenderAids関数
//
/////////////////////////////////////////////////////////
void SteeringBehaviors::RenderAids( )
{ 
  // 操舵力を与える
  gdi->RedPen();

  gdi->Line(m_pPlayer->Pos(), m_pPlayer->Pos() + m_vSteeringForce * 20);  
}


