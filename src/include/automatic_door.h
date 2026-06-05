//=========================================================
//
// 自動ドアの処理 [ automatic_door.h ]
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
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "objectX.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// 自動ドアクラスを定義
//*********************************************************
class CAutoMaticDoor : public CObjectX
{
public:

	CAutoMaticDoor(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CAutoMaticDoor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);

	/// <summary>
	/// 矩形コライダー取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline CBoxCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CAutoMaticDoor* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:

private:

	std::unique_ptr<CBoxCollider> m_pCollider;	// 矩形のコライダー
	bool m_isZTestEneble;						// 透明化判定を受けるかどうか
};