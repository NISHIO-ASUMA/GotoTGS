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
		MOVE_POINTTYPE_MAX
	};

	CAuditor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CAuditor();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void DrawEyeSight(void);
	void MovePointOutSide(const D3DXVECTOR3& pos);
	bool CheckEyesight(const D3DXVECTOR3& TargetPos);

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
		static constexpr float SPHERE_RANGE = 80.0f; // 球形範囲
		static constexpr float BOX_RANGE = 50.0f;	 // 矩形範囲
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

private:
	void MovingTypeOutSide(void);

	void UpdateOffice(void);
	void UpdateSoba(void);
	void UpdateGameCenter(void);
	void UpdateMapLeft(void);

private:
	std::unique_ptr<CBoxCollider> m_pBoxColiider;		// 矩形コライダー
	std::unique_ptr<CSphereCollider> m_pSphereColiider;	// 球形コライダー
	MOVE_POINTTYPE m_MoveTypeData;						// 移動の種類

private:
	int m_nViewIdx;							// ビューポイントの巡回インデックス番号
	int m_nOfficeViewIdx;					// オフィス内の巡回インデックス番号
	int m_nCoolTime;						// セットポジションについた時のクールタイム
	int m_nTargetIdx;						// ターゲットID
};