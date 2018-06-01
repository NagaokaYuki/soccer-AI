/*=======================================================================

	サッカーボール [soccerBall.cpp]

												長岡佑樹

=========================================================================*/

////////////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
////////////////////////////////////////////////////////////////
#include "soccerBall.h"
#include "geometry.h"
#include "debugConsole.h"
#include "cgdi.h"
#include "paramLoader.h"
#include "wall2D.h"

////////////////////////////////////////////////////////////////
//
//	AddNoiseToKick関数
//
//	プレイヤーのキックの精度を変えるために使用する
//	ボールの位置とボールターゲットをパラメータとして使用し,
//	ボールをキックする前にそれを呼び出す
//
////////////////////////////////////////////////////////////////
Vector2D AddNoiseToKick(Vector2D BallPos, Vector2D BallTarget)
{

  double displacement = (Pi - Pi*Prm.PlayerKickingAccuracy) * RandomClamped();

  Vector2D toTarget = BallTarget - BallPos;

  Vec2DRotateAroundOrigin(toTarget, displacement);

  return toTarget + BallPos;
}

  
////////////////////////////////////////////////////////////////
//
//	Kick関数
//
//	ボールに力を加える
//	新しいベロシティを切り捨てて、最大許容値を超えないようにする
//
////////////////////////////////////////////////////////////////
void SoccerBall::Kick(Vector2D direction, double force)
{  
  // 方向が正規化されていることを確認
  direction.Normalize();
  
  // 加速度を計算
  Vector2D acceleration = (direction * force) / m_dMass;

  // 速度を更新する
  m_vVelocity = acceleration;
}

////////////////////////////////////////////////////////////////
//
//	Update関数
//
//	ボール物理学を更新し、衝突をテストし、
//	それに応じてボールの速度を調整する
//
////////////////////////////////////////////////////////////////
void SoccerBall::Update()
{
  // 古いポジションの記録を保持
  m_vOldPos = m_vPosition;
  
  // 衝突判定
  TestCollisionWithWalls(m_PitchBoundary);

  // 最初にスピードが正であることを確認
  if (m_vVelocity.LengthSq() > Prm.Friction * Prm.Friction)
  {
    m_vVelocity += Vec2DNormalize(m_vVelocity) * Prm.Friction;

    m_vPosition += m_vVelocity;

    // 更新見出し
    m_vHeading = Vec2DNormalize(m_vVelocity);
  }   
}

////////////////////////////////////////////////////////////////
//
//	TimeToCoverDistance関数
//
//	2つのベクトルによって与えられる力とカバーする距離が
//	与えられると、ボールが2つの点の間をどのくらい移動するかを計算
//	
////////////////////////////////////////////////////////////////
double SoccerBall::TimeToCoverDistance(Vector2D A,
                                      Vector2D B,
                                      double force)const
{
  // ボールの速度を計算
  double speed = force / m_dMass;

  // ２つの位置の距離間を計算
  double DistanceToCover =  Vec2DDistance(A, B);

  double term = speed*speed + 2.0*DistanceToCover*Prm.Friction;

  // 負の場合ボールが点Bに到達できない
  if (term <= 0.0) return -1.0;

  double v = sqrt(term);

  return (v-speed)/Prm.Friction;
}

////////////////////////////////////////////////////////////////
//
//	FuturePosition関数
//
// 与えられた時間によって、ボールの未来の位置を返す
//	
////////////////////////////////////////////////////////////////
Vector2D SoccerBall::FuturePosition(double time)const
{
  // s =距離、a =摩擦、u =開始速度

  // ベクトル
  Vector2D ut = m_vVelocity * time;

  // スカラー
  double half_a_t_squared = 0.5 * Prm.Friction * time * time;

  // その値に正規化された速度ベクトルを掛けることによって
  // スカラー量をベクトルに変換
  Vector2D ScalarToVector = half_a_t_squared * Vec2DNormalize(m_vVelocity);

  // 予測される位置はボールの位置 + ベクトル + スカラー
  return Pos() + ut + ScalarToVector;
}

