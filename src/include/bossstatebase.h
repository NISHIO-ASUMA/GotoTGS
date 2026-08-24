//=========================================================
//
// 社長ステート基底クラス処理 [ bossstatebase.h ]
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
class CBoss;

//*********************************************************
// 社長ステートベースクラスを定義
//*********************************************************
class CBossStateBase : public CStateBase
{
public:

	CBossStateBase();
	~CBossStateBase();

	virtual void OnStart(void) override {};
	virtual void OnUpdate(void) override {};
	virtual void OnExit(void) override {};

	int GetID(void) const { return m_nID; }
	void SetID(const int& nID) { m_nID = nID; }

	void SetOwner(CBoss* pBoss) { m_pBoss = pBoss; }

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
	CBoss* m_pBoss; // 対象インスタンス

private:
	int m_nID;		 // ID番号
};