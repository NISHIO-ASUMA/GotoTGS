//=========================================================
//
// 敵ステート基底クラス処理 [ enemystatebase.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "statebase.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CEnemy;

//*********************************************************
// 敵ステートベースクラスを定義
//*********************************************************
class CEnemyStateBase : public CStateBase
{
public:

	CEnemyStateBase();
	~CEnemyStateBase();

	virtual void OnStart(void) override {};
	virtual void OnUpdate(void) override {};
	virtual void OnExit(void) override {};

	int GetID(void) const { return m_nID; }
	void SetID(const int& nID) { m_nID = nID; }

	void SetOwner(CEnemy* pEnemy) { m_pEnemy = pEnemy; }

public:
	//************************
	// ステート列挙型
	//************************
	enum STATEID
	{
		ID_NEUTRAL, // 通常状態
		ID_DOUBT,	// 疑い状態
		ID_CHASE,	// 追跡状態
		ID_MAX
	};

protected:
	CEnemy* m_pEnemy; // 敵のインスタンス

private:
	int m_nID;		 // ID番号
};