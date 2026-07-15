//=========================================================
//
// 警戒ゲージ処理 [ vigilancegauge.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルード
//*********************************************************
#include "object2Dmulti.h"

//*********************************************************
//  警戒ゲージに対するクラスを定義
//*********************************************************
class CVigilancegauge:public CObject2DMulti
{
public:

	// 警戒度のゲージの構造体
	struct Gauge
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXCOLOR col;		// 色
		float fWidth;		// 横幅
		float fHeight;		// 縦幅
		int nAnchorType;	// アンカーポイント
	};

	CVigilancegauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilancegauge();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void SetRatio(float fRatio) { m_fRatio = fRatio; }	// 比率

	// 情報取得処理
	float GetRatio(void) const { return m_fRatio; }				// 現在の比率
	int GetLevelCount(void) const { return m_nLevelCount; }		// 現在のレベルカウント

	// 生成処理
	static CVigilancegauge* Create(const Gauge& gauge, const char* BTEXTURE, const char* MTEXTURE);

private:

	// メンバ変数
	float m_fRatio;		// 現在の比率
	int m_nLevelCount;	// 現在のレベルカウント
};