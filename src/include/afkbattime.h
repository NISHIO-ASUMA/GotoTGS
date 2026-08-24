//=========================================================
//
// サボりの出来ない時間を表示するui処理 [ afkbattime.h ]
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
#include "billboardmulti.h"

//*********************************************************
// サボりが出来ない時間を表示するuiクラスを定義
//*********************************************************
class CAfkCoolTimeUi : public CBillboardMulti
{
public:

	CAfkCoolTimeUi(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CAfkCoolTimeUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void StartSet(void);
	void SetUse(const bool isUse) { m_isUse = isUse; }
	bool GetUse(void) const { return m_isUse; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <returns></returns>
	static CAfkCoolTimeUi* Create(const D3DXVECTOR3& pos);

private:
	bool m_isUse;		// 使用中かどうか
	float m_fRatio;		// 割合値
};