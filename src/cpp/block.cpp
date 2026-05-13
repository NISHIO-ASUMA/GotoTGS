//=========================================================
//
// ブロック処理 [ block.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "block.h"

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
CBlock::CBlock(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CBlock::~CBlock()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CBlock* CBlock::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const D3DXVECTOR3& scale, 
	const char* pModelName
)
{
	// インスタンス生成
	CBlock* pBlock = new CBlock;
	if (pBlock == nullptr) return nullptr;

	// オブジェクト設定
	pBlock->SetFilePass(pModelName);
	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetScale(scale);

	// 初期化失敗時
	if (FAILED(pBlock->Init())) return nullptr;

	return pBlock;
}
//=========================================================
// ブロックの初期化処理
//=========================================================
HRESULT CBlock::Init(void)
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

	// 特定のモデルのサイズ調整
	if (str == "data/MODEL/STAGEOBJ/desk00.x")
	{
		// デスクの当たり判定を少し大きくする
		Size.x += Config::VALUESIZE;
		Size.y += Config::VALUESIZE;
		Size.z += Config::VALUESIZE;
	}

	// オブジェクトの回転角度を取得
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot(); 

	// 回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 特定のオブジェクトの当たり判定を消す
	if (str == "data/MODEL/STAGEOBJ/chair00.x" || str == "data/MODEL/STAGEOBJ/pc00.x")
	{
		m_pCollider = nullptr;
	}
	else
	{
		// 矩形コライダー生成処理
		m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size, matRot);
	}
	
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBlock::Uninit(void)
{
	// 矩形コライダーの破棄
	m_pCollider.reset();

	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CBlock::Update(void)
{
	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー座標の更新
	if (m_pCollider) m_pCollider->SetPos(pos);
}
//=========================================================
// 描画処理
//=========================================================
void CBlock::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}
//=========================================================
// 当たり判定
//=========================================================
bool CBlock::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pCollider.get(), pOther, OutPos);
}