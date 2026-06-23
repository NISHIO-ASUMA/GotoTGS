//=========================================================
//
// 警戒度のゲージ処理 [ vigilancegauge.h ]
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
#include "animationobject2d.h"

//*********************************************************
//  警戒度のゲージに対するクラスを定義
//*********************************************************
class CVigilancegauge
{
public:

	// 警戒度のゲージの構造体
	struct Vigilancegauge
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		D3DXVECTOR2 tex;
		float fWidth;
		float fHeight;
		bool isLoop;
		bool bUse;
	};

	CVigilancegauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilancegauge();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 生成処理
	static CVigilancegauge* Create(const Vigilancegauge& vigilancegauge);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr int MAX_FREAM = 180;					// 最大フレーム数
		static constexpr const char* TEXNAME = "alert000.png";	// テクスチャ名
	};
};