////////////////////////////////////////////////////////////////
//
//	Render関数
//	
////////////////////////////////////////////////////////////////
void SoccerBall::Render()
{
  gdi->BlackBrush();

  gdi->Circle(m_vPosition, m_dBoundingRadius);
}

////////////////////////////////////////////////////////////////
//
//	TestCollisionWithWalls関数
//	
////////////////////////////////////////////////////////////////
void SoccerBall::TestCollisionWithWalls(const std::vector<Wall2D>& walls)
{  
  // どれが最も近いかを調べる
  int idxClosest = -1;

  Vector2D VelNormal = Vec2DNormalize(m_vVelocity);

  Vector2D IntersectionPoint, CollisionPoint;

  double DistToIntersection = MaxFloat;

  // 最も近い交差する壁にインデックスを格納する場合,
  // 各壁を反復し、ボールが交差するかどうかを計算
  for (unsigned int w=0; w<walls.size(); ++w)
  {
    // 壁に衝突するボール上の点を計算
    // ボールの半径で乗算された壁の法線の逆数であり、
    // ボールの中心（その位置）に加算
    Vector2D ThisCollisionPoint = Pos() - (walls[w].Normal() * BRadius());

    // 衝突点が飛行機に当たる場所を正確に計算
    if (WhereIsPoint(ThisCollisionPoint,
                     walls[w].From(),
                     walls[w].Normal()) == plane_backside)
    {
      double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
                                                         walls[w].Normal(),
                                                         walls[w].From(),
                                                         walls[w].Normal());

      IntersectionPoint = ThisCollisionPoint + (DistToWall * walls[w].Normal());
      
    }

    else
    {
      double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
                                                         VelNormal,
                                                         walls[w].From(),
                                                         walls[w].Normal());

      IntersectionPoint = ThisCollisionPoint + (DistToWall * VelNormal);
    }
    
    // 交差点が実際に線分上にあることを確認
    bool OnLineSegment = false;

    if (LineIntersection2D(walls[w].From(), 
                           walls[w].To(),
                           ThisCollisionPoint - walls[w].Normal()*20.0,
                           ThisCollisionPoint + walls[w].Normal()*20.0))
    {

      OnLineSegment = true;                                               
    }

  
                                                                          //Note, there is no test for collision with the end of a line segment
    
    // 衝突点が速度ベクトルの範囲内にあるかどうかを確認
    // その場合は、現在のタイムステップと次のタイムステップの間に
    // ボールが壁と衝突する
    double distSq = Vec2DDistanceSq(ThisCollisionPoint, IntersectionPoint);

    if ((distSq <= m_vVelocity.LengthSq()) && (distSq < DistToIntersection) && OnLineSegment)            
    {        
      DistToIntersection = distSq;
      idxClosest = w;
      CollisionPoint = IntersectionPoint;
    }     
  }

    
  // 衝突の正確な時間を計算しないように、
  // 速度が反射する前に壁の法線と反対であるかどうかを確認する
  // オーバーシュートがあるため、ボールがプレーエリアに完全に再入される前に
  // ライン上に戻って反射する場合を防止する
  if ( (idxClosest >= 0 ) && VelNormal.Dot(walls[idxClosest].Normal()) < 0)
  {
    m_vVelocity.Reflect(walls[idxClosest].Normal());   
  }
}

////////////////////////////////////////////////////////////////
//
//	PlaceAtLocation関数
//	
//	ボールを所望の位置に位置決めし、ボールの速度をゼロに設定
//
////////////////////////////////////////////////////////////////
void SoccerBall::PlaceAtPosition(Vector2D NewPos)
{
  m_vPosition = NewPos;

  m_vOldPos = m_vPosition;
  
  m_vVelocity.Zero();
}

