//=========================================================
//
// エフェクト処理 [ effect.cpp ]
// Author : Kaiti Aoto
//
//=========================================================

//**********************************************************
// クラス定義ヘッダーファイル
//**********************************************************
#include "effect.h"

//**********************************************************
// インクルードファイル
//**********************************************************
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CEffect::CEffect(int nPriority):CObject(nPriority),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_move(VECTOR3_NULL),
m_pVtxBuff(nullptr),
m_fRadius(NULL),
m_nIdxTex(NULL),
m_nLife(NULL),
m_bUse(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEffect::~CEffect()
{

}
//=========================================================
// 生成処理
//=========================================================
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& move, const D3DXCOLOR& col, int nLife, float fRadius, TYPE type)
{
	// インスタンス生成
	CEffect* pEffect = new CEffect;
	if (pEffect == nullptr) return nullptr;
	
	pEffect->SetPos(pos);
	pEffect->SetRot(rot);
	pEffect->SetMove(move);
	pEffect->SetCol(col);
	pEffect->SetLife(nLife);
	pEffect->SetRadius(fRadius);
	pEffect->SetType(type);
	pEffect->Init();
	
	// 初期化失敗時
	if (FAILED(pEffect->Init())) return nullptr;

	return pEffect;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEffect::Init(void)
{
	m_bUse = true;

	//デバイスの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(- m_fRadius,+ m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ m_fRadius,+ m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- m_fRadius,- m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ m_fRadius,- m_fRadius, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetInstance()->GetTexture();
	if (m_Type == TYPE_NONE)
	{
		m_nIdxTex = pTex->Register("data/TEXTURE/effect000.jpg");
	}
	else if (m_Type == TYPE_THUNDER)
	{
		m_nIdxTex = pTex->Register("data/TEXTURE/effect001.png");
	}
	
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEffect::Uninit(void)
{
	m_bUse = false;

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CEffect::Update(void)
{
	if (!m_bUse)
	{
		Uninit();
		return;
	}

	m_pos += m_move;

	m_fRadius -= 0.2f;
	if (m_fRadius <= 0.0f)
	{
		m_fRadius = 0.0f;
		m_bUse = false;
		return;
	}

	m_col.a -= 0.01f;
	if (m_col.a <= 0.0f)
	{
		m_col.a = 0.0f;
		m_bUse = false;
		return;
	}

	m_nLife--;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_bUse = false;
	}

	SetSize(m_fRadius);
	SetColor(m_col);
}
//=========================================================
// 描画処理
//=========================================================
void CEffect::Draw(void)
{
	// デバイス取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ取得
	CTexture* pTex = CManager::GetInstance()->GetTexture();

	if (m_bUse == true)
	{
		//ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//Ｚテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//ALPHAテストの設定
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		D3DXMATRIX mtxView;
		//ビューマトリックス取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//カメラの逆行列を設定
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex));

		//ポリゴンを描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// Z
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// 加算合成
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Alpha
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		//ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}
//=========================================================
// 大きさを設定する関数
//=========================================================
void CEffect::SetSize(float fRadius)
{
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(- fRadius,+ fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ fRadius,+ fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- fRadius,- fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ fRadius,- fRadius, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 色を設定する関数
//=========================================================
void CEffect::SetColor(const D3DXCOLOR& col)
{
	m_col = col;
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}