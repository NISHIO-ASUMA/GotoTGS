//=========================================================
//
// 警戒度のアイコン処理 [ vigilanceicon.h ]
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
// 警戒度アイコンに対するクラスを定義
//*********************************************************
class CVigilanceicon :public CObject2D
{
public:

	// 警戒度アイコンの構造体
	struct Icon
	{
		D3DXVECTOR3 pos;
		float fWidth;
		float fHeight;
	};

	CVigilanceicon(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CVigilanceicon();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 生成処理
	static CVigilanceicon* Create(const Icon& icon);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr const char* TEXNAME = "vigilanceicon.png";	// テクスチャの名前
	};
};