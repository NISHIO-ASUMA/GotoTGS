//=========================================================
//
// インスタンシング用モーション処理 [ motioninstancing.h ]
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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "model.h"
#include "instancemotionmanager.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CInstanceModel;

//*********************************************************
// インスタンシングするモデルのモーションクラスを定義
//*********************************************************
class CMotionInstancing
{
public:

	//*******************************
	// 結果を保持する構造体
	//*******************************
	struct MOTION_CACHE_FRAME
	{
		std::vector<D3DXVECTOR3> pos; // 座標
		std::vector<D3DXVECTOR3> rot; // 角度
	};

	//*******************************
	// 反映先の構造体情報
	//*******************************
	struct MOTION_CACHE
	{
		int motionType;				// モーションの種類
		int key;					// モーションキー情報
		int counter;				// フレームカウンター
		bool isBlend;				// ブレンドフラグ
		MOTION_CACHE_FRAME frame;	// 構造体変数
	};

	CMotionInstancing();
	~CMotionInstancing();

	void Update(std::vector<CInstanceModel*> pModel);
	void Debug(void);
	
	/// <summary>
	/// フレーム間計算
	/// </summary>
	/// <param name="nStartMotion">開始モーション</param>
	/// <param name="nEndMotion">終了カウント</param>
	/// <param name="nMotionType">モーションの種類インデックス</param>
	/// <returns></returns>
	bool CheckFrame(int nStartMotion, int nEndMotion, int nMotionType);

	void SetMotion(int motiontype);
	void SetMotion(int nMotionType, bool isBlend, int nBlendFrame);
	inline void SetResetFrame(int nFrame) { m_nCounterMotion = nFrame; }

	inline int GetMotionType(void) const { return m_motiontype; }
	inline bool GetFinishMotion(void) const { return m_isFinishMotion; }

	/// <summary>
	/// インデックス登録処理
	/// </summary>
	/// <param name="pMotionName">モーションスクリプト名</param>
	/// <param name="pModel">モデルクラスの動的配列</param>
	/// <param name="nDestMotions">モーション数</param>
	/// <param name="isShadow">マトリックスシャドウを使うかどうか</param>
	void RegisterPath(const char* pMotionName, std::vector<CInstanceModel*>& pModel, int nDestMotions, const bool isShadow);

	/// <summary>
	/// モーション読み込み関数
	/// </summary>
	/// <param name="pFilename">モーションスクリプト名</param>
	/// <param name="pModel">モデルクラスの動的配列</param>
	/// <param name="nDestMotions">モーション数</param>
	/// <param name="isShadow">マトリックスシャドウを使うかどうか</param>
	/// <returns></returns>
	static std::unique_ptr<CMotionInstancing>Load(const char* pFilename, std::vector<CInstanceModel*>& pModel, int nDestMotions, const bool isShadow = false);

private:

	static constexpr int NEUTRAL = 0; // ニュートラル番号

private:

	int m_nNumModels;		// モデル総数
	int m_nNumKey;			// キーの総数
	int m_nKey;				// 現在のキーNo
	int m_nCounterMotion;	// モーションのカウンター
	int m_nNextKey;			// 次のキー
	int m_motiontype;		// モーションタイプ
	int m_nMotionIdx;		// モーションファイル番号

	int m_motiontypeBlend;	// ブレンドの種類
	int m_nFrameBlend;		// ブレンドのフレーム数
	int m_nCounterBlend;	// ブレンドカウンター
	int m_nKeyBlend;		// ブレンドモーションの現在のキー
	int m_nNextKeyBlend;	// ブレンドモーションの次のキー
	int m_nNumAllFrame;		// キーごとのフレーム数
	int m_nAllFrameCount;	// 全体フレーム数

	bool m_isBlendMotion;	// ブレンドがあるかどうか
	bool m_isFinishMotion;	// モーションが終わったかどうか
	bool m_isFirstMotion;	// モーションが始まったフラグ
	bool m_isLoopMotion;	// ループするかどうか

	MOTION_CACHE m_cache;	// 構造体変数
	bool m_cacheDirty;		// キャッシュフラグ

	void MathMotionFrame(const CInstanceMotionManager::MOTIONFILE& info); // フレーム計算関数
	void ApplyCache(std::vector<CInstanceModel*>& pModel, const CInstanceMotionManager::MOTIONFILE& info); // 計算結果適用関数
};