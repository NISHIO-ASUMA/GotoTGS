//=========================================================
//
// 矩形の当たり判定処理 [ collisionbox.h ]
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
#include "collision.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// 矩形の当たり判定をするクラスを定義
//*********************************************************
class CCollisionBox : public CCollision
{
public:

	CCollisionBox();
	~CCollisionBox();

	/// <summary>
	/// 矩形同士の当たり判定を実行する
	/// </summary>
	/// <param name="thisCollider">自分が持ってる矩形コライダー</param>
	/// <param name="OtherCollider">判定先の矩形コライダー</param>
	/// <param name="ExtrusionPos">ヒット時の押し出し座標</param>
	/// <returns></returns>
	static bool Collision
	(
		CBoxCollider* thisCollider, 
		CBoxCollider* OtherCollider,
		D3DXVECTOR3 * ExtrusionPos
	);

	/// <summary>
	/// 半径計算補助関数
	/// </summary>
	/// <param name="halfSize">半分のサイズ</param>
	/// <param name="axis">軸</param>
	/// <param name="localX">ローカルのXの値</param>
	/// <param name="localY">ローカルのYの値</param>
	/// <param name="localZ">ローカルのZの値</param>
	/// <returns></returns>
	static float MathRadius
	(
		const D3DXVECTOR3& halfSize,
		const D3DXVECTOR3& axis, 
		const D3DXVECTOR3& localX, 
		const D3DXVECTOR3& localY, 
		const D3DXVECTOR3& localZ
	);

	/// <summary>
	/// 矩形同士の当たり判定を実行する ( 回転軸考慮 )
	/// </summary>
	/// <param name="thisCollider">自分が持ってる矩形コライダー</param>
	/// <param name="OtherCollider">判定先の矩形コライダー</param>
	/// <param name="ExtrusionPos">ヒット時の押し出し座標</param>
	/// <returns></returns>
	static bool CollisionEx
	(
		CBoxCollider* thisCollider, 
		CBoxCollider* OtherCollider, 
		D3DXVECTOR3* pOutPos
	);

private:

	static constexpr int NUM_AXIS = 3;   // 軸数
	static constexpr int CROSS_AXIS = 6; // 外積軸数
	static constexpr int MAX_AXIS = 15;  // 最大判定軸数

private:
};