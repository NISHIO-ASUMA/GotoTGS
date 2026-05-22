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
		DRAWTYPE_NONE = -1,
		DRAWTYPE_A,
		DRAWTYPE_S,
		DRAWTYPE_W,
		DRAWTYPE_D,
		DRAWTYPE_MAX,
	};

	//****************************
	// 頂点ポイント
	//****************************
	enum VTXTYPE
	{
		VTXTYPE_CENTER,
		VTXTYPE_LEFT,
		VTXTYPE_RIGHT,
		VTXTYPE_MAX,
	};

	//****************************
	// 書類の種類
	//****************************
	enum DOCUMENT
	{
		DOCUMENT_RED,
		DOCUMENT_BLUE,
		DOCUMENT_YELLOW,
		DOCUMENT_MAX,
	};

	//****************************
	// UIの構造体
	//****************************
	struct UI
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;			// 色情報
		VTXTYPE VTXtype;
		float fWidth;
		float fHeight;
		float fDigit;
		int nKeytype;
		int nIdx;
	};

	CDeskworkUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CDeskworkUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetTexture(const char* pTexName);												// テクスチャ設定
	void SetSize(const float& fWidth, const float& fHeight);							// サイズ設定
	void SetDigit(const int& nType, const float& nDigit);								// UV設定
	void ChangeCol(const D3DXCOLOR& col);												// カラー設定
	void SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col);	// 点滅処理
	void SetVTX(const VTXTYPE& VTXtype);												// 頂点ポイント設定

	inline void SetPos(const D3DXVECTOR3& pos) { m_ui.pos = pos; }
	inline void SetCol(const D3DXCOLOR& col) { m_ui.col = col; }
	inline void SetWidth(const float& fWidth) { m_ui.fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_ui.fHeight = fHeight; }
	inline void SetDigit(const float& fDigit) { m_ui.fDigit = fDigit; }
	inline void SetKeyType(const int& keytype) { m_ui.nKeytype = keytype; }
	inline void SetIdx(const int& nIdx) { m_ui.nIdx = nIdx; }
	inline void SetVTXtype(const VTXTYPE& VTXtype) { m_ui.VTXtype = VTXtype; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_ui.pos; }
	inline D3DXCOLOR GetCol(void) const { return m_ui.col; }
	inline float GetWidth(void) const { return m_ui.fWidth; }
	inline float GetHeight(void) const { return m_ui.fHeight; }
	inline float GetDigit(void) const { return m_ui.fDigit; }
	inline int GetKeyType(void) const { return m_ui.nKeytype; }
	inline int GetIdx(void) const { return m_ui.nIdx; }

	// 生成処理
	static CDeskworkUI* Create(const UI& ui);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* TEXNAME_KEYTYPE = "deskwork_UI.png";	// キーのテクスチャ名
		static constexpr const char* TEXNAME_GAGE = "gage000.jpg";			// ゲージのテクスチャ名
		static constexpr float END_FLOAT = 1.0f;							// 補完割合値
		static constexpr float RATIO = 2.0f;								// 補完率
	};

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ

	UI m_ui;								// UIの情報
	float m_TexU, m_TexU1, m_TexV;			// テクスチャ座標
	int m_nIdxTexture;						// テクスチャの番号
	int n_nColorCount;						// 色変更カウント
};