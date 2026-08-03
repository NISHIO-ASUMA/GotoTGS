//=========================================================
//
// 過労死シーンのオブジェクト管理 [ overworkresultobject.h ]
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

//*********************************************************
// 前方宣言
//*********************************************************
class CBlockManager;

//*********************************************************
// 過労死シーンで使うオブジェクトを管理するクラスを定義
//*********************************************************
class COverWorkResultObject
{
public:

	~COverWorkResultObject();

	HRESULT Init(void);
	void Uninit(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static COverWorkResultObject* GetInstance(void)
	{
		static COverWorkResultObject Instance;
		return &Instance;
	}

private:

	COverWorkResultObject();					// コンストラクタ

	std::unique_ptr<CBlockManager>m_pBlock;		// ステージマップ
};