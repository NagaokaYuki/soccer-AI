/*=======================================================================

	�S�[�� [goal.h]

												
												�����C��
	�N���X�ŃT�b�J�[�̃s�b�`�̖ڕW���`��,�ڕW��
	���E�̃|�X�g��\��2��2D�x�N�g���ɂ���Ē�`

	�e�^�C���X�e�b�v�ł́A�S�[�����̓_���ꂽ���ǂ�����
	���f���邽�߂�Scored���\�b�h���Ăяo��

=========================================================================*/
#ifndef GOAL_H
#define GOAL_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "SoccerBall.h"
#include "Vector2D.h"
#include "geometry.h"

/////////////////////////////////////////////////////////
//
//	Goal�N���X
//
/////////////////////////////////////////////////////////
class Goal 
{

private:

  Vector2D   m_vLeftPost;
  Vector2D   m_vRightPost;

  // �ڕW�̌�����\���x�N�g��
  Vector2D   m_vFacing;

  // �S�[�����C���̒��S�̈ʒu
  Vector2D   m_vCenter;

  // Scored�i�j���ڕW�����o���邽�тɁA���ꂪ�C���N�������g�����
  int        m_iNumGoalsScored;

public:

  Goal(Vector2D left, Vector2D right, Vector2D facing):m_vLeftPost(left),
                                                       m_vRightPost(right),
                                                       m_vCenter((left+right)/2.0),
                                                       m_iNumGoalsScored(0),
                                                       m_vFacing(facing)
  {  }

  // ���݂̃{�[���̈ʒu�ƈȑO�̃{�[���̈ʒu���^����ꂽ�ꍇ�A
  // ���̃��\�b�h�́A�{�[�����S�[�����C���𒴂����ꍇ��true��Ԃ��A
  // m_iNumGoalsScored���C���N�������g����
  inline bool Scored(const SoccerBall*const ball);

  Vector2D Center()const{return m_vCenter;}
  Vector2D Facing()const{return m_vFacing;}
  Vector2D LeftPost()const{return m_vLeftPost;}
  Vector2D RightPost()const{return m_vRightPost;}

  int      NumGoalsScored()const{return m_iNumGoalsScored;}
  void     ResetGoalsScored(){m_iNumGoalsScored = 0;}
};

bool Goal::Scored(const SoccerBall*const ball)
{
  if (LineIntersection2D(ball->Pos(), ball->OldPos(), m_vLeftPost, m_vRightPost))
  {
    ++m_iNumGoalsScored;

    return true;
  }

  return false;
}


#endif