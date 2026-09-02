//=========================================================
//
// リザルトのキャラクター管理クラス [ resultcastmanager.h ]
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
class CResultCast;

//*********************************************************
// モブの複数管理クラスを定義
//*********************************************************
class CResultCastManager
{
public:

	~CResultCastManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	/// <summary>
	/// シングルトン取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CResultCastManager* GetInstance(void)
	{
		static CResultCastManager Instance;
		return &Instance;
	};


private:

	CResultCastManager();

private:
	std::vector<CResultCast*>m_pCasts; // キャストの管理配列
};