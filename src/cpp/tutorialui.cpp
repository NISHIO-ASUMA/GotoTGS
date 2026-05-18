//=========================================================
//
// チュートリアルUI処理 [ tutorialui.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "tutorialui.h"

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

//=========================================================
// コンストラクタ
//=========================================================
CTutorialUI::CTutorialUI(int nPriority) : CBillboard(nPriority),
m_pCollider(nullptr),
m_bLook(false),
m_bTime(true)
{

}

//=========================================================
// デストラクタ
//=========================================================
CTutorialUI::~CTutorialUI()
{

}

//=========================================================
// 生成処理
//=========================================================
CTutorialUI* CTutorialUI::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fWidth, float fHeight, const char* pTexName)
{
	// インスタンス生成
	CTutorialUI* pTutorialUI = new CTutorialUI;
	if (pTutorialUI == nullptr) return nullptr;

	// オブジェクトセット
	pTutorialUI->SetPos(pos);
	pTutorialUI->SetSize(fWidth, fHeight);
	pTutorialUI->SetRot(rot);
	pTutorialUI->SetTexture(pTexName);
	pTutorialUI->SetEnableZtest(true);

	// 初期化失敗時
	if (FAILED(pTutorialUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pTutorialUI;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialUI::Init(void)
{
	// 親クラスの初期化処理
	CBillboard::Init();

	// 球形コライダーを生成
	m_pCollider = CSphereCollider::Create(GetPos(),75.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialUI::Uninit(void)
{
	// 親クラスの終了処理
	CBillboard::Uninit();

	// スフィアコライダーの破棄
	m_pCollider.reset();

}
//=========================================================
// 更新処理
//=========================================================
void CTutorialUI::Update(void)
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

	// タイムが570秒以下だったら
	if (nNowTime <= 570)m_bTime = false;

	// 親クラスの更新処理
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CTutorialUI::Draw(void)
{
	// 親クラスの描画処理
	if(m_bLook || m_bTime)	CBillboard::Draw();
}
//=========================================================
// 球形当たり判定処理
//=========================================================
bool CTutorialUI::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (!m_pCollider) return false;

	//球形当たり判定を返す
	return CCollisionSphere::Collision(m_pCollider.get(),pOther);
}