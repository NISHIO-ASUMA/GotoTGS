//=========================================================
//
// 進捗ゲージの処理 [ progressgauge.h ]
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
class CGaugeneedle;

//*********************************************************
// 進捗ゲージクラスを定義
//*********************************************************
class CProgressgauge:public CObject2D
{
public:

	CProgressgauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CProgressgauge();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 情報取得処理
	CGaugeneedle* GetGaugeneedle(void) const { return m_gaugeneedle; }

	// 生成処理
	static CProgressgauge* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float MAX_ANGLE = 0.41f;		// 傾く角度の最大値
		static constexpr float NEEDLE_WIDTH = 20.0f;	// 横幅
		static constexpr float NEEDLE_HEIGHT = 100.0f;	// 縦幅
	};

	// メンバ変数
	int m_nTask;					// タスクをこなした数
	int m_nAFK;						// さぼりをこなした数
	CGaugeneedle* m_gaugeneedle;	// 指針のポインタ
};