//=========================================================
//
// 進捗ゲージの処理 [ progressgauge.h ]
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
#include "object.h"

//*********************************************************
// 進捗ゲージクラスを定義
//*********************************************************
class CProgressgauge:public CObject
{
public:

	CProgressgauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CProgressgauge();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetTexture(const char* pTexName);												// テクスチャ設定
	void SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col);	// 点滅処理
	void AddTask(void);																	// タスクの数
	void AddAFK(void);																	// さぼりの数
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetOffsetPos(const D3DXVECTOR3& pos) { m_Offsetpos = pos; }
	inline void SetLeftCol(const D3DXCOLOR& col) { m_Leftcol = col; }
	inline void SetRightCol(const D3DXCOLOR& col) { m_Rightcol = col; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetOffsetPos(void) const { return m_Offsetpos; }
	inline D3DXCOLOR SetLeftCol(void) const { return m_Leftcol; }
	inline D3DXCOLOR SetRightCol(void) const { return m_Rightcol; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }
	inline int GetTaskCount(void) const { return m_nTask; }
	inline int GetAFKCount(void) const { return m_nAFK; }

	// 生成処理
	static CProgressgauge* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float LIFE_RATIO = 0.7f;		// 体力の初期割合
		static constexpr float GAUGE_RATIO = 0.005f;	// ゲージの増減割合
	};

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_Offsetpos;				// 初期位置
	D3DXCOLOR m_Leftcol;					// 左ゲージの色
	D3DXCOLOR m_Rightcol;					// 右ゲージの色
	float m_fWidth;							// 横幅
	float m_fHeight;						// 縦幅
	int m_nTask;							// タスクをこなした数
	int m_nAFK;								// さぼりをこなした数
	int m_nIdxTexture;						// テクスチャの番号
};