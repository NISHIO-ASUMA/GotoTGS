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
class CPointObject;
class COutsideUI;
class CMeshCylinder;

//*********************************************************
// クライアント役を複数管理するクラスを定義
//*********************************************************
class CClientManager
{
public:

	//***********************
	// 人数の管理列挙型
	//***********************
	enum CLIENT
	{// 人数
		CLIENT_1 = 0,	// 1人目
		CLIENT_2,		// 2人目
		CLIENT_3,		// 3人目
		CLIENT_MAX		// 人数の総数
	};

	~CClientManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// 設定処理
	void SetClient(void);
	void ClearClient(void);
	void ActiveChangeSystem(void);

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

	// 情報取得処理
	CPointObject* GetPointObject(const int nIdx) { return m_pPointObject[nIdx]; }
	COutsideUI* GetOutsideUI(const int nIdx) { return m_pOutsideUI[nIdx]; }

private:
	CClientManager();

private:

	// 位置情報
	const D3DXVECTOR3 m_pos[CLIENT_MAX] =
	{
		{ 1280.0f, 0.0f, -860.0f },	// 蕎麦屋(一番近い) [ 0 ]
		{ 2120.0f, 0.0f, -920.0f },	// 2番目に遠い		[ 1	]
		{ 2160.0f, 0.0f, 1100.0f }	// 一番遠い(左側)	[ 2	]
	};

	// 向き情報
	const D3DXVECTOR3 m_rot[CLIENT_MAX] =
	{
		{ 0.0f,D3DX_PI,0.0f },
		{ 0.0f,D3DX_PI * 0.5f,0.0f },
		{ 0.0f,D3DX_PI * 0.5f,0.0f }
	};

private:
	// 外のクライアント役の配列
	std::vector<CClient*> m_pClient;

private:
	CPointObject* m_pPointObject[CLIENT_MAX];	// 矢印のポインタ
	COutsideUI* m_pOutsideUI[CLIENT_MAX];		// UIのポインタ
	CMeshCylinder* m_pMeshCylinder[CLIENT_MAX]; // 円柱の配列

private:
	int m_nNowIndex;							// ランダムで出された番号
};