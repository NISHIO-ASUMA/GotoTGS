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
// ブロックオブジェクトクラスを定義
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
	bool Collision(CSphereCollider* pOther);

	inline CSphereCollider* GetCollider(void) { return m_pCollider.get(); }

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool GetAfk(void) { return m_bAfk; }

private:
	std::unique_ptr<CSphereCollider> m_pCollider;	// 球形のコライダー
	D3DXVECTOR3 m_pos;								// 位置
	float m_fRadius;								// UI表示範囲の半径用変数
	bool m_bAfk;									// さぼり判定用変数
};

