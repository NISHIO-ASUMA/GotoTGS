//=========================================================
//
// マップに配置するUIの球形判定処理 [ worldUIcollision.h ]
// Author: Takahashi Misaki
//
// NOTE : 2026/05/18 20:30
//		  西尾 : 関連処理の雛形を作っておきました
//		  タスクの追加を外部でできるようにしてみた
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

	//***************************************
	// ADD : 西尾
	// インタラクトに必要なデータ集合構造体
	//***************************************
	struct InteractData
	{
		D3DXVECTOR3 pos;							// 座標
		float fRadius;								// 半径
		int nType;									// 種類
		std::unique_ptr<CSphereCollider> pCollider; // 球形のコライダー
	};

	//**********************************
	// UIの種類
	//**********************************
	enum TYPE
	{
		TYPE_NONE = -1,		// 作業をしていない状態 Misaki
		TYPE_PC,			// デスク作業
		TYPE_COPY,			// コピー機作業
		TYPE_DOCUMENT,		// 書類作業
		TYPE_OUTSIDE,		// 外出作業
		TYPE_MAX
	};

public:
	
	~CWorldUICollision();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// 情報取得処理
	const TYPE& GetType(void) const { return m_nType; }

	//**********************
	// ADD : 西尾

	/// <summary>
	/// タスク追加の窓口関数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	void AddInteractEvent(const D3DXVECTOR3& pos, const float& fRadius, const TYPE& type);

	/// <summary>
	/// 取得関数
	/// </summary>
	const std::vector<std::unique_ptr<InteractData>>& GetInteractPoints(void) const { return m_pInteractPoints; }

	//**********************

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
	void LoadJson(void);

private:

	// メンバ変数
	TYPE m_nType;														// 種類
	std::vector<std::unique_ptr<InteractData>> m_pInteractPoints;		// コライダーを格納した動的配列
};