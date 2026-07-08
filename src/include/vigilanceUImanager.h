//=========================================================
//
// 警戒度UIマネージャー処理 [ vigilanceUImanager.h ]
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
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CVigilanceicon;
class CVigilancegauge;

//*********************************************************
//  警戒度UIマネージャーに対するクラスを定義
//*********************************************************
class CVigilanceUIManager:public CObject
{
public:

	CVigilanceUIManager(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilanceUIManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetUse(const bool& bUse) { m_bUse = bUse; }

	// 情報取得処理
	bool GetUse(void) { return m_bUse; }

	// 生成処理
	static CVigilanceUIManager* Create(const bool& bUse);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float POS_X_VALUE = 150.0f;	// 縦幅
		static constexpr float POS_Y_VALUE = 10.0f;		// 横幅

		// アイコンの定数
		static constexpr float ICON_WIDTH = 100.0f;							// 横幅
		static constexpr float ICON_HEIGHT = 100.0f;						// 縦幅
		static constexpr float ICON_POS_X = 100.0f;							// X軸の位置
		static constexpr float ICON_POS_Y = 600.0f;							// Y軸の位置
		static constexpr const char* ICON_TEXNAME = "vigilanceicon.png";	// アイコンのテクスチャ

		// ゲージの定数
		static constexpr float GAUGE_WIDTH = 100.0f;									// 横幅
		static constexpr float GAUGE_HEIGHT = 100.0f;									// 縦幅
		static constexpr float GAUGE_POS_X = ICON_POS_X + POS_X_VALUE;					// X軸の位置
		static constexpr float GAUGE_POS_Y = ICON_POS_Y - POS_Y_VALUE;					// Y軸の位置
		static constexpr const char* GAUGE_FREAM_TEXNAME = "vigilancegauge000.png";		// フレームのテクスチャ
		static constexpr const char* GAUGE_TEXNAME_BASE = "vigilancegauge001.png";		// 基本のテクスチャ
		static constexpr const char* GAUGE_TEXNAME_MULTI = "gauge_pre.png";				// マルチテクスチャ

	};

	// メンバ変数
	CVigilanceicon* pIcon;		// アイコンのポインタ
	CVigilancegauge* pGauge;	// ゲージのポインタ
	bool m_bUse;				// 使用するかどうか
	int m_nFram;
	int m_nCount;
};