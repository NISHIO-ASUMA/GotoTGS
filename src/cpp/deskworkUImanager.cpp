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
	Uninit();
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
	// 位置
	D3DXVECTOR3 pos = m_pos;
	pos.x = m_pos.x - Config::VALUE_WIDTH;

	// キーの種類
	CDeskworkUI::KEYTYPE keytype[Config::UI_NUM];
	keytype[0] = CDeskworkUI::KEYTYPE::DRAWTYPE_W;
	keytype[1] = CDeskworkUI::KEYTYPE::DRAWTYPE_S;
	keytype[2] = CDeskworkUI::KEYTYPE::DRAWTYPE_D;


	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{
		// UIの生成処理
		m_pDeskworkUI[nCount] = CDeskworkUI::Create(pos, Config::UI_WIDTH, Config::UI_HEIGHT, keytype[nCount], nCount);

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
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの終了処理
		m_pDeskworkUI[nCount] = nullptr;
	}
}

//=========================================================
// 更新処理
//=========================================================
void CDeskworkUIManager::Update(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの更新処理
		m_pDeskworkUI[nCount]->Update();
	}
}

//=========================================================
// 描画処理
//=========================================================
void CDeskworkUIManager::Draw(void)
{
	for (int nCount = 0; nCount < Config::UI_NUM; nCount++)
	{// タスクUIの描画処理
		m_pDeskworkUI[nCount]->Draw();
	}
}