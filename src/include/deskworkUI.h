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
		KEYTYPE_BOARD = 0,
		KEYTYPE_PAD,
		KEYTYPE_MAX,
	};

	//****************************
	// キーボードのキータイプ
	//****************************
	enum KEYBOARD
	{		
		KEYBOARD_NONE = -1,
		KEYBOARD_A,
		KEYBOARD_S,
		KEYBOARD_W,
		KEYBOARD_D,
		KEYBOARD_MAX,
	};

	//****************************
	// コントローラーのキータイプ
	//****************************
	enum KYAPAD
	{
		KEYPAD_NONE = -1,
		KEYPAD_A,
		KEYPAD_B,
		KEYPAD_X,
		KEYPAD_Y,
		KYAPAD_MAX,
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
		D3DXVECTOR3 pos;		// 位置
		D3DXCOLOR col;			// 色情報
		VTXTYPE VTXtype;		// 頂点座標のタイプ
		float fWidth;			// 横幅
		float fHeight;			// 縦幅
		float fDigit;			// テクスチャの分割数
		int nKeytype;			// キータイプ
		int nKey;				// 現在のキー
		int nIdx;				// 番号
	};

	CDeskworkUI(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CDeskworkUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void ChangeCol(const D3DXCOLOR& col);										// カラー設定
	void SetVTX(VERTEX_2D* pVtx);												// 頂点ポイント設定
	void SetSize(const float& fWidth, const float& fHeight, VERTEX_2D* pVtx);	// サイズ設定
	void SetDigit(VERTEX_2D* pVtx);												// UV設定
	void SetTexture(const char* pTexName);										// テクスチャ設定

	inline void SetPos(const D3DXVECTOR3& pos) { m_UI.pos = pos; }
	inline void SetCol(const D3DXCOLOR& col) { m_UI.col = col; }
	inline void SetVTXtype(const VTXTYPE& VTXtype) { m_UI.VTXtype = VTXtype; }
	inline void SetWidth(const float& fWidth) { m_UI.fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_UI.fHeight = fHeight; }
	inline void SetDigit(const float& fDigit) { m_UI.fDigit = fDigit; }
	inline void SetKeyType(const int& keytype) { m_UI.nKeytype = keytype; }
	inline void SetKey(const int& key) { m_UI.nKey = key; }
	inline void SetIdx(const int& nIdx) { m_UI.nIdx = nIdx; }

	// 情報取得処理
	inline D3DXVECTOR3 GetPos(void) const { return m_UI.pos; }
	inline D3DXCOLOR GetCol(void) const { return m_UI.col; }
	inline float GetWidth(void) const { return m_UI.fWidth; }
	inline float GetHeight(void) const { return m_UI.fHeight; }
	inline float GetDigit(void) const { return m_UI.fDigit; }
	inline int GetKeyType(void) const { return m_UI.nKeytype; }
	inline int GetKey(void) const { return m_UI.nKey; }
	inline int GetIdx(void) const { return m_UI.nIdx; }

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
		static constexpr float TEX_V_VALUE = 0.5f;							// テクスチャの縦の補完値
		static constexpr float RATIO = 2.0f;								// 補完率
	};

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	UI m_UI;								// UIの情報
	float m_TexU, m_TexU1, m_TexV;			// テクスチャ座標
	int m_nIdxTexture;						// テクスチャの番号
	int n_nColorCount;						// 色変更カウント

};