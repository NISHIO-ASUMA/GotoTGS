//========================================================
//
// 外の監査役の処理 [ auditor.h ]
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
// 外にいる監査役のキャラクタークラスを定義
//*********************************************************
class CAuditor : public CMoveCharactor
{
public:

	CAuditor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CAuditor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void MovePointOutSide(const D3DXVECTOR3& pos);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CAuditor* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

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
	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

private:
	int m_nViewIdx;							// ビューポイントの巡回インデックス番号
	int m_nOfficeViewIdx;					// オフィス内の巡回インデックス番号
	int m_nCoolTime;						// セットポジションについた時のクールタイム
};