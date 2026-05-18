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
		TYPE_PC,
		TYPE_COPY,
		TYPE_MAX
	};

	~CWorldUICollision();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// 情報取得処理
	const CSphereCollider* SphereColliderUI(const int& nIdx) {return m_SphereColliderUI[nIdx].get();}
	const TYPE& GetType(void) const { return m_nType; }

	// 生成処理
	static CWorldUICollision* Create(const D3DXVECTOR3& pos, const float& fRadius,const TYPE& type);

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CWorldUICollision* GetInstance(void)
	{
		static CWorldUICollision Instance;
		return &Instance;
	};

private:

	// 格納コンストラクタ
	CWorldUICollision();
	void SetActionTask(const TYPE& type);

private:

	// メンバ変数
	TYPE m_nType;														// 種類
	std::vector<std::unique_ptr<CSphereCollider>> m_SphereColliderUI;	// コライダーを格納した動的配列
};