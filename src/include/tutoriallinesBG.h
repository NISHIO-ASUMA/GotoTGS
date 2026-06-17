//=========================================================
//
// チュートリアルのセリフ背景処理 [ tutoriallinesBG.h ]
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
// チュートリアルのセリフ背景に対するクラスを定義
//*********************************************************
class CTutorialLinesBG :public CObject2D
{
public:

	CTutorialLinesBG(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTutorialLinesBG();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CTutorialLinesBG* Create(const D3DXVECTOR3& pos);

	// 情報取得処理

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float POS_X = HALFWIDTH;						// X軸の位置
		static constexpr float POS_Y = SCREEN_HEIGHT * 0.8f;			// Y軸の位置
		static constexpr float WIDTH = SCREEN_WIDTH;					// 横幅
		static constexpr float HEIGHT = 200.0f;							// 縦幅
		static constexpr const char* TEXNAME = "Tutoriallines000.png";	// 背景のテクスチャ名

	};

	// メンバ変数

};