//=============================================================================
//
// スコア処理 [score.cpp]
// Author : NAGAOKA YUKI
//
//=============================================================================
#include "main.h"
#include "time.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

///////////////////////////////////////////////
//
//		マクロ定義
//
////////////////////////////////////////////////
#define TIME_POS_X			(500.0f)
#define TIME_POS_Y			(0.0f)
#define TIME_WIDTH			(20.0f)
#define TIME_HEIGHT			(20.0f)

///////////////////////////////////////////////
//
//		プロトタイプ宣言
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		グローバル変数
//
////////////////////////////////////////////////

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CTime *CTime::Create(void)
{
	CTime *pTime;
	pTime = new CTime(7);
	pTime->Init();
	return pTime;
}

///////////////////////////////////////////////
//
//		コンストラクタ
//
////////////////////////////////////////////////
CTime::CTime(int Priority):CScene(Priority)
{
	for(int i = 0; i < TIME_NUM; i++)
	{
		m_apNumber[i] = NULL;
	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nTime = 0;
}
///////////////////////////////////////////////
//
//		デストラクタ
//
////////////////////////////////////////////////
CTime::~CTime()
{
	
}

///////////////////////////////////////////////
//
//		初期化処理
//
////////////////////////////////////////////////
void CTime::Init(void)
{
	for(int i = 0; i < TIME_NUM; i++)
	{
		m_apNumber[i] = CNumber::Create();
		m_apNumber[i]->SetPosition(D3DXVECTOR3(TIME_POS_X - (TIME_WIDTH * i), TIME_POS_Y, 0.0f));
		m_apNumber[i]->SetSize(D3DXVECTOR2(TIME_WIDTH, TIME_HEIGHT));
	}
}

///////////////////////////////////////////////
//
//		終了処理
//
////////////////////////////////////////////////
void CTime::Uninit(void)
{
	for(int i = 0; i < TIME_NUM; i ++)
	{
		m_apNumber[i]->Uninit();
		delete m_apNumber[i];
		m_apNumber[i] = NULL;
	}
}

///////////////////////////////////////////////
//
//		更新処理
//
////////////////////////////////////////////////
void CTime::Update(void)
{
	int number = 0;
	int value = m_nTime;

	m_FrameTime++;

	if(m_FrameTime == 30)
	{
		m_nTime++;
	}

	m_FrameTime = m_FrameTime * (m_FrameTime % 60 != 0);

	value = m_nTime;


	for(int i = 0; i < TIME_NUM; i ++)
	{
		number = value %10;
		value /= 10;
		m_apNumber[i]->SetNumber(number);
		m_apNumber[i]->Update();
	}
}

///////////////////////////////////////////////
//
//		描画処理
//
////////////////////////////////////////////////
void CTime::Draw(void)
{
	for(int i = 0; i < TIME_NUM; i ++)
	{
		m_apNumber[i]->Draw();
	}
}


