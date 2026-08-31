//=========================================================
//
// 外の監査人ステート基底クラス処理 [ auditorstatebase.h ]
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
class CAuditor;

//*********************************************************
// 監査人ステートベースクラスを定義
//*********************************************************
class CAuditorStateBase : public CStateBase
{
public:

	CAuditorStateBase();
	~CAuditorStateBase();

	virtual void OnStart(void) override {};
	virtual void OnUpdate(void) override {};
	virtual void OnExit(void) override {};

	int GetID(void) const { return m_nID; }
	void SetID(const int& nID) { m_nID = nID; }

	/// <summary>
	/// 自身のポインタを設定
	/// </summary>
	/// <param name="pAuditor">監査役自身のポインタ</param>
	void SetOwner(CAuditor* pAuditor) { m_pAuditor = pAuditor; }

public:
	//************************
	// ステート列挙型
	//************************
	enum STATEID
	{
		ID_NEUTRAL,		// 通常状態
		ID_DOUBT,		// 疑い状態
		ID_CHASE,		// 追跡状態
		ID_MAX
	};

protected:
	CAuditor* m_pAuditor; // インスタンス

private:
	int m_nID;			 // ID番号
};