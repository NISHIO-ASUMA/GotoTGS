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
	/// 配列の中のドアを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CAutoMaticDoor* GetAutoDoor(const int nIdx) { return m_pAutoDoors[nIdx]; }

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

	/// <summary>
	/// 開閉開始処理
	/// </summary>
	/// <param name=""></param>
	void StartOpen(const int& nIdx)
	{
		// 1つのグループにつき2枚のドアが登録
		int nTargetIdx1 = nIdx * 2;
		int nTargetIdx2 = nIdx * 2 + 1;

		// 配列の最大サイズを取得
		int nTotalDoors = GetAll();

		// 1枚目のドアの開閉処理
		if (nTargetIdx1 < nTotalDoors && m_pAutoDoors[nTargetIdx1] != nullptr)
		{
			m_pAutoDoors[nTargetIdx1]->OpenDoorFlag();
		}

		// 2枚目のドアの開閉処理
		if (nTargetIdx2 < nTotalDoors && m_pAutoDoors[nTargetIdx2] != nullptr)
		{
			m_pAutoDoors[nTargetIdx2]->OpenDoorFlag();
		}
	}

private:

	CAutoMaticDoorManager();

private:

	std::vector<CAutoMaticDoor*>m_pAutoDoors;			 // 配置する動的配列
};