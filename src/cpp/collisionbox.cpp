//=========================================================
//
// 矩形の当たり判定処理 [ collisionbox.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "collisionbox.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"

//*********************************************************
// 定数空間
//*********************************************************
namespace BOXINFO
{
	constexpr float PUSHVALUE = 0.05f;		// 押し出し加算値
};

//=========================================================
// コンストラクタ
//=========================================================
CCollisionBox::CCollisionBox() : CCollision(CCollision::TYPE_BOX)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CCollisionBox::~CCollisionBox()
{
	
}
//===========================================================
// 矩形同士のコリジョン判定関数
//===========================================================
bool CCollisionBox::Collision(CBoxCollider* thisCollider, CBoxCollider* OtherCollider, D3DXVECTOR3* ExtrusionPos)
{
	//===========================
	// コライダーポインタ情報
	//===========================
	CBoxCollider* pMyCollider = thisCollider;
	CBoxCollider* pOtherCollider = OtherCollider;

	//===========================
	// 自分と相手の座標を取得
	//===========================
	D3DXVECTOR3 MyPos = pMyCollider->GetPos();
	D3DXVECTOR3 OtherPos = pOtherCollider->GetPos();

	//===========================
	// コライダーサイズを取得
	//===========================
	D3DXVECTOR3 MySize = pMyCollider->GetInfo().Size;
	D3DXVECTOR3 OtherSize = pOtherCollider->GetInfo().Size;

	//================================
	// 自分と相手の半分のサイズを取得
	//================================
	D3DXVECTOR3 MyHalfSize = MySize * HALF;
	D3DXVECTOR3 OtherHalfSize = OtherSize * HALF;

	//===============================
	// 最大最小座標を計算する
	//===============================
	D3DXVECTOR3 MyPosMax = MyPos + MyHalfSize;
	D3DXVECTOR3 MyPosMin = MyPos - MyHalfSize;
	D3DXVECTOR3 OtherPosMax = OtherPos + OtherHalfSize;
	D3DXVECTOR3 OtherPosMin = OtherPos - OtherHalfSize;
	
	//===========================
	// 過去の座標を取得
	//===========================
	D3DXVECTOR3 MyPosOldMin = pMyCollider->GetInfo().posOld - MyHalfSize;
	D3DXVECTOR3 MyPosOldMax = pMyCollider->GetInfo().posOld + MyHalfSize;
	D3DXVECTOR3 OtherPosOldMin = pOtherCollider->GetInfo().posOld - OtherHalfSize;
	D3DXVECTOR3 OtherPosOldMax = pOtherCollider->GetInfo().posOld + OtherHalfSize;

	// 高さの範囲内に入って無かったら判定をしない
	const bool isHitRangeY = MyPosOldMin.y <= OtherPosOldMax.y && MyPosOldMax.y >= OtherPosOldMin.y;
	if (!isHitRangeY) return false;

	//==============================
	// z軸の範囲内に入っているとき
	//==============================
	if (MyPosOldMin.z < OtherPosMax.z && MyPosMax.z > OtherPosMin.z)
	{
		if (MyPosOldMax.x <= OtherPosMin.x && MyPosMax.x > OtherPosMin.x)
		{// 左からめり込む

			// 左側への押し出し座標を計算
			ExtrusionPos->x = OtherPosMin.x - MyHalfSize.x - BOXINFO::PUSHVALUE;
			return true;
		}
		else if (MyPosOldMin.x >= OtherPosOldMax.x && MyPosMin.x < OtherPosMax.x)
		{// 右からめり込む
			
			// 右側への押し出し座標を計算
			ExtrusionPos->x = OtherPosMax.x + MyHalfSize.x + BOXINFO::PUSHVALUE;
			return true;
		}
	}
	
	//==============================
	// x軸の範囲内に入っているとき
	//==============================
	if (MyPosOldMin.x < OtherPosMax.x && MyPosMax.x > OtherPosMin.x)
	{
		if (MyPosOldMax.z <= OtherPosMin.z && MyPosMax.z > OtherPosMin.z)
		{// 手前からめり込む

			// 手前への押しだし座標を計算
			ExtrusionPos->z = OtherPosMin.z - MyHalfSize.z - BOXINFO::PUSHVALUE;
			return true;
		}
		else if (MyPosOldMin.z >= OtherPosMax.z && MyPosMin.z < OtherPosMax.z)
		{// 奥から手前にめり込む

			// 奥側に押し出す座標を計算する
			ExtrusionPos->z = OtherPosMax.z + MyHalfSize.z + BOXINFO::PUSHVALUE;
			return true;
		}
	}

	// 未ヒット時
	return false;
}
//===========================================================
// 投影半径計算関数
//===========================================================
float CCollisionBox::MathRadius
(
	const D3DXVECTOR3& halfSize, 
	const D3DXVECTOR3& axis, 
	const D3DXVECTOR3& localX, 
	const D3DXVECTOR3& localY, 
	const D3DXVECTOR3& localZ
)
{
	// 各ローカル軸を判定軸に投影した長さの合計
	return  halfSize.x * fabsf(D3DXVec3Dot(&localX, &axis)) + // X軸の長さの内積
			halfSize.y * fabsf(D3DXVec3Dot(&localY, &axis)) + // Y軸の長さの内積
			halfSize.z * fabsf(D3DXVec3Dot(&localZ, &axis));  // Z軸の長さの内積
}
//=========================================================
// 回転を考慮した当たり判定 ( OBB )
//=========================================================
bool CCollisionBox::CollisionEx(CBoxCollider* thisCollider, CBoxCollider* OtherCollider, D3DXVECTOR3* pOutPos)
{
	//-----------------------------------
	// 座標情報
	//-----------------------------------
	D3DXVECTOR3 posThis = thisCollider->GetPos();
	D3DXVECTOR3 posOther = OtherCollider->GetPos();

	//-----------------------------------
	// 半分のサイズ
	//-----------------------------------
	D3DXVECTOR3 halfThis = thisCollider->GetInfo().Size * 0.5f;
	D3DXVECTOR3 halfOther = OtherCollider->GetInfo().Size * 0.5f;

	//-----------------------------------
	// 回転軸情報
	//-----------------------------------
	D3DXMATRIX matThis = thisCollider->GetInfo().MtxRotation;
	D3DXMATRIX matOther = OtherCollider->GetInfo().MtxRotation;

	//-----------------------------------
	// 3軸生成
	//-----------------------------------
	D3DXVECTOR3 axisThis[NUM_AXIS] = 
	{
		D3DXVECTOR3(matThis._11, matThis._12, matThis._13), 
		D3DXVECTOR3(matThis._21, matThis._22, matThis._23), 
		D3DXVECTOR3(matThis._31, matThis._32, matThis._33) 
	};

	D3DXVECTOR3 axisOther[NUM_AXIS] = 
	{
		D3DXVECTOR3(matOther._11, matOther._12, matOther._13), 
		D3DXVECTOR3(matOther._21, matOther._22, matOther._23), 
		D3DXVECTOR3(matOther._31, matOther._32, matOther._33) 
	};

	// 中心間ベクトル
	D3DXVECTOR3 VecCenter = posOther - posThis;

	// 判定軸のリストアップ
	D3DXVECTOR3 AxisList[MAX_AXIS];

	AxisList[0] = axisThis[0]; // 自身のローカル軸
	AxisList[1] = axisThis[1];
	AxisList[2] = axisThis[2];

	AxisList[3] = axisOther[0]; // 判定先のローカル軸
	AxisList[4] = axisOther[1];
	AxisList[5] = axisOther[2];

	// 外積による軸
	int nIdx = CROSS_AXIS;

	for (int nCnthis = 0; nCnthis < NUM_AXIS; nCnthis++)
	{
		for (int nCntOther = 0; nCntOther < NUM_AXIS; nCntOther++)
		{
			// 外積を生成
			D3DXVec3Cross(&AxisList[nIdx++], &axisThis[nCnthis], &axisOther[nCntOther]);
		}
	}

	float fMinOverlap = FLT_MAX; // 最小の重なり量
	D3DXVECTOR3 pushAxis = VECTOR3_NULL; // 押し出す方向の格納先

	//各軸で重なりチェック
	for (int nAxis = 0; nAxis < MAX_AXIS; nAxis++)
	{
		// 軸がゼロベクトルの場合はスキップ
		float fLengthSq = D3DXVec3LengthSq(&AxisList[nAxis]);
		if (fLengthSq < 0.001f) continue;

		// 判別用変数
		D3DXVECTOR3 unitAxis;
		D3DXVec3Normalize(&unitAxis, &AxisList[nAxis]);

		// 中心間距離の投影
		float fDistance = fabsf(D3DXVec3Dot(&VecCenter, &unitAxis));

		// 2つの矩形のそれぞれの投影半径
		float fRadiusThis = MathRadius(halfThis, unitAxis, axisThis[0], axisThis[1], axisThis[2]);
		float fRadiusOther = MathRadius(halfOther, unitAxis, axisOther[0], axisOther[1], axisOther[2]);

		// 重なり量を計算
		float fOverlap = (fRadiusThis + fRadiusOther) - fDistance;

		if (fOverlap < 0.0f)
		{
			return false; // 隙間があるので当たっていない
		}

		// ブルブル防止の重み付け
		float weight = (nAxis < CROSS_AXIS) ? 1.0f : 1.1f;

		// 最小の重なりを更新
		if (fOverlap * weight < fMinOverlap)
		{
			fMinOverlap = fOverlap;
			pushAxis = unitAxis;

			// 内積を使って判別
			if (D3DXVec3Dot(&VecCenter, &pushAxis) > 0.0f)
			{
				pushAxis = -pushAxis;
			}
		}
	}

	// 最終押し出し座標を設定する
	*pOutPos = posThis + (pushAxis * (fMinOverlap + BOXINFO::PUSHVALUE));

	return true; // 全ての軸で重なっていたら衝突
}