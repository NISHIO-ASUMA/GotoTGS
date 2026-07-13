//=========================================================
//
// ゲージの指針の処理 [ gaugeneedle.h ]
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
#include "object2Drotation.h"

//*********************************************************
// ゲージの指針クラスを定義
//*********************************************************
class CGaugeneedle :public CObject2DRotation
{
public:

	CGaugeneedle(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CGaugeneedle();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void AddTask(void) { m_nTask++; }	// タスクの数加算
	void AddAFK(void) { m_nAFK++; }		// さぼりの数加算

	// 情報取得処理
	inline int GetTaskCount(void) const { return m_nTask; }
	inline int GetAFKCount(void) const { return m_nAFK; }

	// 生成処理
	static CGaugeneedle* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight);

private:
	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float PIVOT_X = 0.5f;	// X軸の回転基準点
		static constexpr float PIVOT_Y = 0.9f;	// Y軸の回転基準点
	};

	// メンバ変数
	int m_nTask;							// タスクをこなした数
	int m_nAFK;								// さぼりをこなした数
};