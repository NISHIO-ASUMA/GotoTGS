//=========================================================
//
// デスク処理 [ desk.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "desk.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "xfilemanager.h"
#include "boxcollider.h"
#include "collisionbox.h"

//=========================================================
// コンストラクタ
//=========================================================
CDesk::CDesk(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CDesk::~CDesk()
{

}
//=========================================================
// 生成処理
//=========================================================
CDesk* CDesk::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const char* pModelName)
{
	// インスタンス生成
	CDesk* pDesk = new CDesk;
	if (pDesk == nullptr) return nullptr;

	// オブジェクト設定
	pDesk->SetPos(pos);
	pDesk->SetRot(rot);
	pDesk->SetScale(scale);
	pDesk->SetFilePass(pModelName);

	// 初期化失敗時
	if (FAILED(pDesk->Init())) return nullptr;

	return pDesk;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CDesk::Init(void)
{
	// 親クラスの初期化処理
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号を取得
	int nModelIdx = GetModelIdx();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// モデルのパス取得
	std::string str = pXManager->GetInfo(nModelIdx).FilePath;

	// コライダーの設定
	if (str == "data/MODEL/STAGEOBJ/desk00.x") m_pCollider = nullptr;
	else
		m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size);

	// マトリックスシャドウを有効化する
	if (str == "data/MODEL/STAGEOBJ/wallback.x")
		SetShadow(false);
	else
		SetShadow(true);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CDesk::Uninit(void)
{
	// 矩形コライダーの破棄
	m_pCollider.reset();

	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CDesk::Update(void)
{
	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー座標の更新
	if (m_pCollider) m_pCollider->SetPos(pos);
}
//=========================================================
// 描画処理
//=========================================================
void CDesk::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}
//=========================================================
// 当たり判定
//=========================================================
bool CDesk::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pCollider.get(), pOther, OutPos);
}