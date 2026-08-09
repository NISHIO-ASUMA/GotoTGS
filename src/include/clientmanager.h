//========================================================
//
// クライアント役の複数管理処理 [ clientmanager.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルード
//*********************************************************
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CClient;

//*********************************************************
// クライアント役を複数管理するクラスを定義
//*********************************************************
class CClientManager
{
public:

	~CClientManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CClientManager* GetInstance(void)
	{
		static CClientManager Instance;
		return &Instance;
	}

public:

	/// <summary>
	/// 管理クラス内で生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	void CreateInManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	/// <summary>
	/// 配列のサイズを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetAll(void) const { return static_cast<int>(m_pClient.size()); }

	/// <summary>
	/// 動的配列の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CClient*>& GetVClient(void) { return m_pClient; }

	/// <summary>
	/// 配列順に取得
	/// </summary>
	/// <param name="nIdx"></param>
	/// <returns></returns>
	CClient* GetIdxClient(const int nIdx) const { return m_pClient[nIdx]; }

private:
	CClientManager();

private:
	std::vector<CClient*> m_pClient;	// 外のクライアント役の配列
};