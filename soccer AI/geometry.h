/*=======================================================================

	�W�I���g�� [geometry.h]

												�����C��

=========================================================================*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "utils.h"
#include "Vector2D.h"
#include "C2DMatrix.h"
#include "Transformations.h"
#include <math.h>
#include <vector>

/////////////////////////////////////////////////////////
//
//	DistanceToRayPlaneIntersection�֐�
//
// �v���[���ƃ��C���^�������,���̊֐��̓��C�ɉ�����
// �ǂ̂��炢�̋����Ŋ֐S�������邩�����肷��
// ���������s�ł���ꍇ�͕��̒l��Ԃ�
//
/////////////////////////////////////////////////////////
inline double DistanceToRayPlaneIntersection(Vector2D RayOrigin,
                                             Vector2D RayHeading,
                                             Vector2D PlanePoint,  //any point on the plane
                                             Vector2D PlaneNormal)
{
  
  double d     = - PlaneNormal.Dot(PlanePoint);
  double numer = PlaneNormal.Dot(RayOrigin) + d;
  double denom = PlaneNormal.Dot(RayHeading);
  
  // �@���x�N�g���ɕ��s
  if ((denom < 0.000001) && (denom > -0.000001))
  {
   return (-1.0);
  }

  return -(numer / denom);	
}

enum span_type{plane_backside, plane_front, on_plane};


/////////////////////////////////////////////////////////
//
//	WhereIsPoint�֐�
//
/////////////////////////////////////////////////////////
inline span_type WhereIsPoint(Vector2D point,
                              Vector2D PointOnPlane, // ���ʏ�̔C�ӂ̓_
                              Vector2D PlaneNormal) 
{
 Vector2D dir = PointOnPlane - point;

 double d = dir.Dot(PlaneNormal);
 
 if (d<-0.000001)
 {
  return plane_front;	
 }

 else if (d>0.000001)
 {
  return plane_backside;	
 }

  return on_plane;	
}

const double pi = 3.14159;

/////////////////////////////////////////////////////////
//
//	GetRayCircleIntersect�֐�
//
/////////////////////////////////////////////////////////
inline double GetRayCircleIntersect(Vector2D RayOrigin,
                                    Vector2D RayHeading,
                                    Vector2D CircleOrigin,
                                    double  radius)
{
	
   Vector2D ToCircle = CircleOrigin-RayOrigin;
   double length      = ToCircle.Length();
   double v           = ToCircle.Dot(RayHeading);
   double d           = radius*radius - (length*length - v*v);

   // ��_���Ȃ��ꍇ�́A-1��Ԃ�
   if (d < 0.0) return (-1.0);

   // �ŏ��̌�_�܂ł̋�����Ԃ�
   return (v - sqrt(d));
}

/////////////////////////////////////////////////////////
//
//	DoRayCircleIntersect�֐�
//
/////////////////////////////////////////////////////////
inline bool DoRayCircleIntersect(Vector2D RayOrigin,
                                 Vector2D RayHeading,
                                 Vector2D CircleOrigin,
                                 double     radius)
{
	
   Vector2D ToCircle = CircleOrigin-RayOrigin;
   double length      = ToCircle.Length();
   double v           = ToCircle.Dot(RayHeading);
   double d           = radius*radius - (length*length - v*v);

   // ��_���Ȃ��ꍇ�́A-1��Ԃ�
   return (d < 0.0);
}

////////////////////////////////////////////////////////////////
//
//	GetTangentPoints�֐�
//
// �_P����є��aR�̉~��C�𒆐S�Ƃ��ė^������ƁA
// ���̊֐��́AP����~�ւ̐ڐ��ƌ�������~���2�̓_�����肷��
// P���~���ɂ���ꍇ��false��Ԃ�
// 
////////////////////////////////////////////////////////////////
inline bool GetTangentPoints (Vector2D C, double R, Vector2D P, Vector2D& T1, Vector2D& T2)
{
  Vector2D PmC = P - C;
  float SqrLen = (float)PmC.LengthSq();
  float RSqr = (float)R * (float)R;
  if ( SqrLen <= RSqr )
  {
      // P�͉~�̓����܂��͏�
      return false;
  }

  float InvSqrLen = 1/SqrLen;
  float Root = sqrt(fabs(SqrLen - RSqr));

  T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
  T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
  T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
  T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

  return true;
}

////////////////////////////////////////////////////////////////
//
//	GetTangentPoints�֐�
//
// ����AB�Ɠ_P���^������ƁA
// ���̊֐��͂����̊Ԃ̐����������v�Z����
// 
////////////////////////////////////////////////////////////////
inline double DistToLineSegment(Vector2D A,
                                Vector2D B,
                                Vector2D P)
{
  // �p�x��PA��AB�̊Ԃɓ݊p�ł���΁A�ł��߂����_��A
  double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistance(A, P);

  // �p�x��PB��AB�Ƃ̊Ԃɓ݊p�ł���ꍇ�AAB�͓݊p�ł���A�ł��߂����_��B
  double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistance(B, P);
    
  // AB�ɉ�����P�ɍł��߂��_���v�Z����
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  // �_P�̋������v�Z����
  return Vec2DDistance(P,Point);
}

////////////////////////////////////////////////////////////////
//
//	DistToLineSegmentSq�֐�(sqrt)
//
////////////////////////////////////////////////////////////////
inline double DistToLineSegmentSq(Vector2D A,
                                 Vector2D B,
                                 Vector2D P)
{
  // �p�x��PA��AB�̊Ԃɓ݊p�ł���΁A�ł��߂����_��A
  double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistanceSq(A, P);

  // �p�x��PB��AB�Ƃ̊Ԃɓ݊p�ł���ꍇ�AAB�͓݊p�ł���A�ł��߂����_��B
  double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistanceSq(B, P);
    
  // AB�ɉ�����P�ɍł��߂��_���v�Z����
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  // �_P�̋������v�Z����
  return Vec2DDistanceSq(P,Point);
}

////////////////////////////////////////////////////////////////
//
//	LineIntersection2D�֐�
//
//	2D���AB��2�s���^����ꂽ�ꍇ�A
//  CD�͌�������������Ɛ^��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool LineIntersection2D(Vector2D A,
                               Vector2D B,
                               Vector2D C, 
                               Vector2D D)
{
  double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
  double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

  if (Bot == 0)//parallel
  {
    return false;
  }

  double invBot = 1.0/Bot;
	double r = rTop * invBot;
	double s = sTop * invBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
    // ������������ 
    return true;
  }

    // �����������Ȃ� 
  return false;
}

////////////////////////////////////////////////////////////////
//
//	LineIntersection2D�֐�
//
//	2D���AB��2�s���^����ꂽ�ꍇ�A
//	CD�͌��������������true��Ԃ��A
//  dist�͌�����AB�ɉ����Đ����鋗���ɐݒ�
//
////////////////////////////////////////////////////////////////
inline bool LineIntersection2D(Vector2D A,
                        Vector2D B,
                        Vector2D C, 
                        Vector2D D,
                        double &dist)
{

  double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
  double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);


  if (Bot == 0)// parallel
  {
    if (isEqual(rTop, 0) && isEqual(sTop, 0))
    {
      return true;
    }
    return false;
  }

	double r = rTop/Bot;
	double s = sTop/Bot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
  	dist = Vec2DDistance(A,B) * r;

    return true;
  }

	else
  {
		dist = 0;

    return false;
  }
}

////////////////////////////////////////////////////////////////
//
//	LineIntersection2D�֐�
//
//	2D���AB��2�s���^����ꂽ�ꍇ�A
//	CD�͌��������������true��Ԃ��A
//  dist�͌�����AB�ɉ����Ĕ������鋗���ɐݒ�
//  �܂��A2�����x�N�g���_����_�ɐݒ�
//
////////////////////////////////////////////////////////////////
inline bool LineIntersection2D(Vector2D   A,
                               Vector2D   B,
                               Vector2D   C, 
                               Vector2D   D,
                               double&     dist,
                               Vector2D&  point)
{

  double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
	double rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
	double sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	if ( (rBot == 0) || (sBot == 0))
	{
		// ���͕��s
		return false;
	}

	double r = rTop/rBot;
	double s = sTop/sBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
  	dist = Vec2DDistance(A,B) * r;

    point = A + r * (B - A);

    return true;
  }

	else
  {
		dist = 0;

    return false;
  }
}

////////////////////////////////////////////////////////////////
//
//	ObjectIntersection2D�֐�
//
//	������2�̃|���S�����e�X�g
//
////////////////////////////////////////////////////////////////
inline bool ObjectIntersection2D(const std::vector<Vector2D>& object1,
                                 const std::vector<Vector2D>& object2)
{
  // object2�̊e�Z�O�����g�ɑ΂���object1�̊e�������e�X�g����
  for (unsigned int r=0; r<object1.size()-1; ++r)
  {
    for (unsigned int t=0; t<object2.size()-1; ++t)
    {
      if (LineIntersection2D(object2[t],
                             object2[t+1],
                             object1[r],
                             object1[r+1]))
      {
        return true;
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	SegmentObjectIntersection2D�֐�
//
//  ��_�̃|���S���ɑ΂��Đ������e�X�g����
//
////////////////////////////////////////////////////////////////
inline bool SegmentObjectIntersection2D(const Vector2D& A,
                                 const Vector2D& B,
                                 const std::vector<Vector2D>& object)
{
  // �I�u�W�F�N�g�̊e�Z�O�����g�ɑ΂���AB���e�X�g����
  for (unsigned int r=0; r<object.size()-1; ++r)
  {
    if (LineIntersection2D(A, B, object[r], object[r+1]))
    {
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesOverlapped�֐�
//
// 2�̉~���d�Ȃ��Ă���ꍇ��true��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool TwoCirclesOverlapped(double x1, double y1, double r1,
                          double x2, double y2, double r2)
{
  double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
                                    (y1-y2) * (y1-y2));

  if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
  {
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesOverlapped�֐�
//
// 2�̉~���d�Ȃ��Ă���ꍇ��true��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool TwoCirclesOverlapped(Vector2D c1, double r1,
                          Vector2D c2, double r2)
{
  double DistBetweenCenters = sqrt( (c1.x-c2.x) * (c1.x-c2.x) +
                                    (c1.y-c2.y) * (c1.y-c2.y));

  if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
  {
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesEnclosed�֐�
//
//	1�̉~�����̉~���͂ޏꍇ��true��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool TwoCirclesEnclosed(double x1, double y1, double r1,
                        double x2, double y2, double r2)
{
  double DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
                                    (y1-y2) * (y1-y2));

  if (DistBetweenCenters < fabs(r1-r2))
  {
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesIntersectionPoints�֐�
//
//	2�̉~������΁A���̊֐��͌�_���v�Z���܂�
//	�I�[�o�[���b�v����
//
////////////////////////////////////////////////////////////////
inline bool TwoCirclesIntersectionPoints(double x1, double y1, double r1,
                                  double x2, double y2, double r2,
                                  double &p3X, double &p3Y,
                                  double &p4X, double &p4Y)
{
  // �ŏ��ɏd�����Ă��Ȃ����ǂ����m�F
  if (!TwoCirclesOverlapped(x1,y1,r1,x2,y2,r2))
  {
    return false;
  }

  // �~�̒��S�Ԃ̋������v�Z����
  double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
  
  // �e�~�̒��S�����_�����Ԑ��̒��S�܂ł̋������v�Z
  double a = (r1 - r2 + (d * d)) / (2 * d);
  double b = (r2 - r1 + (d * d)) / (2 * d);
  
  // ��_�����Ԑ��̒��S�ł���_P2���Z�o����
  double p2X, p2Y;

  p2X = x1 + a * (x2 - x1) / d;
  p2Y = y1 + a * (y2 - y1) / d;

  // �ŏ��̓_���v�Z����
  double h1 = sqrt((r1 * r1) - (a * a));

  p3X = p2X - h1 * (y2 - y1) / d;
  p3Y = p2Y + h1 * (x2 - x1) / d;


  // 2�Ԗڂ̓_���v�Z����
  double h2 = sqrt((r2 * r2) - (a * a));

  p4X = p2X + h2 * (y2 - y1) / d;
  p4Y = p2Y - h2 * (x2 - x1) / d;

  return true;

}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesIntersectionArea�֐�
//
//  2�̉~���d�Ȃ荇���Ă��邩�ǂ����𒲂�
//  �����ł���΁A�����ɂ���Ē�`���ꂽ�̈���v�Z����
//
////////////////////////////////////////////////////////////////
inline double TwoCirclesIntersectionArea(double x1, double y1, double r1,
                                  double x2, double y2, double r2)
{
  // �ŏ��Ɍ�_���v�Z����
  double iX1, iY1, iX2, iY2;

  if(!TwoCirclesIntersectionPoints(x1,y1,r1,x2,y2,r2,iX1,iY1,iX2,iY2))
  {
    return 0.0; // �I�[�o�[���b�v���Ȃ�
  }

  // �~�̒��S�Ԃ̋������v�Z����
  double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));

  // A��B��2�̉~�̒��S�ł���AC��D����_�ł���Ƃ���
  double CBD = 2 * acos((r2*r2 + d*d - r1*r1) / (r2 * d * 2)); 

  double CAD = 2 * acos((r1*r1 + d*d - r2*r2) / (r1 * d * 2));


  // �~CDCD�ɂ���ċ�؂�ꂽ�e�~�̃Z�O�����g���A
  // �~BCD�̃Z�N�^�̗̈���Ƃ�A�O�pBCD�̗̈���������Ƃɂ����
  // ����,���l�ɃZ�N�^ACD�̖ʐς����߁A
  // �O�pACD�̖ʐς�����

  double area = 0.5f*CBD*r2*r2 - 0.5f*r2*r2*sin(CBD) +
                0.5f*CAD*r1*r1 - 0.5f*r1*r1*sin(CAD);

  return area;
}

////////////////////////////////////////////////////////////////
//
//	CircleArea�֐�
//
//	���a���w�肵�āA�~�̖ʐς��v�Z����  
//
////////////////////////////////////////////////////////////////
inline double CircleArea(double radius)
{
  return pi * radius * radius;
}

////////////////////////////////////////////////////////////////
//
//	PointInCircle�֐�
//
//  �_p���w�肳�ꂽ�~�̔��a���ɂ���ꍇ��true��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool PointInCircle(Vector2D Pos,
						  double    radius,
                          Vector2D p)
{
  double DistFromCenterSquared = (p-Pos).LengthSq();

  if (DistFromCenterSquared < (radius*radius))
  {
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////
//
//	PointInCircle�֐�
//
//  ����AB�����aradius�̈ʒuP�̉~�ƌ�������ꍇ��true��Ԃ��܂�
//
////////////////////////////////////////////////////////////////
inline bool   LineSegmentCircleIntersection(Vector2D A,
                                            Vector2D B,
                                            Vector2D P,
                                            double    radius)
{
  // �~�̒��S��������܂ł̋��������߂�i������2���Ԃœ����j
  double DistToLineSq = DistToLineSegmentSq(A, B, P);

  if (DistToLineSq < radius*radius)
  {
    return true;
  }

  else
  {
    return false;
  }

}

////////////////////////////////////////////////////////////////
//
//	PointInCircle�֐�
//
//
//  ����AB�Ɖ~�̈ʒu�Ɣ��a���^������ƁA
//  ���̊֐��͌�_�����邩�ǂ����𔻒肵�A
//  �ł��߂���_�̈ʒu�����_�Ɋi�[����
//
//  ��_��������Ȃ��ꍇ��false��Ԃ�
//
////////////////////////////////////////////////////////////////
inline bool GetLineSegmentCircleClosestIntersectionPoint(Vector2D A,
                                                         Vector2D B,
                                                         Vector2D pos,
                                                         double    radius,
                                                         Vector2D& IntersectionPoint)
{
	Vector2D toBNorm = Vec2DNormalize(B-A);

  // A�����_�Ƃ���x�N�g��B-A�ɂ���Ē�`����郍�[�J����Ԃɉ~���ړ�����
  Vector2D LocalPos = PointToLocalSpace(pos, toBNorm, toBNorm.Perp(), A);

  bool ipFound = false;

  // ���[�J�����W+���a�����̏ꍇ�A�~�͌��
  // �_A�̂悤�Ɍ�_�͂Ȃ��ALocalPos.x����
  // ���a������A-B���傫���ꍇ�A�~�͐����ƌ����ł��Ȃ�
  if ( (LocalPos.x+radius >= 0) &&
     ( (LocalPos.x-radius)*(LocalPos.x-radius) <= Vec2DDistanceSq(B, A)) )
  {
	  // x������I�u�W�F�N�g�̈ʒu�܂ł̋��������a�����������ꍇ�A
	  // ���ݓI�Ȍ�_������
     if (fabs(LocalPos.y) < radius)
     {
        // ���C��/�T�[�N���̌����e�X�g���s��
        // �~�̒��S��A�AB�ŕ\�����
        // ��_��x = A +/- qrt�ir ^ 2-B ^ 2�j�Ay = 0�̎��ŗ^������
        //�@x�̍ŏ��̐��̒l������
        double a = LocalPos.x;
        double b = LocalPos.y;       

        double ip = a - sqrt(radius*radius - b*b);

        if (ip <= 0)
        {
          ip = a + sqrt(radius*radius - b*b);
        }

        ipFound = true;

        IntersectionPoint = A+ toBNorm*ip;
     }
   }

  return ipFound;
}

#endif

              




















