//========================================================
//
// 外の監査役の処理 [ auditor.h ]
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
class CPlayer;
class CBillboard;
class CStateMachine;
class CAuditorStateBase;

//*********************************************************
// 外にいる監査役のキャラクタークラスを定義
//*********************************************************
class CAuditor : public CMoveCharactor
{
public:

	//***************************
	// モーション列挙型
	//***************************
	enum MOTION
	{
		NEUTRAL,	// ニュートラル
		MOVE,		// 移動
		DOUBT,		// 疑い
		CHASEDASH,	// 追いかけ
		CATCH,		// 捕まえる
		MAX
	};

	//***************************
	// 移動タイプの定義
	//***************************
	enum MOVE_POINTTYPE
	{
		OFFICENEAR,		// オフィス付近
		GAMECENTER,		// ゲーセン
		SOBAANDBAR,		// 蕎麦と居酒屋
		MAPLEFT,		// マップの左側
		BIGOUTSIDE,		// 大外回り
		MOVE_POINTTYPE_MAX
	};

	CAuditor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CAuditor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void MovingTypeOutSide(void);
	void ChaseMove(void);

	void DrawEyeSight(void);
	void ChangeState(CAuditorStateBase* pNewState, int nID);
	bool CheckObstacle(void);
	bool CheckRayToAngleRange(void);
	bool CheckEyesight(void);

	void StartChase(bool isStart) { m_isStartChase = isStart; }
	CBillboard* GetChaseIcon(void) const { return m_pChaseIcon; }
	CPlayer* GetPlayer(void) const { return m_pDestCharactor; }

	/// <summary>
	/// プレイヤーのポインタを設定する
	/// </summary>
	/// <param name="pPlayer">外部で生成されたポインタ</param>
	void SetPlayer(CPlayer* pPlayer = nullptr) { m_pDestCharactor = pPlayer; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CAuditor* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const MOVE_POINTTYPE& type);

private:
	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr float SPHERE_RANGE = 80.0f;		// 球形範囲
		static constexpr float BOX_RANGE = 50.0f;			// 矩形範囲
		static constexpr int DIVIDE = 16;					// メッシュの分割数
		static constexpr int COOL_TIME = 60;				// クールタイム
		static constexpr int COOL_TIME_DOUBLE = 120;		// 2倍のクールタイム
	};

	//***************************
	// 扇形の当たり判定
	//***************************
	struct Eyesight
	{
		static constexpr float EYE_RADIUS = 100.0f;		// 視界の届く距離
		static constexpr float EYE_ANGLE = 70.0f;		// 視野角
		static constexpr float EYE_HEIGHT = 50.0f;		// 視界の高さ制限
	};

private:

	void UpdateOffice(void);
	void UpdateSoba(void);
	void UpdateGameCenter(void);
	void UpdateMapLeft(void);
	void UpdateBigPoint(void);

private:
	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー
	MOVE_POINTTYPE m_MoveTypeData;						// 移動の種類

private:
	int m_nViewIdx;							// ビューポイントの巡回インデックス番号
	int m_nOfficeViewIdx;					// オフィス内の巡回インデックス番号
	int m_nCoolTime;						// セットポジションについた時のクールタイム
	int m_nTargetIdx;						// ターゲットID
	int m_nStopTime;						// 停止しているカウント

private:
	bool m_isTargetChase;					// 追跡判定
	bool m_isStartChase;					// チェイス開始判定フラグ

private:
	CPlayer* m_pDestCharactor;				// 対象キャラクター
	CBillboard* m_pChaseIcon;				// 追跡アイコンのビルボード
	CStateMachine* m_pMachine;				// ステートマシン用ポインタ変数
};