//========================================================
//
// 敵の処理 [ enemy.h ]
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

//*********************************************************
// インクルードファイル
//*********************************************************
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;
class CBoxCollider;
class CStateMachine;
class CEnemyStateBase;

//*********************************************************
// 敵キャラクタークラスを定義
//*********************************************************
class CEnemy : public CMoveCharactor
{
public:

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawEyeSight(void);
	bool CheckEyesight(const D3DXVECTOR3& TargetPos);
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CEnemy* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	/// <summary>
	/// ステート変更関数
	/// </summary>
	/// <param name="pState">起こしたい状態(クラス)</param>
	/// <param name="nID">ステートのID</param>
	void ChangeState(CEnemyStateBase* pState, int nID);

public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動 
		SEARCH,		// 疑いモーション
		MAX
	};

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 80.0f; // 球形範囲
		static constexpr float BOX_RANGE = 60.0f;	 // 矩形範囲
		static constexpr int DIVIDE = 16;			 // メッシュの分割数
	};

	//***************************
	// 扇形の当たり判定
	//***************************
	struct Eyesight
	{
		static constexpr float EYE_RADIUS = 100.0f;		// 視界の届く距離
		static constexpr float EYE_ANGLE = 45.0f;		// 視野角
		static constexpr float EYE_HEIGHT = 50.0f;		// 視界の高さ制限
	};

	void SetTargetChaseFlag(const bool& targetflag) { m_isTargetChase = targetflag;}

	void UpdateMoveViewPoint(void);

/// <summary>
/// 西尾追加 : 窓口の関数をまとめて格納しているpublic メソッド
/// </summary>
public:

	void DecrementStopTime(void) { if (m_nStopTime > 0) m_nStopTime--; } // 減算関数

	void OnSpottedPlayer(const D3DXVECTOR3& pos) { m_playerTargetPos = pos; }
	void SetTargetIdx(int idx) { m_nTargetIdx = idx; }
	void SetStopTime(int time) { m_nStopTime = time; }

	int  GetStopTime(void) const { return m_nStopTime; }
	int GetTargetIndex(void) const { return m_nTargetIdx; }
	D3DXVECTOR3 GetPlayerTargetPos(void) const { return m_playerTargetPos; }

private:

	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

	bool m_isCheckPoint;								// ポイントに到着したかどうか
	int m_nStopTime;									// 停止しているカウント
	int m_nTargetIdx;									// 向かう目的地のインデックス
	bool m_isTargetChase;								// 追跡

	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	D3DXVECTOR3 m_playerTargetPos;						// プレイヤーの最新座標
};