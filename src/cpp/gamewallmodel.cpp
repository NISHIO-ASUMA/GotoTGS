//=========================================================
//
// 世界の透明な壁オブジェクト処理 [ gamewallmodel.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gamewallmodel.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "xfilemanager.h"
#include "boxcollider.h"
#include "boxtospherecollision.h"

//=========================================================
// コンストラクタ
//=========================================================
CGameWallModel::CGameWallModel(int nPriority) : CObjectX(nPriority), 
m_pCollider(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CGameWallModel::~CGameWallModel()
{

}
//=========================================================
// 生成処理
//=========================================================
CGameWallModel* CGameWallModel::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const D3DXVECTOR3& scale, 
	const char* pModelName
)
{
	// インスタンス生成
	CGameWallModel* pWall = new CGameWallModel;
	if (pWall == nullptr) return nullptr;

	// オブジェクト設定
	pWall->SetPos(pos);
	pWall->SetRot(rot);
	pWall->SetScale(scale);
	pWall->SetFilePass(pModelName);

	// 初期化失敗時
	if (FAILED(pWall->Init())) return nullptr;

	return pWall;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameWallModel::Init(void)
{
	// 親クラスの初期化処理
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号を取得
	int nModelIdx = GetModelIdx();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// コライダー生成
	m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size);

	// マトリックスシャドウを無効にする
	SetShadow(false);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameWallModel::Uninit(void)
{
	// nullチェック
	m_pCollider.reset();

	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CGameWallModel::Update(void)
{
	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー座標の更新
	if (m_pCollider) m_pCollider->SetPos(pos);
}
//=========================================================
// 描画処理
//=========================================================
void CGameWallModel::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}
//=========================================================
// 当たり判定処理
//=========================================================
bool CGameWallModel::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (m_pCollider == nullptr) return false;

	// 矩形と球のヒット関数
	return CBoxToSphereCollision::Collision(m_pCollider.get(),pOther);
}