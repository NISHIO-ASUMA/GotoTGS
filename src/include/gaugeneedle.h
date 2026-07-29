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

	// 角度正規化
	inline void NormalizAngle(float& fAngle);

	// 設定処理
	void AddTask(void);		// タスクの数加算
	void AddAFK(void);		// さぼりの数加算

	// 情報取得処理
	inline int GetTaskCount(void) const { return m_nTask; }
	inline int GetAFKCount(void) const { return m_nAFK; }
	inline bool GetIsFinish(void) const { return m_isFinish; }

	// 生成処理
	static CGaugeneedle* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight);

private:
	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float PIVOT_X = 0.5f;		// X軸の回転基準点
		static constexpr float PIVOT_Y = 0.9f;		// Y軸の回転基準点
		static constexpr float MAX_ANGLE = 0.43f;	// 傾く角度の最大値
		static constexpr float MOVE_ANGLE = 0.1f;	// 移動量
		static constexpr int MAX_FREAM = 60;		// 移動に掛かるフレーム数
	};

	// メンバ変数
	int m_nTask;		// タスクをこなした数
	int m_nAFK;			// さぼりをこなした数
	int m_nCount;		// 現在のカウント
	bool m_bStart;		// 動いているかどうか
	bool m_bDir;		// どの方向に進むか
	float m_fOldAngle;	// 元の角度
	bool m_isFinish;	// タスクの最大値判定
};