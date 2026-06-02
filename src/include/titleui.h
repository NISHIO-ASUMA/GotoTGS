//=========================================================
//
// タイトルに出現するUIの処理 [ titleui.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// タイトルのUIクラスを定義
//*********************************************************
class CTitleUi : public CObject2D
{
public:

	CTitleUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTitleUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetIsUse(const bool& set) { m_isUseFlag = set; }

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <returns></returns>
	static CTitleUi* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth,
		const float& fHeight,
		const char* Filename,
		const bool& isUseSet
	);

private:
	bool m_isUseFlag;			// 使用フラグ
};