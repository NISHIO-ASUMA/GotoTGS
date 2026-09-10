//========================================================
//
// 動くUI処理 [ moveui.h ]
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
// 動きがあるUIクラスの定義
//*********************************************************
class CMoveUi : public CObject2D
{
public:

	CMoveUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CMoveUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetIsUse(bool isUseFlag) { m_isUse = isUseFlag; }

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="nFlashFrame">点滅間隔</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <returns></returns>
	static CMoveUi* Create
	(
		const D3DXVECTOR3& pos,
		const int& nFlashFrame,
		const float& fWidth,
		const float& fHeight,
		const char* Filename
	);

private:
	bool m_isUse;					// 表示するかどうか
};