/*=======================================================================

	ウィンドウ表示処理 [C2DMatrix.h]

												長岡佑樹

=========================================================================*/
#ifndef C2DMATRIX_H
#define C2DMATRIX_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <math.h>
#include <vector>
#include "utils.h"
#include "Vector2D.h"

/////////////////////////////////////////////////////////
//
//	C2DMatrixクラス
//
/////////////////////////////////////////////////////////
class C2DMatrix
{
private:
	struct Matrix
	{
		double _11, _12, _13;
		double _21, _22, _23;
		double _31, _32, _33;
		
		Matrix()
		{
		  _11=0.0; _12=0.0; _13=0.0;
		  _21=0.0; _22=0.0; _23=0.0;
		  _31=0.0; _32=0.0; _33=0.0;
		}
	};

	Matrix m_Matrix;
	
	// m_MatrixとmInを掛け合わせる
	inline void  MatrixMultiply(Matrix &mIn);

public:

	C2DMatrix()
	{
		// 行列を単位行列に初期化する
		Identity();
	}
	
	// 単位行列を作成する
	inline void Identity();
	
	// 変換行列を作成する
	inline void Translate(double x, double y);
	
	// スケール行列を作成する
	inline void Scale(double xScale, double yScale);
	
	// 回転行列を作成する
	inline void  Rotate(double rotation);
	
	// fwdとサイド2Dベクトルから回転行列を作成す
	inline void  Rotate(const Vector2D &fwd, const Vector2D &side);
	
	 // 点のstd :: vectorに変換行列を適用する
	inline void TransformVector2Ds(std::vector<Vector2D> &vPoints);
	
	// 点に変換行列を適用する
	inline void TransformVector2Ds(Vector2D &vPoint);
	
	// マトリックス要素へのアクセサ
	void _11(double val){m_Matrix._11 = val;}
	void _12(double val){m_Matrix._12 = val;}
	void _13(double val){m_Matrix._13 = val;}
	
	void _21(double val){m_Matrix._21 = val;}
	void _22(double val){m_Matrix._22 = val;}
	void _23(double val){m_Matrix._23 = val;}
	
	void _31(double val){m_Matrix._31 = val;}
	void _32(double val){m_Matrix._32 = val;}
	void _33(double val){m_Matrix._33 = val;}
};

/////////////////////////////////////////////////////////
//
//	MatrixMultiply関数
//
//	2つの行列を一緒に乗算する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::MatrixMultiply(Matrix &mIn)
{
	C2DMatrix::Matrix mat_temp;
	
	mat_temp._11 = (m_Matrix._11*mIn._11) + (m_Matrix._12*mIn._21) + (m_Matrix._13*mIn._31);
	mat_temp._12 = (m_Matrix._11*mIn._12) + (m_Matrix._12*mIn._22) + (m_Matrix._13*mIn._32);
	mat_temp._13 = (m_Matrix._11*mIn._13) + (m_Matrix._12*mIn._23) + (m_Matrix._13*mIn._33);
	
	mat_temp._21 = (m_Matrix._21*mIn._11) + (m_Matrix._22*mIn._21) + (m_Matrix._23*mIn._31);
	mat_temp._22 = (m_Matrix._21*mIn._12) + (m_Matrix._22*mIn._22) + (m_Matrix._23*mIn._32);
	mat_temp._23 = (m_Matrix._21*mIn._13) + (m_Matrix._22*mIn._23) + (m_Matrix._23*mIn._33);
	
	mat_temp._31 = (m_Matrix._31*mIn._11) + (m_Matrix._32*mIn._21) + (m_Matrix._33*mIn._31);
	mat_temp._32 = (m_Matrix._31*mIn._12) + (m_Matrix._32*mIn._22) + (m_Matrix._33*mIn._32);
	mat_temp._33 = (m_Matrix._31*mIn._13) + (m_Matrix._32*mIn._23) + (m_Matrix._33*mIn._33);
	
	m_Matrix = mat_temp;
}

