//=========================================================
//
// ダメージポップアップ処理 [ damegapop.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "scorepop.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CScorePop::CScorePop(int nPriority) : CObject(nPriority),
m_col(COLOR_WHITE),
m_fHeight(NULL),
m_fWidth(NULL),
m_nDamage(NULL),
m_nLifeCount(NULL),
m_pDamagePopNumber{},
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CScorePop::~CScorePop()
{

}
//=========================================================
// 生成処理
//=========================================================
CScorePop* CScorePop::Create
(
	const D3DXVECTOR3& pos, 
	int nDamage, 
	const float& fWidth, 
	const float& fHeight
)
{
	// インスタンス生成
	CScorePop* pNewPop = new CScorePop;
	if (pNewPop == nullptr) return nullptr;

	// 値の設定
	pNewPop->SetPop(nDamage);
	pNewPop->SetPos(pos);
	pNewPop->SetWidth(fWidth);
	pNewPop->SetHeight(fHeight);
	
	// 初期化失敗時
	if (FAILED(pNewPop->Init())) return nullptr;

	// ポインタを返す
	return pNewPop;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CScorePop::Init(void)
{
	// ローカル配列に格納
	std::vector<int> digitValues;

	// ダメージ設定数を格納
	int nDamage = m_nDamage;
	if (nDamage == 0) digitValues.push_back(0);

	while (nDamage > 0)
	{
		// 値を先頭に割り込んで追加していく
		digitValues.insert(digitValues.begin(), nDamage % Config::NUM_DIGIT);

		// 桁を計算
		nDamage /= Config::NUM_DIGIT;
	}

	// 配列の最大サイズを格納
	int numDigits = digitValues.size();

	// 寿命とカラーの設定
	m_nLifeCount = Config::MAX_LIFE_FRAME;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// デバイスとマトリックス
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXVECTOR3 camRight(mtxView._11, mtxView._21, mtxView._31);

	// 全体の横幅
	float totalWidth = numDigits * m_fWidth;

	// 確定した桁数分、要素を動的配列に生成
	for (int nCnt = 0; nCnt < numDigits; nCnt++)
	{
		// インスタンス生成
		CNumber3D* pNum3D = new CNumber3D;

		// 横幅のオフセット
		float offsetX = (nCnt * m_fWidth * 2.0f) - (totalWidth * 0.5f) + (m_fWidth * 0.5f);
		D3DXVECTOR3 initDigitPos = m_pos + camRight * offsetX;

		// 割り出された正しい並び位置で初期化
		pNum3D->Init(initDigitPos, m_fWidth, m_fHeight);

		// サイズの設定
		pNum3D->SetSize(m_fWidth, m_fHeight);

		// 桁数をセット
		pNum3D->SetDigit(digitValues[nCnt]);

		// テクスチャセット
		pNum3D->SetTexture(Config::TEX_NAME);

		// 初期カラー設定
		pNum3D->SetCol(m_col);

		// 使用フラグ設定
		pNum3D->SetIsUse(true);

		// 配列へ追加
		m_pDamagePopNumber.push_back(pNum3D);
	}
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CScorePop::Uninit(void)
{
	// 配列内の配列の要素の破棄
	for (auto number : m_pDamagePopNumber)
	{
		if (number != nullptr)
		{
			number->Uninit();
			delete number;
			number = nullptr;
		}
	}
	
	// クラスの破棄
	m_pDamagePopNumber.clear();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CScorePop::Update(void)
{
	// 寿命チェック
	m_nLifeCount--;
	if (m_nLifeCount <= 0)
	{
		Uninit();
		return;
	}

	// 上昇値
	m_pos.y += Config::UP_VALUE;

	// 自身の座標を設定する
	this->SetPos(m_pos);

	// フェードアウト処理
	if (m_nLifeCount < Config::INVISIVLE_COUNT)
	{
		// 透明度をだんだん下げる
		m_col.a = static_cast<float>(m_nLifeCount) / 30.0f;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView;

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXVECTOR3 camRight(mtxView._11, mtxView._21, mtxView._31);

	// 桁数を取得
	int numDigits = m_pDamagePopNumber.size();
	float totalWidth = numDigits * m_fWidth;

	// ローカルの配列
	std::vector<int> currentDigits;
	int tempDamage = m_nDamage;

	// 初期化時と同じ設定
	if (tempDamage == 0) currentDigits.push_back(0);
	while (tempDamage > 0)
	{
		currentDigits.insert(currentDigits.begin(), tempDamage % Config::NUM_DIGIT);
		tempDamage /= Config::NUM_DIGIT;
	}

	// 描画位置の更新ループ
	for (int nCnt = 0; nCnt < numDigits; nCnt++)
	{
		if (m_pDamagePopNumber[nCnt] == nullptr) continue;

		float offsetX = (nCnt * m_fWidth * 1.2f) - (totalWidth * 0.5f) + (m_fWidth * 0.5f);
		D3DXVECTOR3 targetDigitPos = m_pos + camRight * offsetX;

		// 数字の座標をずらす
		m_pDamagePopNumber[nCnt]->SetPos(targetDigitPos);
		m_pDamagePopNumber[nCnt]->SetCol(m_col);

		// 数字の桁数更新
		m_pDamagePopNumber[nCnt]->SetDigit(currentDigits[nCnt]);
		m_pDamagePopNumber[nCnt]->Update();
	}
}
//=========================================================
// 描画処理
//=========================================================
void CScorePop::Draw(void)
{
	// 数字の描画
	for (auto number : m_pDamagePopNumber)
	{
		if (number != nullptr && number->GetIsUse())
		{
			number->SetMtx();
			number->Draw();
		}
	}
}