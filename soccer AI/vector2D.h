/*=======================================================================

	ベクター2D  [vector2D.h]

												長岡佑樹

=========================================================================*/
#ifndef S2DVECTOR_H
#define S2DVECTOR_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <math.h>
#include <windows.h>
#include <iosfwd>
#include <limits>
#include "utils.h"

/////////////////////////////////////////////////////////
//
//	Vector2Dクラス
//
/////////////////////////////////////////////////////////
struct Vector2D
{
  double x;
  double y;
  
  Vector2D():x(0.0),y(0.0){}
  Vector2D(double a, double b):x(a),y(b){}

  // xとyに0をセットする
  void Zero(){x=0.0; y=0.0;}

  // xとyが0ならばtrueを返す
  bool isZero()const{return (x*x + y*y) < MinDouble;}

  // ベクトルの長さを返す
  inline double    Length()const;

  // ベクトルの長さの2乗を返す(これによりsqrtを避ける)
  inline double    LengthSq()const;

  // 正規化する
  inline void      Normalize();

  // thisとv2の内積を返す
  inline double    Dot(const Vector2D& v2)const;

  // v2がthisベクトルに対して時計回り方向にあるなら正の値,
  // 反時計回り方向にあるなら負(Windowsアプリのようにy軸は下向き,
  // X軸は右向きと仮定)を返す
  inline int       Sign(const Vector2D& v2)const;

  // thisに垂直なベクトルを返す
  inline Vector2D  Perp()const;

  // ベクトルの長さが最大値を超えないようxとyの値を補正する 
  inline void      Truncate(double max);

  // thisベクトルとパラメーターとして渡されたベクトル間の距離を返す
  inline double    Distance(const Vector2D &v2)const;

  // ↑の平方版
  inline double    DistanceSq(const Vector2D &v2)const;

  inline void      Reflect(const Vector2D& norm);

  // このベクトルの逆ベクトルを返す
  inline Vector2D  GetReverse()const;


  // 必要となるいくつかの演算
  const Vector2D& operator+=(const Vector2D &rhs)
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  const Vector2D& operator-=(const Vector2D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;

    return *this;
  }

  const Vector2D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;

    return *this;
  }

  bool operator==(const Vector2D& rhs)const
  {
    return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
  }

  bool operator!=(const Vector2D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y);
  }
  
};

/////////////////////////////////////////////////////////
//
//	オペレーターオーバーロード
//
/////////////////////////////////////////////////////////
inline Vector2D operator*(const Vector2D &lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);

////////////////////////////////////////////////////
//
// Length関数
//  
////////////////////////////////////////////////////
inline double Vector2D::Length()const
{
  return sqrt(x * x + y * y);
}


////////////////////////////////////////////////////
//
// LengthSq関数
//  
////////////////////////////////////////////////////
inline double Vector2D::LengthSq()const
{
  return (x * x + y * y);
}


////////////////////////////////////////////////////
//
// Dot関数
//  
////////////////////////////////////////////////////
inline double Vector2D::Dot(const Vector2D &v2)const
{
  return x*v2.x + y*v2.y;
}

////////////////////////////////////////////////////
//
// Sign関数
//  
////////////////////////////////////////////////////
enum {clockwise = 1, anticlockwise = -1};

