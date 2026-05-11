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
#include "object2D.h"

//*********************************************************
// タスクUIクラスを定義
//*********************************************************
class CDeskworkUI :public CObject2D
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

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	inline void SetKyeType(const KEYTYPE& kyetype) { m_Kyetype = kyetype; }

	// 情報取得処理
	inline KEYTYPE GetKyeType(void) const { return m_Kyetype; }

	// 生成処理
	static CDeskworkUI* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight, const KEYTYPE& kyetype);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* TEXNAME = "deskwork_UI.jpg";	// テクスチャ名
		static constexpr float VALUE_TEXU = 0.25f;					// テクスチャのUV座標
	};

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 座標
	float m_fWIdth;		// 横幅
	float m_fHeight;	// 縦幅
	KEYTYPE m_Kyetype;	// キーの種類
};