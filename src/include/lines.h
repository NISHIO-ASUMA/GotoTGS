//=========================================================
//
// セリフ処理 [ lines.h ]
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
// セリフに対するクラスを定義
//*********************************************************
class CLines :public CObject2D
{
public:

	CLines(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CLines();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CLines* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	// 設定処理
	void SetUse(const bool& bUse) { m_bUse = bUse; }

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* SERIHU_001 = "Tutoriallines001.png";	// セリフ1

	};

	// メンバ変数
	bool m_bUse;					// 使用しているかどうか
};