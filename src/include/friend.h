//=========================================================
//
// 同僚処理 [ friend.h ]
// Author: Shouya Chikada
// 
// NOTE : 
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "nomovecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;

//*********************************************************
// 同僚オブジェクトクラスを定義
//*********************************************************
class CFriend : public CNoMoveCharactor
{
public:
	CFriend(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CFriend();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);
	/// <summary>
	/// 同僚生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CFriend* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot
	);

	inline CBoxCollider* GetBoxCollider(void) { return m_pBoxCollider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereCollider.get(); }

public:
	//************************
	// モーション列挙型
	//************************
	enum MOTION
	{
		NEUTRAL,
		MAX
	};

private:
	std::unique_ptr<CBoxCollider> m_pBoxCollider;		// 矩形のコライダー
	std::unique_ptr<CSphereCollider> m_pSphereCollider;	// 球形のコライダー
};
