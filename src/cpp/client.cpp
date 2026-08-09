//=========================================================
//
// 外のクライアントキャラクター処理 [ client.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "client.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "sound.h"
#include "motion.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "clientutility.h"

//=========================================================
// コンストラクタ
//=========================================================
CClient::CClient(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeTime(NULL),
m_pSphereCollider(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CClient::~CClient()
{

}
//=========================================================
// 生成処理
//=========================================================
CClient* CClient::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CClient* pClient = new CClient;
	if (pClient == nullptr) return nullptr;

	// オブジェクト設定
	pClient->SetPos(pos);
	pClient->SetRot(rot);
	pClient->SetUseOutLine(true);
	pClient->SetOutLineColor(D3DXVECTOR4(1.0f, 0.64f, 0.0f, 1.0f));
	pClient->SetOutLineSize(0.38f);

	// 初期化失敗時
	if (FAILED(pClient->Init())) return nullptr;

	return pClient;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CClient::Init(void)
{
	// 親クラスの初期化処理
	CNoMoveCharactor::Init();

	// モーションロード
	MotionLoad(CLIENT_INFO::MOTIONNAME,MOTION::MAX,false);

	// コライダー生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), CLIENT_INFO::SPHERESIZE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CClient::Uninit(void)
{
	// ポインタの破棄
	m_pSphereCollider.reset();

	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CClient::Update(void)
{
	// 状態遷移
	if (m_isChange)
	{
		m_nChangeTime--; // 減算

		if (m_nChangeTime >= NULL)
		{
			// モーション更新のみ
			CNoMoveCharactor::UpdateMotionOnly();
			return;
		}

		// カウントが終了したらフラグを戻す
		m_isChange = false;

		// モーションをもとに戻す
		GetMotion()->SetMotion(MOTION::NEUTRAL, true, 3);
	}

	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CClient::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}
//=========================================================
// 動作変更
//=========================================================
void CClient::ActionSet(void)
{
	// モーション変更
	GetMotion()->SetMotion(MOTION::ACTION, true, 3);

	// フラグ変更
	m_isChange = true;

	// カウント変更
	m_nChangeTime = CLIENT_INFO::MAX_CHANGETIME;
}
//=========================================================
// 球形の当たり判定
//=========================================================
bool CClient::Collision(CSphereCollider* pOther)
{
	// nullチェック
	if (!m_pSphereCollider) return false;

	return CCollisionSphere::Collision(m_pSphereCollider.get(),pOther);
}