//========================================================
//
// 社長処理 [ boss.h ]
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
class CBoxCollider;

//*********************************************************
// ボスキャラクタークラスを定義
//*********************************************************
class CBoss : public CMoveCharactor
{
public:

	CBoss(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CBoss();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void MoveInOffice(const D3DXVECTOR3& pos);
	void MoveOfficePoint(const D3DXVECTOR3& pos);

	void DrawEyeSight(void);
	bool CheckEyesight(const D3DXVECTOR3& TargetPos);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CBoss* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動	
		MAX
	};

private:
	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 80.0f; // 球形範囲
		static constexpr float BOX_RANGE = 50.0f;	 // 矩形範囲
		static constexpr int DIVIDE = 16;			 // メッシュの分割数
	};

	//***************************
	// 扇形の当たり判定
	//***************************
	struct Eyesight
	{
		static constexpr float EYE_RADIUS = 100.0f;		// 視界の届く距離
		static constexpr float EYE_ANGLE = 45.0f;		// 視野角
		static constexpr float EYE_HEIGHT = 50.0f;		// 視界の高さ制限
	};

private:
	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

private:
	int m_nViewIdx;							// ビューポイントの巡回インデックス番号
	int m_nOfficeViewIdx;					// オフィス内の巡回インデックス番号
	int m_nCoolTime;						// セットポジションについた時のクールタイム

private:
	bool m_isOutSideIn;						// 外から移動しているフラグ
	bool m_isOfficeMove;					// オフィス内で移動するフラグ
};