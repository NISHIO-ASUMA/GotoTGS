//=========================================================
//
// 外タスクのタイマー処理 [ outsidetasktimer.h ]
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
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;

//*********************************************************
// 外タスクのタイマークラスを定義
//*********************************************************
class COutSideTaskTimer :public CObject
{
public:
	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 150.0f;			// 横幅の増加分
		static constexpr float VALUE_FLOAT = 2.0f;				// ずらす基準値
		static constexpr const char* TEXNAME = "number_k.png";	// テクスチャ名
		static constexpr int DIGIT_TIME = 2;					// 桁数
		static constexpr int DIVIDE = 10;						// 桁分割の値
		static constexpr int CARVETIME = 60;					// カウント上限
		static constexpr int NUMTIME = 60;						// 最大タイマー ( この最大値は仮で設定する )
	};

	COutSideTaskTimer(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~COutSideTaskTimer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static COutSideTaskTimer* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight);

private:

	int m_nMaxTime;		// 開始時の最大時間
	int m_nAllTime;		// 全体の時間
	int m_nDecTime;		// 経過時間
	int m_nCounter;		// カウンター
	float m_fWidth;		// 横幅
	float m_fHeight;	// 縦幅

	CNumber* m_pNumberMinutes[Config::DIGIT_TIME];		// 分数のポインタ
	D3DXVECTOR3 m_pos;									// 位置
};