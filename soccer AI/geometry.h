/*=======================================================================

	ジオメトリ [geometry.h]

												長岡佑樹

=========================================================================*/
#ifndef GEOMETRY_H
#define GEOMETRY_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
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
//	DistanceToRayPlaneIntersection関数
//
// プレーンとレイが与えられると,この関数はレイに沿って
// どのくらいの距離で関心が生じるかを決定する
// 光線が平行である場合は負の値を返す
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
  
  // 法線ベクトルに平行
  if ((denom < 0.000001) && (denom > -0.000001))
  {
   return (-1.0);
  }

  return -(numer / denom);	
}

enum span_type{plane_backside, plane_front, on_plane};


/////////////////////////////////////////////////////////
//
//	WhereIsPoint関数
//
/////////////////////////////////////////////////////////
inline span_type WhereIsPoint(Vector2D point,
                              Vector2D PointOnPlane, // 平面上の任意の点
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
//	GetRayCircleIntersect関数
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

   // 交点がない場合は、-1を返す
   if (d < 0.0) return (-1.0);

   // 最初の交点までの距離を返す
   return (v - sqrt(d));
}

/////////////////////////////////////////////////////////
//
//	DoRayCircleIntersect関数
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

   // 交点がない場合は、-1を返す
   return (d < 0.0);
}

////////////////////////////////////////////////////////////////
//
//	GetTangentPoints関数
//
// 点Pおよび半径Rの円がCを中心として与えられると、
// この関数は、Pから円への接線と交差する円上の2つの点を決定する
// Pが円内にある場合はfalseを返す
// 
////////////////////////////////////////////////////////////////
inline bool GetTangentPoints (Vector2D C, double R, Vector2D P, Vector2D& T1, Vector2D& T2)
{
  Vector2D PmC = P - C;
  float SqrLen = (float)PmC.LengthSq();
  float RSqr = (float)R * (float)R;
  if ( SqrLen <= RSqr )
  {
      // Pは円の内側または上
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
//	GetTangentPoints関数
//
// 線分ABと点Pが与えられると、
// この関数はそれらの間の垂直距離を計算する
// 
////////////////////////////////////////////////////////////////
inline double DistToLineSegment(Vector2D A,
                                Vector2D B,
                                Vector2D P)
{
  // 角度がPAとABの間に鈍角であれば、最も近い頂点はA
  double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistance(A, P);

  // 角度がPBとABとの間に鈍角である場合、ABは鈍角であり、最も近い頂点はB
  double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistance(B, P);
    
  // ABに沿ってPに最も近い点を計算する
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  // 点Pの距離を計算する
  return Vec2DDistance(P,Point);
}

////////////////////////////////////////////////////////////////
//
//	DistToLineSegmentSq関数(sqrt)
//
////////////////////////////////////////////////////////////////
inline double DistToLineSegmentSq(Vector2D A,
                                 Vector2D B,
                                 Vector2D P)
{
  // 角度がPAとABの間に鈍角であれば、最も近い頂点はA
  double dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistanceSq(A, P);

  // 角度がPBとABとの間に鈍角である場合、ABは鈍角であり、最も近い頂点はB
  double dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistanceSq(B, P);
    
  // ABに沿ってPに最も近い点を計算する
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  // 点Pの距離を計算する
  return Vec2DDistanceSq(P,Point);
}

////////////////////////////////////////////////////////////////
//
//	LineIntersection2D関数
//
//	2D空間ABの2行が与えられた場合、
//  CDは交差が発生すると真を返す
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
    // 線が交差する 
    return true;
  }

    // 線が交差しない 
  return false;
}

