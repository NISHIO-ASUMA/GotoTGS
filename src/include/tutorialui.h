//=========================================================
//
// チュートリアルUI処理 [ tutorialui.h ]
// Author: Shouya Chikada
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
#include "billboard.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CTutorialUI : public CBillboard
{
public:

	CTutorialUI(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CTutorialUI();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(CSphereCollider* pOther);

	inline CSphereCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成処理</param>
	/// <param name="rot">角度</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pTexName">テクスチャファイル名</param>
	/// <returns></returns>
	static CTutorialUI* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		float fWidth,
		float fHeight,
		const char* pTexName
	);

private:
	std::unique_ptr<CSphereCollider> m_pCollider;	// 球形のコライダー
	bool m_bLook;									// 見えるかどうかの判定変数
};

