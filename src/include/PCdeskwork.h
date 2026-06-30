//=========================================================
//
// PCタスク処理 [ PCdeskwork.h ]
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
// PCタスククラスを定義
//*********************************************************
class CPCDeskwork :public CDeskworkUIManager
{
public:

	CPCDeskwork();
	~CPCDeskwork();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void SetAlphaUI(const bool& bUse = false);

	// 情報取得処理
	CDeskworkUI* GetDeskUI(const int nIdx) const { return m_pDeskUI[nIdx]; }
	inline int GetNowIdx(void) const { return m_nNowIdx; }
	inline int GetNumUI(void) const { return Config::UI_NUM; }

	// 生成処理
	static CPCDeskwork* Create(const D3DXVECTOR3& pos);

private:

	// メンバ関数
	bool CoolTime(const auto& pClear);	// クールタイム中の処理
	void Task(const auto& pClear);		// タスク中の処理
	bool ControlResult(void);			// コントローラーを押した結果の処理

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr float VALUE_WIDTH = 200.0f;	// 横幅の増加分
		static constexpr float VALUE_TEXU = 0.25f;		// テクスチャのUV座標
		static constexpr int UI_NUM = 3;				// UIの数
		static constexpr float UI_WIDTH = 100.0f;		// UIの横幅
		static constexpr float UI_HEIGHT = 100.0f;		// UIの縦幅
		static constexpr int TIME_COOL = 60;			// クールタイム
	};

	// メンバ変数
	CDeskworkUI* m_pDeskUI[Config::UI_NUM];		// UIのポインタ
	D3DXVECTOR3 m_Offsetpos;					// 元の位置
	D3DXVECTOR3 m_BasePos;						// イージングで移動する基準の座標の入れ物
	int m_nNowIdx;								// 現在選択している番号
	bool m_bFalse;								// 失敗したかどうか
};