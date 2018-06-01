/*=======================================================================

	プレイヤーベース [playerBase.h]

												長岡佑樹


	プレイヤーの基本クラス。
	プレイヤーはオートリストクラスを継承するため、
	作成されたプレイヤーは他のゲームオブジェクトが
	簡単にアクセスできるように自動的に追加される

=========================================================================*/
#pragma warning (disable:4786)
#ifndef PLAYERBASE_H
#define PLAYERBASE_H

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <cassert>
#include "autolist.h"
#include "vector2D.h"
#include "movingEntity.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;
class Region;

/////////////////////////////////////////////////////////
//
//	PlayerBaseクラス
//
/////////////////////////////////////////////////////////
class PlayerBase : public MovingEntity,
                   public AutoList<PlayerBase>
{
public:
	enum player_role{goal_keeper, attacker, defender};

protected:
	
	// プレイヤーのチームにおける役割
	player_role             m_PlayerRole;
	
	// SoccerTeamのポインタ
	SoccerTeam*             m_pTeam;
	
	// SteeringBehaviorsのポインタ
	SteeringBehaviors*      m_pSteering;
	
	// プレイヤーの自エリア
	int                     m_iHomeRegion;
	
	// プレイヤーがキックオフする前の領域
	int                     m_iDefaultRegion;
	
	// ボールまでの距離(四角形の空間)
	// 各時間ステップごとに1回計算され、これに保存する
	double                   m_dDistSqToBall;
	
	
	// 頂点バッファ
	std::vector<Vector2D>   m_vecPlayerVB;

	// 変換された頂点バッファ
	std::vector<Vector2D>   m_vecPlayerVBTrans;

public:
	PlayerBase(SoccerTeam*    home_team,
			   int            home_region,
			   Vector2D       heading,
			   Vector2D       velocity,
			   double          mass,
			   double          max_force,
			   double          max_speed,
			   double          max_turn_rate,
			   double          scale,
			   player_role    role);
	
	virtual ~PlayerBase();
	
	// プレイヤーのゾーン内に対戦相手がいる場合はtrueを返す
	bool        isThreatened()const;
	
	// プレイヤーをボールまたはプレイヤーの現在のターゲットに向けるように回転させる
	void        TrackBall();
	void        TrackTarget();
	
	// サポートしている場所に最も近いプレイヤーの状態を変更して
	// 攻撃側プレイヤーをサポートするように指示する
	void        FindSupport()const;
	
	// ゴールキーパーがボールをつかむことができる場合,trueを返す
	bool        BallWithinKeeperRange()const;
	
	// ボールがキック範囲内にある場合はtrueを返す
	bool        BallWithinKickingRange()const;
	
	// ボールが受信機の範囲内に入った場合にtrueを返す
	bool        BallWithinReceivingRange()const;
	
	// プレイヤーが自エリアの境界内に位置する場合はtrueを返す
	bool        InHomeRegion()const;
	
	// プレイヤーが攻撃者よりも前にある場合はtrueを返す
	bool        isAheadOfAttacker()const;
	
	// プレイヤーが指定されたサポートスポットにいる場合はtrueを返す
	bool        AtSupportSpot()const;
	
	// プレイヤーがステアリングターゲットにある場合はtrueを返す
	bool        AtTarget()const;
	
	// プレイヤーがボールのチーム内で最も近いプレイヤーの場合はtrueを返す
	bool        isClosestTeamMemberToBall()const;
	
	// positionで指定されたポイントがプレイヤーの前にある場合はtrueを返す
	bool        PositionInFrontOfPlayer(Vector2D position)const;
	
	// プレイヤーがボールのピッチ上で最も近いプレイヤーの場合はtrueを返す
	bool        isClosestPlayerOnPitchToBall()const;
	
	// プレーヤーがコントロールプレーヤーである場合はtrueを返す
	bool        isControllingPlayer()const;
	
	// プレイヤーが指定されたホットエリアにいる場合はtrueを返す
	// 相手のゴールに近いエリア
	bool        InHotRegion()const;
	
	player_role Role()const{return m_PlayerRole;}
	
	double       DistSqToBall()const{return m_dDistSqToBall;}
	void        SetDistSqToBall(double val){m_dDistSqToBall = val;}
	
	// 相手エリア/自エリアまでの距離を計算する
	// パスをする度に使用する
	double       DistToOppGoal()const;
	double       DistToHomeGoal()const;
	
	void        SetDefaultHomeRegion(){m_iHomeRegion = m_iDefaultRegion;}
	
	SoccerBall* const        Ball()const;
	SoccerPitch* const       Pitch()const;
	SteeringBehaviors*const  Steering()const{return m_pSteering;}
	const Region* const      HomeRegion()const;
	void                     SetHomeRegion(int NewRegion){m_iHomeRegion = NewRegion;}
	SoccerTeam*const         Team()const{return m_pTeam;}
	
};

#endif