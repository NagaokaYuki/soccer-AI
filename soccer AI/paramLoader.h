/*=======================================================================

	パラムローダー  [paramLoader.h]

												長岡佑樹


	初期化ファイルからのデフォルトのパラメータ値のロードを
	処理するシングルトンクラス

=========================================================================*/
#ifndef PARAMLOADER
#define PARAMLOADER
#pragma warning(disable:4800)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <cassert>
#include "iniFileLoaderBase.h"


#define Prm (*ParamLoader::Instance())

class ParamLoader : public iniFileLoaderBase
{
private:

  ParamLoader():iniFileLoaderBase("Params.ini")
  {    
       
    GoalWidth                   = GetNextParameterDouble(); 
    
    NumSupportSpotsX            = GetNextParameterInt();    
    NumSupportSpotsY            = GetNextParameterInt();  
    
    Spot_PassSafeScore                     = GetNextParameterDouble();
    Spot_CanScoreFromPositionScore         = GetNextParameterDouble();
    Spot_DistFromControllingPlayerScore     = GetNextParameterDouble();
    Spot_ClosenessToSupportingPlayerScore  = GetNextParameterDouble();
    Spot_AheadOfAttackerScore              = GetNextParameterDouble();

    SupportSpotUpdateFreq       = GetNextParameterDouble(); 
    
    ChancePlayerAttemptsPotShot = GetNextParameterDouble();
    ChanceOfUsingArriveTypeReceiveBehavior = GetNextParameterDouble();
    
    BallSize                    = GetNextParameterDouble();    
    BallMass                    = GetNextParameterDouble();    
    Friction                    = GetNextParameterDouble(); 
    
    KeeperInBallRange           = GetNextParameterDouble();    
    PlayerInTargetRange         = GetNextParameterDouble(); 
    PlayerKickingDistance       = GetNextParameterDouble(); 
    PlayerKickFrequency         = GetNextParameterDouble();


    PlayerMass                  = GetNextParameterDouble(); 
    PlayerMaxForce              = GetNextParameterDouble();    
    PlayerMaxSpeedWithBall      = GetNextParameterDouble();   
    PlayerMaxSpeedWithoutBall   = GetNextParameterDouble();   
    PlayerMaxTurnRate           = GetNextParameterDouble();   
    PlayerScale                 = GetNextParameterDouble();      
    PlayerComfortZone           = GetNextParameterDouble();  
    PlayerKickingAccuracy       = GetNextParameterDouble();

    NumAttemptsToFindValidStrike = GetNextParameterInt();


    
    MaxDribbleForce             = GetNextParameterDouble();    
    MaxShootingForce            = GetNextParameterDouble();    
    MaxPassingForce             = GetNextParameterDouble();  
    
    WithinRangeOfHome           = GetNextParameterDouble();    
    WithinRangeOfSupportSpot    = GetNextParameterDouble();    
    
    MinPassDist                 = GetNextParameterDouble();
    GoalkeeperMinPassDist       = GetNextParameterDouble();
    
    GoalKeeperTendingDistance   = GetNextParameterDouble();    
    GoalKeeperInterceptRange    = GetNextParameterDouble();
    BallWithinReceivingRange    = GetNextParameterDouble();
    
    bStates                     = GetNextParameterBool();    
    bIDs                        = GetNextParameterBool(); 
    bSupportSpots               = GetNextParameterBool();     
    bRegions                    = GetNextParameterBool();
    bShowControllingTeam        = GetNextParameterBool();
    bViewTargets                = GetNextParameterBool();
    bHighlightIfThreatened      = GetNextParameterBool();

    FrameRate                   = GetNextParameterInt();

    SeparationCoefficient       = GetNextParameterDouble(); 
    ViewDistance                = GetNextParameterDouble(); 
    bNonPenetrationConstraint   = GetNextParameterBool(); 


    BallWithinReceivingRangeSq = BallWithinReceivingRange * BallWithinReceivingRange;
    KeeperInBallRangeSq      = KeeperInBallRange * KeeperInBallRange;
    PlayerInTargetRangeSq    = PlayerInTargetRange * PlayerInTargetRange;   
    PlayerKickingDistance   += BallSize;
    PlayerKickingDistanceSq  = PlayerKickingDistance * PlayerKickingDistance;
    PlayerComfortZoneSq      = PlayerComfortZone * PlayerComfortZone;
    GoalKeeperInterceptRangeSq     = GoalKeeperInterceptRange * GoalKeeperInterceptRange;
    WithinRangeOfSupportSpotSq = WithinRangeOfSupportSpot * WithinRangeOfSupportSpot;
  }
  
public:

