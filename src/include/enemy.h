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
class CPlayer;
class CBillboard;

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
		CHASEDASH,	// 追いかけモーション
		CATCH,		// 捕まえるモーション
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

	//***************************
	// 敵のレベル
	//***************************
	enum LEVEL_SYSTEM
	{
		LEVEL_SYSTEM_0,
		LEVEL_SYSTEM_1,
		LEVEL_SYSTEM_2,
		LEVEL_SYSTEM_3,
		LEVEL_SYSTEM_4,
		LEVEL_SYSTEM_5,
		LEVEL_SYSTEM_MAX
	};

public:

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void DrawEyeSight(void);
	void ChaseMoving(void);
	void SetTargetChaseFlag(const bool& targetflag) { m_isTargetChase = targetflag; }
	void StartChase(const bool isStartflag) { m_isStartChase = isStartflag; }
	bool CheckEyesight(void);
	bool CheckRayToAngleRange(void);
	bool CheckObstacle(void);
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
		static constexpr float SPHERE_RANGE = 50.0f;		// 球形範囲
		static constexpr float BOX_RANGE = 50.0f;			// 矩形範囲
		static constexpr float CATCH_RANGE = 6.0f;			// 捕まる範囲
		static constexpr int DIVIDE = 16;					// メッシュの分割数
		static constexpr int COOL_TIME = 60;				// クールタイム
		static constexpr int COOL_TIME_DOUBLE = 120;		// 2倍のクールタイム
	};

	//***************************
	// 扇形の当たり判定
	//***************************
	struct Eyesight
	{
		static constexpr float EYE_RADIUS = 60.0f;		// 視界の届く距離
		static constexpr float EYE_ANGLE = 70.0f;		// 視野角
		static constexpr float EYE_HEIGHT = 50.0f;		// 視界の高さ制限
		static constexpr float EYE_RANGE = 250.0f;		// 視界の長さ
	};

	//***************************
	// レベルによるパラメータ
	//***************************
	struct LevelConfig
	{
		static constexpr float MIN_LEVEL = 0.0f;
		static constexpr float MAX_LEVEL = 5.0f;

		// 速度の最小・最大値
		static constexpr float MIN_SPEED = 1.0f; 
		static constexpr float MAX_SPEED = 3.0f;

		// 視界の最小・最大角度
		static constexpr float MIN_EYE_ANGLE = 70.0f; 
		static constexpr float MAX_EYE_ANGLE = 110.0f;

		static constexpr int MAX_LEVEL_POINT = 10;			// 最大レベル10
		static constexpr float LEVELUP_NEED_POINT = 100.0f;	// 1レベル上がるのに必要なポイント数
	};

	/// <summary>
	/// 通常の敵の動き
	/// </summary>
	/// <param name=""></param>
	void UpdateMoveViewPoint(void);

	/// <summary>
	/// 煙草の回りを回る敵の動き
	/// </summary>
	/// <param name=""></param>
	void UpdateMovingSmoke(void);

	/// <summary>
	/// TV回りを巡回する敵の動き
	/// </summary>
	/// <param name=""></param>
	void UpdateMovingTV(void);

public:

	void DecrementStopTime(void) { if (m_nStopTime > 0) m_nStopTime--; }
	void OnSpottedPlayer(const D3DXVECTOR3& pos) { m_playerTargetPos = pos; }
	void SetTargetIdx(int idx) { m_nTargetIdx = idx; }
	void SetStopTime(int time) { m_nStopTime = time; }
	void SetMoveType(const MOVETYPE& Type) { m_MoveType = Type; }
	void SetCharactorPointer(CPlayer* pCharactor) { m_pDestCharactor = pCharactor; }

	void SetMoveSpeed(void);
	void SetEyeAngle(void);
	void AddLevel(const float fValue);
	void LevelDown(const float fValue = 0.1f);
	
	// 各種設定の個別と一括更新
	void UpdateLevelParameters(void);

	int  GetStopTime(void) const { return m_nStopTime; }
	int GetTargetIndex(void) const { return m_nTargetIdx; }
	int GetNowLevel(void) const { return m_nLevel; }
	bool GetStartChaseFlags(void) const { return m_isStartChase; }
	float GetLevel(void) const { return m_fLevelPoint; }

	D3DXVECTOR3 GetPlayerTargetPos(void) const { return m_playerTargetPos; }
	MOVETYPE GetMoveType(void) const { return m_MoveType; }

	CPlayer* GetInCharactor(void) const { return m_pDestCharactor; }
	CBillboard* GetChaseIcon(void) const { return m_pChaseIcon; }
	CSphereCollider* GetSphereCollider(void) override { return m_pSphereColiider.get(); }

private:

	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー

private:
	CBillboard* m_pChaseIcon;							// 追跡アイコンのビルボード
	CStateMachine* m_pMachine;							// ステートマシン用ポインタ変数
	CPlayer* m_pDestCharactor;							// 判定先のキャラクターポインタ
	D3DXVECTOR3 m_playerTargetPos;						// プレイヤーの最新座標
	MOVETYPE m_MoveType;								// 動きの種類

private:
	bool m_isCheckPoint;								// ポイントに到着したかどうか
	bool m_isTargetChase;								// 追跡判定
	bool m_isStartChase;								// チェイス開始判定フラグ
	int m_nStopTime;									// 停止しているカウント
	int m_nTargetIdx;									// 向かう目的地のインデックス
	int m_nLevel;										// 現在のキャラクターのレベル
	float m_fLevelPoint;								// 警戒度のレベル値 ( これが変数 )
	float m_fMoveSpeed;									// 移動速度
	float m_fEyeAngle;									// 視界の角度の値
};