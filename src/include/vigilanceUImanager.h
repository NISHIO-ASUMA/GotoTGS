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
#include "vigilanceicon.h"
#include "vigilancegauge.h"

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
	CVigilanceicon* GetIcon(void) const { return m_pIcon; }		// アイコン
	CVigilancegauge* GetGauge(void) const { return m_pGauge; }	// ゲージ
	bool GetUse(void) { return m_bUse; }						// 使用しているかどうか

	// 生成処理
	static CVigilanceUIManager* Create(const bool& bUse);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		// アイコンの定数
		static constexpr float ICON_WIDTH = 85.0f;					// 横幅
		static constexpr float ICON_HEIGHT = 85.0f;					// 縦幅
		static constexpr float ICON_POS_X = 130.0f;					// X軸の位置
		static constexpr float ICON_POS_Y = 600.0f;					// Y軸の位置
		static constexpr const char* ICON_BOSS = "BOSS.png";		// ボスのテクスチャ
		static constexpr const char* ICON_POLICE = "POLICE.png";	// 警察のテクスチャ
		static constexpr const char* ICON_AUDITOR = "Auditor.png";	// 監査役のテクスチャ
		static constexpr const char* ICON_ENEMY = "Enemy.png";		// 上司役のテクスチャ

		// ゲージの定数
		static constexpr float GAUGE_WIDTH = 120.0f;					// 横幅
		static constexpr float GAUGE_HEIGHT = 120.0f;					// 縦幅
		static constexpr const char* GAUGE_FREAM = "iconfream000.png";	// フレームのテクスチャ
		static constexpr const char* GAUGE_BASE = "circle002.png";		// 基本のテクスチャ
		static constexpr const char* GAUGE_MULTI = "gauge_pre.png";		// マルチテクスチャ

	};

	// メンバ変数
	CVigilanceicon* m_pIcon;		// アイコンのポインタ
	CVigilancegauge* m_pGauge;		// ゲージのポインタ
	bool m_bUse;					// 使用するかどうか
};