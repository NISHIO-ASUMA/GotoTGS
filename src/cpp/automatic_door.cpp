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

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoor::CAutoMaticDoor(int nPriority) : CObjectX(nPriority),
m_isZTestEneble(false),
m_pCollider(nullptr)
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
	const char* pModelName
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

	// オブジェクトの回転角度を取得
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot();

	// 回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成処理
	m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size, matRot);

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