/*=======================================================================

	サッカーチーム [soccerTeam.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "soccerTeam.h"
#include "soccerPitch.h"
#include "goal.h"
#include "playerBase.h"
#include "goalKeeper.h"
#include "fieldPlayer.h"
#include "utils.h"
#include "steeringBehaviors.h"
#include "goalKeeperStates.h"
#include "paramLoader.h"
#include "geometry.h"
#include "entityManager.h"
#include "messageDispatcher.h"
#include "soccerMessages.h"
#include "teamStates.h"
#include "debugConsole.h"
#include <windows.h>

using std::vector;

/////////////////////////////////////////////////////////
//
//	コンストラクタ
//
/////////////////////////////////////////////////////////
SoccerTeam::SoccerTeam(Goal*        home_goal,
                       Goal*        opponents_goal,
                       SoccerPitch* pitch,
                       team_color   color):m_pOpponentsGoal(opponents_goal),
                                           m_pHomeGoal(home_goal),
                                           m_pOpponents(NULL),
                                           m_pPitch(pitch),
                                           m_Color(color),
                                           m_dDistSqToBallOfClosestPlayer(0.0),
                                           m_pSupportingPlayer(NULL),
                                           m_pReceivingPlayer(NULL),
                                           m_pControllingPlayer(NULL),
                                           m_pPlayerClosestToBall(NULL)
{
	// ステートマシン生成
	m_pStateMachine = new StateMachine<SoccerTeam>(this);
	
	m_pStateMachine->SetCurrentState(Defending::Instance());
	m_pStateMachine->SetPreviousState(Defending::Instance());
	m_pStateMachine->SetGlobalState(NULL);
	
	// プレイヤー生成
	CreatePlayers();
	
	// デフォルトを設定
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
	  (*it)->Steering()->SeparationOn();   
	}
	
	// sweet sotを生成
	m_pSupportSpotCalc = new SupportSpotCalculator(Prm.NumSupportSpotsX,
	                                               Prm.NumSupportSpotsY,
	                                               this);
}

/////////////////////////////////////////////////////////
//
//	デストラクタ
//
/////////////////////////////////////////////////////////
SoccerTeam::~SoccerTeam()
{
	delete m_pStateMachine;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	for (it; it != m_Players.end(); ++it)
	{
	  delete *it;
	}
	
	delete m_pSupportSpotCalc;
}

/////////////////////////////////////////////////////////
//
//	Update関数
//
/////////////////////////////////////////////////////////
void SoccerTeam::Update()
{
	// フレームごとに1回だけ計算する
	CalculateClosestPlayerToBall();

	// 攻撃/防御の動作を切り替え
	// ホイッスルが吹かれる前にチームをキックオフ位置に
	// 戻さなければならない「キックオフ」状態も処理する
	m_pStateMachine->Update();
	
	// 各プレーヤーを更新
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
	  (*it)->Update();
	}
}

/////////////////////////////////////////////////////////
//
//	CalculateClosestPlayerToBall関数
//
//	m_iClosestPlayerToBallをボールに最も近いプレーヤーに設定
//
/////////////////////////////////////////////////////////
void SoccerTeam::CalculateClosestPlayerToBall()
{
	double ClosestSoFar = MaxFloat;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		// distを計算
		double dist = Vec2DDistanceSq((*it)->Pos(), Pitch()->Ball()->Pos());

		// この値を各プレーヤーに記録しておく
		(*it)->SetDistSqToBall(dist);

		if (dist < ClosestSoFar)
		{
			ClosestSoFar = dist;
			
			m_pPlayerClosestToBall = *it;
		}
	}
	
	m_dDistSqToBallOfClosestPlayer = ClosestSoFar;
}

/////////////////////////////////////////////////////////
//
//	DetermineBestSupportingAttacker関数
//
//	SupportSpotに最も近いプレーヤーを計算
//
/////////////////////////////////////////////////////////
PlayerBase* SoccerTeam::DetermineBestSupportingAttacker()
{
	double ClosestSoFar = MaxFloat;
	
	PlayerBase* BestPlayer = NULL;
	
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		// 攻撃者だけがBestSupportingSpotを利用
		if ( ((*it)->Role() == PlayerBase::attacker) && ((*it) != m_pControllingPlayer) )
		{
		  double dist = Vec2DDistanceSq((*it)->Pos(), m_pSupportSpotCalc->GetBestSupportingSpot());
		
		  // 現在までに距離が最も近く、
		  // プレーヤーがゴールキーパーではなく、
		  // プレーヤーが現在ボールをコントロールしていない場合は、
		  // このプレーヤーの記録を残す
		  if ((dist < ClosestSoFar) )
		  {
			  ClosestSoFar = dist;
			  BestPlayer = (*it);
		  }
		}
	}
	return BestPlayer;
}

/////////////////////////////////////////////////////////
//
//	FindPass関数
//
/////////////////////////////////////////////////////////
bool SoccerTeam::FindPass(const PlayerBase*const passer,
                         PlayerBase*&           receiver,
                         Vector2D&              PassTarget,
                         double                  power,
                         double                  MinPassingDistance)const
{  
  
	std::vector<PlayerBase*>::const_iterator curPlyr = Members().begin();
	
	double    ClosestToGoalSoFar = MaxFloat;
	Vector2D Target;
	
	// プレイヤーのすべてのチームメンバーを繰り返して、
	// どれがボールを通過する位置にあるのかを計算
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		if ( (*curPlyr != passer) &&            
		    (Vec2DDistanceSq(passer->Pos(), (*curPlyr)->Pos()) > 
		     MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, *curPlyr, Target, power))
			{
				// パスのターゲットが今までに見つかった相手のゴールラインに
				// 最も近い場合は記録
				double Dist2Goal = fabs(Target.x - OpponentsGoal()->Center().x);
				
				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;
					
					// 選手を記録する
					receiver = *curPlyr;
					
					// パスする相手も記録
					PassTarget = Target;
				}     
			}
		}
	}// 次のチームメンバー
	
	if (receiver) return true;
	
	else return false;
}


///////////////////////////////////////////////////////////////////
//
//	GetBestPassToReceiver関数
//
// 3つのパスが計算される
// 1つは受けとる側の現在の位置に直接向いており、
// 2つはボールの位置から受けとる側から半径(範囲)の円までの接線
// これらのパスは、相手がインターセプトしてプレーエリア内で終了することを
// 確認するためにテストする。すべてのパスが無効の場合
// 関数はfalseを返し、それ以外の場合、
// 関数は相手のゴールエリアに最も近いボールを受け取り、パスを返す
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::GetBestPassToReceiver(const PlayerBase* const passer,
                                       const PlayerBase* const receiver,
                                       Vector2D&               PassTarget,
                                       double                   power)const
{  
	// ボールがこのレシーバに到達するまでにどれだけの時間がかかるかを計算
	double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
	                                                  receiver->Pos(),
	                                                  power);
	
	// 与えられたパワーで蹴られた後にボールが
	// レシーバに到達できない場合はfalseを返す
	if (time < 0) return false;
	
	// この時間に受けとる側がカバーできる最大距離
	double InterceptRange = time * receiver->MaxSpeed();
	
	// インターセプト範囲をスケーリングする
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;
	
	// ボールからレシーバのレンジサークルへの接線の切片に
	// 位置するパスターゲットを計算
	Vector2D ip1, ip2;
	
	GetTangentPoints(receiver->Pos(),
	                 InterceptRange,
	                 Pitch()->Ball()->Pos(),
	                 ip1,
	                 ip2);
	
	const int NumPassesToTry = 3;
	Vector2D Passes[NumPassesToTry] = {ip1, receiver->Pos(), ip2};
	
	//  1. これまでに発見されたこのレシーバの最も近い有効パス
	//  2. エリア
	//  3. 相手がインターセプトできない
	double ClosestSoFar = MaxFloat;
	bool  bResult      = false;
	
	for (int pass=0; pass<NumPassesToTry; ++pass)
	{    
		double dist = fabs(Passes[pass].x - OpponentsGoal()->Center().x);
		
		if (( dist < ClosestSoFar) &&
		    Pitch()->PlayingArea()->Inside(Passes[pass]) &&
		    isPassSafeFromAllOpponents(Pitch()->Ball()->Pos(),
		                               Passes[pass],
		                               receiver,
		                               power))
		    
		{
			ClosestSoFar = dist;
			PassTarget   = Passes[pass];
			bResult      = true;
		}
	}
	
	return bResult;
}
/////////////////////////////////////////////////////////
//
//	isPassSafeFromOpponent関数
//
//  'from'から 'to'へのパスが相手のプレーヤーによって
//  インターセプトされるかどうかをテスト
//
/////////////////////////////////////////////////////////
bool SoccerTeam::isPassSafeFromOpponent(Vector2D    from,
                                        Vector2D    target,
                                        const PlayerBase* const receiver,
                                        const PlayerBase* const opp,
                                        double       PassingForce)const
{
	// 相手を自エリア内に移動させる
	Vector2D ToTarget = target - from;
	Vector2D ToTargetNormalized = Vec2DNormalize(ToTarget);
	
	Vector2D LocalPosOpp = PointToLocalSpace(opp->Pos(),
	                                       ToTargetNormalized,
	                                       ToTargetNormalized.Perp(),
	                                       from);
	
	// 相手がキッカーの後ろにいる場合
	// パスが可能
	// ボールが相手の最大速度よりも速い速度で蹴られるという仮定
	if ( LocalPosOpp.x < 0 )
	{
		return true;
	}
	
	// 対戦相手がターゲットより遠く離れている場合、
	// 対戦相手がレシーバーの前の位置に到達できるかどうかを検討する
	if (Vec2DDistanceSq(from, target) < Vec2DDistanceSq(opp->Pos(), from))
	{
		if (receiver)
		{
		  if ( Vec2DDistanceSq(target, opp->Pos())  > 
		       Vec2DDistanceSq(target, receiver->Pos()) )
		  {
			  return true;
		  }
		
		  else
		  {
			  return false;
		  }
		}
		
		else
		{
			return true;
		} 
	}
	
	// 対戦相手の位置と直交する位置までの距離をボールがどのくらいの時間保持するかを計算
	double TimeForBall = 
	Pitch()->Ball()->TimeToCoverDistance(Vector2D(0,0),
	                                     Vector2D(LocalPosOpp.x, 0),
	                                     PassingForce);
	
	// 相手がどれくらい遠くまで走れるかを計算
	double reach = opp->MaxSpeed() * TimeForBall +
	              Pitch()->Ball()->BRadius()+
	              opp->BRadius();
	
	// 対戦相手のy位置までの距離が、
	// 対戦相手のランニング範囲とボールの半径および相手の半径を足した値より小さい場合、
	// ボールは傍受される
	if ( fabs(LocalPosOpp.y) < reach )
	{
	  return false;
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////
//
//	isPassSafeFromAllOpponents関数
//
//  'from'の位置から 'target'の位置までのパスを各メンバーに対してテスト
//  インターセプトされずにパスを作成できる場合はtrueを返す
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::isPassSafeFromAllOpponents(Vector2D                from,
                                            Vector2D                target,
                                            const PlayerBase* const receiver,
                                            double     PassingForce)const
{
	std::vector<PlayerBase*>::const_iterator opp = Opponents()->Members().begin();
	
	for (opp; opp != Opponents()->Members().end(); ++opp)
	{
		if (!isPassSafeFromOpponent(from, target, receiver, *opp, PassingForce))
		{
			debug_on
			  
			return false;
		}
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////
//
//	CanShoot関数
//
//  ボールの位置、蹴り込み力、vector2Dへの参照が与えられると
//  この関数は相手のゴールに沿ったランダムな位置をサンプリングし、
//  ボールが与えられた力でその方向に蹴られるならばゴールができるかをチェック
//  可能なショットが見つかった場合、関数はtrueを返し、
//  ターゲット位置がベクトルShotTargetに格納される
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::CanShoot(Vector2D  BallPos,
                          double     power, 
                          Vector2D& ShotTarget)const
{
	// ランダムに作成されたショットターゲットの数
	int NumAttempts = Prm.NumAttemptsToFindValidStrike;
	
	while (NumAttempts--)
	{
		// 相手のゴールの口に沿ってランダムなポジションを選ぶ
		ShotTarget = OpponentsGoal()->Center();
		
		// ショット位置のy値は2つのゴールポストの間にある
		int MinYVal = (int)(OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius());
		int MaxYVal = (int)(OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius());
		
		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);
		
		double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
		                                                  ShotTarget,
		                                                  power);
		
		// シュートが対戦相手のいずれかがそれをインターセプトできるかどうかを調べるテスト
		if (time >= 0)
		{
			if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
			{
				return true;
			}
		}
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////
//	
//	ReturnAllFieldPlayersToHome関数 
// 
//	すぐに自エリアに戻るようにすべてのプレイヤーにメッセージを送信
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::ReturnAllFieldPlayersToHome()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->Role() != PlayerBase::goal_keeper)
		{
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
									1,
									(*it)->ID(),
									Msg_GoHome,
									NULL);
		}
	}
}

///////////////////////////////////////////////////////////////////
//	
//	Render関数 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::Render()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Render();
	}
	
	// ディスプレイの上部にコントロールチームとプレーヤーを表示
	if (Prm.bShowControllingTeam)
	{
		gdi->TextColor(Cgdi::white);
		
		if ( (Color() == blue) && InControl())
		{
			gdi->TextAtPos(20,3,"Blue in Control");
		}
		else if ( (Color() == red) && InControl())
		{
			gdi->TextAtPos(20,3,"Red in Control");
		}
		if (m_pControllingPlayer != NULL)
		{
			gdi->TextAtPos(Pitch()->cxClient()-150, 3, "Controlling Player: " + ttos(m_pControllingPlayer->ID()));
		}
	}
	
	if (Prm.bSupportSpots && InControl())
	{
		m_pSupportSpotCalc->Render();
	}

#ifdef SHOW_TEAM_STATE
	if (Color() == red)
	{
		gdi->TextColor(Cgdi::white);
		
		if (CurrentState() == Attacking::Instance())
		{
			gdi->TextAtPos(160, 20, "Attacking");
		}
		if (CurrentState() == Defending::Instance())
		{
			gdi->TextAtPos(160, 20, "Defending");
		}
		if (CurrentState() == PrepareForKickOff::Instance())
		{
			gdi->TextAtPos(160, 20, "Kickoff");
		}
	}
	else
	{
	  if (CurrentState() == Attacking::Instance())
	  {
		  gdi->TextAtPos(160, Pitch()->cyClient()-40, "Attacking");
	  }
	  if (CurrentState() == Defending::Instance())
	  {
		  gdi->TextAtPos(160, Pitch()->cyClient()-40, "Defending");
	  }
	  if (CurrentState() == PrepareForKickOff::Instance())
	  {
		  gdi->TextAtPos(160, Pitch()->cyClient()-40, "Kickoff");
	  }
	}
#endif

#ifdef SHOW_SUPPORTING_PLAYERS_TARGET
	if (m_pSupportingPlayer)
	{
		gdi->BlueBrush();
		gdi->RedPen();
		gdi->Circle(m_pSupportingPlayer->Steering()->Target(), 4);
	}
#endif

}

///////////////////////////////////////////////////////////////////
//	
//	CreatePlayers関数 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::CreatePlayers()
{
	if (Color() == blue)
	{
		// ゴールキーパー生成
		m_Players.push_back(new GoalKeeper(this,
										   1,
										   TendGoal::Instance(),
										   Vector2D(0,1),
										   Vector2D(0.0, 0.0),
										   Prm.PlayerMass,
										   Prm.PlayerMaxForce,
										   Prm.PlayerMaxSpeedWithoutBall,
										   Prm.PlayerMaxTurnRate,
										   Prm.PlayerScale));
		// プレイヤー生成
		m_Players.push_back(new FieldPlayer(this,
											6,
											Wait::Instance(),
											Vector2D(0,1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::attacker));
	
	
	
		m_Players.push_back(new FieldPlayer(this,
											8,
											Wait::Instance(),
											Vector2D(0,1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::attacker));
		
		m_Players.push_back(new FieldPlayer(this,
											3,
											Wait::Instance(),
											Vector2D(0,1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::defender));
		
		m_Players.push_back(new FieldPlayer(this,
											5,
											Wait::Instance(),
											Vector2D(0,1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::defender));
	
	}
	else
	{
		// ゴールキーパ生成
		m_Players.push_back(new GoalKeeper(this,
										   16,
										   TendGoal::Instance(),
										   Vector2D(0,-1),
										   Vector2D(0.0, 0.0),
										   Prm.PlayerMass,
										   Prm.PlayerMaxForce,
										   Prm.PlayerMaxSpeedWithoutBall,
										   Prm.PlayerMaxTurnRate,
										   Prm.PlayerScale));
		
		// プレイヤー生成
		m_Players.push_back(new FieldPlayer(this,
											9,
											Wait::Instance(),
											Vector2D(0,-1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::attacker));
		
		m_Players.push_back(new FieldPlayer(this,
											11,
											Wait::Instance(),
											Vector2D(0,-1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::attacker));
		
		m_Players.push_back(new FieldPlayer(this,
											12,
											Wait::Instance(),
											Vector2D(0,-1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::defender));
		
		m_Players.push_back(new FieldPlayer(this,
											14,
											Wait::Instance(),
											Vector2D(0,-1),
											Vector2D(0.0, 0.0),
											Prm.PlayerMass,
											Prm.PlayerMaxForce,
											Prm.PlayerMaxSpeedWithoutBall,
											Prm.PlayerMaxTurnRate,
											Prm.PlayerScale,
											PlayerBase::defender));
	                    
	}
	
	// エンティティマネージャーにプレーヤーを登録
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		EntityMgr->RegisterEntity(*it);
	}
}

///////////////////////////////////////////////////////////////////
//	
//	GetPlayerFromID関数 
//  
///////////////////////////////////////////////////////////////////
PlayerBase* SoccerTeam::GetPlayerFromID(int id)const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->ID() == id) return *it;
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////
//	
//	SetPlayerHomeRegion関数 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::SetPlayerHomeRegion(int plyr, int region)const
{
	assert ( (plyr>=0) && (plyr<(int)m_Players.size()) );
	m_Players[plyr]->SetHomeRegion(region);
}

///////////////////////////////////////////////////////////////////
//	
//	UpdateTargetsOfWaitingPlayers関数 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::UpdateTargetsOfWaitingPlayers()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{  
		if ( (*it)->Role() != PlayerBase::goal_keeper )
		{
			//cast to a field player
			FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);
			
			if ( plyr->GetFSM()->isInState(*Wait::Instance()) ||
			     plyr->GetFSM()->isInState(*ReturnToHomeRegion::Instance()) )
			{
				plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
//	
//	AllPlayersAtHome関数
//
//  いずれかのチームが自エリアに位置していない場合はfalseを返す
//  
///////////////////////////////////////////////////////////////////
bool SoccerTeam::AllPlayersAtHome()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->InHomeRegion() == false)
		{
			return false;
		}
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////
//	
//	RequestPass関数
//
//  いずれかのチームが自エリアに位置していない場合はfalseを返す
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::RequestPass(FieldPlayer* requester)const
{
	if (RandFloat() > 0.1) return;
	
	if (isPassSafeFromAllOpponents(ControllingPlayer()->Pos(),
								   requester->Pos(),
								   requester,
								   Prm.MaxPassingForce))
	{
		// プレーヤーにパスを渡すように伝え、
		// パスが来ていることを受信者に知らせる
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
								requester->ID(),
								ControllingPlayer()->ID(),
								Msg_PassToMe,
								requester); 
	
	}
}

///////////////////////////////////////////////////////////////////
//	
//	isOpponentWithinRadius関数
//
//  相手のプレーヤーがパラメータとして与えられた
//  位置の半径内にある場合はtrueを返す
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::isOpponentWithinRadius(Vector2D pos, double rad)
{
  std::vector<PlayerBase*>::const_iterator end = Opponents()->Members().end();
  std::vector<PlayerBase*>::const_iterator it;

  for (it=Opponents()->Members().begin(); it !=end; ++it)
  {
    if (Vec2DDistanceSq(pos, (*it)->Pos()) < rad*rad)
    {
      return true;
    }
  }

  return false;
}
