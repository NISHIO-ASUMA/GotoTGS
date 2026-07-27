//========================================================
//
// 受付役の人の処理 [ receptionist.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルード
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "nomovecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;
class CBoxCollider;

//*********************************************************
// 外サボり受付役のキャラクタークラスを定義
//*********************************************************
class CReceptionist : public CNoMoveCharactor
{
public:

	CReceptionist(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CReceptionist();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CReceptionist* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	inline CBoxCollider* GetBoxCollider(void) { return m_pBoxColiider.get(); }
	inline CSphereCollider* GetSphereCollider(void) { return m_pSphereColiider.get(); }

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		ACTION,		// アクション ( 2,3回うなずいている感じ )
		MAX
	};

private:
	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー
private:
};