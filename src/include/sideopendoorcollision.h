//=========================================================
//
// サイドに回転して開くドア当たり判定管理処理 [ sideopendoorcollision.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"

//*********************************************************
// 回転して開くドアの当たり判定管理クラスを定義
//*********************************************************
class CSideOpenDoorCollision
{
public:

	//*************************
	// 判定処理構造体
	//*************************
	struct CollisionData
	{
		D3DXVECTOR3 pos;	// 座標
		float fRadius;		// 半径
		std::vector<int> targetDoorIndices;			// 開けるドア数のインデックス
		std::unique_ptr<CSphereCollider> pCollider; // 球形のコライダー
	};

public:

	~CSideOpenDoorCollision();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

public:

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CSideOpenDoorCollision* GetInstance(void)
	{
		static CSideOpenDoorCollision Instance;
		return &Instance;
	}

	/// <summary>
	/// コライダー追加関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fRadius">判定半径</param>
	void AddCollider(const D3DXVECTOR3& pos, const float& fRadius, const std::vector<int>& nIdx);

	/// <summary>
	/// 判定コライダーの取得関数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<std::unique_ptr<CollisionData>>& GetColliders(void) const { return m_SideDoorCollider; }

private:

	CSideOpenDoorCollision();

	void LoadJson(void);

private:

	std::vector<std::unique_ptr<CollisionData>> m_SideDoorCollider; // 判定コライダーの配列
};