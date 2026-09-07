//========================================================
//
// クライアント役の複数管理処理 [ clientmanager.cpp ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "clientmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************

//=========================================================
// 髙橋追加
//=========================================================
#include "client.h"
#include "outsideUI.h"
#include "titleuimanager.h"
#include "tutorialuimanager.h"
#include "pointobject.h"

//*********************************************************
// 名前空間 [髙橋追加]
//*********************************************************
namespace Client
{
	const float UI_HEIGHT = 75.0f;								// UIの高さ
	const float ARROW_HEIGHT = 125.0f;							// 矢印の高さ
	constexpr const char* UI_FBUTTON = "Fbutton.png";			// (キーボード)uiのテクスチャ名
	constexpr const char* UI_STARTBUTTON = "startbutton.png";	// (パッド)uiのテクスチャ名
};

//========================================================
// コンストラクタ
//========================================================
CClientManager::CClientManager() : m_pClient{},
m_pPointObject{},
m_pOutsideUI{}
{

}

//========================================================
// デストラクタ
//========================================================
CClientManager::~CClientManager()
{
}

//========================================================
// 初期化処理
//========================================================
HRESULT CClientManager::Init(void)
{
	// ポイントのクリア
	m_pClient.clear();

	//****************************************************
	// 髙橋追加
	//***************************************************

	// 人数分生成する
	for (int nCount = 0; nCount < CLIENT_MAX; nCount++)
	{
		// クライアントの生成
		CreateInManager(m_pos[nCount], m_rot[nCount]);
	}

	// 操作の種類を設定する(パッドかキーマウかどうか)
	int nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();

	// 使用するテクスチャの名前を保存する変数
	const char* TextureName = {};

	if (nControlTypes == CTutorialUIManager::CONTROLTYPE_KEY)
	{// キーマウ操作の場合
		TextureName = Client::UI_FBUTTON;
	}
	else if (nControlTypes == CTutorialUIManager::CONTROLTYPE_PAD)
	{// パッド操作の場合
		TextureName = Client::UI_STARTBUTTON;
	}

	// 外出タスク用UIと矢印の生成
	for (int nCount = 0; nCount < CLIENT_MAX; nCount++)
	{
		// 位置
		D3DXVECTOR3 pos = m_pos[nCount];

		// UI分の高さを加算する
		pos.y = m_pos[nCount].y + Client::UI_HEIGHT;

		// UIの生成
		m_pOutsideUI[nCount] = COutsideUI::Create(pos, VECTOR3_NULL, TextureName);

		// 矢印分の高さを加算する
		pos.y = m_pos[nCount].y + Client::ARROW_HEIGHT;

		// クライアントの位置を示す矢印の生成
		m_pPointObject[nCount] = CPointObject::Create(pos,
			D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f),
			D3DXVECTOR3(HALF, HALF, HALF),
			"STAGEOBJ/yajirusi.x");

		// 非表示の状態にする
		m_pPointObject[nCount]->SetIsDraw(false);
		m_pOutsideUI[nCount]->SetDrawFlags(false);
	}
	
	return S_OK;
}

//========================================================
// 終了処理
//========================================================
void CClientManager::Uninit(void)
{
	// ポイントのクリア
	m_pClient.clear();
}

//========================================================
// 更新処理
//========================================================
void CClientManager::Update(void)
{

}

//========================================================
// クライアントの設定処理
//========================================================
void CClientManager::SetClient(void)
{
	// 乱数の種
	srand((unsigned int)time(0));

	int nNowClient = rand() % CLIENT_MAX;

	// 表示する状態にする
	m_pPointObject[nNowClient]->SetIsDraw(true);
	m_pOutsideUI[nNowClient]->SetDrawFlags(true);
}

//========================================================
// クライアントの削除処理
//========================================================
void CClientManager::ClearClient(void)
{
	for (int nCount = 0; nCount < CLIENT_MAX; nCount++)
	{
		// 非表示の状態にする
		m_pPointObject[nCount]->SetIsDraw(false);
		m_pOutsideUI[nCount]->SetDrawFlags(false);
	}
}

//========================================================
// マネージャー内での生成処理
//========================================================
void CClientManager::CreateInManager(const D3DXVECTOR3& POS, const D3DXVECTOR3& ROT)
{
	// 新規生成
	m_pClient.push_back(CClient::Create(POS, ROT));
}