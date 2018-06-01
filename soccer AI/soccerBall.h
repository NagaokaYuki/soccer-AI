/*=======================================================================

	サッカーボール [soccerBall.h]

												長岡佑樹

	サッカーボールを実装するクラス
	このクラスはMovingEntityから継承され、衝突テストと
	位置予測のためのさらなる機能を提供する

=========================================================================*/
#ifndef SOCCERBALL_H
#define SOCCERBALL_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include "movingEntity.h"
#include "main.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class Wall2D;
class PlayerBase;

/////////////////////////////////////////////////////////
//
//	SoccerBallクラス
//
/////////////////////////////////////////////////////////
class SoccerBall : public MovingEntity
{
private:
	
	// 最終更新時にボールの位置を記録する
	Vector2D                  m_vOldPos;
	
	// ピッチ境界を構成する壁へのローカル参照
	const std::vector<Wall2D>& m_PitchBoundary;                                      
	
	LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ

protected:
	D3DXVECTOR3 m_pos;										// 表示座標
	D3DXVECTOR2 m_size;										// 幅,高さ

public:
	
	// ボールがボールと衝突したかどうかを調べ、
	// それに応じてボールの速度を反映する
  void TestCollisionWithWalls(const std::vector<Wall2D>& walls);

  SoccerBall(Vector2D           pos,            
             double               BallSize,
             double               mass,
             std::vector<Wall2D>& PitchBoundary):
  
      // 基本クラスを設定
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

  // 基本クラスUpdateを実装
  void      Update();

  // 基本クラスレンダリングを実装
  void      Render();

  // メッセージ無し
  bool      HandleMessage(const Telegram& msg){return false;}

  // ボールに方向性の力を加える(キック)
  void      Kick(Vector2D direction, double force);

  // キック力と開始点と終了点によって定義されるトラバースまでの距離を考慮して、
  // ボールが距離をカバーするのにかかる時間を計算
  double    TimeToCoverDistance(Vector2D from,
                               Vector2D to,
                               double     force)const;

  // ボールが時間で表示される場所を計算
  Vector2D FuturePosition(double time)const;

  // プレイヤーとゴールキーパーがボールを捕るために使用
  // そのプレイヤーはボールを保持しているとみなされ、
  // それに応じてm_pOwnerが調整される
  void      Trap(){m_vVelocity.Zero();}  

  Vector2D  OldPos()const{return m_vOldPos;}
  
  // ボールを所望の位置に置き、その速度をゼロに設定
  void      PlaceAtPosition(Vector2D NewPos);
};

// プレイヤーのキックの精度を変えるために使用する
Vector2D AddNoiseToKick(Vector2D BallPos, Vector2D BallTarget);

#endif