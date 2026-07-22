//========================================================
//
// 社長処理 [ boss.cpp ]
// Author: Asuma Nishio
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "boss.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "spherecollider.h"
#include "template.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace BOSS_INFO
{
	constexpr int NUM_OUTSIDEPOINT = 6;

	// ローカルビューポイント
	const D3DXVECTOR3 LocalMovePoint[NUM_OUTSIDEPOINT] =
	{
		{677.5f,0.0f,313.0f},
		{663.0f,0.0f,115.0f},
		{420.5f,0.0f,115.0f},
		{400.0f,0.0f,190.0f},
		{134.0f,0.0f,190.0f},
		{130.0f,0.0f,310.0f},
	};
};

//*********************************************************
// 定数名前空間
//*********************************************************
namespace OFFICE_INFO
{
	constexpr int COOLTIME = 60;
	constexpr int NUM_OFFICEPOINT = 12;

	// ローカルビューポイント
	const D3DXVECTOR3 OfficeMovePoint[NUM_OFFICEPOINT] =
	{
		{-50.0f,0.0f,323.0f},
		{-83.0f,0.0f,240.0f},
		{-125.0f,0.0f,90.0f},
		{-227.0f,0.0f,69.0f},
		{-217.0f,0.0f,-140.0f},
		{85.0f,0.0f,-153.0f},
		{90.0f,0.0f,36.0f},
		{405.0f,0.0f,23.0f},
		{413.0f,0.0f,183.0f},
		{169.0f,0.0f,191.0f},
		{-83.0f,0.0f,240.0f},
		{-50.0f,0.0f,323.0f},
	};
};

//========================================================
// コンストラクタ
//========================================================
CBoss::CBoss(int nPriority) : CMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_nViewIdx(NULL),
m_nOfficeViewIdx(NULL),
m_nCoolTime(NULL),
m_isOutSideIn(true),
m_isOfficeMove(false)
{

}
//========================================================
// デストラクタ
//========================================================
CBoss::~CBoss()
{

}
//========================================================
// 生成処理
//========================================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CBoss* pBoss = new CBoss;
	if (pBoss == nullptr) return nullptr;

	// オブジェクト設定
	pBoss->SetPos(pos);
	pBoss->SetRot(rot);

	// 初期化設定時
	if (FAILED(pBoss->Init())) return nullptr;

	return pBoss;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CBoss::Init(void)
{
	// キャラクター初期化
	CMoveCharactor::Init();

	// モーションロード
	MotionLoad("data/MOTION/Boss/BossMotion.txt", MOTION::MAX, false);

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CBoss::Uninit(void)
{
	// キャラクター終了
	CMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CBoss::Update(void)
{
	// 現在地の座標を取得
	auto pos = GetPos();

	if (m_isOutSideIn)
		MoveInOffice(pos); // オフィス内に来る

	if (m_isOfficeMove)
		MoveOfficePoint(pos); // オフィス内で巡回する処理

#if 0
	// 取得
	auto pos = GetPos();

	// 移動処理
	D3DXVECTOR3 vec = BOSS_INFO::Destpos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vec);

	// 到着判定
	if (distance <= 5.0f)
	{
		// 座標を目的地に合わせる
		SetPos(BOSS_INFO::Destpos);

		// 目的地に到着した瞬間にモーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vec);

	// 移動量
	moveVec *= 2.0f;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
#endif
	// キャラクター座標更新
	CMoveCharactor::UpdatePosition();

	// 更新された座標を取得
	auto UpdatePos = GetPos();

	// 親キャラクター更新
	CMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CBoss::Draw(void)
{
	// キャラクター描画
	CMoveCharactor::Draw();
}
//========================================================
// オフィス内に侵入する処理
//========================================================
void CBoss::MoveInOffice(const D3DXVECTOR3& pos)
{
	// もし最後のインデックスだったら
	if (m_nViewIdx >= BOSS_INFO::NUM_OUTSIDEPOINT)
	{
		m_isOutSideIn = false;
		m_isOfficeMove = true;
		return;
	}

	// ターゲットの座標
	D3DXVECTOR3 targetPos = BOSS_INFO::LocalMovePoint[m_nViewIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= 2.0f)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// インデックス設定
		m_nViewIdx++;

		// モーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.0f;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE,true,3);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
}
//========================================================
// オフィス内で巡回をする処理
//========================================================
void CBoss::MoveOfficePoint(const D3DXVECTOR3& pos)
{
	// 停止カウント中の処理
	if (m_nCoolTime > 0)
	{
		// 減算
		m_nCoolTime--;

		// 待機中はニュートラルモーション
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ターゲットの座標
	D3DXVECTOR3 targetPos = OFFICE_INFO::OfficeMovePoint[m_nOfficeViewIdx];

	// 目的地へのベクトルを計算
	D3DXVECTOR3 vecToTarget = targetPos - pos;

	// 目的地までの距離を計算
	float distance = D3DXVec3Length(&vecToTarget);

	// 到着判定
	if (distance <= 2.0f)
	{
		// 座標を目的地に合わせる
		SetPos(targetPos);

		// クールタイム開始
		m_nCoolTime = OFFICE_INFO::COOLTIME;

		// インデックス設定
		m_nOfficeViewIdx = Wrap(m_nOfficeViewIdx + 1, 0, OFFICE_INFO::NUM_OFFICEPOINT - 1);

		// モーションを切り替える
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 5);
		return;
	}

	// ベクトルを正規化
	D3DXVECTOR3 moveVec;
	D3DXVec3Normalize(&moveVec, &vecToTarget);

	// 移動量
	moveVec *= 1.0f;
	SetMove(moveVec);

	// 移動モーションを設定
	GetMotion()->SetMotion(MOTION::MOVE);

	// 角度を計算
	float angleY = atan2(-moveVec.x, -moveVec.z);

	// 現在の目標角度
	D3DXVECTOR3 rotDest = GetRotDest();

	// 角度を正規化
	rotDest.y = NormalAngle(angleY);

	// 目標角度をセット
	SetRotDest(rotDest);
}