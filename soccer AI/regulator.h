/*=======================================================================

	レギュレーター [regulator.h]

												長岡佑樹

	このクラスを使用してコードフローを調整する
	(更新関数の場合など）コードセクションをフローさせたい頻度で
	クラスをインスタンス化し（毎秒10回など）、
	次に許可するReady（）がtrueを返す場合は、プログラムフローを続行

=========================================================================*/
#ifndef REGULATOR
#define REGULATOR

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "main.h"
#include "mmsystem.h" 
#include "utils.h"

/////////////////////////////////////////////////////////
//
//	Regulatorクラス
//
/////////////////////////////////////////////////////////
class Regulator
{
private:
	// 更新間隔
	double m_dUpdatePeriod;
	
	// 次回レギュレータがコードフローを許可するとき
	DWORD m_dwNextUpdateTime;

public:
	Regulator(double NumUpdatesPerSecondRqd)
	{
		m_dwNextUpdateTime = (DWORD)(timeGetTime()+RandFloat()*1000);
		
		if (NumUpdatesPerSecondRqd > 0)
		{
		  m_dUpdatePeriod = 1000.0 / NumUpdatesPerSecondRqd; 
		}
		
		else if (isEqual(0.0, NumUpdatesPerSecondRqd))
		{
		  m_dUpdatePeriod = 0.0;
		}
		
		else if (NumUpdatesPerSecondRqd < 0)
		{
		  m_dUpdatePeriod = -1;
		}
	}

	// 現在の時刻がm_dwNextUpdateTimeを超えた場合にtrueを返す
	bool isReady()
	{
		// レギュレータがゼロ周波数でインスタンス化された場合、
		// ステルスモードになります（調整しません）
		if (isEqual(0.0, m_dUpdatePeriod)) return true;

		// レギュレータが負の周波数でインスタンス化されている場合
		// コードが流れない
		if (m_dUpdatePeriod < 0) return false;
		
		DWORD CurrentTime = timeGetTime();
		
		// 必要な更新ステップごとに更新期間が変わる可能性があるミリ秒数
		// このクラスの複数のクライアントが、
		// 更新が均等に広がっていることを確認するため
		static const double UpdatePeriodVariator = 10.0;
		
		if (CurrentTime >= m_dwNextUpdateTime)
		{
		  m_dwNextUpdateTime = (DWORD)(CurrentTime + m_dUpdatePeriod + RandInRange(-UpdatePeriodVariator, UpdatePeriodVariator));
		
		  return true;
		}
		
		return false;
	}
};

#endif