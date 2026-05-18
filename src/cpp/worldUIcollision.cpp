//=========================================================
//
// マップに配置するUIの球形判定処理 [ worldUIcollision.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "worldUIcollision.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "collisionsphere.h"

//=========================================================
// コンストラクタ
//=========================================================
CWorldUICollision::CWorldUICollision(void) : m_nType(TYPE_MAX)
{

}

//=========================================================
// デストラクタ
//=========================================================
CWorldUICollision::~CWorldUICollision()
{

}

//=========================================================
// 生成処理
//=========================================================
CWorldUICollision* CWorldUICollision::Create(const D3DXVECTOR3& pos, const float& fRadius, const TYPE& type)
{
	// インスタンス生成
	CWorldUICollision* pCollider = new CWorldUICollision;

	// 上手く生成されていない場合
	if (pCollider == nullptr) return nullptr;

	// 初期化が失敗した場合
	if (FAILED(pCollider->Init())) return nullptr;

	// 各種値の設定
	pCollider->m_SphereColliderUI.push_back(CSphereCollider::Create(pos, fRadius));
	pCollider->m_nType = type;

	return pCollider;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorldUICollision::Init(void)
{

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CWorldUICollision::Uninit(void)
{
	// リストから削除
	m_SphereColliderUI.clear();
}

//=========================================================
// 更新処理
//=========================================================
void CWorldUICollision::Update(void)
{
	// 当たり判定の更新
	for (int nCount = 0; nCount < (int)m_SphereColliderUI.size(); nCount++)
	{
		// 位置の設定
		m_SphereColliderUI[nCount]->SetPos(m_SphereColliderUI[nCount]->GetPos());
	}
}
