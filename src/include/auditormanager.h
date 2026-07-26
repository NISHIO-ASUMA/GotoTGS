//========================================================
//
// 外の監査役複数管理処理 [ auditormanager.h ]
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
#include <memory>
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CAuditor;

//*********************************************************
// 外の監査役を複数管理するクラスを定義
//*********************************************************
class CAuditorManager
{
public:

	~CAuditorManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CAuditorManager* GetInstance(void)
	{
		static CAuditorManager Instance;
		return &Instance;
	}

public:

	/// <summary>
	/// managerで生成する
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	void CreateInManager(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	/// <summary>
	/// 配列のサイズ(最大の長さ)を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetAll(void) const { return static_cast<int>(m_pAuditor.size()); }

	/// <summary>
	/// 動的配列の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CAuditor*>& GetVAuditor(void) { return m_pAuditor; }

	/// <summary>
	/// 配列順に取得
	/// </summary>
	/// <param name="nIdx"></param>
	/// <returns></returns>
	CAuditor* GetIdxAuditor(const int nIdx) const { return m_pAuditor[nIdx]; }

private:

	CAuditorManager();


private:
	std::vector<CAuditor*> m_pAuditor;	// 外の監査役の配列
};