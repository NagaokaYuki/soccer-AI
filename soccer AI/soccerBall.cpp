/*=======================================================================

	�T�b�J�[�{�[�� [soccerBall.cpp]

												�����C��

=========================================================================*/

////////////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
////////////////////////////////////////////////////////////////
#include "soccerBall.h"
#include "geometry.h"
#include "debugConsole.h"
#include "cgdi.h"
#include "paramLoader.h"
#include "wall2D.h"

////////////////////////////////////////////////////////////////
//
//	AddNoiseToKick�֐�
//
//	�v���C���[�̃L�b�N�̐��x��ς��邽�߂Ɏg�p����
//	�{�[���̈ʒu�ƃ{�[���^�[�Q�b�g���p�����[�^�Ƃ��Ďg�p��,
//	�{�[�����L�b�N����O�ɂ�����Ăяo��
//
////////////////////////////////////////////////////////////////
Vector2D AddNoiseToKick(Vector2D BallPos, Vector2D BallTarget)
{

  double displacement = (Pi - Pi*Prm.PlayerKickingAccuracy) * RandomClamped();

  Vector2D toTarget = BallTarget - BallPos;

  Vec2DRotateAroundOrigin(toTarget, displacement);

  return toTarget + BallPos;
}

  
////////////////////////////////////////////////////////////////
//
//	Kick�֐�
//
//	�{�[���ɗ͂�������
//	�V�����x���V�e�B��؂�̂ĂāA�ő勖�e�l�𒴂��Ȃ��悤�ɂ���
//
////////////////////////////////////////////////////////////////
void SoccerBall::Kick(Vector2D direction, double force)
{  
  // ���������K������Ă��邱�Ƃ��m�F
  direction.Normalize();
  
  // �����x���v�Z
  Vector2D acceleration = (direction * force) / m_dMass;

  // ���x���X�V����
  m_vVelocity = acceleration;
}

////////////////////////////////////////////////////////////////
//
//	Update�֐�
//
//	�{�[�������w���X�V���A�Փ˂��e�X�g���A
//	����ɉ����ă{�[���̑��x�𒲐�����
//
////////////////////////////////////////////////////////////////
void SoccerBall::Update()
{
  // �Â��|�W�V�����̋L�^��ێ�
  m_vOldPos = m_vPosition;
  
  // �Փ˔���
  TestCollisionWithWalls(m_PitchBoundary);

  // �ŏ��ɃX�s�[�h�����ł��邱�Ƃ��m�F
  if (m_vVelocity.LengthSq() > Prm.Friction * Prm.Friction)
  {
    m_vVelocity += Vec2DNormalize(m_vVelocity) * Prm.Friction;

    m_vPosition += m_vVelocity;

    // �X�V���o��
    m_vHeading = Vec2DNormalize(m_vVelocity);
  }   
}

////////////////////////////////////////////////////////////////
//
//	TimeToCoverDistance�֐�
//
//	2�̃x�N�g���ɂ���ė^������͂ƃJ�o�[���鋗����
//	�^������ƁA�{�[����2�̓_�̊Ԃ��ǂ̂��炢�ړ����邩���v�Z
//	
////////////////////////////////////////////////////////////////
double SoccerBall::TimeToCoverDistance(Vector2D A,
                                      Vector2D B,
                                      double force)const
{
  // �{�[���̑��x���v�Z
  double speed = force / m_dMass;

  // �Q�̈ʒu�̋����Ԃ��v�Z
  double DistanceToCover =  Vec2DDistance(A, B);

  double term = speed*speed + 2.0*DistanceToCover*Prm.Friction;

  // ���̏ꍇ�{�[�����_B�ɓ��B�ł��Ȃ�
  if (term <= 0.0) return -1.0;

  double v = sqrt(term);

  return (v-speed)/Prm.Friction;
}

////////////////////////////////////////////////////////////////
//
//	FuturePosition�֐�
//
// �^����ꂽ���Ԃɂ���āA�{�[���̖����̈ʒu��Ԃ�
//	
////////////////////////////////////////////////////////////////
Vector2D SoccerBall::FuturePosition(double time)const
{
  // s =�����Aa =���C�Au =�J�n���x

  // �x�N�g��
  Vector2D ut = m_vVelocity * time;

  // �X�J���[
  double half_a_t_squared = 0.5 * Prm.Friction * time * time;

  // ���̒l�ɐ��K�����ꂽ���x�x�N�g�����|���邱�Ƃɂ����
  // �X�J���[�ʂ��x�N�g���ɕϊ�
  Vector2D ScalarToVector = half_a_t_squared * Vec2DNormalize(m_vVelocity);

  // �\�������ʒu�̓{�[���̈ʒu + �x�N�g�� + �X�J���[
  return Pos() + ut + ScalarToVector;
}

