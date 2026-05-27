//=========================================================
//
// AFKたばこ処理 [ afksmoke.h ]
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
// インクルードファイル
//*********************************************************
#include "afk.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CAfksmoke : public CAfksmoke
{
public:

	CAfk();
	~CAfk();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool Collision(CSphereCollider* pOther);

	inline CSphereCollider* GetCollider(void) { return m_pCollider.get(); }

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

private:
	std::unique_ptr<CSphereCollider> m_pCollider;	// 球形のコライダー
	D3DXVECTOR3 m_pos;								// 位置
	float m_fRadius;								// UI表示範囲の半径用変数
};

