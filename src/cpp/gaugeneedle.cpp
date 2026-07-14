//=========================================================
//
// ゲージの指針の処理 [ gaugeneedle.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gaugeneedle.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "easing.h"
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CGaugeneedle::CGaugeneedle(int nPriority) :CObject2DRotation(nPriority),
m_nTask(NULL),			// タスクをこなした数
m_nAFK(NULL),			// さぼりをこなした数
m_nCount(NULL),			// 現在のカウント
m_bStart(false),		// 動いているかどうか
m_bDir(false),			// どの方向に進むか
m_fOldAngle(NULL)		// 元の角度
{

}

//=========================================================
// デストラクタ
//=========================================================
CGaugeneedle::~CGaugeneedle()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CGaugeneedle* CGaugeneedle::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// ゲージの指針のポインタ
	CGaugeneedle* pGaugeneedle = new CGaugeneedle;

	// ヌルチェック
	if (pGaugeneedle == nullptr) return nullptr;

	// ゲージの指針の設定
	pGaugeneedle->SetPos(pos);														// 位置
	pGaugeneedle->SetPivot(D3DXVECTOR3(Config::PIVOT_X, Config::PIVOT_Y, 0.0f));	// 回転の基準点
	pGaugeneedle->SetAngle(NULL);													// 角度
	pGaugeneedle->m_fOldAngle = NULL;												// 元の角度
	pGaugeneedle->SetCol(COLOR_WHITE);												// 色
	pGaugeneedle->SetSize(D3DXVECTOR2(fWidth, fHeight));							// サイズ
	pGaugeneedle->SetTexture("needle000.png");										// テクスチャ

	// 初期化が失敗した場合
	if (FAILED(pGaugeneedle->Init())) return nullptr;

	return pGaugeneedle;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CGaugeneedle::Init(void)
{
	// 親の初期化処理
	CObject2DRotation::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CGaugeneedle::Uninit(void)
{
	// 親の終了処理
	CObject2DRotation::Uninit();

}

//=========================================================
// 更新処理
//=========================================================
void CGaugeneedle::Update(void)
{
	// 親の更新処理
	CObject2DRotation::Update();

	// 動いていない場合、更新しない
	if (m_bStart != true) return;

	// カウントが最大に達している場合
	if (m_nCount >= Config::MAX_FREAM)
	{
		// カウントの初期化
		m_nCount = NULL;

		// 動かさない状態にする
		m_bStart = false;

		// 現在の角度を代入する
		m_fOldAngle = GetAngle();

		return;
	}

	float fFream = NULL;		// 1フレームの割合
	float fNowRatio = NULL;		// 現在のフレームの割合
	float fMoveAngle = NULL;	// 移動量

	// 1フレームの割合
	fFream = CEasing::SetEase(m_nCount, Config::MAX_FREAM);

	// 揺れながら移動
	fNowRatio = CEasing::EaseOutElastic(fFream);

	if (m_bDir != false)
	{// 右方向に進む場合
		fMoveAngle = GetAngle() + (Config::MOVE_ANGLE * (1.0f - fNowRatio));
	}
	else
	{// 左方向に移動
		fMoveAngle = GetAngle() - (Config::MOVE_ANGLE * (1.0f - fNowRatio));
	}

	// 角度正規化
	NormalizAngle(fMoveAngle);

	// 角度の設定
	SetAngle(fMoveAngle);

	// カウントを1つ増やす
	m_nCount++;
}

//=========================================================
// 描画処理
//=========================================================
void CGaugeneedle::Draw(void)
{
	// 親の描画処理
	CObject2DRotation::Draw();

}

//=========================================================
// タスクをこなした時の処理
//=========================================================
void CGaugeneedle::AddTask(void)
{
	// こなしたタスクの数を一つ増やす
	m_nTask++;

	// ゲージを動かしている状態にする
	m_bStart = true;

	// 左に進むようにする
	m_bDir = false;
}

//=========================================================
// さぼりをこなした時の処理
//=========================================================
void CGaugeneedle::AddAFK(void)
{
	// こなしたさぼりの数を一つ増やす
	m_nAFK++;

	// ゲージを動かしている状態にする
	m_bStart = true;

	// 右に進むようにする
	m_bDir = true;
}

//=========================================================
// 角度正規化処理
//=========================================================
void CGaugeneedle::NormalizAngle(float& fAngle)
{
	// 最大・最小を超える場合、角度を固定化
	if (fAngle < -D3DX_PI * Config::MAX_ANGLE)
	{// 最小
		fAngle = -D3DX_PI * Config::MAX_ANGLE;
	}
	else if (fAngle > D3DX_PI * Config::MAX_ANGLE)
	{// 最大
		fAngle = D3DX_PI * Config::MAX_ANGLE;
	}

	// 正規化
	fAngle = NormalAngle(fAngle);
}
