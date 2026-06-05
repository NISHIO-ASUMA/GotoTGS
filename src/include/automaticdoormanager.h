//=========================================================
//
// 自動ドア管理処理 [ automaticdoormanager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "automatic_door.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// 自動ドア管理クラスを定義
//*********************************************************
class CAutoMaticDoorManager
{
public:

	~CAutoMaticDoorManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pAutoDoors.size()); }

	/// <summary>
	/// 配列の中のブロックを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CAutoMaticDoor* GetAutoDoor(const int nIdx) { return m_pAutoDoors[nIdx]; }

	/// <summary>
	/// 球の当たり判定を返す
	/// </summary>
	/// <param name="pOther">判定先のコライダー</param>
	/// <returns></returns>
	bool CollisionSphere(CSphereCollider* pOther);

public:

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CAutoMaticDoorManager* GetInstance(void)
	{
		static CAutoMaticDoorManager Instance;
		return &Instance;
	}

private:

	CAutoMaticDoorManager();

private:

	std::vector<CAutoMaticDoor*>m_pAutoDoors;			 // 配置する動的配列
	std::unique_ptr<CSphereCollider>m_pSphereCollider;	 // 判定用の球コライダー
};