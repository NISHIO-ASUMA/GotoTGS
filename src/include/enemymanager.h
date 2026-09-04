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
#include "enemy.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CGametime;
class CPlayer;

//*********************************************************
// 敵の複数管理クラスを定義
//*********************************************************
class CEnemyManager
{
public:

	~CEnemyManager();

	HRESULT Init(CPlayer* pCharactor = nullptr);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 全体のレベルアップ処理
	/// </summary>
	/// <param name=""></param>
	void SetLevleUpper(void);

	/// <summary>
	/// タイマークラスのポインタを受け取る
	/// </summary>
	/// <param name="pTime"></param>
	void SetTimeContainer(CGametime* pTime = nullptr) { m_pTimeContainer = pTime; }

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
	inline CEnemy* GetEnemyIdx(const int& nIdx) { return m_pEnemys[nIdx]; }

	/// <summary>
	/// 敵の追加関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	void AddEnemy(const D3DXVECTOR3& pos);

	/// <summary>
	/// キャラクターのポインタをセットする
	/// </summary>
	/// <param name="pCharactor">判定したいキャラクターのポインタ</param>
	void SetCharactor(CPlayer* pCharactor) { m_pDestCharactorPointer = pCharactor; }

	/// <summary>
	/// 管理クラス内で生成
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	CEnemy* CreateManager(const D3DXVECTOR3& pos,const D3DXVECTOR3& rot,const CEnemy::MOVETYPE& MoveType);

	/// <summary>
	/// 格納されているキャラクターのポインタを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	CPlayer* GetCharactorPointer(void) const { return m_pDestCharactorPointer; }

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

private:

	CEnemyManager();

	void LoadJson(void);

private:
	std::vector<CEnemy*>m_pEnemys;		// 敵の管理配列
	CGametime* m_pTimeContainer;		// タイムクラスの入れ物
	CPlayer* m_pDestCharactorPointer;	// 動くキャラクターのクラスの入れ物

private:
	int m_nStageCount;		// 敵の増加時間カウント
	int m_nIntervalCount;	// 生成インターバルカウント
};