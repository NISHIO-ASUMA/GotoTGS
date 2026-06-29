//=========================================================
//
// コピー機タスク処理 [ COPYdeskwork.h ]
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
#include "deskworkUImanager.h"

//*********************************************************
// コピー機タスククラスを定義
//*********************************************************
class CCOPYDeskwork :public CDeskworkUIManager
{
public:

	//****************************
	// テクスチャタイプ
	//****************************
	enum TEXTURE
	{
		TEXTURE_KEY,
		TEXTURE_GAGE,
		TEXTURE_MAX,
	};

	CCOPYDeskwork();
	~CCOPYDeskwork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void SetAlphaUI(const bool& bUse = false);

	// 情報取得処理
	CDeskworkUI* GetDeskUI(const int nIdx) const { return m_pDeskUI[nIdx]; }

	// 生成処理
	static CCOPYDeskwork* Create(const D3DXVECTOR3& pos);

private:
	// メンバ関数
	bool CoolTime(const auto& pClear);				// クールタイム中の処理
	void Task(const auto& pClear);					// タスク中の処理
	void ControlResult(int& nCount);		// コントローラーを押した結果の処理

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_X = 6.0f;		// X軸の位置の増加分
		static constexpr float VALUE_Y = 250.0f;	// Y軸の位置の増加分
		static constexpr float VALUE_TEXU = 0.25f;	// テクスチャのUV座標
		static constexpr int UI_NUM = TEXTURE_MAX;	// UIの数
		static constexpr float UI_WIDTH = 100.0f;	// UIの横幅
		static constexpr float UI_HEIGHT = 100.0f;	// UIの縦幅
		static constexpr float GAGE_WIDTH = 927.0f;	// ゲージの横幅
		static constexpr float GAGE_HEIGHT = 24.5f;	// ゲージの縦幅
		static constexpr int TIME_PUSH = 120;		// ボタンを押す時間
		static constexpr int TIME_COOL = 60;		// クールタイム
	};

	// メンバ変数
	CDeskworkUI* m_pDeskUI[Config::UI_NUM];		// UIのポインタ
};