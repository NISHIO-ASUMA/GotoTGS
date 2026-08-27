//=========================================================
//
// キー入力で開く両開きドアの管理クラス [ sideopendoormanager.h ]
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
#include "sideopendoor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// キー入力で開くドアの管理クラスを定義
//*********************************************************
class CSideOpenDoorManager
{
public:

	~CSideOpenDoorManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pSideOpenDoors.size()); }

	/// <summary>
	/// 配列の中のドアを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CSideOpenDoor* GetSideOpenDoor(const int nIdx) { return m_pSideOpenDoors[nIdx]; }

public:

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CSideOpenDoorManager* GetInstance(void)
	{
		static CSideOpenDoorManager Instance;
		return &Instance;
	}

	void OpenSideDoor(const std::vector<int>& targetIndices); // 命令飛ばし
	void CloseDoorInOffice(void);

private:

	CSideOpenDoorManager();

private:

	std::vector<CSideOpenDoor*>m_pSideOpenDoors;  // 配置する動的配列
};