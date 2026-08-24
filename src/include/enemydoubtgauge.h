//=========================================================
//
// 疑いゲージマルチオブジェクトクラス [ enemydoubtgauge.h ]
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
#include "object2Dmulti.h"

//*********************************************************
// 疑いゲージのマルチオブジェクトクラスを定義
//*********************************************************
class CEnemyDoubtGauge : public CObject2DMulti
{
public:

	CEnemyDoubtGauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CEnemyDoubtGauge();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pBaseTex">ベーステクスチャ名</param>
	/// <param name="pMultitexture">マルチテクスチャ名</param>
	/// <returns></returns>
	static CEnemyDoubtGauge* Create
	(
		const D3DXVECTOR3& pos, 
		const float fWidth, 
		const float fHeight, 
		const char* pBaseTex, 
		const char* pMultitexture
	);

public:

	/// <summary>
	/// 割合値の変更
	/// </summary>
	/// <param name="fRatio">変更値</param>
	void SetRatio(const float fRatio);

	void SetIsDraw(const bool& isDraw) { m_isDraw = isDraw; }
	void SetIsComplete(const bool& isDraw) { m_isComplete = isDraw; }
	void SetUpGauge(const bool& isFlags) { m_isUpGauge = isFlags; }
	void SetNormal(const bool& isFlags) { m_isNormalSet = isFlags; }

	bool GetIsDraw(void) const { return m_isDraw; }
	bool GetIsComplete(void) const { return m_isComplete; }
	bool GetUpGauge(void) const { return m_isUpGauge; }
	bool GetNormalFlag(void) { return m_isNormalSet; }

	void SetTargetPos(const D3DXVECTOR3& targetPos) { m_TargetPos = targetPos; }

private:

	float m_fRatio;		// 計算割合用の変数

private:
	bool m_isDraw;		// 描画フラグ
	bool m_isComplete;	// 達成したか
	bool m_isUpGauge;	// 上昇するフラグ
	bool m_isNormalSet;	// 通常に戻るフラグ
	D3DXVECTOR3 m_TargetPos; // 追従対象（敵）の3Dワールド座標
};