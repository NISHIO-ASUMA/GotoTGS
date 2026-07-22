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
	// 社長が向かう座標
	const D3DXVECTOR3 Destpos = { 3.0f,21.0f,350.0f}; 

	// ローカルビューポイント
	const D3DXVECTOR3 LocalMovePoint[6] =
	{
		{677.5f,0.0f,313.0f},
		{663.0f,0.0f,115.0f},
		{420.5f,0.0f,115.0f},
		{400.0f,0.0f,190.0f},
		{134.0f,0.0f,65.0f},
		{130.0f,0.0f,310.0f},
	};
};

//========================================================
// コンストラクタ
//========================================================
CBoss::CBoss(int nPriority) : CMoveCharactor(nPriority),
m_pBoxColiider(nullptr),
m_pSphereColiider(nullptr),
m_nViewIdx(NULL)
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
{// ビューポイントを巡回して止まる処理
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

	// キャラクター更新
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