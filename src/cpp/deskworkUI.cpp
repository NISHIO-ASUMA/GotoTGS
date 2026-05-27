//=========================================================
//
// タスクUI処理 [ deskworkUI.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "deskworkUI.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CDeskworkUI::CDeskworkUI(int nPriority) : CObject(nPriority)
{

}

//=========================================================
// デストラクタ
//=========================================================
CDeskworkUI::~CDeskworkUI()
{


}

//=========================================================
// 生成処理処理
//=========================================================
CDeskworkUI* CDeskworkUI::Create(const UI& ui)
{
	// インスタンス生成
	CDeskworkUI* pDeskworkUI = new CDeskworkUI;

	// ヌルチェック
	if (pDeskworkUI == nullptr) return nullptr;

	// 各種値の設定
	pDeskworkUI->SetPos(ui.pos);			// 位置
	pDeskworkUI->SetCol(ui.col);			// 色
	pDeskworkUI->SetWidth(ui.fWidth);		// 横幅
	pDeskworkUI->SetHeight(ui.fHeight);		// 縦幅
	pDeskworkUI->SetDigit(ui.fDigit);		// 分割数
	pDeskworkUI->SetKeyType(ui.nKeytype);	// キーの種類
	pDeskworkUI->SetIdx(ui.nIdx);			// 番号
	pDeskworkUI->SetVTXtype(ui.VTXtype);	// 頂点ポイント

	if (ui.nKeytype != DRAWTYPE_NONE)
	{// キータイプを指定しているなら
		// テクスチャの設定
		pDeskworkUI->SetTexture(Config::TEXNAME_KEYTYPE);
	}
	else
	{
		// テクスチャの設定
		pDeskworkUI->SetTexture(Config::TEXNAME_GAGE);
	}

	// 初期化が失敗した場合
	if (FAILED(pDeskworkUI->Init())) return nullptr;

	return pDeskworkUI;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskworkUI::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポイントの設定処理
	SetVTXtype(m_UI.VTXtype);

	// rhwの設定(1.0fで固定)
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_UI.col;

	// UV設定
	SetDigit(m_UI.nKeytype, m_UI.fDigit);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskworkUI::Uninit(void)
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
void CDeskworkUI::Update(void)
{
	// 位置設定
	SetPos(m_UI.pos);

	// サイズ設定
	SetSize(m_UI.fWidth, m_UI.fHeight);

	// UV設定
	SetDigit(m_UI.nKeytype, m_UI.fDigit);

}

//=========================================================
// 描画処理
//=========================================================
void CDeskworkUI::Draw(void)
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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//==========================================================
// テクスチャ処理
//==========================================================
void CDeskworkUI::SetTexture(const char* pTexName)
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
// サイズ処理
//==========================================================
void CDeskworkUI::SetSize(const float& fWidth, const float& fHeight)
{
	// メンバに格納
	m_UI.fWidth = fWidth;
	m_UI.fHeight = fHeight;

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_UI.VTXtype)
	{
	case VTXTYPE_CENTER:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	case VTXTYPE_LEFT:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x, m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	case VTXTYPE_RIGHT:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x, m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//===========================================================
// カラー設定
//===========================================================
void CDeskworkUI::ChangeCol(const D3DXCOLOR& col)
{
	// メンバに格納
	m_UI.col = col;

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーを設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_UI.col;

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
// 点滅処理
//==========================================================
void CDeskworkUI::SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーカウントを加算
	n_nColorCount++;

	// 点滅する周期を計算する
	int nCycle = nEndFrame - nStartFrame;
	if (nCycle <= 0) nCycle = 1;

	// 進行度を設定
	float fProgress = static_cast<float>((n_nColorCount - nStartFrame) % nCycle) / static_cast<float>(nCycle);

	// 透明度を格納する
	float alpha = NULL;

	if (fProgress < HALF)
	{
		// 線形補間
		alpha = Lerp(HALF, Config::END_FLOAT, fProgress * Config::RATIO);
	}
	else
	{
		// 線形補間
		alpha = Lerp(Config::END_FLOAT, HALF, (fProgress - HALF) * Config::RATIO);
	}

	// カラー設定
	D3DXCOLOR ChangeCol(col.r, col.g, col.b, alpha);

	// 現在カラーに適用
	SetCol(ChangeCol);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
// UV設定処理
//==========================================================
void CDeskworkUI::SetDigit(const int& nType, const float& nDigit)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の計算
	float fTexU = (float)(nType) * nDigit;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU + nDigit, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexU, Config::END_FLOAT);
	pVtx[3].tex = D3DXVECTOR2(fTexU + nDigit, Config::END_FLOAT);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//==========================================================
// 頂点ポイント処理
//==========================================================
void CDeskworkUI::SetVTX(const VTXTYPE& VTXtype)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (VTXtype)
	{
	case VTXTYPE_CENTER:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	case VTXTYPE_LEFT:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x			  , m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x			  , m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x + m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	case VTXTYPE_RIGHT:

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_UI.pos.x			   , m_UI.pos.y - m_UI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_UI.pos.x - m_UI.fWidth, m_UI.pos.y + m_UI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_UI.pos.x			   , m_UI.pos.y + m_UI.fHeight, 0.0f);

		break;

	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

}