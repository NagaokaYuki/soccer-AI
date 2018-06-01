/*=======================================================================

	ステアリングビヘイバー  [steeringBehaviors.h]

												長岡佑樹

=========================================================================*/
#ifndef SteeringBehaviorsS_H
#define SteeringBehaviorsS_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include <windows.h>
#include <string>
#include "vector2D.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class PlayerBase;
class SoccerPitch;
class SoccerBall;
class CWall;
class CObstacle;

/////////////////////////////////////////////////////////
//
//	SteeringBehaviorsクラス
//
/////////////////////////////////////////////////////////
class SteeringBehaviors
{
private:
  
  PlayerBase*   m_pPlayer;		// プレイヤー変数                                              
								
  SoccerBall*   m_pBall;		// ボール変数

  //　選択された全ての挙動の複合効果によって生成された操舵力
  Vector2D     m_vSteeringForce;

  // ボールまたは予想されるボールの位置
  Vector2D     m_vTarget;

  // プレイヤーがターゲットから挿入しようとする距離
  double        m_dInterposeDist;

  double        m_dMultSeparation;

  double        m_dViewDistance;

  // ビヘイビアをアクティブにする必要があるかどうかを示すバイナリフラグ
  int           m_iFlags;

  enum behavior_type
  {
    none               = 0x0000,	// 無し 
    seek               = 0x0001,	// シーク
    arrive             = 0x0002,	// 着く
    separation         = 0x0004,	// 分離
    pursuit            = 0x0008,	// 追求
    interpose          = 0x0010		// 介入
  };

  // 隣人にタグを付けるためにグループ行動によって使用
  bool         m_bTagged;
  
  // どれくらい速く目標に向かって減速すべきかを決定する
  enum Deceleration{slow = 3, normal = 2, fast = 1};


  // この動作は、エージェントを目標位置に向かって移動させる
  Vector2D Seek(Vector2D target);

  // ゼロ速度でターゲットに到達しようする
  Vector2D Arrive(Vector2D target, Deceleration decel);
   
  // ターゲットがどこにあるのかを予測し、その場所を探す
  Vector2D Pursuit(const SoccerBall* ball);
 
  Vector2D Separation();

  // 相手とオブジェクトとの間の位置にエージェントを誘導しようとするもの
  Vector2D Interpose(const SoccerBall* ball,
                     Vector2D pos,
                     double    DistFromTarget);


  // ビュー半径内の任意の近隣を見つける
  void      FindNeighbours();


  // 特定のビットがセットされているかどうかを調べる
  bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}

  bool      AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);

  Vector2D  SumForces();

  // ドリブルのための頂点バッファー
  std::vector<Vector2D> m_Antenna;

  
public:

  SteeringBehaviors(PlayerBase*       agent,
                    SoccerPitch*  world,
                    SoccerBall*   ball);

  virtual ~SteeringBehaviors(){}

 
  Vector2D Calculate();

  // 進行方向に平行な操舵力の成分を算出する
  double    ForwardComponent();

  // 垂直な操舵力の成分を計算する
  double    SideComponent();

  Vector2D Force()const{return m_vSteeringForce;}

  // 各ビヘイビアがどのように計算されているかを見るためのビジュアルエイドと情報をレンダリングする
  void      RenderInfo();
  void      RenderAids();

  Vector2D  Target()const{return m_vTarget;}
  void      SetTarget(const Vector2D t){m_vTarget = t;}

  double     InterposeDistance()const{return m_dInterposeDist;}
  void      SetInterposeDistance(double d){m_dInterposeDist = d;}

  bool      Tagged()const{return m_bTagged;}
  void      Tag(){m_bTagged = true;}
  void      UnTag(){m_bTagged = false;}
  

  void SeekOn(){m_iFlags |= seek;}
  void ArriveOn(){m_iFlags |= arrive;}
  void PursuitOn(){m_iFlags |= pursuit;}
  void SeparationOn(){m_iFlags |= separation;}
  void InterposeOn(double d){m_iFlags |= interpose; m_dInterposeDist = d;}

  
  void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
  void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
  void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
  void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
  void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}


  bool SeekIsOn(){return On(seek);}
  bool ArriveIsOn(){return On(arrive);}
  bool PursuitIsOn(){return On(pursuit);}
  bool SeparationIsOn(){return On(separation);}
  bool InterposeIsOn(){return On(interpose);}

};




#endif