//========================================================
//
// 動くUI処理 [ moveui.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "moveui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace MOVE_UI
{
	constexpr float MOVE_DOWN_SPEED = 2.0f;		// 下移動速度
	constexpr float MOVE_RIGHT_SPEED = 3.0f;	// 右移動速度
	constexpr float SCREEN_OUT_X = 1600.0f;		// 画面外とみなすX座標
};

//========================================================
// コンストラクタ
//========================================================
CMoveUi::CMoveUi(int nPriority) : CObject2D(nPriority),
m_isUse(false),
m_DestPos(VECTOR3_NULL),
m_StartPos(VECTOR3_NULL),
m_State(MOVE_DOWN)
{

}
//========================================================
// デストラクタ
//========================================================
CMoveUi::~CMoveUi()
{

}
//========================================================
// 生成処理
//========================================================
CMoveUi* CMoveUi::Create
(
	const D3DXVECTOR3& pos,
	const int& nFlashFrame,
	const float& fWidth,
	const float& fHeight,
	const char* Filename)
{
	// インスタンス生成
	CMoveUi* pUi = new CMoveUi;
	if (!pUi) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init()))
		return nullptr;

	// オブジェクト設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);
	pUi->SetTexture(Filename);
	pUi->SetAnchor();
	pUi->SetParameter(pos);

	return pUi;
}
//========================================================
// パラメーターセット
//========================================================
void CMoveUi::SetParameter(const D3DXVECTOR3& pos)
{
	// 出現時の初期座標を記憶
	m_StartPos = pos;

	// 格納
	m_DestPos = pos;

	// 基準からの目標座標を設定
	m_DestPos.y = m_DestPos.y + 60.0f;

	// 状態クリア
	ResetState();
}
//========================================================
// 初期化処理
//========================================================
HRESULT CMoveUi::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CMoveUi::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CMoveUi::Update(void)
{
	if (!m_isUse) return;

	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 調整用定数

	switch (m_State)
	{
	case STATE::MOVE_DOWN:
		// 下方向へ移動 (+y)
		pos.y += MOVE_UI::MOVE_DOWN_SPEED;

		// 目標座標に到達したら停止
		if (pos.y >= m_DestPos.y)
		{
			pos.y = m_DestPos.y; 
			m_State = STATE::WAIT;
		}
		break;

	case STATE::WAIT:

		// 座標で止まっておく
		SetPos(pos);
		break;

	case STATE::MOVE_RIGHT:
		// 右方向へ移動
		pos.x += MOVE_UI::MOVE_RIGHT_SPEED;

		// 画面外へ出たら非表示
		if (pos.x >= MOVE_UI::SCREEN_OUT_X)
		{
			ResetState();		// 座標を出現位置に戻し、ステートを初期化
			m_isUse = false;	// 非有効化
			return;
		}
		break;
	}

	// 座標の更新
	SetPos(pos);

	// 親クラスの更新
	CObject2D::Update();
}
//========================================================
// 描画処理
//========================================================
void CMoveUi::Draw(void)
{
	if (!m_isUse) return;

	// 親クラスの描画
	CObject2D::Draw();
}
//========================================================
// リセット関数
//========================================================
void CMoveUi::ResetState(void)
{
	// 各種状態の設定
	SetPos(m_StartPos);
	m_State = STATE::MOVE_DOWN;
}