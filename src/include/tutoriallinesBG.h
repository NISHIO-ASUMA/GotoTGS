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
	static CTutorialLinesBG* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const char* TEXNAME);

	// 設定処理
	void SetUse(const bool& bUse) { m_bUse = bUse; }

private:

	// メンバ変数
	bool m_bUse;
};