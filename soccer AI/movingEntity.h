/*=======================================================================

	���[�r���O�G���e�B�e�B [movingEntity.h]

												�����C��

	�ړ�����G���e�B�e�B���`�����{�N���X

=========================================================================*/
#ifndef MOVING_ENTITY
#define MOVING_ENTITY

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <cassert>
#include "vector2D.h"
#include "baseGameEntity.h"

/////////////////////////////////////////////////////////
//
//	MovingEntity�N���X
//
/////////////////////////////////////////////////////////
class MovingEntity : public BaseGameEntity
{
protected:
	Vector2D    m_vVelocity;
	
	// �G���e�B�e�B�������Ă���������w�����K�����ꂽ�x�N�g��
	Vector2D    m_vHeading;
	
	// m_vHeading�x�N�g���ɐ����ȃx�N�g��
	Vector2D    m_vSide; 
	
	double      m_dMass;
	
	// �G���e�B�e�B���ړ��ł���ő呬�x
	double      m_dMaxSpeed;
	
	// �G���e�B�e�B������̗͂𔭊��ł���ő�̗�
	double      m_dMaxForce;
	
	// ��]����ő呬�x
	double      m_dMaxTurnRate;

public:
	
	MovingEntity(Vector2D position,
				double   radius,
				Vector2D velocity,
				double   max_speed,
				Vector2D heading,
				double   mass,
				Vector2D scale,
				double   turn_rate,
				double   max_force):BaseGameEntity(BaseGameEntity::GetNextValidID()),
													m_vHeading(heading),
													m_vVelocity(velocity),
													m_dMass(mass),
													m_vSide(m_vHeading.Perp()),
													m_dMaxSpeed(max_speed),
													m_dMaxTurnRate(turn_rate),
													m_dMaxForce(max_force)
	{
		m_vPosition = position;
		m_dBoundingRadius = radius; 
		m_vScale = scale;
	}

	virtual ~MovingEntity(){}

	Vector2D  Velocity()const{return m_vVelocity;}
	void      SetVelocity(const Vector2D& NewVel){m_vVelocity = NewVel;}
	
	double    Mass()const{return m_dMass;}
	
	Vector2D  Side()const{return m_vSide;}
	
	double    MaxSpeed()const{return m_dMaxSpeed;}                       
	void      SetMaxSpeed(double new_speed){m_dMaxSpeed = new_speed;}
	
	double    MaxForce()const{return m_dMaxForce;}
	void      SetMaxForce(double mf){m_dMaxForce = mf;}
	
	bool      IsSpeedMaxedOut()const{return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.LengthSq();}
	double    Speed()const{return m_vVelocity.Length();}
	double    SpeedSq()const{return m_vVelocity.LengthSq();}
	
	Vector2D  Heading()const{return m_vHeading;}
	void      SetHeading(Vector2D new_heading);
	bool      RotateHeadingToFacePosition(Vector2D target);
	
	double    MaxTurnRate()const{return m_dMaxTurnRate;}
	void      SetMaxTurnRate(double val){m_dMaxTurnRate = val;}
	
};

///////////////////////////////////////////////////////////////////////////
//
//	RotateHeadingToFacePosition�N���X
//
//	�ڕW�ʒu���^������ƁA�G���e�B�e�B�̕����x�N�g���ƃT�C�h�x�N�g�����A
//	�^�[�Q�b�g�ɒ��ڌ����܂�m_dMaxTurnRate�ȉ��̗ʂ�����]����
//	�������ړI�̕����Ɍ����Ă���Ƃ���true��Ԃ�
//
///////////////////////////////////////////////////////////////////////////
inline bool MovingEntity::RotateHeadingToFacePosition(Vector2D target)
{
	Vector2D toTarget = Vec2DNormalize(target - m_vPosition);
	
	double dot = m_vHeading.Dot(toTarget);
	
	// �������̃R���p�C�������x���������߁A
	// acos�ɗL���ł��邱�Ƃ�ۏ؂��邽�߂ɒl���N�����v�����
	Clamp(dot, -1, 1);
	
	// �ŏ��ɕ����x�N�g���ƃ^�[�Q�b�g�̊Ԃ̊p�x������
	double angle = acos(dot);
	
	// �v���[���[���^�[�Q�b�g�ɖʂ��Ă���ꍇ��true��Ԃ�
	if (angle < 0.00001) return true;
	
	// �ő��]���ɂȂ�悤�ɗʂ��N�����v����
	if (angle > m_dMaxTurnRate) angle = m_dMaxTurnRate;
  
	// ��]�s����g�p���āA�v���[���[��
	// �����x�N�g��������ɉ����ĉ�]������
	C2DMatrix RotationMatrix;
  
	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));	
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	// �Ō��m_vSide���č쐬����
	m_vSide = m_vHeading.Perp();

	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SetHeading�N���X
//
//	�w�肳�ꂽ���o���������[���̃x�N�g���łȂ����Ƃ��܂��`�F�b�N����
//	�V�������o�����L���ł���ꍇ�A�����̓G���e�B�e�B�̌��o����
//	�T�C�h�x�N�g��������ɉ����Đݒ肷��
//
///////////////////////////////////////////////////////////////////////////
inline void MovingEntity::SetHeading(Vector2D new_heading)
{
	assert( (new_heading.LengthSq() - 1.0) < 0.00001);
	
	m_vHeading = new_heading;
	
	// �T�C�h�x�N�g���͏�Ɍ��o���ɐ����łȂ���΂Ȃ�Ȃ�
	m_vSide = m_vHeading.Perp();
}

#endif