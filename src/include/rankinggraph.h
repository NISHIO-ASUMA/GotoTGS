//=========================================================
//
// ランキングの横棒グラフ処理 [ rankingglaph.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <array>
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2d.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CLoad;

//*********************************************************
// ランキングの横棒グラフ処理クラス定義
//*********************************************************
class CRankingGraph : public CObject2D
{
public:

	CRankingGraph(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CRankingGraph();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CRankingGraph* Create
	(
		const D3DXVECTOR3& pos,
		float fWidth,
		float fHeight
	);

public:
	void SettingUi(const D3DXVECTOR3& pos, float fWidth, float fHeight);

private:

	//*************************************
	// 定数構造体宣言
	//*************************************
	struct Config
	{
		static constexpr int DATA_MAX = 5;			// 表示する横棒の数
		static constexpr float POS_Y_VALUE = 65.0f;	// ずらすY座標値
	};
};