////////////////////////////////////////////////////////////////
//
//	Render�֐�
//	
////////////////////////////////////////////////////////////////
void SoccerBall::Render()
{
  gdi->BlackBrush();

  gdi->Circle(m_vPosition, m_dBoundingRadius);
}

////////////////////////////////////////////////////////////////
//
//	TestCollisionWithWalls�֐�
//	
////////////////////////////////////////////////////////////////
void SoccerBall::TestCollisionWithWalls(const std::vector<Wall2D>& walls)
{  
  // �ǂꂪ�ł��߂����𒲂ׂ�
  int idxClosest = -1;

  Vector2D VelNormal = Vec2DNormalize(m_vVelocity);

  Vector2D IntersectionPoint, CollisionPoint;

  double DistToIntersection = MaxFloat;

  // �ł��߂���������ǂɃC���f�b�N�X���i�[����ꍇ,
  // �e�ǂ𔽕����A�{�[�����������邩�ǂ������v�Z
  for (unsigned int w=0; w<walls.size(); ++w)
  {
    // �ǂɏՓ˂���{�[����̓_���v�Z
    // �{�[���̔��a�ŏ�Z���ꂽ�ǂ̖@���̋t���ł���A
    // �{�[���̒��S�i���̈ʒu�j�ɉ��Z
    Vector2D ThisCollisionPoint = Pos() - (walls[w].Normal() * BRadius());

    // �Փ˓_����s�@�ɓ�����ꏊ�𐳊m�Ɍv�Z
    if (WhereIsPoint(ThisCollisionPoint,
                     walls[w].From(),
                     walls[w].Normal()) == plane_backside)
    {
      double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
                                                         walls[w].Normal(),
                                                         walls[w].From(),
                                                         walls[w].Normal());

      IntersectionPoint = ThisCollisionPoint + (DistToWall * walls[w].Normal());
      
    }

    else
    {
      double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
                                                         VelNormal,
                                                         walls[w].From(),
                                                         walls[w].Normal());

      IntersectionPoint = ThisCollisionPoint + (DistToWall * VelNormal);
    }
    
    // �����_�����ۂɐ�����ɂ��邱�Ƃ��m�F
    bool OnLineSegment = false;

    if (LineIntersection2D(walls[w].From(), 
                           walls[w].To(),
                           ThisCollisionPoint - walls[w].Normal()*20.0,
                           ThisCollisionPoint + walls[w].Normal()*20.0))
    {

      OnLineSegment = true;                                               
    }

  
                                                                          //Note, there is no test for collision with the end of a line segment
    
    // �Փ˓_�����x�x�N�g���͈͓̔��ɂ��邩�ǂ������m�F
    // ���̏ꍇ�́A���݂̃^�C���X�e�b�v�Ǝ��̃^�C���X�e�b�v�̊Ԃ�
    // �{�[�����ǂƏՓ˂���
    double distSq = Vec2DDistanceSq(ThisCollisionPoint, IntersectionPoint);

    if ((distSq <= m_vVelocity.LengthSq()) && (distSq < DistToIntersection) && OnLineSegment)            
    {        
      DistToIntersection = distSq;
      idxClosest = w;
      CollisionPoint = IntersectionPoint;
    }     
  }

    
  // �Փ˂̐��m�Ȏ��Ԃ��v�Z���Ȃ��悤�ɁA
  // ���x�����˂���O�ɕǂ̖@���Ɣ��΂ł��邩�ǂ������m�F����
  // �I�[�o�[�V���[�g�����邽�߁A�{�[�����v���[�G���A�Ɋ��S�ɍē������O��
  // ���C����ɖ߂��Ĕ��˂���ꍇ��h�~����
  if ( (idxClosest >= 0 ) && VelNormal.Dot(walls[idxClosest].Normal()) < 0)
  {
    m_vVelocity.Reflect(walls[idxClosest].Normal());   
  }
}

////////////////////////////////////////////////////////////////
//
//	PlaceAtLocation�֐�
//	
//	�{�[�������]�̈ʒu�Ɉʒu���߂��A�{�[���̑��x���[���ɐݒ�
//
////////////////////////////////////////////////////////////////
void SoccerBall::PlaceAtPosition(Vector2D NewPos)
{
  m_vPosition = NewPos;

  m_vOldPos = m_vPosition;
  
  m_vVelocity.Zero();
}

