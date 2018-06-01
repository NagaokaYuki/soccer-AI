/*=======================================================================

	�X�e�A�����O�r�w�C�o�[ [steeringBehaviors.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�R���X�g���N�^
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
//	AccumulateForce�֐�
//
//	�ԗ����K�p�����܂܂�
//	�ő�X�e�A�����O�̗͂ʂ��v�Z���A
//	���̗ʂ����Z���邽�߂ɓK�p����
//
/////////////////////////////////////////////////////////
bool SteeringBehaviors::AccumulateForce(Vector2D &sf, Vector2D ForceToAdd)
{
  // �ŏ��ɂǂꂾ���̑��Ǘ͂��g���ׂ������v�Z
  double MagnitudeSoFar = sf.Length();

  double magnitudeRemaining = m_pPlayer->MaxForce() - MagnitudeSoFar;

  // �g�p����]�͂��c���Ă��Ȃ��ꍇ��false��Ԃ�
  if (magnitudeRemaining <= 0.0) return false;

  // �ǉ��������͂̑傫�����v�Z
  double MagnitudeToAdd = ForceToAdd.Length();
  
  // ���ۂɒǉ��ł���̗͂ʂ��v�Z
  if (MagnitudeToAdd > magnitudeRemaining)
  {
    MagnitudeToAdd = magnitudeRemaining;
  }

  // ���Ǘ͂ɉ�����
  sf += (Vec2DNormalize(ForceToAdd) * MagnitudeToAdd); 
  
  return true;
}

/////////////////////////////////////////////////////////
//
//	Calculate�֐�
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Calculate()
{                                                                         
  // �͂����Z�b�g����
  m_vSteeringForce.Zero();

  // �X�̑��Ǘ͂̒l��ێ�
  m_vSteeringForce = SumForces();

  // �͂��ő勖�e�Ԃ𒴂��Ȃ��悤�ɂ���
  m_vSteeringForce.Truncate(m_pPlayer->MaxForce());

  return m_vSteeringForce;
}

/////////////////////////////////////////////////////////
//
//	SumForces�֐�
//
//	�ő呀�Ǘ͂̑傫���ɒB����܂ł����̗͂�
//	�A�L�������[�g���A���̎��_�ŁA
//	���̓_�ɒ~�ς��ꂽ���Ǘ͂��߂����
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::SumForces()
{
   Vector2D force;
  
  // ��ɗאl�Ƀ^�O��t����
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
//	ForwardComponent�֐�
//
/////////////////////////////////////////////////////////
double SteeringBehaviors::ForwardComponent()
{
  return m_pPlayer->Heading().Dot(m_vSteeringForce);
}

/////////////////////////////////////////////////////////
//
//	SideComponent�֐�
//
/////////////////////////////////////////////////////////
double SteeringBehaviors::SideComponent()
{
  return m_pPlayer->Side().Dot(m_vSteeringForce) * m_pPlayer->MaxTurnRate();
}

/////////////////////////////////////////////////////////
//
//	Seek�֐�
//	
//	�ڕW���^����ꂽ�ꍇ�A���̋����́A
//	�G�[�W�F���g���^�[�Q�b�g�Ɛ��񂳂��A�G�[�W�F���g��
//	���]�̕����Ɉړ������鑀�Ǘ͂�߂�
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
//	Arrive�֐�
//	
//	�^�[�Q�b�g�Ƀ[�����x�œ��B����
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Arrive(Vector2D    target,
                                   Deceleration deceleration)
{
	Vector2D ToTarget = target - m_pPlayer->Pos();

	// �^�[�Q�b�g�܂ł̋������v�Z
	double dist = ToTarget.Length();
	
	if (dist > 0)
	{
		// Deceleration��int�Ƃ��ė񋓂���邽�߁A
		// ���̒l�͌����̔������ɕK�v
		const double DecelerationTweaker = 0.3;
		
		// ���]�̌����x���^����ꂽ�Ƃ��ɖڕW�ɓ��B����̂ɕK�v�ȑ��x���v�Z
		double speed =  dist / ((double)deceleration * DecelerationTweaker);                    
		
		// ���x���ő�l�𒴂��Ȃ��悤�ɂ���
		speed = min(speed, m_pPlayer->MaxSpeed());
		
		// ToTarget�x�N�g���𐳋K������
		Vector2D DesiredVelocity =  ToTarget * speed / dist;
		
		return (DesiredVelocity - m_pPlayer->Velocity());
	}
	
	return Vector2D(0,0);
}

/////////////////////////////////////////////////////////
//
//	Pursuit�֐�
//	
//	���̓���̓G�[�W�F���g���{�[���̕��Ɍ�����͂����o��
//
/////////////////////////////////////////////////////////
Vector2D SteeringBehaviors::Pursuit(const SoccerBall* ball)
{
  Vector2D ToBall = ball->Pos() - m_pPlayer->Pos();
 
  // ��ǂݎ��Ԃ́A�{�[���ƒǐՎ҂Ƃ̊Ԃ̋����ɔ�Ⴗ��
  double LookAheadTime = 0.0;

  if (ball->Speed() != 0.0)
  {
    LookAheadTime = ToBall.Length() / ball->Speed();
  }

  // �����̃{�[���������_�łǂ��ɂȂ邩���v�Z
  m_vTarget = ball->FuturePosition(LookAheadTime);

  // �{�[���̖����̗\�z�ʒu��ǋ�
  return Arrive(m_vTarget, fast);
}

/////////////////////////////////////////////////////////
//
//	FindNeighbours�֐�
//	
//	�\�ߒ�`���ꂽ���a���̔C�ӂ̕��Ƀ^�O��t����
//
/////////////////////////////////////////////////////////
void SteeringBehaviors::FindNeighbours()
{
	std::list<PlayerBase*>& AllPlayers = AutoList<PlayerBase>::GetAllMembers();
	std::list<PlayerBase*>::iterator curPlyr;
	
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
		// ���݂̃^�O�����ׂď���
		(*curPlyr)->Steering()->UnTag();
		
		// sqrts������邽�߂ɋ��������ō��
		Vector2D to = (*curPlyr)->Pos() - m_pPlayer->Pos();
		
		if (to.LengthSq() < (m_dViewDistance * m_dViewDistance))
		{
			(*curPlyr)->Steering()->Tag();
		}
	}
}

/////////////////////////////////////////////////////////
//
//	Separation�֐�
//	
//	�ׂ��甽������͂��v�Z
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

      // �G�[�W�F���g�̋����ɔ���Ⴗ��͂��X�P�[�����O����
      SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.Length();
    }
  }

  return SteeringForce;
}

/////////////////////////////////////////////////////////
//
//	Interpose�֐�
//	
//	�ΐ푊��ƃI�u�W�F�N�g�̈ʒu���^����ꂽ�ꍇ�A
//	���̃��\�b�h�̓G�[�W�F���g�ԂɃG�[�W�F���g��
//	�����I�ɔz�u���悤�Ƃ���̂�Ԃ�
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
//	RenderAids�֐�
//
/////////////////////////////////////////////////////////
void SteeringBehaviors::RenderAids( )
{ 
  // ���Ǘ͂�^����
  gdi->RedPen();

  gdi->Line(m_pPlayer->Pos(), m_pPlayer->Pos() + m_vSteeringForce * 20);  
}


