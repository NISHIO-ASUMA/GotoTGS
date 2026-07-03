//=========================================================
//
// 敵の複数管理クラス [ enemymanager.h ]
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
#include <memory>
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CEnemy;

//*********************************************************
// 敵の複数管理クラスを定義
//*********************************************************
class CEnemyManager
{
public:

	~CEnemyManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列の最大数を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const int GetAllEnemys(void) const { return m_pEnemys.size(); }

	/// <summary>
	/// 動的配列本体の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CEnemy*>& GetEnemyData(void) const { return m_pEnemys; }

	/// <summary>
	/// インデックス指定で取得
	/// </summary>
	/// <param name="nIdx">取得する番号</param>
	/// <returns></returns>
	inline const CEnemy* GetEnemyIdx(const int& nIdx) { return m_pEnemys[nIdx]; }

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CEnemyManager* GetInstance(void)
	{
		static CEnemyManager Instance;
		return &Instance;
	};

	/// <summary>
	/// 管理クラス内で生成
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	CEnemy* CreateManager(const D3DXVECTOR3& pos,const D3DXVECTOR3& rot);

private:

	CEnemyManager();

	void LoadJson(void);

private:
	std::vector<CEnemy*>m_pEnemys; // 敵の管理配列

private:
	int m_nStageCount; // 敵の増加時間カウント
};