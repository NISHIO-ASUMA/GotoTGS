//=========================================================
//
// AfkManager処理 [ afkmanager.h ]
// Author: Shouya Chikada
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
class CAfksmoke;
class CAfkTV;
class CAfkMagazine;

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CAfkManager
{
public:

	CAfkManager();
	~CAfkManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// ゲッター
	CAfksmoke* GetAfkSmoke(void) { return m_pAfkSmoke; }
	CAfkTV* GetAfkTV(void) { return m_pAfkTV; }

	// インスタンス生成用関数
	static CAfkManager* Instance(void);

private:
	static CAfkManager* m_pInstance;	// シングルトン変数

	CAfksmoke* m_pAfkSmoke;				// たばこさぼりのポインタ
	CAfkTV* m_pAfkTV;					// テレビさぼりのポインタ
	CAfkMagazine* m_pAfkMagazine;		// 漫画さぼりのポインタ

};

