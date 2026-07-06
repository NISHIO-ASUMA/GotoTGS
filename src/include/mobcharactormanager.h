//=========================================================
//
// モブキャラクター管理クラス [ mobcharactormanager.h ]
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
class CWalkFriend;

//*********************************************************
// モブの複数管理クラスを定義
//*********************************************************
class CMobCharactorManager
{
public:

	~CMobCharactorManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列の最大数を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const int GetAllMobs(void) const { return m_pWalkings.size(); }

	/// <summary>
	/// 動的配列本体の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CWalkFriend*>& GetMobCharactorData(void) const { return m_pWalkings; }

	/// <summary>
	/// インデックス指定で取得
	/// </summary>
	/// <param name="nIdx">取得する番号</param>
	/// <returns></returns>
	inline const CWalkFriend* GetMobIdx(const int& nIdx) { return m_pWalkings[nIdx]; }

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CMobCharactorManager* GetInstance(void)
	{
		static CMobCharactorManager Instance;
		return &Instance;
	};

	/// <summary>
	/// 管理クラス内で生成
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name= "pMotionName">モーションファイル</param>
	/// <returns></returns>
	CWalkFriend* CreateManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const char* pMotionName);

private:

	CMobCharactorManager();

	void LoadJson(void);

private:
	std::vector<CWalkFriend*>m_pWalkings; // モブの管理配列
};