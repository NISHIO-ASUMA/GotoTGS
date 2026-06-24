//=========================================================
//
// チュートリアルUI処理 [ tutorialui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afkui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "player.h"
#include "gamesceneobject.h"
#include "gametime.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace AfkUI
{
#ifdef _DEBUG
	constexpr int nDeleteTime = 590;		// チュートリアル用UIの表示の仕方を切り替えるための時間
#else
	constexpr int nDeleteTime = 140;		// チュートリアル用UIの表示の仕方を切り替えるための時間
#endif
};

//=========================================================
// コンストラクタ
//=========================================================
CAfkUI::CAfkUI(int nPriority) : CBillboard(nPriority),
m_pCollider(nullptr),
m_fRadius(NULL),
m_bLook(false),
m_bTime(true)
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfkUI::~CAfkUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfkUI* CAfkUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float& fWidth, const float& fHeight, const float& fRadius, const char* pTexName)
{
	// インスタンス生成
	CAfkUI* pAfkUI = new CAfkUI;
	if (pAfkUI == nullptr) return nullptr;

	// オブジェクトセット
	pAfkUI->SetPos(pos);
	pAfkUI->SetSize(fWidth, fHeight);
	pAfkUI->SetRot(rot);
	pAfkUI->SetRadius(fRadius);
	pAfkUI->SetTexture(pTexName);
	pAfkUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pAfkUI->Init())) return nullptr;

	// UIのポインタを返す
	return pAfkUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfkUI::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	// 球形コライダーを生成
	m_pCollider = CSphereCollider::Create(GetPos(), m_fRadius);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfkUI::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();

	// スフィアコライダーの破棄
	m_pCollider.reset();

}
//=========================================================
// 更新処理
//=========================================================
void CAfkUI::Update(void)
{
	// プレイヤーの情報を取得し判定を生成
	const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();

	// コライダー取得とnullチェック
	CSphereCollider* Collider = Player->GetSphereCollider();
	if (Collider == nullptr) return;

	// 当たり判定の実行
	if (Collision(Collider))m_bLook = true;
	else m_bLook = false;

	// 現在の時間を取得
	int nNowTime = CGameSceneObject::GetInstance()->GetTime()->GetAllTime();

	// タイムが以下だったら
	if (nNowTime <= AfkUI::nDeleteTime)m_bTime = false;

	// 親クラスの更新処理
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CAfkUI::Draw(void)
{
	// 親クラスの描画処理
	if (m_bLook || m_bTime)
	{
		// 親クラスの描画
		CBillboard::Draw();
	}
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CAfkUI::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pCollider.get(), pOther);
}