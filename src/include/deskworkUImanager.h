//=========================================================
//
// タスクUIマネージャー処理 [ deskworkUIManager.h ]
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
#include "deskworkUI.h"

//*********************************************************
// タスクUIマネージャークラスを定義
//*********************************************************
class CDeskworkUIManager
{
public:

	// タスクの種類
	enum DESKWORK
	{
		DESKWORK_NONE = 0,
		DESKWORK_PC,
		DESKWORK_COPY,
	};

	CDeskworkUIManager();
	virtual ~CDeskworkUIManager();

	virtual HRESULT Init(void);
	virtual void Uninit(void) = 0;
	virtual void Update(void);
	virtual void Draw(void) = 0;

	// 設定処理
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetUse(const bool& bUse) { m_bUse = bUse; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline bool GetUse(void) const { return m_bUse; }

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 座標
	bool m_bUse;		// 使用しているかどうか	
};

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
	void SetAlphaUI(void);

	// 情報取得処理
	CDeskworkUI* GetDeskUI(const int nIdx) const { return m_pDeskUI[nIdx]; }
	inline int GetNowIdx(void) const { return m_nNowIdx; }
	inline int GetNumUI(void) const { return Config::UI_NUM; }

	// 生成処理
	static CPCDeskwork* Create(const D3DXVECTOR3& pos);

private:

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
	int m_nNowIdx;								// 現在選択している番号
	int m_nCountTime;							// タイマーカウント
	bool m_bTime;								// クールタイムが始まっているかどうか

};