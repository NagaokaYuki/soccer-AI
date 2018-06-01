/*=======================================================================

	�g�����X�t�H�[���[�V�����Y [transformations.h]

												�����C��

	���[���h�ƃ��[�J����Ԃ̊Ԃ�2D�x�N�g����ϊ�����֐�

=========================================================================*/
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

////////////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
////////////////////////////////////////////////////////////////
#include <vector>
#include "Vector2D.h"
#include "C2DMatrix.h"
#include "Transformations.h"

////////////////////////////////////////////////////////////////
//
//	WorldTransform�֐�
//
//	�^����ꂽstd :: vector��2�����x�N�g���A�ʒu�A�����A�X�P�[���A
//	���̊֐��́A2D�x�N�g�����I�u�W�F�N�g�̃��[���h��Ԃɕϊ����܂�
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> WorldTransform(std::vector<Vector2D> &points,
                                            const Vector2D   &pos,
                                            const Vector2D   &forward,
                                            const Vector2D   &side,
                                            const Vector2D   &scale)
{
	// ���̒��_��ϊ����悤�Ƃ��Ă���o�b�t�@�ɃR�s�[����
	std::vector<Vector2D> TranVector2Ds = points;
	
	// �ϊ��s����쐬����
	C2DMatrix matTransform;
	
	// scale
	if ( (scale.x != 1.0) || (scale.y != 1.0) )
	{
		matTransform.Scale(scale.x, scale.y);
	}

	// rotate
	matTransform.Rotate(forward, side);

	// translate
	matTransform.Translate(pos.x, pos.y);
	
	// �I�u�W�F�N�g�̒��_��ϊ�����
	matTransform.TransformVector2Ds(TranVector2Ds);
	
	return TranVector2Ds;
}

////////////////////////////////////////////////////////////////
//
//	WorldTransform�֐�
//
//	2D�x�N�g����std :: vector���^����ꂽ�ꍇ�A
//  ���̊֐���2D�x�N�g�����I�u�W�F�N�g�̃��[���h��Ԃɕϊ����܂�
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> WorldTransform(std::vector<Vector2D> &points,
                                 const Vector2D   &pos,
                                 const Vector2D   &forward,
                                 const Vector2D   &side)
{
	// ���̒��_��ϊ����悤�Ƃ��Ă���o�b�t�@�ɃR�s�[����
	std::vector<Vector2D> TranVector2Ds = points;
	
	// �ϊ��s����쐬����
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate(forward, side);

	// translate
	matTransform.Translate(pos.x, pos.y);
	
	// �I�u�W�F�N�g�̒��_��ϊ�����
	matTransform.TransformVector2Ds(TranVector2Ds);

	return TranVector2Ds;
}

////////////////////////////////////////////////////////////////
//
//	PointToWorldSpace�֐�
//
//	�G�[�W�F���g�̃��[�J����Ԃ��烏�[���h��ԂɃ|�C���g��ϊ�����
//
////////////////////////////////////////////////////////////////
inline Vector2D PointToWorldSpace(const Vector2D &point,
                                    const Vector2D &AgentHeading,
                                    const Vector2D &AgentSide,
                                    const Vector2D &AgentPosition)
{
	// �|�C���g�̃R�s�[�����
	Vector2D TransPoint = point;
	
	// �ϊ��s����쐬����
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	// translate
	matTransform.Translate(AgentPosition.x, AgentPosition.y);
	
	// �I�u�W�F�N�g�̒��_��ϊ�����
	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	VectorToWorldSpace�֐�
//
//		�G�[�W�F���g�̃��[�J����Ԃ��烏�[���h��ԂɃx�N�g����ϊ�����
//
////////////////////////////////////////////////////////////////
inline Vector2D VectorToWorldSpace(const Vector2D &vec,
                                     const Vector2D &AgentHeading,
                                     const Vector2D &AgentSide)
{
	// �|�C���g�̃R�s�[�����
	Vector2D TransVec = vec;
	
	// �ϊ��s����쐬����
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	// �I�u�W�F�N�g�̒��_��ϊ�����
	matTransform.TransformVector2Ds(TransVec);
	
	return TransVec;
}

////////////////////////////////////////////////////////////////
//
//	PointToLocalSpace�֐�
//
////////////////////////////////////////////////////////////////
inline Vector2D PointToLocalSpace(const Vector2D &point,
                             Vector2D &AgentHeading,
                             Vector2D &AgentSide,
                              Vector2D &AgentPosition)
{

	// �|�C���g�̃R�s�[�����
	Vector2D TransPoint = point;
  
	// �ϊ��s����쐬����
	C2DMatrix matTransform;

	double Tx = -AgentPosition.Dot(AgentHeading);
	double Ty = -AgentPosition.Dot(AgentSide);
	
	// �ϊ��s����쐬����
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	matTransform._31(Tx);           matTransform._32(Ty);
	
	// ���_��ϊ�����
	matTransform.TransformVector2Ds(TransPoint);
	
	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	VectorToLocalSpace�֐�
//
////////////////////////////////////////////////////////////////
inline Vector2D VectorToLocalSpace(const Vector2D &vec,
                             const Vector2D &AgentHeading,
                             const Vector2D &AgentSide)
{ 
	// �|�C���g�̃R�s�[�����
	Vector2D TransPoint = vec;
  
	// �ϊ��s����쐬����
	C2DMatrix matTransform;

	// �ϊ��s����쐬����
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	
	// ���_��ϊ�����
	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	Vec2DRotateAroundOrigin�֐�
//
//  ���_�̎���Ƀx�N�g������]����
//
////////////////////////////////////////////////////////////////
inline void Vec2DRotateAroundOrigin(Vector2D& v, double ang)
{
	// �ϊ��s����쐬����
	C2DMatrix mat;
	
	//rotate
	mat.Rotate(ang);
	
	// �I�u�W�F�N�g�̒��_��ϊ�����
	mat.TransformVector2Ds(v);
}

////////////////////////////////////////////////////////////////
//
//	CreateWhiskers�֐�
//
//  �N�_���痣�ꂽ��A�̃E�B�X�J�[�̍ŏI�ʒu���܂�
//  �x�N�g����Ԃ��A�����̊Ԃ̋����͓������Ȃ�
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> CreateWhiskers(unsigned int  NumWhiskers,
                                            float        WhiskerLength,
                                            float        fov,
                                            Vector2D      facing,
                                            Vector2D      origin)
{
  // �e�E�B�X�J�[�𕪗�����p�x�̑傫��
  float SectorSize = fov/(float)(NumWhiskers-1);

  std::vector<Vector2D> whiskers;
  Vector2D temp;
  float angle = -fov * 0.5f; 

  for (unsigned int w=0; w<NumWhiskers; ++w)
  {
    // ���̊p�x�ŊO���ɐL�т�E�B�X�J�[�����
    temp = facing;
    Vec2DRotateAroundOrigin(temp, angle);
    whiskers.push_back(origin + WhiskerLength * temp);

    angle+=SectorSize;
  }

  return whiskers;
}

#endif