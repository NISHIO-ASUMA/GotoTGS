//=========================================================
//
// 世界の壁を複数管理するクラス [ worldwallmanager.h ]
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
// 前方宣言
//*********************************************************
class CGameWallModel;

//*********************************************************
// 世界の壁一括管理クラスを定義
//*********************************************************
class CWorldWallManager
{
public:

	CWorldWallManager();
	~CWorldWallManager();

	HRESULT Init(const char * pLoadName);
	void Uninit(void);

	/// <summary>
	/// 配置されている壁の取得
	/// </summary>
	/// <param name="nIdx">取得する番号</param>
	/// <returns></returns>
	CGameWallModel* GetWorldWall(const int nIdx) const { return m_pWall[nIdx]; }

	/// <summary>
	/// 配列のサイズを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetSize(void) const { return static_cast<int>(m_pWall.size()); }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルファイル名</param>
	/// <returns></returns>
	CGameWallModel* CreateManager
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		const D3DXVECTOR3& scale, 
		const char* pModelName
	);

private:

	std::vector<CGameWallModel*>m_pWall;		// 確保する配列情報
};