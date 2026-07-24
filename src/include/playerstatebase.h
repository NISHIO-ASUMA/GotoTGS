//=========================================================
//
// プレイヤーステート基底クラス処理 [ playerstatebase.h ]
// Author: Shouya Chikada
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
class CPlayer;

//*********************************************************
// プレイヤーステートベースクラスを定義
//*********************************************************
class CPlayerStateBase : public CStateBase
{
public:
	CPlayerStateBase();
	~CPlayerStateBase();

	virtual void OnStart(void) override {};
	virtual void OnUpdate(void) override {};
	virtual void OnExit(void) override {};

	int GetID(void) const { return m_nID; }
	void SetID(const int& nID) { m_nID = nID; }

	void SetOwner(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

	// インスタンス生成用関数
	static CPlayerStateBase* GetInstance(void);

public:
	//************************
	// ステート列挙型
	//************************
	enum STATEID
	{
		ID_NEUTRAL,
		ID_MOVE,
		ID_SMOKE,
		ID_MAGAZINE,
		ID_TV,
		ID_GAMECENTER,
		ID_FOOD,
		ID_MAX
	};

protected:
	CPlayer* m_pPlayer; // プレイヤーのポインタ変数

private:
	static CPlayerStateBase* m_pInstance;	// シングルトン変数
	int m_nID;							// ID番号
};