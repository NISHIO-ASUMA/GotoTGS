//=========================================================
//
// AFKたばこ処理 [ afksmoke.cpp ]
// Author: Shouya Chikada
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "afksmoke.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"
#include "collisionsphere.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace UI
{

};

//=========================================================
// コンストラクタ
//=========================================================
CAfksmoke::CAfksmoke()
{

}

//=========================================================
// デストラクタ
//=========================================================
CAfksmoke::~CAfksmoke()
{

}

//=========================================================
// 生成処理
//=========================================================
CAfksmoke* CAfksmoke::Create(const D3DXVECTOR3& pos, const float& fRadius)
{
	// インスタンス生成
	CAfksmoke* pAfkSmoke = new CAfksmoke;
	//if (pAfkSmoke == nullptr) return nullptr;

	//// オブジェクトセット
	//pAfkSmoke->SetPos(pos);
	//pAfkSmoke->SetSize(fWidth, fHeight);
	//pAfkSmoke->SetRot(rot);
	//pAfkSmoke->SetRadius(fRadius);
	//pAfkSmoke->SetTexture(pTexName);
	//pAfkSmoke->SetEnableZtest(true);

	//// 初期化失敗時
	//if (FAILED(pTutorialUI->Init())) return nullptr;

	// チュートリアルUIのポインタを返す
	return pAfkSmoke;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAfksmoke::Init(void)
{
	//// 親クラスの初期化処理
	//CBillboard::Init();

	//// 球形コライダーを生成
	//m_pCollider = CSphereCollider::Create(GetPos(), m_fRadius);

	//return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAfksmoke::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CAfksmoke::Update(void)
{
	////// プレイヤーの情報を取得し判定を生成
	////const auto& Player = CGameSceneObject::GetInstance()->GetPlayer();

	////// コライダー取得とnullチェック
	////CSphereCollider* Collider = Player->GetSphereCollider();
	////if (Collider == nullptr) return;

	////// 当たり判定の実行
	////if (Collision(Collider))m_bLook = true;
	////else m_bLook = false;

	////// 現在の時間を取得
	////int nNowTime = CGameSceneObject::GetInstance()->GetTime()->GetAllTime();

	////// タイムが以下だったら
	////if (nNowTime <= UI::nDeleteTime)m_bTime = false;

	////// 親クラスの更新処理
	////CBillboard::Update();
}