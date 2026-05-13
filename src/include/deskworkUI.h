//=========================================================
//
// タスクUI処理 [ deskworkUI.h ]
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
#include "manager.h"
#include "object.h"

//*********************************************************
// タスクUIクラスを定義
//*********************************************************
class CDeskworkUI:public CObject
{
public:

	//****************************
	// キータイプ
	//****************************
	enum KEYTYPE
	{		
		DRAWTYPE_A,
		DRAWTYPE_S,
		DRAWTYPE_W,
		DRAWTYPE_D,
		DRAWTYPE_MAX,
	};

	CDeskworkUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CDeskworkUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetTexture(const char* pTexName);												// テクスチャ設定
	void SetSize(const float fWidth, const float fHeight);								// サイズ設定
	void SetDigit(const KEYTYPE nDigit);												// UV設定
	void ChangeCol(const D3DXCOLOR& col);												// カラー設定
	void SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col);	// 点滅処理

	inline void SetKyeType(const KEYTYPE& kyetype) { m_Kyetype = kyetype; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }
	inline void SetIdx(const int& nIdx) { m_nIdx = nIdx; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetCol(const D3DXCOLOR& col) { m_col = col; }

	// 情報取得処理
	inline KEYTYPE GetKyeType(void) const { return m_Kyetype; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }
	inline int GetIdx(void) const { return m_nIdx; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }

	// 生成処理
	static CDeskworkUI* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight, const KEYTYPE& kyetype, const int& nIdx);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* TEXNAME = "deskwork_UI.png";	// テクスチャ名
		static constexpr float DIGIT_VALUE = 0.25f;					// テクスチャ分割数割合
		static constexpr float END_FLOAT = 1.0f;					// 補完割合値
		static constexpr float RATIO = 2.0f;						// 補完率
	};

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXVECTOR3 m_pos;						// 座標
	D3DXCOLOR m_col;						// 色情報
	float m_fWidth, m_fHeight;				// 横幅、縦幅
	float m_TexU, m_TexU1, m_TexV;			// テクスチャ座標
	KEYTYPE m_Kyetype;						// キーの種類
	int m_nIdx;								// 番号
	int m_nIdxTexture;						// テクスチャの番号
	int n_nColorCount;						// 色変更カウント
};