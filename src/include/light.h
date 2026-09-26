//=========================================================
//
// ライト処理 [ light.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// ライトクラスを定義
//*********************************************************
class CLight
{
public:

	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetLight(void);

	// ディレクショナルライトの色（明るさ）を変更する窓口
	void ChangeLight(float fRatio);

	// 個別に RGBA を指定したい場合（オーバーロード）
	void ChangeLight(const D3DCOLORVALUE& color);

	void Reset(void);

private:
	static inline constexpr int NUMLIGHT = 4;		// 設置する数

	D3DLIGHT9 m_aLight[NUMLIGHT];   // ライト数
	D3DXVECTOR3 m_vecDir[NUMLIGHT];	// ベクトル
};
