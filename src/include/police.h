//========================================================
//
// 警察キャラクターの処理 [ police.h ]
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
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// 警察キャラクタークラスを定義
//*********************************************************
class CPolice : public CMoveCharactor
{
public:

	CPolice(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPolice();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CSphereCollider* pOther);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPolice* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動
		FOLLOW,		// 追従
		MAX
	};

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 60.0f;	// 球形範囲
	};

	//***************************
	// 定数構造体
	//***************************
	struct SERCHINFO
	{
		static constexpr float EYE_RANGE = 45.0f;		// 視界判定
	};

private:

private:

	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー
};
