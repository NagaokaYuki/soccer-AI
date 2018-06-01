/*=======================================================================

	�X�e�A�����O�r�w�C�o�[  [steeringBehaviors.h]

												�����C��

=========================================================================*/
#ifndef SteeringBehaviorsS_H
#define SteeringBehaviorsS_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include <windows.h>
#include <string>
#include "vector2D.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class PlayerBase;
class SoccerPitch;
class SoccerBall;
class CWall;
class CObstacle;

/////////////////////////////////////////////////////////
//
//	SteeringBehaviors�N���X
//
/////////////////////////////////////////////////////////
class SteeringBehaviors
{
private:
  
  PlayerBase*   m_pPlayer;		// �v���C���[�ϐ�                                              
								
  SoccerBall*   m_pBall;		// �{�[���ϐ�

  //�@�I�����ꂽ�S�Ă̋����̕������ʂɂ���Đ������ꂽ���Ǘ�
  Vector2D     m_vSteeringForce;

  // �{�[���܂��͗\�z�����{�[���̈ʒu
  Vector2D     m_vTarget;

  // �v���C���[���^�[�Q�b�g����}�����悤�Ƃ��鋗��
  double        m_dInterposeDist;

  double        m_dMultSeparation;

  double        m_dViewDistance;

  // �r�w�C�r�A���A�N�e�B�u�ɂ���K�v�����邩�ǂ����������o�C�i���t���O
  int           m_iFlags;

  enum behavior_type
  {
    none               = 0x0000,	// ���� 
    seek               = 0x0001,	// �V�[�N
    arrive             = 0x0002,	// ����
    separation         = 0x0004,	// ����
    pursuit            = 0x0008,	// �ǋ�
    interpose          = 0x0010		// ���
  };

  // �אl�Ƀ^�O��t���邽�߂ɃO���[�v�s���ɂ���Ďg�p
  bool         m_bTagged;
  
  // �ǂꂭ�炢�����ڕW�Ɍ������Č������ׂ��������肷��
  enum Deceleration{slow = 3, normal = 2, fast = 1};


  // ���̓���́A�G�[�W�F���g��ڕW�ʒu�Ɍ������Ĉړ�������
  Vector2D Seek(Vector2D target);

  // �[�����x�Ń^�[�Q�b�g�ɓ��B���悤����
  Vector2D Arrive(Vector2D target, Deceleration decel);
   
  // �^�[�Q�b�g���ǂ��ɂ���̂���\�����A���̏ꏊ��T��
  Vector2D Pursuit(const SoccerBall* ball);
 
  Vector2D Separation();

  // ����ƃI�u�W�F�N�g�Ƃ̊Ԃ̈ʒu�ɃG�[�W�F���g��U�����悤�Ƃ������
  Vector2D Interpose(const SoccerBall* ball,
                     Vector2D pos,
                     double    DistFromTarget);


  // �r���[���a���̔C�ӂ̋ߗׂ�������
  void      FindNeighbours();


  // ����̃r�b�g���Z�b�g����Ă��邩�ǂ����𒲂ׂ�
  bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}

  bool      AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);

  Vector2D  SumForces();

  // �h���u���̂��߂̒��_�o�b�t�@�[
  std::vector<Vector2D> m_Antenna;

  
public:

  SteeringBehaviors(PlayerBase*       agent,
                    SoccerPitch*  world,
                    SoccerBall*   ball);

  virtual ~SteeringBehaviors(){}

 
  Vector2D Calculate();

  // �i�s�����ɕ��s�ȑ��Ǘ͂̐������Z�o����
  double    ForwardComponent();

  // �����ȑ��Ǘ͂̐������v�Z����
  double    SideComponent();

  Vector2D Force()const{return m_vSteeringForce;}

  // �e�r�w�C�r�A���ǂ̂悤�Ɍv�Z����Ă��邩�����邽�߂̃r�W���A���G�C�h�Ə��������_�����O����
  void      RenderInfo();
  void      RenderAids();

  Vector2D  Target()const{return m_vTarget;}
  void      SetTarget(const Vector2D t){m_vTarget = t;}

  double     InterposeDistance()const{return m_dInterposeDist;}
  void      SetInterposeDistance(double d){m_dInterposeDist = d;}

  bool      Tagged()const{return m_bTagged;}
  void      Tag(){m_bTagged = true;}
  void      UnTag(){m_bTagged = false;}
  

  void SeekOn(){m_iFlags |= seek;}
  void ArriveOn(){m_iFlags |= arrive;}
  void PursuitOn(){m_iFlags |= pursuit;}
  void SeparationOn(){m_iFlags |= separation;}
  void InterposeOn(double d){m_iFlags |= interpose; m_dInterposeDist = d;}

  
  void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
  void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
  void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
  void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
  void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}


  bool SeekIsOn(){return On(seek);}
  bool ArriveIsOn(){return On(arrive);}
  bool PursuitIsOn(){return On(pursuit);}
  bool SeparationIsOn(){return On(separation);}
  bool InterposeIsOn(){return On(interpose);}

};




#endif