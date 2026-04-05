//=========================================================
//
// インスタンシング適用モデル処理 [ instancingmodel.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "instancemodel.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "instancemodelmanager.h"
#include "instancing.h"
#include "pausemanager.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace INSTANCINGINFO
{
	constexpr int MAX_UPDATECOUNT = 2; // 更新フレーム
}

//=========================================================
// コンストラクタ
//=========================================================
CInstanceModel::CInstanceModel() : m_pParent(nullptr),
m_pTexture(nullptr),
m_nModelIdx(-1),
m_nUpdateCount(NULL),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_offPos(VECTOR3_NULL),
m_offRot(VECTOR3_NULL),
m_prevPos(VECTOR3_NULL),
m_nextPos(VECTOR3_NULL),
m_prevRot(VECTOR3_NULL),
m_nextRot(VECTOR3_NULL),
m_scale(INITSCALE),
m_interp(NULL),
m_isShadow(false),
m_mtxworld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CInstanceModel::~CInstanceModel()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CInstanceModel* CInstanceModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename, const bool isShadow)
{
	// インスタンス生成
	CInstanceModel* pModelinst = new CInstanceModel;
	if (pModelinst == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pModelinst->Init(pos, rot, pFilename, isShadow))) return  nullptr;

	// ポインタを返す
	return pModelinst;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CInstanceModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename, const bool isShadow)
{
	// モデルセット
	SetModelPass(pFilename);

	// 位置の設定
	m_pos = pos;
	m_rot = rot;

	// オフセット位置を設定
	m_offPos = pos;
	m_offRot = rot;

	// フラグを設定する
	SetMtxShadow(isShadow);

	// 結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CInstanceModel::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CInstanceModel::Update(const D3DXMATRIX& mtx)
{
#ifdef NDEBUG
	// インデックスが-1なら
	if (m_nModelIdx == -1)
		return;
	
	// ファイルマネージャー取得
	CInstanceModelManager* pIMgr = CManager::GetInstance()->GetInstanceModelManager();
	if (!pIMgr) return;

	// 配列情報
	const auto& fileData = pIMgr->GetList();
	if (m_nModelIdx >= static_cast<int>(fileData.size())) return;

	// 配列取得
	const auto& model = fileData[m_nModelIdx];
	if (!model.pMesh) return;

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_offRot.y, m_rot.x + m_offRot.x, m_rot.z + m_offRot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_offPos.x, m_pos.y + m_offPos.y, m_pos.z + m_offPos.z);

	// マトリックスに反映
	m_mtxworld = mtxScale * mtxRot * mtxTrans;

	// 親のペアネント格納用変数
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// 親が存在する
		// ワールドマトリックス取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// マトリックスを取得
		mtxParent = mtx;
	}

	// 親のマトリックスとかけ合わせる
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxParent);

	// インスタンシングに登録
	const auto& Rendere = CManager::GetInstance()->GetRenderer();
	Rendere->AddInstanceObject(m_nModelIdx, this);

#else
	if (m_nModelIdx == -1) return;

	// デバイス取得
	const auto& Rendere = CManager::GetInstance()->GetRenderer();

	// 30fps更新
	if (++m_nUpdateCount >= INSTANCINGINFO::MAX_UPDATECOUNT)
	{
		// カウントを初期化
		m_nUpdateCount = 0;

		// 前回の座標と角度を入れ替え
		m_prevPos = m_nextPos;
		m_prevRot = m_nextRot;

		// 現在の座標と角度を設定する
		m_nextPos = m_pos;
		m_nextRot = m_rot;

		// 補完値を0にする
		m_interp = 0.0f;
	}

	// 毎フレーム補間する
	m_interp = min(m_interp + 1.0f / INSTANCINGINFO::MAX_UPDATECOUNT, 1.0f);
	float fDis = m_interp;

	D3DXVECTOR3 ipos = m_prevPos + (m_nextPos - m_prevPos) * fDis;
	D3DXVECTOR3 irot = m_prevRot + (m_nextRot - m_prevRot) * fDis;

	// 計算用のローカルマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// 拡大率を設定
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,irot.y + m_offRot.y,irot.x + m_offRot.x,irot.z + m_offRot.z);

	// 位置を設定
	D3DXMatrixTranslation(&mtxTrans,ipos.x + m_offPos.x,ipos.y + m_offPos.y,ipos.z + m_offPos.z);

	// 行列同士をかけ合わせる
	m_mtxworld = mtxScale * mtxRot * mtxTrans;

	// 親モデルを設定する
	D3DXMATRIX mtxParent = (m_pParent)? m_pParent->GetMtxWorld(): mtx;

	// ワールドマトリックスを設定する
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxParent);

	// インスタンシング描画のオブジェクトに設定
	Rendere->AddInstanceObject(m_nModelIdx, this);
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CInstanceModel::Draw(const D3DXMATRIX& mtx)
{
	// ポーズ中以外ならここで処理を返す
	if (!CPauseManager::GetPause()) return;

	// デバイスポインタを取得
	const auto& Rendere = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Rendere->GetDevice();

	// ファイルマネージャー取得
	CInstanceModelManager* pIMgr = CManager::GetInstance()->GetInstanceModelManager();
	if (!pIMgr) return;

	// 配列情報
	const auto& fileData = pIMgr->GetList();
	if (m_nModelIdx >= static_cast<int>(fileData.size())) return;

	// 配列取得
	const auto& model = fileData[m_nModelIdx];
	if (!model.pMesh) return;

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_offRot.y, m_rot.x + m_offRot.x, m_rot.z + m_offRot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_offPos.x, m_pos.y + m_offPos.y, m_pos.z + m_offPos.z);

	// 行列同士をかけ合わせる
	m_mtxworld = mtxScale * mtxRot * mtxTrans;

	// 親のペアネント格納用変数
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// 親が存在する
		// ワールドマトリックス取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// マトリックス取得
		mtxParent = mtx;
	}

	// 親のマトリックスとかけ合わせる
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// 現在のマテリアル
	pDevice->GetMaterial(&matDef);

	// マテリアルが取得できたら
	if (model.pBuffMat)
	{
		// マテリアルデータのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

		// テクスチャ取得
		CTexture* pTex = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// マテリアルのセット
			pDevice->SetMaterial(&pMat[nCnt].MatD3D);

			// テクスチャ取得
			int texIdx = model.pTexture[nCnt];

			// テクスチャセット
			pDevice->SetTexture(0, (texIdx >= 0) ? pTex->GetAddress(texIdx) : nullptr);

			// モデルの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//=========================================================
// モデルパス設定
//=========================================================
void CInstanceModel::SetModelPass(const char* pModelName)
{
	// マネージャーから取得
	const auto& InstModelManager = CManager::GetInstance()->GetInstanceModelManager();
	if (InstModelManager == nullptr) return;

	// インデックスセット
	m_nModelIdx = InstModelManager->Register(pModelName);
}
//=========================================================
// ターゲットの座標に設定
//=========================================================
void CInstanceModel::SetTargetPos(const D3DXVECTOR3& pos)
{
	m_prevPos = m_nextPos;
	m_nextPos = pos;
	m_interp = 0.0f;
}
//=========================================================
// ターゲットの角度に設定
//=========================================================
void CInstanceModel::SetTargetRot(const D3DXVECTOR3& rot)
{
	m_prevRot = m_nextRot;
	m_nextRot = rot;
	m_interp = 0.0f;
}