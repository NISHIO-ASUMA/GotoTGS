//===================================================================
//
// インスタンシングキャラクタークラス [ instancingcharactor.cpp ]
// Author: Asuma Nishio
// 
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "instancingcharactor.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "blackboard.h"
#include "node.h"
#include "motionmanager.h"
#include "outline.h"
#include "motioninstancing.h"
#include "instancemodel.h"
#include "pausemanager.h"
#include "shadowstencil.h"

//===================================================================
// コンストラクタ
//===================================================================
CInstancingCharactor::CInstancingCharactor(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_move(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_rotDest(VECTOR3_NULL),
m_scale(INITSCALE),
m_type(TYPE_NONE),
m_pModel{},
m_pMotion{},
m_pShadowS{},
m_fMoveValue(NULL),
m_isStencilUse(false),
m_isOutLine(false),
m_isInstancing(false),
m_mtxworld{}
{
}
//===================================================================
// デストラクタ
//===================================================================
CInstancingCharactor::~CInstancingCharactor()
{
	
}
//===================================================================
// 初期化処理
//===================================================================
HRESULT CInstancingCharactor::Init(void)
{
	// 対象オブジェクトに設定
	SetObjType(CObject::TYPE_INSTANCE);

	// モデル配列の初期化
	m_pModel.clear();

	// 有効時
	if (m_isStencilUse)
	{
		// 影モデル生成
		m_pShadowS = CShadowStencil::Create(m_pos, m_rot);
	}

	return S_OK;
}
//===================================================================
// 終了処理
//===================================================================
void CInstancingCharactor::Uninit(void)
{
	// 動的確保分のモデルの破棄
	for (auto iter = m_pModel.begin(); iter != m_pModel.end(); iter++)
	{
		// nullチェック
		if ((*iter) != nullptr)
		{
			// 終了処理
			(*iter)->Uninit();

			// ポインタの破棄
			delete (*iter);

			// null初期化
			(*iter) = nullptr;
		}
	}

	// 配列のクリア
	m_pModel.clear();

	// ポインタ破棄
	m_pMotion.reset();

	// ノードクラスツリーの破棄
	if (m_pBehaviorTree)
	{
		m_pBehaviorTree->Exit();
		delete m_pBehaviorTree;
		m_pBehaviorTree = nullptr;
	}

	// ブラックボードポインタの破棄
	if (m_pBlackBoard)
	{
		m_pBlackBoard->Clear();
		delete m_pBlackBoard;
		m_pBlackBoard = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//===================================================================
// 更新処理
//===================================================================
void CInstancingCharactor::Update(void)
{
	// 移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.75f;
	m_move.z += (0.0f - m_move.z) * 0.75f;

	// 角度補完
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// 左回転
		// 角度
		m_rot.y = m_rot.y + D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{// 右回転
		// 角度
		m_rot.y = m_rot.y - D3DX_PI * 2.0f;
	}

	// 現在の角度
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.25f;

	// ステンシルシャドウの更新
	if (m_pShadowS)
	{
		// オブジェクト設定
		m_pShadowS->SetPos(m_pos);
		m_pShadowS->SetRot(m_rot);
	}

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスの掛け算
	m_mtxworld = mtxScal * mtxRot * mtxTrans;

#ifdef NDEBUG
	// モーションの更新
	if (m_pMotion) m_pMotion->Update(m_pModel);
#endif

	// モデルの更新処理
	for (auto& Model : m_pModel)
	{
		Model->Update(m_mtxworld);
	}
}
//===================================================================
// 描画処理
//===================================================================
void CInstancingCharactor::Draw(void)
{
	// デバイス取得
	const auto& Rendere = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Rendere->GetDevice();

	// 自身のワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// モデルの更新処理
	for (auto& Model : m_pModel)
	{
		Model->Draw(m_mtxworld);
	}
}
//===================================================================
// 座標のみの更新処理
//===================================================================
void CInstancingCharactor::UpdatePosition(void)
{
	m_posOld = m_pos;
	m_pos += m_move;
}
//===================================================================
// モーションスクリプト読み込み処理
//===================================================================
void CInstancingCharactor::MotionLoad(const char* pScriptName, int nDestMotions, const bool isShadow)
{
	// モーションのポインタを取得する
	m_pMotion = CMotionInstancing::Load(pScriptName, m_pModel, nDestMotions, isShadow);
}