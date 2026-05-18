//=========================================================
//
// マップに配置するUIの球形判定処理 [ worldUIcollision.h ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// マップに配置するUIの球形判定クラスを定義
//*********************************************************
class CWorldUICollision
{
public:

	//**********************
	// UIの種類
	//**********************
	enum TYPE
	{
		TYPE_PC = 0,
		TYPE_COPY,
		TYPE_MAX
	};

	CWorldUICollision();
	~CWorldUICollision();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// 情報取得処理
	const CSphereCollider* SphereColliderUI(const int& nIdx) {return m_SphereColliderUI[nIdx].get();}

	// 生成処理
	static CWorldUICollision* Create(const D3DXVECTOR3& pos, const float& fRadius,const TYPE& type);

private:

	// メンバ変数
	TYPE m_nType;														// 種類
	std::vector<std::unique_ptr<CSphereCollider>> m_SphereColliderUI;	// コライダーを格納した動的配列
};