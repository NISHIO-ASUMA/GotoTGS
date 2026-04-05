//=========================================================
//
// 柱メッシュ処理 [ meshpiler.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "meshpiler.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "manager.h"
#include "template.h"

//*********************************************************
// 名前空間
//*********************************************************
namespace PILERINFO
{
	constexpr int NUMX = 30;	// 頂点分割数 ( X )
	constexpr int NUMZ = 1;		// 頂点分割数 ( Z )
	constexpr int LIFE = 60;	// 寿命規定値
	constexpr float VALUEHEIGHT = 800.0f;	// 高さの最大加算量
	constexpr float COLLISIONRADIUS = 60.0f;// コリジョンする半径
	constexpr float MOVESPEED = 15.0f;	// 横幅
};

//=========================================================
// コンストラクタ
//=========================================================
CMeshPiler::CMeshPiler(int nPriority) : CObject(nPriority),
m_pIdx(nullptr),
m_pVtx(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_MeshPiler{},
m_mtxWorld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CMeshPiler::~CMeshPiler()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CMeshPiler* CMeshPiler::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CMeshPiler* pMesh = new CMeshPiler;
	if (pMesh == nullptr) return nullptr;

	// オブジェクト設定
	pMesh->SetTexture();
	pMesh->m_pos = pos;

	// 初期化失敗時
	if (FAILED(pMesh->Init())) return nullptr;

	return pMesh;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMeshPiler::Init(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点数,インデックス数,ポリゴン数を計算する
	m_MeshPiler.nNumAllVtx = (PILERINFO::NUMX + 1) * (PILERINFO::NUMZ + 1);
	m_MeshPiler.nNumPrimitive = ((PILERINFO::NUMX * PILERINFO::NUMZ) * 2) + (4 * (PILERINFO::NUMZ - 1));
	m_MeshPiler.nNumIdx = (m_MeshPiler.nNumPrimitive + 2);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshPiler.nNumAllVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtx,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshPiler.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdx,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	//頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	float fTexX = 1.0f / PILERINFO::NUMX;
	float fTexY = 1.0f / PILERINFO::NUMZ;
	int nCnt = 0;

	// 法線設定用
	D3DXVECTOR3 nor = VECTOR3_NULL;

	// 縦
	for (int nCntZ = 0; nCntZ <= PILERINFO::NUMZ; nCntZ++)
	{
		// 横
		for (int nCntX = 0; nCntX <= PILERINFO::NUMX; nCntX++)
		{
			// 角度計算
			float fAngle = (D3DX_PI * 2.0f) / PILERINFO::NUMX * nCntX;

			// 頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(sinf((fAngle)) * PILERINFO::COLLISIONRADIUS, nCntZ * PILERINFO::VALUEHEIGHT, cosf((fAngle)) * PILERINFO::COLLISIONRADIUS);

			// 法線ベクトルの設定
			nor = pVtx[nCnt].pos - m_pos;	// 各頂点から原点の値を引く
			D3DXVec3Normalize(&pVtx[nCnt].nor, &nor);	// 法線の正規化

			// 法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nCnt].col = COLOR_WHITE;

			// テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX, nCntZ * fTexY);

			// 加算
			nCnt++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtx->Unlock();

	// インデックスポインタを宣言
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdx->Lock(0, 0, (void**)&pIdx, 0);

	WORD IndxNum = PILERINFO::NUMX + 1;// X

	WORD IdxCnt = 0;// 配列

	WORD Num = 0;

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < PILERINFO::NUMZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= PILERINFO::NUMX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// 最後の行じゃなかったら
		if (IndxCount1 < m_MeshPiler.nNumDigitZ - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	m_pIdx->Unlock();

	// メンバ変数の初期化
	m_MeshPiler.nLife = PILERINFO::LIFE;

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMeshPiler::Uninit(void)
{
	// 頂点バッファの解放
	if (m_pVtx != nullptr)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}

	// インデックスバッファの解放
	if (m_pIdx != nullptr)
	{
		m_pIdx->Release();
		m_pIdx = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMeshPiler::Update(void)
{
	// 経過時間をカウント
	m_MeshPiler.nLifeFrame++;

	// 減算
	m_MeshPiler.nLife--;

	// 角度回転
	 m_rot.y += 0.45f;

	 // 正規化関数
	 m_rot.y = NormalAngle(m_rot.y);

	// 高さを増加させる
	 m_MeshPiler.fNowHeight += PILERINFO::MOVESPEED;

	// 最大値で止める
	if (m_MeshPiler.fNowHeight > PILERINFO::VALUEHEIGHT)
		m_MeshPiler.fNowHeight = PILERINFO::VALUEHEIGHT;

	// 寿命が0以下になったら
	if (m_MeshPiler.nLife <= NULL)
	{
		// 終了処理
		Uninit();

		// 処理終了
		return;
	}

	// 頂点情報を更新
	VERTEX_3D* pVtx = nullptr;

	// ロックに成功したら
	if (SUCCEEDED(m_pVtx->Lock(0, 0, (void**)&pVtx, 0)))
	{
		int nCnt = 0;

		for (int nCntZ = 0; nCntZ <= PILERINFO::NUMZ; nCntZ++)
		{
			for (int nCntX = 0; nCntX <= PILERINFO::NUMX; nCntX++)
			{
				float fAngle = (D3DX_PI * 2.0f) / PILERINFO::NUMX * nCntX;

				// 高さをm_fNowHeightに置く
				float fHeight = (float)nCntZ * m_MeshPiler.fNowHeight;

				pVtx[nCnt].pos = D3DXVECTOR3(
					sinf(fAngle) * PILERINFO::COLLISIONRADIUS,
					fHeight,
					cosf(fAngle) * PILERINFO::COLLISIONRADIUS
				);

				nCnt++;
			}
		}

		// アンロック
		m_pVtx->Unlock();
	}

	//計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスの行列計算
	m_mtxWorld = mtxRot * mtxTrans;
}
//=========================================================
// 描画処理
//=========================================================
void CMeshPiler::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdx);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ読み込み
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャセット
	pDevice->SetTexture(0, pTexture->GetAddress(m_MeshPiler.nTexIdx));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_MeshPiler.nNumAllVtx, 0, m_MeshPiler.nNumPrimitive);

	//テクスチャを戻す
	pDevice->SetTexture(0, nullptr);
}

//=========================================================
// 当たり判定処理
//=========================================================
bool CMeshPiler::Collision(D3DXVECTOR3* CollisionPos)
{
	// 現在座標の取得
	D3DXVECTOR3 NowPos = GetPos();

	// XZ平面での距離を求める
	D3DXVECTOR2 diffXZ = D3DXVECTOR2(CollisionPos->x - NowPos.x,CollisionPos->z - NowPos.z);
	float fRangeXZ = D3DXVec2Length(&diffXZ);

	// 高さ範囲をチェック
	float fMinY = NowPos.y;
	float fMaxY = NowPos.y + PILERINFO::VALUEHEIGHT;

	if (fRangeXZ < PILERINFO::COLLISIONRADIUS &&
		CollisionPos->y >= fMinY && CollisionPos->y <= fMaxY)
	{
		// 終了処理
		Uninit();

		// 当たった判定を返す
		return true;
	}

	return false;
}
//=========================================================
// テクスチャ割り当て処理
//=========================================================
void CMeshPiler::SetTexture(void)
{
	// テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャ割り当て
	m_MeshPiler.nTexIdx = pTexture->Register("data/TEXTURE/MeshPiler.jpg");
}