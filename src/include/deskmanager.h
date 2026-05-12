//=========================================================
//
// デスク管理処理 [ deskmanager.h ]
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
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "desk.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// デスク一括管理クラスを定義
//*********************************************************
class CDeskManager
{
public:

	CDeskManager();
	~CDeskManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	HRESULT Load(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pDesks.size()); }

	/// <summary>
	/// 配列の中のブロックを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CDesk* GetBlock(const int nIdx) { return m_pDesks[nIdx]; }

	/// <summary>
	/// ブロック生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">生成ファイル名</param>
	/// <returns></returns>
	CDesk* CreateManager
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:

	static constexpr const char* FILE_NAME = "data/JSON/Map.json";	// ファイル名
	std::vector<CDesk*>m_pDesks;									// 配置するデスクの動的配列
};