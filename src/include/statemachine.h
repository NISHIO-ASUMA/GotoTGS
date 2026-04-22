//=========================================================
//
// ステートマシンクラス処理 [ statemachine.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 前方宣言
//*********************************************************
class CStateBase;

//*********************************************************
// ステートマシンクラスを定義
//*********************************************************
class CStateMachine
{
public:

	CStateMachine();
	~CStateMachine();

	void Update(void);
	void ChangeState(CStateBase* pNewState);
	void OnExit(void);

	int GetNowStateID(void);
	CStateBase* GetNowState() const { return m_pNowState; }

private:

	CStateBase* m_pNowState; // 現在のステート状態
};