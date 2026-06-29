//=========================================================
//
// ドアUI処理 [ doorui.h ]
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
#include "object2D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CDoorUI :public CObject2D
{
public:
	CDoorUI();
	~CDoorUI();

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDoorUI* Create(void);

	// インスタンス生成用関数
	static CDoorUI* Instance(void);
	
	void EasingSine(void);
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 当たり判定関数
	bool CollisionSphere(CSphereCollider* pOther);
	bool GetDisplay(void) { return m_bDisplay; }

	// ゲッター
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

private:
	static CDoorUI* m_pInstance;						// シングルトン変数
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー

	D3DXVECTOR3 m_pos;									// 位置
	float m_fCountFrame;								// イージング用フレームカウント用変数
	float m_fMaxFrame;									// 最大フレーム変数
	bool m_bEasing;										// イージング用変数
	bool m_bDisplay;									// 表示するかの判定変数
	bool m_bUse;										// 使用しているかの判定変数
};



