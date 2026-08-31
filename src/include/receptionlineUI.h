//=========================================================
//
// 受付人のセリフUI処理 [ receptionlineUI.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "tutorialui.h"

//*********************************************************
// 受付人のセリフUIのクラスを定義
//*********************************************************
class CReceptionlineUI : public CTutorialUI
{
public:

	CReceptionlineUI(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CReceptionlineUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="pTexName">テクスチャファイル名</param>
	/// <returns></returns>
	static CReceptionlineUI* Create
	(
		const D3DXVECTOR3& pos,
		const char* pTexName
	);

private:

	// 現在のカウント
	int m_nCount;
};

