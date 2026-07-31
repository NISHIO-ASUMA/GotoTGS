//=========================================================
//
// AFK処理 [ afk.h ]
// Author: Shouya Chikada
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
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// サボりクラスを定義
//*********************************************************
class CAfk
{
public:
	CAfk();
	~CAfk();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fRadius">半径</param>
	/// <returns></returns>
	HRESULT Init
	(
		const D3DXVECTOR3& pos,
		const float& fRadius
	);

	void Uninit(void);
	void Update(void);

	// 当たり判定関数
	bool CollisionSphere(CSphereCollider* pOther);

	// ゲッター
	D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }
	bool GetAfk(void) { return m_bAfk; }

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }

private:
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー

	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_posOld;								// 前回の位置
	D3DXVECTOR3 m_Size;									// サイズ
	float m_fRadius;									// UI表示範囲の半径用変数
	bool m_bAfk;										// さぼり判定用変数
};

