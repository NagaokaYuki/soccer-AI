/*=======================================================================

	サッカーチーム [soccerTeam.h]

												長岡佑樹

  サッカーをするエージェントのチームを定義するクラス
  SoccerTeamには複数のフィールドプレーヤーと1人のゴールキーパーが含まれ
  SoccerTeamは有限状態マシンとして実装され、攻撃、防御、およびKickOffの状態を持つ

=========================================================================*/
#ifndef SOCCERTEAM_H
#define SOCCERTEAM_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <vector>

#include "Region.h"
#include "SupportSpotCalculator.h"
#include "StateMachine.h"

/////////////////////////////////////////////////////////
//
//	前方宣言
//
/////////////////////////////////////////////////////////
class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;

/////////////////////////////////////////////////////////
//
//	SoccerTeamクラス
//
/////////////////////////////////////////////////////////                
class SoccerTeam 
{
public:
  
  enum team_color {blue, red};

private:

   // 状態マシンクラスのインスタンス
  StateMachine<SoccerTeam>*  m_pStateMachine;

  // チームカラー
  team_color                m_Color;

  // チームメンバーへのポインタ
  std::vector<PlayerBase*>  m_Players;

  // サッカーのピッチへのポインタ
  SoccerPitch*              m_pPitch;

  // ゴールへのポインタ
  Goal*                     m_pOpponentsGoal;
  Goal*                     m_pHomeGoal;
  
  // 相手チームへのポインタ
  SoccerTeam*               m_pOpponents;
   
  // キープレイヤーへのポインタ
  PlayerBase*               m_pControllingPlayer;
  PlayerBase*               m_pSupportingPlayer;
  PlayerBase*               m_pReceivingPlayer;
  PlayerBase*               m_pPlayerClosestToBall;

  // 最も近いプレーヤーがボールからの距離の自乗距離
  double                     m_dDistSqToBallOfClosestPlayer;

  // プレイヤーはこれを使用して競技場の戦略的ポジションを決定
  SupportSpotCalculator*    m_pSupportSpotCalc;


  // チームのすべての選手を作成
  void CreatePlayers();

  // ボールに最も近いプレーヤーを指すようにm_pClosestPlayerToBallを設定
  void CalculateClosestPlayerToBall();


public:

  SoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~SoccerTeam();

  void        Render()const;
  void        Update();

  // すべてのフィールドプレーヤーの状態が
  // ReturnToHomeRegionの状態に変更される
  // ゴールキーパーが所持している場合に主に使用される
  void        ReturnAllFieldPlayersToHome()const;

  // プレイヤーがゴールにシュートする時、
  // 正規化されたベクトルに設定する場合にtrueを返す
  // falseはゼロベクトルに設定
  bool        CanShoot(Vector2D  BallPos,
                       double     power, 
                       Vector2D& ShotTarget = Vector2D())const;

  // 最善のパスを見つける関数
  bool        FindPass(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                  power,
                      double                  MinPassingDistance)const;

  // 3つの潜在的なパスが計算される。 1つは受信機の現在の位置に直接向いており、
  // 2つはボール位置から受信機から半径「範囲」の円までの接線である。
  // これらのパスは、相手が傍受してプレーエリア内で終了することを確認するためにテスト
  // もしすべてのパスが無効だった場合、関数はfalseを返す
  // それ以外の場合、関数は相手のゴールエリアに最も近いボールを受け取るパスを返す
  bool        GetBestPassToReceiver(const PlayerBase* const passer,
                                    const PlayerBase* const receiver,
                                    Vector2D& PassTarget,
                                    const double power)const;

  // 'from'から 'target'までのパスが強制的に蹴られたかどうかをテストする
  bool        isPassSafeFromOpponent(Vector2D    from,
                                     Vector2D    target,
                                     const PlayerBase* const receiver,
                                     const PlayerBase* const opp,
                                     double       PassingForce)const;

  // 'from'の位置から 'target'の位置までのパスを
  // 各メンバーに対してテスト
  // インターセプトされない場合trueを返す
  bool        isPassSafeFromAllOpponents(Vector2D from,
                                         Vector2D target,
                                         const PlayerBase* const receiver,
                                         double     PassingForce)const;

  // 位置の半径内に対戦相手がある場合はtrueを返す
  bool        isOpponentWithinRadius(Vector2D pos, double rad);

  // リクエスタとコントロールプレーヤの間でパスが可能かどうかをテストする 
  void        RequestPass(FieldPlayer* requester)const;

  // 最も支持しやすい位置を計算し、
  // 最も適切な攻撃者がその地点に移動するのを見つける
  PlayerBase* DetermineBestSupportingAttacker();
  

  const std::vector<PlayerBase*>& Members()const{return m_Players;}  

  StateMachine<SoccerTeam>* GetFSM()const{return m_pStateMachine;}
  
  Goal*const           HomeGoal()const{return m_pHomeGoal;}
  Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}

  SoccerPitch*const    Pitch()const{return m_pPitch;}           

  SoccerTeam*const     Opponents()const{return m_pOpponents;}
  void                 SetOpponents(SoccerTeam* opps){m_pOpponents = opps;}

  team_color           Color()const{return m_Color;}

  void                 SetPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}
  PlayerBase*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
  
  double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

  Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

  PlayerBase*          SupportingPlayer()const{return m_pSupportingPlayer;}
  void                 SetSupportingPlayer(PlayerBase* plyr){m_pSupportingPlayer = plyr;}

  PlayerBase*          Receiver()const{return m_pReceivingPlayer;}
  void                 SetReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

  PlayerBase*          ControllingPlayer()const{return m_pControllingPlayer;}
  void                 SetControllingPlayer(PlayerBase* plyr)
  {
    m_pControllingPlayer = plyr;
    Opponents()->LostControl();
  }


  bool  InControl()const{if(m_pControllingPlayer)return true; else return false;}
  void  LostControl(){m_pControllingPlayer = NULL;}

  PlayerBase*  GetPlayerFromID(int id)const;
  

  void SetPlayerHomeRegion(int plyr, int region)const;

  void DetermineBestSupportingPosition()const{m_pSupportSpotCalc->DetermineBestSupportingPosition();}

  void UpdateTargetsOfWaitingPlayers()const;

  // いずれかのチーム内に位置していない場合はfalseを返す
  bool AllPlayersAtHome()const;

  std::string Name()const{if (m_Color == blue) return "Blue"; return "Red";}

};

#endif