/*=======================================================================

	�T�b�J�[�{�[�� [soccerBall.h]

												�����C��

	�T�b�J�[�{�[������������N���X
	���̃N���X��MovingEntity����p������A�Փ˃e�X�g��
	�ʒu�\���̂��߂̂���Ȃ�@�\��񋟂���

=========================================================================*/
#ifndef SOCCERBALL_H
#define SOCCERBALL_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include "movingEntity.h"
#include "main.h"

/////////////////////////////////////////////////////////
//
//	�O���錾
//
/////////////////////////////////////////////////////////
class Wall2D;
class PlayerBase;

/////////////////////////////////////////////////////////
//
//	SoccerBall�N���X
//
/////////////////////////////////////////////////////////
class SoccerBall : public MovingEntity
{
private:
	
	// �ŏI�X�V���Ƀ{�[���̈ʒu���L�^����
	Vector2D                  m_vOldPos;
	
	// �s�b�`���E���\������ǂւ̃��[�J���Q��
	const std::vector<Wall2D>& m_PitchBoundary;                                      
	
	LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^

protected:
	D3DXVECTOR3 m_pos;										// �\�����W
	D3DXVECTOR2 m_size;										// ��,����

public:
	
	// �{�[�����{�[���ƏՓ˂������ǂ����𒲂ׁA
	// ����ɉ����ă{�[���̑��x�𔽉f����
  void TestCollisionWithWalls(const std::vector<Wall2D>& walls);

  SoccerBall(Vector2D           pos,            
             double               BallSize,
             double               mass,
             std::vector<Wall2D>& PitchBoundary):
  
      // ��{�N���X��ݒ�
      MovingEntity(pos,
                  BallSize,
                  Vector2D(0,0),
                  -1.0,                //max speed - unused
                  Vector2D(0,1),
                  mass,
                  Vector2D(1.0,1.0),  //scale     - unused
                  0,                   //turn rate - unused
                  0),                  //max force - unused
     m_PitchBoundary(PitchBoundary)
  {}
  
  void Init();

  // ��{�N���XUpdate������
  void      Update();

  // ��{�N���X�����_�����O������
  void      Render();

  // ���b�Z�[�W����
  bool      HandleMessage(const Telegram& msg){return false;}

  // �{�[���ɕ������̗͂�������(�L�b�N)
  void      Kick(Vector2D direction, double force);

  // �L�b�N�͂ƊJ�n�_�ƏI���_�ɂ���Ē�`�����g���o�[�X�܂ł̋������l�����āA
  // �{�[�����������J�o�[����̂ɂ����鎞�Ԃ��v�Z
  double    TimeToCoverDistance(Vector2D from,
                               Vector2D to,
                               double     force)const;

  // �{�[�������Ԃŕ\�������ꏊ���v�Z
  Vector2D FuturePosition(double time)const;

  // �v���C���[�ƃS�[���L�[�p�[���{�[����߂邽�߂Ɏg�p
  // ���̃v���C���[�̓{�[����ێ����Ă���Ƃ݂Ȃ���A
  // ����ɉ�����m_pOwner�����������
  void      Trap(){m_vVelocity.Zero();}  

  Vector2D  OldPos()const{return m_vOldPos;}
  
  // �{�[�������]�̈ʒu�ɒu���A���̑��x���[���ɐݒ�
  void      PlaceAtPosition(Vector2D NewPos);
};

// �v���C���[�̃L�b�N�̐��x��ς��邽�߂Ɏg�p����
Vector2D AddNoiseToKick(Vector2D BallPos, Vector2D BallTarget);

#endif