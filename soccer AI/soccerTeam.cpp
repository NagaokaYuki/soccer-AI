/*=======================================================================

	�T�b�J�[�`�[�� [soccerTeam.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
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
//	�R���X�g���N�^
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
	// �X�e�[�g�}�V������
	m_pStateMachine = new StateMachine<SoccerTeam>(this);
	
	m_pStateMachine->SetCurrentState(Defending::Instance());
	m_pStateMachine->SetPreviousState(Defending::Instance());
	m_pStateMachine->SetGlobalState(NULL);
	
	// �v���C���[����
	CreatePlayers();
	
	// �f�t�H���g��ݒ�
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
	  (*it)->Steering()->SeparationOn();   
	}
	
	// sweet sot�𐶐�
	m_pSupportSpotCalc = new SupportSpotCalculator(Prm.NumSupportSpotsX,
	                                               Prm.NumSupportSpotsY,
	                                               this);
}

/////////////////////////////////////////////////////////
//
//	�f�X�g���N�^
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
//	Update�֐�
//
/////////////////////////////////////////////////////////
void SoccerTeam::Update()
{
	// �t���[�����Ƃ�1�񂾂��v�Z����
	CalculateClosestPlayerToBall();

	// �U��/�h��̓����؂�ւ�
	// �z�C�b�X�����������O�Ƀ`�[�����L�b�N�I�t�ʒu��
	// �߂��Ȃ���΂Ȃ�Ȃ��u�L�b�N�I�t�v��Ԃ���������
	m_pStateMachine->Update();
	
	// �e�v���[���[���X�V
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
	  (*it)->Update();
	}
}

/////////////////////////////////////////////////////////
//
//	CalculateClosestPlayerToBall�֐�
//
//	m_iClosestPlayerToBall���{�[���ɍł��߂��v���[���[�ɐݒ�
//
/////////////////////////////////////////////////////////
void SoccerTeam::CalculateClosestPlayerToBall()
{
	double ClosestSoFar = MaxFloat;

	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		// dist���v�Z
		double dist = Vec2DDistanceSq((*it)->Pos(), Pitch()->Ball()->Pos());

		// ���̒l���e�v���[���[�ɋL�^���Ă���
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
//	DetermineBestSupportingAttacker�֐�
//
//	SupportSpot�ɍł��߂��v���[���[���v�Z
//
/////////////////////////////////////////////////////////
PlayerBase* SoccerTeam::DetermineBestSupportingAttacker()
{
	double ClosestSoFar = MaxFloat;
	
	PlayerBase* BestPlayer = NULL;
	
	std::vector<PlayerBase*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		// �U���҂�����BestSupportingSpot�𗘗p
		if ( ((*it)->Role() == PlayerBase::attacker) && ((*it) != m_pControllingPlayer) )
		{
		  double dist = Vec2DDistanceSq((*it)->Pos(), m_pSupportSpotCalc->GetBestSupportingSpot());
		
		  // ���݂܂łɋ������ł��߂��A
		  // �v���[���[���S�[���L�[�p�[�ł͂Ȃ��A
		  // �v���[���[�����݃{�[�����R���g���[�����Ă��Ȃ��ꍇ�́A
		  // ���̃v���[���[�̋L�^���c��
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
//	FindPass�֐�
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
	
	// �v���C���[�̂��ׂẴ`�[�������o�[���J��Ԃ��āA
	// �ǂꂪ�{�[����ʉ߂���ʒu�ɂ���̂����v�Z
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		if ( (*curPlyr != passer) &&            
		    (Vec2DDistanceSq(passer->Pos(), (*curPlyr)->Pos()) > 
		     MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, *curPlyr, Target, power))
			{
				// �p�X�̃^�[�Q�b�g�����܂łɌ�����������̃S�[�����C����
				// �ł��߂��ꍇ�͋L�^
				double Dist2Goal = fabs(Target.x - OpponentsGoal()->Center().x);
				
				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;
					
					// �I����L�^����
					receiver = *curPlyr;
					
					// �p�X���鑊����L�^
					PassTarget = Target;
				}     
			}
		}
	}// ���̃`�[�������o�[
	
	if (receiver) return true;
	
	else return false;
}


///////////////////////////////////////////////////////////////////
//
//	GetBestPassToReceiver�֐�
//
// 3�̃p�X���v�Z�����
// 1�͎󂯂Ƃ鑤�̌��݂̈ʒu�ɒ��ڌ����Ă���A
// 2�̓{�[���̈ʒu����󂯂Ƃ鑤���甼�a(�͈�)�̉~�܂ł̐ڐ�
// �����̃p�X�́A���肪�C���^�[�Z�v�g���ăv���[�G���A���ŏI�����邱�Ƃ�
// �m�F���邽�߂Ƀe�X�g����B���ׂẴp�X�������̏ꍇ
// �֐���false��Ԃ��A����ȊO�̏ꍇ�A
// �֐��͑���̃S�[���G���A�ɍł��߂��{�[�����󂯎��A�p�X��Ԃ�
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::GetBestPassToReceiver(const PlayerBase* const passer,
                                       const PlayerBase* const receiver,
                                       Vector2D&               PassTarget,
                                       double                   power)const
{  
	// �{�[�������̃��V�[�o�ɓ��B����܂łɂǂꂾ���̎��Ԃ������邩���v�Z
	double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
	                                                  receiver->Pos(),
	                                                  power);
	
	// �^����ꂽ�p���[�ŏR��ꂽ��Ƀ{�[����
	// ���V�[�o�ɓ��B�ł��Ȃ��ꍇ��false��Ԃ�
	if (time < 0) return false;
	
	// ���̎��ԂɎ󂯂Ƃ鑤���J�o�[�ł���ő勗��
	double InterceptRange = time * receiver->MaxSpeed();
	
	// �C���^�[�Z�v�g�͈͂��X�P�[�����O����
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;
	
	// �{�[�����烌�V�[�o�̃����W�T�[�N���ւ̐ڐ��̐ؕЂ�
	// �ʒu����p�X�^�[�Q�b�g���v�Z
	Vector2D ip1, ip2;
	
	GetTangentPoints(receiver->Pos(),
	                 InterceptRange,
	                 Pitch()->Ball()->Pos(),
	                 ip1,
	                 ip2);
	
	const int NumPassesToTry = 3;
	Vector2D Passes[NumPassesToTry] = {ip1, receiver->Pos(), ip2};
	
	//  1. ����܂łɔ������ꂽ���̃��V�[�o�̍ł��߂��L���p�X
	//  2. �G���A
	//  3. ���肪�C���^�[�Z�v�g�ł��Ȃ�
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
//	isPassSafeFromOpponent�֐�
//
//  'from'���� 'to'�ւ̃p�X������̃v���[���[�ɂ����
//  �C���^�[�Z�v�g����邩�ǂ������e�X�g
//
/////////////////////////////////////////////////////////
bool SoccerTeam::isPassSafeFromOpponent(Vector2D    from,
                                        Vector2D    target,
                                        const PlayerBase* const receiver,
                                        const PlayerBase* const opp,
                                        double       PassingForce)const
{
	// ��������G���A���Ɉړ�������
	Vector2D ToTarget = target - from;
	Vector2D ToTargetNormalized = Vec2DNormalize(ToTarget);
	
	Vector2D LocalPosOpp = PointToLocalSpace(opp->Pos(),
	                                       ToTargetNormalized,
	                                       ToTargetNormalized.Perp(),
	                                       from);
	
	// ���肪�L�b�J�[�̌��ɂ���ꍇ
	// �p�X���\
	// �{�[��������̍ő呬�x�����������x�ŏR����Ƃ�������
	if ( LocalPosOpp.x < 0 )
	{
		return true;
	}
	
	// �ΐ푊�肪�^�[�Q�b�g��艓������Ă���ꍇ�A
	// �ΐ푊�肪���V�[�o�[�̑O�̈ʒu�ɓ��B�ł��邩�ǂ�������������
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
	
	// �ΐ푊��̈ʒu�ƒ�������ʒu�܂ł̋������{�[�����ǂ̂��炢�̎��ԕێ����邩���v�Z
	double TimeForBall = 
	Pitch()->Ball()->TimeToCoverDistance(Vector2D(0,0),
	                                     Vector2D(LocalPosOpp.x, 0),
	                                     PassingForce);
	
	// ���肪�ǂꂭ�炢�����܂ő���邩���v�Z
	double reach = opp->MaxSpeed() * TimeForBall +
	              Pitch()->Ball()->BRadius()+
	              opp->BRadius();
	
	// �ΐ푊���y�ʒu�܂ł̋������A
	// �ΐ푊��̃����j���O�͈͂ƃ{�[���̔��a����ё���̔��a�𑫂����l��菬�����ꍇ�A
	// �{�[���͖T�󂳂��
	if ( fabs(LocalPosOpp.y) < reach )
	{
	  return false;
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////
//
//	isPassSafeFromAllOpponents�֐�
//
//  'from'�̈ʒu���� 'target'�̈ʒu�܂ł̃p�X���e�����o�[�ɑ΂��ăe�X�g
//  �C���^�[�Z�v�g���ꂸ�Ƀp�X���쐬�ł���ꍇ��true��Ԃ�
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
//	CanShoot�֐�
//
//  �{�[���̈ʒu�A�R�荞�ݗ́Avector2D�ւ̎Q�Ƃ��^�������
//  ���̊֐��͑���̃S�[���ɉ����������_���Ȉʒu���T���v�����O���A
//  �{�[�����^����ꂽ�͂ł��̕����ɏR����Ȃ�΃S�[�����ł��邩���`�F�b�N
//  �\�ȃV���b�g�����������ꍇ�A�֐���true��Ԃ��A
//  �^�[�Q�b�g�ʒu���x�N�g��ShotTarget�Ɋi�[�����
//
///////////////////////////////////////////////////////////////////
bool SoccerTeam::CanShoot(Vector2D  BallPos,
                          double     power, 
                          Vector2D& ShotTarget)const
{
	// �����_���ɍ쐬���ꂽ�V���b�g�^�[�Q�b�g�̐�
	int NumAttempts = Prm.NumAttemptsToFindValidStrike;
	
	while (NumAttempts--)
	{
		// ����̃S�[���̌��ɉ����ă����_���ȃ|�W�V������I��
		ShotTarget = OpponentsGoal()->Center();
		
		// �V���b�g�ʒu��y�l��2�̃S�[���|�X�g�̊Ԃɂ���
		int MinYVal = (int)(OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius());
		int MaxYVal = (int)(OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius());
		
		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);
		
		double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
		                                                  ShotTarget,
		                                                  power);
		
		// �V���[�g���ΐ푊��̂����ꂩ��������C���^�[�Z�v�g�ł��邩�ǂ����𒲂ׂ�e�X�g
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
//	ReturnAllFieldPlayersToHome�֐� 
// 
//	�����Ɏ��G���A�ɖ߂�悤�ɂ��ׂẴv���C���[�Ƀ��b�Z�[�W�𑗐M
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
//	Render�֐� 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::Render()const
{
	std::vector<PlayerBase*>::const_iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Render();
	}
	
	// �f�B�X�v���C�̏㕔�ɃR���g���[���`�[���ƃv���[���[��\��
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
//	CreatePlayers�֐� 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::CreatePlayers()
{
	if (Color() == blue)
	{
		// �S�[���L�[�p�[����
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
		// �v���C���[����
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
		// �S�[���L�[�p����
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
		
		// �v���C���[����
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
	
	// �G���e�B�e�B�}�l�[�W���[�Ƀv���[���[��o�^
	std::vector<PlayerBase*>::iterator it = m_Players.begin();
	
	for (it; it != m_Players.end(); ++it)
	{
		EntityMgr->RegisterEntity(*it);
	}
}

///////////////////////////////////////////////////////////////////
//	
//	GetPlayerFromID�֐� 
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
//	SetPlayerHomeRegion�֐� 
//  
///////////////////////////////////////////////////////////////////
void SoccerTeam::SetPlayerHomeRegion(int plyr, int region)const
{
	assert ( (plyr>=0) && (plyr<(int)m_Players.size()) );
	m_Players[plyr]->SetHomeRegion(region);
}

///////////////////////////////////////////////////////////////////
//	
//	UpdateTargetsOfWaitingPlayers�֐� 
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
//	AllPlayersAtHome�֐�
//
//  �����ꂩ�̃`�[�������G���A�Ɉʒu���Ă��Ȃ��ꍇ��false��Ԃ�
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
//	RequestPass�֐�
//
//  �����ꂩ�̃`�[�������G���A�Ɉʒu���Ă��Ȃ��ꍇ��false��Ԃ�
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
		// �v���[���[�Ƀp�X��n���悤�ɓ`���A
		// �p�X�����Ă��邱�Ƃ���M�҂ɒm�点��
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
								requester->ID(),
								ControllingPlayer()->ID(),
								Msg_PassToMe,
								requester); 
	
	}
}

///////////////////////////////////////////////////////////////////
//	
//	isOpponentWithinRadius�֐�
//
//  ����̃v���[���[���p�����[�^�Ƃ��ė^����ꂽ
//  �ʒu�̔��a���ɂ���ꍇ��true��Ԃ�
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