  static ParamLoader* Instance();

  double GoalWidth;

  int   NumSupportSpotsX;
  int   NumSupportSpotsY;

  // サポートスポットを計算するために
  // 使用されるさまざまなルールを調整
  double Spot_PassSafeScore;
  double Spot_CanScoreFromPositionScore;
  double Spot_DistFromControllingPlayerScore;
  double Spot_ClosenessToSupportingPlayerScore;
  double Spot_AheadOfAttackerScore;  
  
  double SupportSpotUpdateFreq ;

  double ChancePlayerAttemptsPotShot; 
  double ChanceOfUsingArriveTypeReceiveBehavior;

  double BallSize;
  double BallMass;
  double Friction;

  double KeeperInBallRange;
  double KeeperInBallRangeSq;

  double PlayerInTargetRange;
  double PlayerInTargetRangeSq;
  
  double PlayerMass;
  
  // 最大力
  double PlayerMaxForce; 
  double PlayerMaxSpeedWithBall;
  double PlayerMaxSpeedWithoutBall;
  double PlayerMaxTurnRate;
  double PlayerScale;
  double PlayerComfortZone;

  double PlayerKickingDistance;
  double PlayerKickingDistanceSq;

  double PlayerKickFrequency; 

  double  MaxDribbleForce;
  double  MaxShootingForce;
  double  MaxPassingForce;

  double  PlayerComfortZoneSq;

  // 0〜1.0の範囲で指定
  // キックに加えられるノイズの量を調整し、
  // 値が低いほどプレイヤーは悪くなる
  double  PlayerKickingAccuracy;

  // SoccerTeam :: CanShootメソッドが有効なショットを見つけようとする回数
  int    NumAttemptsToFindValidStrike;

  double WithinRangeOfHome;

  // プレイヤーが状態を変える前にスイートスポットにどれだけ近づけなければならないか
  double WithinRangeOfSupportSpot;
  double WithinRangeOfSupportSpotSq;
 
  
  // 受信側プレイヤーが通過側プレイヤーからの最小距離
  double   MinPassDist;
  double   GoalkeeperMinPassDist;

  // インターポーズのステアリング動作を使用するとき、
  // キーパーがネットの背面とボールの間に置く距離
  double  GoalKeeperTendingDistance;

  // ボールがゴールキーパーのこの距離以内になると、
  // 状態を変えてボールを傍受する
  double  GoalKeeperInterceptRange;
  double  GoalKeeperInterceptRangeSq;

  // 彼がそれを追い始める前にボールがレシーバにどれほど近くなければならないか
  double  BallWithinReceivingRange;
  double  BallWithinReceivingRangeSq;


  // 表示できるデバッグ情報を制御
  bool  bStates;
  bool  bIDs;
  bool  bSupportSpots;
  bool  bRegions;
  bool  bShowControllingTeam;
  bool  bViewTargets;
  bool  bHighlightIfThreatened;

  int FrameRate;

  
  double SeparationCoefficient;

  // エージェントがそれを知覚する前に隣人がどれほど近くにいなければならないか
  double ViewDistance;

  // ゼロにして制約をオフ
  bool bNonPenetrationConstraint;

};

#endif