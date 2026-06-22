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
	static CTutorialLinesBG* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	// 設定処理
	void SetUse(const bool& bUse) { m_bUse = bUse; }

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* TEXNAME = "Tutoriallines000.png";	// 背景のテクスチャ名

	};

	// メンバ変数
	bool m_bUse;
};