//=========================================================
//
// 世界の透明な壁オブジェクト処理 [ gamewallmodel.h ]
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
class CSphereCollider;

//*********************************************************
// 壁を生成するクラスを定義
//*********************************************************
class CGameWallModel : public CObjectX
{
public:

	CGameWallModel(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CGameWallModel();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CSphereCollider* pOther);
	inline CBoxCollider* GetCollider(void) const { return m_pCollider.get(); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CGameWallModel* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		const D3DXVECTOR3& scale, 
		const char* pModelName
	);

private:
	std::unique_ptr<CBoxCollider>m_pCollider;	// 矩形のコライダー
};