/////////////////////////////////////////////////////////
//
//	TransformVector2Ds関数
//
//	Vector2Dsのstd :: vectorに2D変換行列を適用します
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::TransformVector2Ds(std::vector<Vector2D> &vPoint)
{
  for (unsigned int i=0; i<vPoint.size(); ++i)
  {
    float tempX =(float)(m_Matrix._11*vPoint[i].x) + (float)(m_Matrix._21*vPoint[i].y) + (float)(m_Matrix._31);

    float tempY = (float)(m_Matrix._12*vPoint[i].x) + (float)(m_Matrix._22*vPoint[i].y) + (float)(m_Matrix._32);
  
    vPoint[i].x = tempX;

    vPoint[i].y = tempY;

  }
}

/////////////////////////////////////////////////////////
//
//	TransformVector2Ds関数
//
//	単一のVector2Dに2D変換マトリックスを適用
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::TransformVector2Ds(Vector2D &vPoint)
{
	float tempX = (float)(m_Matrix._11*vPoint.x) + (float)(m_Matrix._21*vPoint.y) + (float)(m_Matrix._31);
	
	float tempY = (float)(m_Matrix._12*vPoint.x) + (float)(m_Matrix._22*vPoint.y) + (float)(m_Matrix._32);
	
	vPoint.x = tempX;
	
	vPoint.y = tempY;
}

/////////////////////////////////////////////////////////
//
//	Identity関数
//
//	単位行列を作成する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::Identity()
{
	m_Matrix._11 = 1; m_Matrix._12 = 0; m_Matrix._13 = 0;
	
	m_Matrix._21 = 0; m_Matrix._22 = 1; m_Matrix._23 = 0;
	
	m_Matrix._31 = 0; m_Matrix._32 = 0; m_Matrix._33 = 1;

}

/////////////////////////////////////////////////////////
//
//	Translate関数
//
//	変換行列を作成する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::Translate(double x, double y)
{
	Matrix mat;
	
	mat._11 = 1; mat._12 = 0; mat._13 = 0;
	
	mat._21 = 0; mat._22 = 1; mat._23 = 0;
	
	mat._31 = x;    mat._32 = y;    mat._33 = 1;
	
	MatrixMultiply(mat);
}

/////////////////////////////////////////////////////////
//
//	Scale関数
//
//	スケール行列を作成する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::Scale(double xScale, double yScale)
{
	C2DMatrix::Matrix mat;
	
	mat._11 = xScale; mat._12 = 0; mat._13 = 0;
	
	mat._21 = 0; mat._22 = yScale; mat._23 = 0;
	
	mat._31 = 0; mat._32 = 0; mat._33 = 1;
	
	MatrixMultiply(mat);
}

/////////////////////////////////////////////////////////
//
//	Rotate関数
//
//	回転行列を作成する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::Rotate(double rot)
{
	C2DMatrix::Matrix mat;
	
	double Sin = sin(rot);
	double Cos = cos(rot);
	
	mat._11 = Cos;  mat._12 = Sin; mat._13 = 0;
	
	mat._21 = -Sin; mat._22 = Cos; mat._23 = 0;
	
	mat._31 = 0; mat._32 = 0;mat._33 = 1;
	
	MatrixMultiply(mat);
}

/////////////////////////////////////////////////////////
//
//	Rotate関数
//
//	2Dベクトルから回転行列を作成する
//
/////////////////////////////////////////////////////////
inline void C2DMatrix::Rotate(const Vector2D &fwd, const Vector2D &side)
{
	C2DMatrix::Matrix mat;
	
	mat._11 = fwd.x;  mat._12 = fwd.y; mat._13 = 0;
	
	mat._21 = side.x; mat._22 = side.y; mat._23 = 0;
	
	mat._31 = 0; mat._32 = 0;mat._33 = 1;
	
	MatrixMultiply(mat);
}

#endif
