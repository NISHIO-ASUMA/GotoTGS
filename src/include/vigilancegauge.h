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
	struct Vigilancegauge
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		D3DXVECTOR2 tex;
		float fWidth;
		float fHeight;
		bool bUse;
	};

	CVigilancegauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilancegauge();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CVigilancegauge* Create(const Vigilancegauge& vigilancegauge);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr const char* TEXNAME_BASE = "vigilancegauge000.png";	// 基本のテクスチャ
		static constexpr const char* TEXNAME_MULTI = nullptr;					// マルチテクスチャ
	};

	bool m_bUse;	// 使用するかどうか
};