inline int Vector2D::Sign(const Vector2D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

////////////////////////////////////////////////////
//
// Perp関数
//  
////////////////////////////////////////////////////
inline Vector2D Vector2D::Perp()const
{
  return Vector2D(-y, x);
}

////////////////////////////////////////////////////
//
// Distance関数
//  
////////////////////////////////////////////////////
inline double Vector2D::Distance(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


////////////////////////////////////////////////////
//
// DistanceSq関数
//  
////////////////////////////////////////////////////
inline double Vector2D::DistanceSq(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

////////////////////////////////////////////////////
//
// Truncate関数
//  
////////////////////////////////////////////////////
inline void Vector2D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

////////////////////////////////////////////////////
//
// Reflect関数
//  
////////////////////////////////////////////////////
inline void Vector2D::Reflect(const Vector2D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

////////////////////////////////////////////////////
//
// GetReverse関数
//  
////////////////////////////////////////////////////
inline Vector2D Vector2D::GetReverse()const
{
  return Vector2D(-this->x, -this->y);
}

////////////////////////////////////////////////////
//
// Normalize関数
//  
////////////////////////////////////////////////////
inline void Vector2D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}

////////////////////////////////////////////////////
//
// Vec2DNormalize関数
//  
////////////////////////////////////////////////////
inline Vector2D Vec2DNormalize(const Vector2D &v)
{
  Vector2D vec = v;

  double vector_length = vec.Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}

////////////////////////////////////////////////////
//
// Vec2DDistance関数
//  
////////////////////////////////////////////////////
inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

////////////////////////////////////////////////////
//
// Vec2DDistanceSq関数
//  
////////////////////////////////////////////////////
inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

////////////////////////////////////////////////////
//
// Vec2DLength関数
//  
////////////////////////////////////////////////////
inline double Vec2DLength(const Vector2D& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

////////////////////////////////////////////////////
//
// Vec2DLengthSq関数
//  
////////////////////////////////////////////////////
inline double Vec2DLengthSq(const Vector2D& v)
{
  return (v.x*v.x + v.y*v.y);
}

////////////////////////////////////////////////////
//
// POINTStoVector関数
//  
////////////////////////////////////////////////////
inline Vector2D POINTStoVector(const POINTS& p)
{
  return Vector2D(p.x, p.y);
}

////////////////////////////////////////////////////
//
// POINTtoVector関数
//  
////////////////////////////////////////////////////
inline Vector2D POINTtoVector(const POINT& p)
{
  return Vector2D((double)p.x, (double)p.y);
}

////////////////////////////////////////////////////
//
// VectorToPOINTS関数
//  
////////////////////////////////////////////////////
inline POINTS VectorToPOINTS(const Vector2D& v)
{
  POINTS p;
  p.x = (short)v.x;
  p.y = (short)v.y;

  return p;
}

////////////////////////////////////////////////////
//
// VectorToPOINT関数
//  
////////////////////////////////////////////////////
inline POINT VectorToPOINT(const Vector2D& v)
{
  POINT p;
  p.x = (long)v.x;
  p.y = (long)v.y;

  return p;
}


////////////////////////////////////////////////////
//
// オペレーターオーバーロード
//  
////////////////////////////////////////////////////
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
  Vector2D result(lhs);
  result *= rhs;
  return result;
}

////////////////////////////////////////////////////
//
// オペレーターオーバーロード
//  
////////////////////////////////////////////////////
inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
  Vector2D result(rhs);
  result *= lhs;
  return result;
}

////////////////////////////////////////////////////
//
// オペレーター　- オーバーロード
//  
////////////////////////////////////////////////////
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  
  return result;
}

////////////////////////////////////////////////////
//
// オペレーター　+ オーバーロード
//  
////////////////////////////////////////////////////
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x += rhs.x;
  result.y += rhs.y;
  
  return result;
}

////////////////////////////////////////////////////
//
// オペレーター　/ オーバーロード
//  
////////////////////////////////////////////////////
inline Vector2D operator/(const Vector2D &lhs, double val)
{
  Vector2D result(lhs);
  result.x /= val;
  result.y /= val;

  return result;
}

////////////////////////////////////////////////////
//
// WrapAround関数
//  
////////////////////////////////////////////////////
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
  if (pos.x > MaxX) {pos.x = 0.0;}

  if (pos.x < 0)    {pos.x = (double)MaxX;}

  if (pos.y < 0)    {pos.y = (double)MaxY;}

  if (pos.y > MaxY) {pos.y = 0.0;}
}

////////////////////////////////////////////////////
//
// NotInsideRegion関数
//  
////////////////////////////////////////////////////
inline bool NotInsideRegion(Vector2D p,
                            Vector2D top_left,
                            Vector2D bot_rgt)
{
  return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y);
}

////////////////////////////////////////////////////
//
// InsideRegion関数
//  
////////////////////////////////////////////////////
inline bool InsideRegion(Vector2D p,
                         Vector2D top_left,
                         Vector2D bot_rgt)
{
  return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y));
}

////////////////////////////////////////////////////
//
// InsideRegion関数
//  
////////////////////////////////////////////////////
inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
  return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

////////////////////////////////////////////////////
//
// isSecondInFOVOfFirst関数
//  
////////////////////////////////////////////////////
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
                                 Vector2D facingFirst,
                                 Vector2D posSecond,
                                 double    fov)
{
  Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}

#endif