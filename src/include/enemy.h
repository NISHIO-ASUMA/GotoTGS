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

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動モーション
		DOUBT,		// 疑いモーション
		MAX
	};

	//***************************
	// 移動する種類
	//***************************
	enum MOVETYPE
	{
		MOVETYPE_NORMAL,	// 通常状態
		MOVETYPE_SMOKE,		// 煙草回りの動き
		MOVETYPE_TV,		// TVと食事スペース辺りの動き
		MOVETYPE_MAX
	};

public:

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawEyeSight(void);
	bool CheckEyesight(void);
	bool Collision(CBoxCollider* pOther, D3DXVECTOR3* pOutPos);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CEnemy* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const MOVETYPE& Movetype);

	/// <summary>
	/// ステート変更関数
	/// </summary>
	/// <param name="pState">起こしたい状態(クラス)</param>
	/// <param name="nID">ステートのID</param>
	void ChangeState(CEnemyStateBase* pState, int nID);

public:

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 80.0f;		// 球形範囲
		static constexpr float BOX_RANGE = 50.0f;			// 矩形範囲
		static constexpr int DIVIDE = 16;					// メッシュの分割数
		static constexpr int COOL_TIME = 60;				// クールタイム
		static constexpr int COOL_TIME_DOUBLE = 120;		 // 2倍のクールタイム
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

	/// <summary>
	/// 通常の敵の動き( 巡回ポイントで動く )
	/// </summary>
	/// <param name=""></param>
	void UpdateMoveViewPoint(void);

	/// <summary>
	/// 煙草の回りを回る敵の動き ( 巡回ポイントで動く )
	/// </summary>
	/// <param name=""></param>
	void UpdateMovingSmoke(void);

	/// <summary>
	/// TV回りを巡回する敵の動き
	/// </summary>
	/// <param name=""></param>
	void UpdateMovingTV(void);

/// <summary>
/// 西尾追加 : 窓口の関数をまとめて格納しているpublic メソッド
/// </summary>
public:

	void DecrementStopTime(void) { if (m_nStopTime > 0) m_nStopTime--; }

	void OnSpottedPlayer(const D3DXVECTOR3& pos) { m_playerTargetPos = pos; }
	void SetTargetIdx(int idx) { m_nTargetIdx = idx; }
	void SetStopTime(int time) { m_nStopTime = time; }
	void SetMoveType(const MOVETYPE& Type) { m_MoveType = Type; }
	void SetCharactorPointer(CMoveCharactor* pCharactor) { m_pDestCharactor = pCharactor; }

	int  GetStopTime(void) const { return m_nStopTime; }
	int GetTargetIndex(void) const { return m_nTargetIdx; }
	MOVETYPE GetMoveType(void) const { return m_MoveType; }
	CMoveCharactor* GetInCharactor(void) const { return m_pDestCharactor; }

	D3DXVECTOR3 GetPlayerTargetPos(void) const { return m_playerTargetPos; }

private:

	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

	bool m_isCheckPoint;								// ポイントに到着したかどうか
	int m_nStopTime;									// 停止しているカウント
	int m_nTargetIdx;									// 向かう目的地のインデックス
	bool m_isTargetChase;								// 追跡

	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	CMoveCharactor* m_pDestCharactor;					// 判定先のキャラクターポインタ
	D3DXVECTOR3 m_playerTargetPos;						// プレイヤーの最新座標
	MOVETYPE m_MoveType;								// 動きの種類
};