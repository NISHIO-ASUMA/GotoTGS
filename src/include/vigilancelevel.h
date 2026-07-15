//=========================================================
//
// 警戒度のレベル処理 [ vigilancelevel.h ]
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
#include "object2D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;

//*********************************************************
// 警戒度レベルに対するクラスを定義
//*********************************************************
class CVigilancelevel :public CObject2D
{
public:

	// 警戒度レベルの構造体
	struct Level
	{
		D3DXVECTOR3 pos;	// 位置
		float fWidth;		// 横幅
		float fHeight;		// 縦幅
	};

	CVigilancelevel(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilancelevel();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void SetLevel(const int& nLevel);	// 数値設定

	// 生成処理
	static CVigilancelevel* Create(const Level& level);

private:

	//*************************
	// 定数構造体
	//*************************
	struct Config
	{
		static constexpr int NUM_SCORE = 2;							// 桁数
		static constexpr int NUM_DIGIT = 10;						// 表示する分割桁数の値
		static constexpr float DIGIT_VALUE = 2.0f;					// 加算量
		static constexpr float INTERVAL_VALUE = 10.0f;				// レベルと数値の間隔
		static constexpr const char* TEX_NUMBER = "number012.png";	// ナンバーのテクスチャ名
		static constexpr const char* TEX_LEVEL = "lebel_red.png";	// レベルのテクスチャ名
	};

	// メンバ変数
	Level m_Level;

	// 静的メンバ変数
	// 配列ナンバーのポインタ
	static CNumber* m_apNumber[Config::NUM_SCORE];

};