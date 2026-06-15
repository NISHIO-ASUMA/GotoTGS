//=========================================================
//
// カットイン処理 [ eventcutin.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "eventcutin.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CEventcutin::CEventcutin(int nPriority):CAnimationObject2D(nPriority),
m_Offsetpos(VECTOR3_NULL),
m_nStopCount(NULL),
m_bTurn(false)
{

}

//=========================================================
// デストラクタ
//=========================================================
CEventcutin::~CEventcutin()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CEventcutin* CEventcutin::Create(const EventCutin& eventcutin)
{
	// インスタンス生成
	CEventcutin* pEventcutin = new CEventcutin;

	if (pEventcutin == nullptr)
	{// ヌルチェック
		return nullptr;
	}

	// 初期位置を設定
	pEventcutin->m_Offsetpos = eventcutin.pos;
	pEventcutin->m_Offsetpos.x -= SCREEN_WIDTH;

	// 各設定処理
	pEventcutin->SetPos(pEventcutin->m_Offsetpos);
	pEventcutin->SetCol(eventcutin.col);
	pEventcutin->SetSize(eventcutin.fWidth, eventcutin.fHeight);
	pEventcutin->SetUV(eventcutin.tex.x, eventcutin.tex.y);
	pEventcutin->SetAnimFlag(eventcutin.isLoop);
	pEventcutin->SetUse(eventcutin.bUse);
	pEventcutin->SetTexture(Config::TEXNAME);

	if (FAILED(pEventcutin->Init()))
	{// 初期化が失敗した場合
		return nullptr;
	}

	return pEventcutin;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CEventcutin::Init(void)
{
	// 表示し続けるフレームのカウントを初期化
	m_nStopCount = NULL;

	// 折り返していない状態にする
	m_bTurn = false;

	// 親の初期化処理
	CAnimationObject2D::Init();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CEventcutin::Uninit(void)
{
	// 親の終了処理
	CAnimationObject2D::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CEventcutin::Update(void)
{
	// 使用していない時
	if (GetUse() != true) return;

	// 位置
	D3DXVECTOR3 pos = GetPos();

	// 現在のカウント
	int nCount = GetFreamCount();

	if (m_nStopCount > NULL)
	{// 表示するカウントが残っているなら
		// 表示するカウントを1つ減らす
		m_nStopCount--;

		return;
	}

	// 現在のカウントを1つ進める
	nCount++;

	// 現在のカウントを設定
	SetFreamCount(nCount);

	// 1フレーム毎のX軸の移動量
	float fMoveX = CEasing::SetEase(nCount, Config::MOVE_FREAM);
	fMoveX = CEasing::EaseOutCubic(fMoveX);

	// 移動量を加算
	pos.x = m_Offsetpos.x + (SCREEN_WIDTH * fMoveX);

	if (nCount >= Config::MOVE_FREAM)
	{
		// 現在のカウントを初期化
		SetFreamCount(NULL);

		if (m_bTurn != false)
		{
			// 初期位置に戻す
			m_Offsetpos.x -= SCREEN_WIDTH;
			pos = m_Offsetpos;

			// 表示しない状態にする
			SetUse(false);

			// 折り返ししていない状態にする
			m_bTurn = false;

		}
		else
		{
			// 初期位置を中心に設定
			m_Offsetpos.x += SCREEN_WIDTH;

			// 表示するフレーム数を代入する
			m_nStopCount = Config::STOP_FREAM;

			// 折り返した状態にする
			m_bTurn = true;

		}

	}

	// 位置を設定
	SetPos(pos);

	// 親クラスの更新処理
	CAnimationObject2D::Update();
}

//=========================================================
// 描画処理
//=========================================================
void CEventcutin::Draw(void)
{
	// 使用していないなら
	if (GetUse() != true) return;

	// 親の描画処理
	CAnimationObject2D::Draw();

}