////////////////////////////////////////////////////////////////
//
//	LineIntersection2D関数
//
//	2D空間ABで2行が与えられた場合、
//	CDは交差が発生するとtrueを返し、
//  distは交差をABに沿って生じる距離に設定
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
//	LineIntersection2D関数
//
//	2D空間ABの2行が与えられた場合、
//	CDは交差が発生するとtrueを返し、
//  distは交差をABに沿って発生する距離に設定
//  また、2次元ベクトル点を交点に設定
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
		// 線は平行
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
//	ObjectIntersection2D関数
//
//	交差の2つのポリゴンをテスト
//
////////////////////////////////////////////////////////////////
inline bool ObjectIntersection2D(const std::vector<Vector2D>& object1,
                                 const std::vector<Vector2D>& object2)
{
  // object2の各セグメントに対してobject1の各線分をテストする
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
//	SegmentObjectIntersection2D関数
//
//  交点のポリゴンに対して線分をテストする
//
////////////////////////////////////////////////////////////////
inline bool SegmentObjectIntersection2D(const Vector2D& A,
                                 const Vector2D& B,
                                 const std::vector<Vector2D>& object)
{
  // オブジェクトの各セグメントに対してABをテストする
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
//	TwoCirclesOverlapped関数
//
// 2つの円が重なっている場合はtrueを返す
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
//	TwoCirclesOverlapped関数
//
// 2つの円が重なっている場合はtrueを返す
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
//	TwoCirclesEnclosed関数
//
//	1つの円が他の円を囲む場合はtrueを返す
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
//	TwoCirclesIntersectionPoints関数
//
//	2つの円があれば、この関数は交点を計算します
//	オーバーラップする
//
////////////////////////////////////////////////////////////////
inline bool TwoCirclesIntersectionPoints(double x1, double y1, double r1,
                                  double x2, double y2, double r2,
                                  double &p3X, double &p3Y,
                                  double &p4X, double &p4Y)
{
  // 最初に重複していないかどうか確認
  if (!TwoCirclesOverlapped(x1,y1,r1,x2,y2,r2))
  {
    return false;
  }

  // 円の中心間の距離を計算する
  double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
  
  // 各円の中心から交点を結ぶ線の中心までの距離を計算
  double a = (r1 - r2 + (d * d)) / (2 * d);
  double b = (r2 - r1 + (d * d)) / (2 * d);
  
  // 交点を結ぶ線の中心である点P2を算出する
  double p2X, p2Y;

  p2X = x1 + a * (x2 - x1) / d;
  p2Y = y1 + a * (y2 - y1) / d;

  // 最初の点を計算する
  double h1 = sqrt((r1 * r1) - (a * a));

  p3X = p2X - h1 * (y2 - y1) / d;
  p3Y = p2Y + h1 * (x2 - x1) / d;


  // 2番目の点を計算する
  double h2 = sqrt((r2 * r2) - (a * a));

  p4X = p2X + h2 * (y2 - y1) / d;
  p4Y = p2Y - h2 * (x2 - x1) / d;

  return true;

}

////////////////////////////////////////////////////////////////
//
//	TwoCirclesIntersectionArea関数
//
//  2つの円が重なり合っているかどうかを調べ
//  そうであれば、結合によって定義された領域を計算する
//
////////////////////////////////////////////////////////////////
inline double TwoCirclesIntersectionArea(double x1, double y1, double r1,
                                  double x2, double y2, double r2)
{
  // 最初に交点を計算する
  double iX1, iY1, iX2, iY2;

  if(!TwoCirclesIntersectionPoints(x1,y1,r1,x2,y2,r2,iX1,iY1,iX2,iY2))
  {
    return 0.0; // オーバーラップしない
  }

  // 円の中心間の距離を計算する
  double d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));

  // AとBが2つの円の中心であり、CとDが交点であるとする
  double CBD = 2 * acos((r2*r2 + d*d - r1*r1) / (r2 * d * 2)); 

  double CAD = 2 * acos((r1*r1 + d*d - r2*r2) / (r1 * d * 2));


  // 円CDCDによって区切られた各円のセグメントを、
  // 円BCDのセクタの領域をとり、三角BCDの領域を引くことによって
  // 見つけ,同様にセクタACDの面積を求め、
  // 三角ACDの面積を引く

  double area = 0.5f*CBD*r2*r2 - 0.5f*r2*r2*sin(CBD) +
                0.5f*CAD*r1*r1 - 0.5f*r1*r1*sin(CAD);

  return area;
}

////////////////////////////////////////////////////////////////
//
//	CircleArea関数
//
//	半径を指定して、円の面積を計算する  
//
////////////////////////////////////////////////////////////////
inline double CircleArea(double radius)
{
  return pi * radius * radius;
}

////////////////////////////////////////////////////////////////
//
//	PointInCircle関数
//
//  点pが指定された円の半径内にある場合はtrueを返す
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
//	PointInCircle関数
//
//  線分ABが半径radiusの位置Pの円と交差する場合はtrueを返します
//
////////////////////////////////////////////////////////////////
inline bool   LineSegmentCircleIntersection(Vector2D A,
                                            Vector2D B,
                                            Vector2D P,
                                            double    radius)
{
  // 円の中心から線分までの距離を求める（距離の2乗空間で働く）
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
//	PointInCircle関数
//
//
//  線分ABと円の位置と半径が与えられると、
//  この関数は交点があるかどうかを判定し、
//  最も近い交点の位置を基準交点に格納する
//
//  交点が見つからない場合はfalseを返す
//
////////////////////////////////////////////////////////////////
inline bool GetLineSegmentCircleClosestIntersectionPoint(Vector2D A,
                                                         Vector2D B,
                                                         Vector2D pos,
                                                         double    radius,
                                                         Vector2D& IntersectionPoint)
{
	Vector2D toBNorm = Vec2DNormalize(B-A);

  // Aを原点とするベクトルB-Aによって定義されるローカル空間に円を移動する
  Vector2D LocalPos = PointToLocalSpace(pos, toBNorm, toBNorm.Perp(), A);

  bool ipFound = false;

  // ローカル座標+半径が負の場合、円は後ろ
  // 点Aのように交点はなし、LocalPos.xから
  // 半径が長さA-Bより大きい場合、円は線分と交差できない
  if ( (LocalPos.x+radius >= 0) &&
     ( (LocalPos.x-radius)*(LocalPos.x-radius) <= Vec2DDistanceSq(B, A)) )
  {
	  // x軸からオブジェクトの位置までの距離が半径よりも小さい場合、
	  // 潜在的な交点がある
     if (fabs(LocalPos.y) < radius)
     {
        // ライン/サークルの交差テストを行う
        // 円の中心はA、Bで表される
        // 交点はx = A +/- qrt（r ^ 2-B ^ 2）、y = 0の式で与えられる
        //　xの最小の正の値を見る
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

              




















