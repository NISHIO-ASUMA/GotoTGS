//=========================================================
//
// 自動ドアの処理 [ automatic_door.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "automatic_door.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "collisionbox.h"
#include "manager.h"
#include "xfilemanager.h"
#include "renderer.h"
#include "player.h"
#include "sound.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace MOVE_INFO
{
	constexpr float SPEED = 2.0f; // 移動速度
	constexpr int MAX_OPNETIME = 120;
};

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoor::CAutoMaticDoor(int nPriority) : CObjectX(nPriority),
m_isZTestEneble(false),
m_pCollider(nullptr),
m_nMoveType(MOVETYPE::MOVETYPE_LEFT),
m_fMoveSpeed(NULL),
m_nState(STATE::STATE_CLOSE_WAIT),
m_nOpenTimer(NULL),
m_vBasePos(VECTOR3_NULL),
m_Size(VECTOR3_NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CAutoMaticDoor::~CAutoMaticDoor()
{

}
//=========================================================
// 生成処理
//=========================================================
CAutoMaticDoor* CAutoMaticDoor::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale, 
	const char* pModelName,
	const MOVETYPE& nType
)
{
	// インスタンス生成
	CAutoMaticDoor* pAutoDoor = new CAutoMaticDoor;
	if (pAutoDoor == nullptr) return nullptr;

	// オブジェクト設定
	pAutoDoor->SetPos(pos);
	pAutoDoor->SetRot(rot);
	pAutoDoor->SetScale(scale);
	pAutoDoor->SetFilePass(pModelName);
	pAutoDoor->SetType(nType);

	// 初期化失敗時
	if (FAILED(pAutoDoor->Init())) return nullptr;

	return pAutoDoor;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAutoMaticDoor::Init(void)
{
	// 親クラスの初期化
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号のモデルを取得
	int nModelIdx = GetModelIdx();

	// モデルの拡大率と既存の拡大率を合わせる
	D3DXVECTOR3 Scale = GetScale();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// サイズセット
	m_Size = Size;

	// オブジェクトの回転角度を取得
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot();

	// 回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成処理
	m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size, matRot);

	// 初期座標を設定
	m_vBasePos = GetPos();

	// 変数初期化
	m_nOpenTimer = NULL;
	m_fMoveSpeed = NULL;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAutoMaticDoor::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CAutoMaticDoor::Update(void)
{
	// 座標取得
	auto pos = GetPos();

	// 移動方設定
	float dir = (m_nMoveType == MOVETYPE_LEFT) ? 1.0f : -1.0f;

	// ドアがどれくらい開くか
	float maxOpenDistance = 50.0f;

	switch (m_nState)
	{
	case STATE_CLOSE_WAIT:
		// プレイヤーが近づくのを待つ
		break;

	case STATE_OPENING:
		// ドアを開ける方向に移動
		pos.z += dir * MOVE_INFO::SPEED;
		SetPos(pos);

		// 目標の開き具合まで達したかチェック
		if (fabsf(pos.z - m_vBasePos.z) >= maxOpenDistance)
		{
			// ピッタリの位置に補正して状態遷移
			pos.z = m_vBasePos.z + (dir * maxOpenDistance);
			SetPos(pos);

			m_nState = STATE_OPEN_WAIT;
			m_nOpenTimer = 0; 
		}
		break;

	case STATE_OPEN_WAIT:
		// 一定時間開いたまま
		m_nOpenTimer++;
		if (m_nOpenTimer >= MOVE_INFO::MAX_OPNETIME)
		{
			// 閉じる
			m_nState = STATE_CLOSING;
		}
		break;

	case STATE_CLOSING:
		// ドアを閉める方向に移動
		pos.z -= dir * MOVE_INFO::SPEED;
		SetPos(pos);

		// 左ドアならベースより小さくなったら、右ドアなら大きくなったら終了
		if ((dir == -1.0f && pos.z >= m_vBasePos.z) ||
			(dir == 1.0f && pos.z <= m_vBasePos.z))
		{
			pos.z = m_vBasePos.z;
			SetPos(pos);
			m_nState = STATE_CLOSE_WAIT;

			// ここでサウンドのフラグリセット
			m_isSoundPlay = false;
		}

		break;
	}

	// 座標の設定
	SetPos(pos);

	// コライダー座標の更新
	if (m_pCollider)
	{
		m_pCollider->SetPosOld(pos);
		m_pCollider->SetPos(pos);
	}

	// 親クラスの更新処理
	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CAutoMaticDoor::Draw(void)
{
	// デバイス取得
	const auto& pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 有効状態なら
	if (m_isZTestEneble)
	{
		// Zバッファの比較はするが、自分のZ値は書き込まない設定にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// 親クラスの描画処理
	CObjectX::Draw();

	// 有効状態なら
	if (m_isZTestEneble)
	{
		// 設定を戻す
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}
//=========================================================
// 当たり判定処理
//=========================================================
bool CAutoMaticDoor::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// コライダーがnullなら
	if (m_pCollider == nullptr) return false;

	// 矩形の当たり判定を返す
	return CCollisionBox::Collision(m_pCollider.get(),pOther,OutPos);
}
//=========================================================
// ドアの開閉判別処理
//=========================================================
void CAutoMaticDoor::OpenDoorFlag(void)
{
	// 閉じている、または閉じ中の時だけ開く
	if (m_nState == STATE_CLOSE_WAIT || m_nState == STATE_CLOSING)
	{
		// 空いている状態にする
		m_nState = STATE_OPENING;

		if (!m_isSoundPlay)
		{
			// サウンド再生
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_AUTODOOR_SE);
			// サウンドフラグ有効化
			m_isSoundPlay = true;
		}
		return;
	}
}