//=========================================================
//
// デスク処理 [ desk.h ]
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
#include "objectX.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// デスクオブジェクトクラスを定義
//*********************************************************
class CDesk : public CObjectX
{
public:

	CDesk(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CDesk();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos);
	inline CBoxCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CDesk* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:
	std::unique_ptr<CBoxCollider> m_pCollider;	// 矩形のコライダー
};

