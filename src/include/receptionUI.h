//=========================================================
//
// 受付人のui処理 [ receptionUI.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;
class CPlayer;
class CReceptionlineUI;

//*********************************************************
// 受付人のuiクラスを定義
//*********************************************************
class CReceptionUI :public CObject2D
{
public:
	CReceptionUI();
	~CReceptionUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void EasingSine(void);
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetPlayerOwner(CPlayer* pOwner = nullptr) { m_pPlayerOwner = pOwner; }

	// 当たり判定関数
	bool CollisionSphere(CSphereCollider* pOther);
	bool GetDisplay(void) { return m_bDisplay; }

	// ゲッター
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }
	CReceptionlineUI* GetLineUI(void) { return m_pLineUI; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pPlayer">判別先のプレイヤーポインタ</param>
	/// <returns></returns>
	static CReceptionUI* Create(CPlayer* pPlayer = nullptr);

private:
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー

	D3DXVECTOR3 m_pos;									// 位置
	float m_fCountFrame;								// イージング用フレームカウント用変数
	float m_fMaxFrame;									// 最大フレーム変数
	bool m_bEasing;										// イージング用変数
	bool m_bDisplay;									// 表示するかの判定変数
	bool m_bUse;										// 使用しているかの判定変数

	CPlayer* m_pPlayerOwner;							// 格納するプレイヤーのポインタ
	CReceptionlineUI* m_pLineUI;						// セリフUIのポインタ
};