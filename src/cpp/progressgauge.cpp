//=========================================================
//
// 進捗ゲージの処理 [ progressgauge.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "progressgauge.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CProgressgauge::CProgressgauge(int nPriority) :CObject(nPriority),
m_pVtxBuff(NULL),			// 頂点バッファ
m_pos(VECTOR3_NULL),		// 位置
m_Leftcol(COLOR_WHITE),		// 左ゲージの色
m_Rightcol(COLOR_WHITE),	// 右ゲージの色
m_fWidth(NULL),				// 横幅
m_fHeight(NULL),			// 縦幅
m_nTask(NULL),				// タスクをこなした数
m_nAFK(NULL),				// さぼりをこなした数
m_nIdxTexture(NULL)			// テクスチャの番号
{

}

//=========================================================
// デストラクタ
//=========================================================
CProgressgauge::~CProgressgauge()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CProgressgauge* CProgressgauge::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// 進捗ゲージのポインタ
	CProgressgauge* pProgressgauge = new CProgressgauge;

	// ヌルチェック
	if (pProgressgauge == nullptr) return nullptr;

	// 進捗ゲージの位置設定
	pProgressgauge->SetPos(pos);
	pProgressgauge->SetOffsetPos(pos);
	pProgressgauge->SetWidth(fWidth);
	pProgressgauge->SetHeight(fHeight);
	pProgressgauge->SetLeftCol(COLOR_GREEN);
	pProgressgauge->SetRightCol(COLOR_RED);
	pProgressgauge->SetTexture("gage000.jpg");

	// 初期化が失敗した場合
	if (FAILED(pProgressgauge->Init())) return nullptr;

	return pProgressgauge;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CProgressgauge::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (BASEVERTEX * 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 左右のゲージの長さ
	float fLeftWidth = m_Offsetpos.x - m_fWidth;
	float fRightWidth = m_Offsetpos.x + m_fWidth;

	// 頂点座標の設定
	// 左端の頂点座標
	pVtx[0].pos = D3DXVECTOR3(fLeftWidth, m_Offsetpos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fLeftWidth, m_Offsetpos.y + m_fHeight, 0.0f);

	// 境界線の頂点座標
	pVtx[1].pos = D3DXVECTOR3(m_Offsetpos.x, m_Offsetpos.y - m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Offsetpos.x, m_Offsetpos.y + m_fHeight, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(m_Offsetpos.x, m_Offsetpos.y - m_fHeight, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(m_Offsetpos.x, m_Offsetpos.y + m_fHeight, 0.0f);

	// 右端の頂点座標
	pVtx[5].pos = D3DXVECTOR3(fRightWidth, m_Offsetpos.y - m_fHeight, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(fRightWidth, m_Offsetpos.y + m_fHeight, 0.0f);

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw =
	pVtx[4].rhw =
	pVtx[5].rhw = 
	pVtx[6].rhw =
	pVtx[7].rhw = 1.0f;

	// 頂点カラーの設定
	// 左ゲージの色
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = m_Leftcol;
	
	// 右ゲージの色
	pVtx[4].col = 
	pVtx[5].col = 
	pVtx[6].col = 
	pVtx[7].col = m_Rightcol;

	// UV設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[6].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CProgressgauge::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}

//=========================================================
// 更新処理
//=========================================================
void CProgressgauge::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	// 境界線の頂点座標
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);

	// 頂点カラーの設定
	// 左ゲージの色
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_Leftcol;

	// 右ゲージの色
	pVtx[4].col =
	pVtx[5].col =
	pVtx[6].col =
	pVtx[7].col = m_Rightcol;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=========================================================
// 描画処理
//=========================================================
void CProgressgauge::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ戻す
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 6);

}

//==========================================================
// テクスチャ処理
//==========================================================
void CProgressgauge::SetTexture(const char* pTexName)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パス設定
	std::string TexPath = "data/TEXTURE/";
	TexPath += pTexName;

	// テクスチャ割り当て
	m_nIdxTexture = pTexture->Register(TexPath.c_str());
}

//==========================================================
// 点滅処理
//==========================================================
void CProgressgauge::SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col)
{

}

//==========================================================
// タスクの数加算処理
//==========================================================
void CProgressgauge::AddTask(void)
{
	// タスクの数と総数を加算
	m_nTask++;

	// 境界線の位置の計算
	float fDiffWidth = m_fWidth * 0.03f;

	// 境界線の位置をずらす
	m_pos.x += fDiffWidth;

	if (m_pos.x >= m_Offsetpos.x + m_fWidth)
	{// 境界線がゲージの端を超える時

		m_pos.x = m_Offsetpos.x + m_fWidth;
	}

}

//==========================================================
// さぼりの数加算処理
//==========================================================
void CProgressgauge::AddAFK(void)
{
	// さぼりの数と総数を加算
	m_nAFK++;

	// 境界線の位置の計算
	float fDiffWidth = m_fWidth * 0.03f;

	// 境界線の位置をずらす
	m_pos.x -= fDiffWidth;

	if (m_pos.x <= m_Offsetpos.x - m_fWidth)
	{// 境界線がゲージの端を超える時

		m_pos.x = m_Offsetpos.x - m_fWidth;
	}
}
