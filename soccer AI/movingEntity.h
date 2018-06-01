/*=======================================================================

	ムービングエンティティ [movingEntity.h]

												長岡佑樹

	移動するエンティティを定義する基本クラス

=========================================================================*/
#ifndef MOVING_ENTITY
#define MOVING_ENTITY

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <cassert>
#include "vector2D.h"
#include "baseGameEntity.h"

/////////////////////////////////////////////////////////
//
//	MovingEntityクラス
//
/////////////////////////////////////////////////////////
class MovingEntity : public BaseGameEntity
{
protected:
	Vector2D    m_vVelocity;
	
	// エンティティが向いている方向を指す正規化されたベクトル
	Vector2D    m_vHeading;
	
	// m_vHeadingベクトルに垂直なベクトル
	Vector2D    m_vSide; 
	
	double      m_dMass;
	
	// エンティティが移動できる最大速度
	double      m_dMaxSpeed;
	
	// エンティティが自らの力を発揮できる最大の力
	double      m_dMaxForce;
	
	// 回転する最大速度
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
//	RotateHeadingToFacePositionクラス
//
//	目標位置が与えられると、エンティティの方向ベクトルとサイドベクトルを、
//	ターゲットに直接向くまでm_dMaxTurnRate以下の量だけ回転させ
//	方向が目的の方向に向いているときにtrueを返す
//
///////////////////////////////////////////////////////////////////////////
inline bool MovingEntity::RotateHeadingToFacePosition(Vector2D target)
{
	Vector2D toTarget = Vec2DNormalize(target - m_vPosition);
	
	double dot = m_vHeading.Dot(toTarget);
	
	// いくつかのコンパイラが精度を失うため、
	// acosに有効であることを保証するために値がクランプされる
	Clamp(dot, -1, 1);
	
	// 最初に方向ベクトルとターゲットの間の角度を決定
	double angle = acos(dot);
	
	// プレーヤーがターゲットに面している場合はtrueを返す
	if (angle < 0.00001) return true;
	
	// 最大回転率になるように量をクランプする
	if (angle > m_dMaxTurnRate) angle = m_dMaxTurnRate;
  
	// 回転行列を使用して、プレーヤーの
	// 方向ベクトルをそれに応じて回転させる
	C2DMatrix RotationMatrix;
  
	RotationMatrix.Rotate(angle * m_vHeading.Sign(toTarget));	
	RotationMatrix.TransformVector2Ds(m_vHeading);
	RotationMatrix.TransformVector2Ds(m_vVelocity);

	// 最後にm_vSideを再作成する
	m_vSide = m_vHeading.Perp();

	return false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SetHeadingクラス
//
//	指定された見出しが長さゼロのベクトルでないことをまずチェックする
//	新しい見出しが有効である場合、方向はエンティティの見出しと
//	サイドベクトルをそれに応じて設定する
//
///////////////////////////////////////////////////////////////////////////
inline void MovingEntity::SetHeading(Vector2D new_heading)
{
	assert( (new_heading.LengthSq() - 1.0) < 0.00001);
	
	m_vHeading = new_heading;
	
	// サイドベクトルは常に見出しに垂直でなければならない
	m_vSide = m_vHeading.Perp();
}

#endif