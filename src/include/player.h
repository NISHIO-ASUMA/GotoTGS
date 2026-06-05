//=========================================================
//
// プレイヤー処理 [ player.cpp ]
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
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;
class CPlayerStateBase;
class CStateMachine;

//*********************************************************
// プレイヤーオブジェクトクラスを定義
//*********************************************************
class CPlayer : public CMoveCharactor
{
public:

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);
	bool CollisionSphere(CSphereCollider* pOther);
	void ChangeState(CPlayerStateBase* pState, int nID);
	void MoveKeyboard(float speed);
	void MoveJoypad(float speed);

	/// <summary>
	/// プレイヤー生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPlayer* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot
	);
	
	bool GetMoveCheck(void) { return m_bMove; }
	bool GetAfkSmoke(void) { return m_bAfkSmoke; }
	bool GetAfkTV(void) { return m_bAfkTV; }
	inline CBoxCollider* GetBoxCollider(void) { return m_pBoxCollider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

public:

	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		MOVE,
		SMOKE,
		TV,
		ACTION3,
		MAX
	};

private:
	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	bool m_bMove;										// 移動したかどうかの判定変数
	bool m_bAfkSmoke;									// たばこさぼりの判定変数
	bool m_bAfkTV;										// TVさぼりの判定変数
};