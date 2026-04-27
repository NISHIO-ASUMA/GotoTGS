//=========================================================
//
// ゲームのタイマー処理 [ gametime.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;

//*********************************************************
// タイマークラスを定義
//*********************************************************
class CGametime :public CObject
{
public:

	CGametime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CGametime();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 各種設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	// 情報取得処理
	inline int GetAllTime(void) const { return m_nAllTime; }
	inline int GetNowTime(void) const { return m_nSeconds; }

	// 生成処理
	static CGametime* Create(const D3DXVECTOR3& pos,float fWidth, float fHeight);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 150.0f;			// 横幅の増加分
		static constexpr float VALUE_FLOAT = 2.0f;				// ずらす基準値
		static constexpr const char* TEXNAME = "time.png";		// テクスチャ名
		static constexpr int DIGIT_TIME = 2;					// 桁数
		static constexpr int DIVIDE = 10;						// 桁分割の値
		static constexpr int CARVETIME = 60;					// カウント上限
#ifdef _DEBUG
		static constexpr int NUMTIME = 600;						// 最大タイマー
#else
		static constexpr int NUMTIME = 150;						// 最大タイマー
#endif // _DEBUG
	};

	// メンバ関数
	void Seconds(void);
	void Minutes(void);

	// メンバ変数
	int m_nAllTime;		// 全体の時間
	int m_nSeconds;		// 現在の時間（秒数）
	int m_nMinutes;		// 現在の時間（分数）
	int m_nDecTime;		// 経過時間
	int m_nCounter;		// カウンター
	float m_fWidth;		// 横幅
	float m_fHeight;	// 縦幅

	CNumber* m_pNumberMinutes[Config::DIGIT_TIME];		// 分数のポインタ
	CNumber* m_pNumberSeconds[Config::DIGIT_TIME];		// 秒数のポインタ
	D3DXVECTOR3 m_pos;									// 位置
};