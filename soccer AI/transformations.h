/*=======================================================================

	トランスフォーメーションズ [transformations.h]

												長岡佑樹

	ワールドとローカル空間の間で2Dベクトルを変換する関数

=========================================================================*/
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

////////////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
////////////////////////////////////////////////////////////////
#include <vector>
#include "Vector2D.h"
#include "C2DMatrix.h"
#include "Transformations.h"

////////////////////////////////////////////////////////////////
//
//	WorldTransform関数
//
//	与えられたstd :: vectorの2次元ベクトル、位置、方向、スケール、
//	この関数は、2Dベクトルをオブジェクトのワールド空間に変換します
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> WorldTransform(std::vector<Vector2D> &points,
                                            const Vector2D   &pos,
                                            const Vector2D   &forward,
                                            const Vector2D   &side,
                                            const Vector2D   &scale)
{
	// 元の頂点を変換しようとしているバッファにコピーする
	std::vector<Vector2D> TranVector2Ds = points;
	
	// 変換行列を作成する
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
	
	// オブジェクトの頂点を変換する
	matTransform.TransformVector2Ds(TranVector2Ds);
	
	return TranVector2Ds;
}

////////////////////////////////////////////////////////////////
//
//	WorldTransform関数
//
//	2Dベクトルのstd :: vectorが与えられた場合、
//  この関数は2Dベクトルをオブジェクトのワールド空間に変換します
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> WorldTransform(std::vector<Vector2D> &points,
                                 const Vector2D   &pos,
                                 const Vector2D   &forward,
                                 const Vector2D   &side)
{
	// 元の頂点を変換しようとしているバッファにコピーする
	std::vector<Vector2D> TranVector2Ds = points;
	
	// 変換行列を作成する
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate(forward, side);

	// translate
	matTransform.Translate(pos.x, pos.y);
	
	// オブジェクトの頂点を変換する
	matTransform.TransformVector2Ds(TranVector2Ds);

	return TranVector2Ds;
}

////////////////////////////////////////////////////////////////
//
//	PointToWorldSpace関数
//
//	エージェントのローカル空間からワールド空間にポイントを変換する
//
////////////////////////////////////////////////////////////////
inline Vector2D PointToWorldSpace(const Vector2D &point,
                                    const Vector2D &AgentHeading,
                                    const Vector2D &AgentSide,
                                    const Vector2D &AgentPosition)
{
	// ポイントのコピーを作る
	Vector2D TransPoint = point;
	
	// 変換行列を作成する
	C2DMatrix matTransform;

	// rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	// translate
	matTransform.Translate(AgentPosition.x, AgentPosition.y);
	
	// オブジェクトの頂点を変換する
	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	VectorToWorldSpace関数
//
//		エージェントのローカル空間からワールド空間にベクトルを変換する
//
////////////////////////////////////////////////////////////////
inline Vector2D VectorToWorldSpace(const Vector2D &vec,
                                     const Vector2D &AgentHeading,
                                     const Vector2D &AgentSide)
{
	// ポイントのコピーを作る
	Vector2D TransVec = vec;
	
	// 変換行列を作成する
	C2DMatrix matTransform;

	//rotate
	matTransform.Rotate(AgentHeading, AgentSide);

	// オブジェクトの頂点を変換する
	matTransform.TransformVector2Ds(TransVec);
	
	return TransVec;
}

////////////////////////////////////////////////////////////////
//
//	PointToLocalSpace関数
//
////////////////////////////////////////////////////////////////
inline Vector2D PointToLocalSpace(const Vector2D &point,
                             Vector2D &AgentHeading,
                             Vector2D &AgentSide,
                              Vector2D &AgentPosition)
{

	// ポイントのコピーを作る
	Vector2D TransPoint = point;
  
	// 変換行列を作成する
	C2DMatrix matTransform;

	double Tx = -AgentPosition.Dot(AgentHeading);
	double Ty = -AgentPosition.Dot(AgentSide);
	
	// 変換行列を作成する
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	matTransform._31(Tx);           matTransform._32(Ty);
	
	// 頂点を変換する
	matTransform.TransformVector2Ds(TransPoint);
	
	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	VectorToLocalSpace関数
//
////////////////////////////////////////////////////////////////
inline Vector2D VectorToLocalSpace(const Vector2D &vec,
                             const Vector2D &AgentHeading,
                             const Vector2D &AgentSide)
{ 
	// ポイントのコピーを作る
	Vector2D TransPoint = vec;
  
	// 変換行列を作成する
	C2DMatrix matTransform;

	// 変換行列を作成する
	matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	
	// 頂点を変換する
	matTransform.TransformVector2Ds(TransPoint);

	return TransPoint;
}

////////////////////////////////////////////////////////////////
//
//	Vec2DRotateAroundOrigin関数
//
//  原点の周りにベクトルを回転する
//
////////////////////////////////////////////////////////////////
inline void Vec2DRotateAroundOrigin(Vector2D& v, double ang)
{
	// 変換行列を作成する
	C2DMatrix mat;
	
	//rotate
	mat.Rotate(ang);
	
	// オブジェクトの頂点を変換する
	mat.TransformVector2Ds(v);
}

////////////////////////////////////////////////////////////////
//
//	CreateWhiskers関数
//
//  起点から離れた一連のウィスカーの最終位置を含む
//  ベクトルを返し、それらの間の距離は等しくなる
//
////////////////////////////////////////////////////////////////
inline std::vector<Vector2D> CreateWhiskers(unsigned int  NumWhiskers,
                                            float        WhiskerLength,
                                            float        fov,
                                            Vector2D      facing,
                                            Vector2D      origin)
{
  // 各ウィスカーを分離する角度の大きさ
  float SectorSize = fov/(float)(NumWhiskers-1);

  std::vector<Vector2D> whiskers;
  Vector2D temp;
  float angle = -fov * 0.5f; 

  for (unsigned int w=0; w<NumWhiskers; ++w)
  {
    // この角度で外側に伸びるウィスカーを作る
    temp = facing;
    Vec2DRotateAroundOrigin(temp, angle);
    whiskers.push_back(origin + WhiskerLength * temp);

    angle+=SectorSize;
  }

  return whiskers;
}

#endif