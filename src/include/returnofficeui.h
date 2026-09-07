//========================================================
//
// オフィスにもどれの警告表示ui [ returnofficeui.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// オフィスにもどれの警告表示uiクラスを定義
//*********************************************************
class CReturnOfficeUi : public CObject2D
{
public:

	CReturnOfficeUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CReturnOfficeUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 設定処理
	void SetUse(const bool& bUse) { m_bUse = bUse; }		

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="nFlashFrame">点滅間隔</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <param name="isUseFlash">点滅するか</param>
	/// <param name="isAlphaEnable">透明度処理を有効化するか</param>
	/// <param name="nAlphaFrame">透明化のフレーム</param>
	/// <param name="isAlphaTest">αテストするか</param>
	/// <param name="bUse">使っているかどうか</param>
	/// <returns></returns>
	static CReturnOfficeUi* Create
	(
		const D3DXVECTOR3& pos,
		const int& nFlashFrame,
		const float& fWidth,
		const float& fHeight,
		const char* Filename,
		bool bUse = true
	);

private:
	int m_nFlashFrame;				// 点滅間隔
	int m_nAlphaFrame;				// フェード全体のフレーム数
	int m_nAlphaCnt;				// 現在のフレーム
	bool m_bUse;					// 表示するかどうか Misaki
};