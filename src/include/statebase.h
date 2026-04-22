//=========================================================
//
// ステート基底クラス処理 [ statebase.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// ステート基底クラスを定義
//*********************************************************
class CStateBase
{
public:

	CStateBase() = default;
	virtual ~CStateBase() = default;

	virtual void OnStart(void) {};
	virtual void OnUpdate(void) {};
	virtual void OnExit(void) {};
	virtual int GetID(void) const = 0;
private:
};