//=========================================================
//
// ブロック管理処理 [ blockmanager.h ]
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
// インクルードファイル
//*********************************************************
#include "block.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CPlayer;
class CMoveCharactor;

//*********************************************************
// ブロック一括管理クラスを定義
//*********************************************************
class CBlockManager
{
public:

	CBlockManager();
	~CBlockManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	HRESULT Load(void);

	/// <summary>
	/// 読み込むファイル名を設定する
	/// </summary>
	/// <param name="pLoadName"></param>
	void SetLoadFileName(const char* pLoadName = "data/JSON/GameMap.json") { m_pLoadFileName = pLoadName; }

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pBlocks.size()); }

	/// <summary>
	/// 配列の中のブロックを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CBlock* GetBlock(const int nIdx) { return m_pBlocks[nIdx]; }

	/// <summary>
	/// ブロック生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">生成ファイル名</param>
	/// <returns></returns>
	CBlock* CreateManager
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

public:

	/// <summary>
	/// キャラクターのポインタを格納する
	/// </summary>
	/// <param name="pCharactor">外部ポインタ</param>
	/// <returns></returns>
	void SetCharactorPoint(CMoveCharactor* pCharactor = nullptr) { m_pCharactor = pCharactor; }

private:

	static constexpr const char* FILE_NAME = "data/JSON/GameMap.json";	// ファイル名

private:
	std::vector<CBlock*>m_pBlocks;										// 配置するブロックの動的配列
	CMoveCharactor* m_pCharactor;										// 動くキャラクター
	const char* m_pLoadFileName;										// 読み込むファイル名
};