//=========================================================
//
// タスクUIマネージャー処理 [ deskworkUImanager.cpp ]
// Author: Takahashi Misaki
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "deskworkUImanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"

//=========================================================
// コンストラクタ
//=========================================================
CDeskworkUIManager::CDeskworkUIManager()
{

}

//=========================================================
// デストラクタ
//=========================================================
CDeskworkUIManager::~CDeskworkUIManager()
{

}

//=========================================================
// 生成処理処理
//=========================================================
CDeskworkUIManager* CDeskworkUIManager::Create(const D3DXVECTOR3& pos)
{
	static CDeskworkUIManager pDeskworkUIManager;

	// 設定処理
	pDeskworkUIManager.SetPos(pos);

	// 初期化が失敗した場合
	if (FAILED(pDeskworkUIManager.Init())) return nullptr;

	return &pDeskworkUIManager;
}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CDeskworkUIManager::Init(void)
{
	srand((unsigned int)time(0));

	// 現在選択されているUIの番号の初期化
	m_nNowIdx = 0;

	// クールタイムが始まっていない状態にする
	m_bTime = false;

	// 位置
	D3DXVECTOR3 pos = m_pos;
	pos.x = m_pos.x - Config::VALUE_WIDTH;

	// キーの種類
	CDeskworkUI::KEYTYPE keytype[Config::UI_NUM];

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクをランダムに設定
		keytype[nCount] = (CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX);

		// UIの生成処理
		m_pDeskUI[nCount] = CDeskworkUI::Create(pos, Config::UI_WIDTH, Config::UI_HEIGHT, keytype[nCount], nCount);

		// 色を透明にする
		m_pDeskUI[nCount]->ChangeCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// UIの位置をずらす
		pos.x += Config::VALUE_WIDTH;
	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CDeskworkUIManager::Uninit(void)
{

}

//=========================================================
// 更新処理
//=========================================================
void CDeskworkUIManager::Update(void)
{
	srand((unsigned int)time(0));

	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pKeyboard == nullptr)
	{// ヌルチェック
		return;
	}

	if (m_bTime == false)
	{// クールタイムが始まっていないなら

		// 現在のタスクUIの更新処理
		m_pDeskUI[m_nNowIdx]->Update();

		if ((pKeyboard->GetTrigger(DIK_W) == true && m_pDeskUI[m_nNowIdx]->GetKyeType() == CDeskworkUI::DRAWTYPE_W) ||
			(pKeyboard->GetTrigger(DIK_A) == true && m_pDeskUI[m_nNowIdx]->GetKyeType() == CDeskworkUI::DRAWTYPE_A) ||
			(pKeyboard->GetTrigger(DIK_S) == true && m_pDeskUI[m_nNowIdx]->GetKyeType() == CDeskworkUI::DRAWTYPE_S) ||
			(pKeyboard->GetTrigger(DIK_D) == true && m_pDeskUI[m_nNowIdx]->GetKyeType() == CDeskworkUI::DRAWTYPE_D))
		{// 正解を押した時

			// 色を半透明にする
			m_pDeskUI[m_nNowIdx]->ChangeCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));

			// 次のタスクに移る
			m_nNowIdx++;
		}

		// タスクが終わっていないなら
		if (m_nNowIdx < Config::UI_NUM)
		{
			return;
		}

	}
	
	// クールタイムを始める
	m_bTime = true;

	if (m_nCountTime <= Config::TIME_COOL)
	{// クールタイムを数える
		m_nCountTime++;

		return;
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// タスクをランダムに設定
		m_pDeskUI[nCount]->SetKyeType((CDeskworkUI::KEYTYPE)(rand() % CDeskworkUI::DRAWTYPE_MAX));

		// 色を不透明にする
		m_pDeskUI[nCount]->ChangeCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	// 現在選択している番号を初期化
	m_nNowIdx = 0;

	// クールタイムを初期化
	m_nCountTime = 0;

	// クールタイムが始まっていない状態にする
	m_bTime = false;
}

//=========================================================
// 描画処理
//=========================================================
void CDeskworkUIManager::Draw(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの描画処理
		m_pDeskUI[nCount]->Draw();
	}
}

//=========================================================
// 透明度の処理
//=========================================================
void CDeskworkUIManager::SetAlphaUI(const bool bUse)
{
	if (bUse == false)
	{
		for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
		{
			// 色を透明にする
			m_pDeskUI[nCount]->ChangeCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

		return;
	}

	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		if (nCount < m_nNowIdx)
		{
			// 色を半透明にする
			m_pDeskUI[nCount]->ChangeCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));

			continue;
		}

		// 色を不透明にする
		m_pDeskUI[nCount]->ChangeCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
