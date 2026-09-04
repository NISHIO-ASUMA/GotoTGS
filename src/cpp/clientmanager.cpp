////========================================================
////
//// クライアント役の複数管理処理 [ clientmanager.cpp ]
//// Author: Asuma Nishio
////
////========================================================
//
////*********************************************************
//// クラス定義ヘッダーファイル
////*********************************************************
//#include "clientmanager.h"
//
////*********************************************************
//// インクルードファイル
////*********************************************************
//
////=========================================================
//// 髙橋追加
////=========================================================
//#include "client.h"
//#include "outsideUI.h"
//#include "titleuimanager.h"
//#include "tutorialuimanager.h"
//
////*********************************************************
//// 名前空間 [髙橋追加]
////*********************************************************
//namespace Client
//{
//	const int Num = 3;											// 総数
//	const D3DXVECTOR3 POS[Num] = {};
//	//POS[0] = { 1245.0f, 0.0f, 461.0f };
//	const D3DXVECTOR3 No1_POS = { 1245.0f, 0.0f, 461.0f };		// 1人目の位置
//	const D3DXVECTOR3 No1_ROT = { 0.0f, 0.0f, 0.0f };			// 1人目の向き
//	const D3DXVECTOR3 No2_POS = { 2120.0f, 0.0f, -920.0f };		// 2人目の位置
//	const D3DXVECTOR3 No2_ROT = { 0.0f, 0.0f, 0.0f };			// 2人目の向き
//	const D3DXVECTOR3 No3_POS = { 2160.0f, 0.0f, 1100.0f };		// 3人目の位置
//	const D3DXVECTOR3 No3_ROT = { 0.0f, 0.0f, 0.0f };			// 3人目の向き
//	const int UI_HEIGHT = 75.0f;								// UIの高さ
//	const int ARROW_HEIGHT = 75.0f;								// 矢印の高さ
//	constexpr const char* UI_FBUTTON = "Fbutton.png";			// (キーボード)uiのテクスチャ名
//	constexpr const char* UI_STARTBUTTON = "startbutton.png";	// (パッド)uiのテクスチャ名
//};
//
////========================================================
//// コンストラクタ
////========================================================
//CClientManager::CClientManager() : m_pClient{}
//{
//
//}
//
////========================================================
//// デストラクタ
////========================================================
//CClientManager::~CClientManager()
//{
//}
//
////========================================================
//// 初期化処理
////========================================================
//HRESULT CClientManager::Init(void)
//{
//	// ポイントのクリア
//	m_pClient.clear();
//
//	//****************************************************
//	// 髙橋追加
//	//***************************************************
//
//	// クライアントの生成
//	CreateInManager(Client::No1_POS, Client::No1_ROT);	// 1人目
//	CreateInManager(Client::No2_POS, Client::No2_ROT);	// 2人目
//	CreateInManager(Client::No3_POS, Client::No3_ROT);	// 3人目
//
//	// 位置
//	D3DXVECTOR3 Offset_pos[Client::Num] = {};
//
//	// 位置を代入する
//	Offset_pos[0] = Client::No1_POS;
//	Offset_pos[1] = Client::No2_POS;
//	Offset_pos[2] = Client::No3_POS;
//
//	// 向き
//	D3DXVECTOR3 Offset_rot[Client::Num] = {};
//
//	// 向きを代入する
//	Offset_rot[0] = Client::No1_ROT;
//	Offset_rot[1] = Client::No2_ROT;
//	Offset_rot[2] = Client::No3_ROT;
//
//	//if ()
//	//{
//
//	//}
//
//	// 操作の種類を設定する(パッドかキーマウかどうか)
//	int nControlTypes = CTitleuiManager::GetInstance()->GetSelectIdx();
//
//	// 使用するテクスチャの名前を保存する変数
//	const char* TextureName = {};

//	if (nControlTypes == CTutorialUIManager::CONTROLTYPE_KEY)
//	{// キーマウ操作の場合
//		TextureName = Client::UI_FBUTTON;
//	}
//	else if (nControlTypes == CTutorialUIManager::CONTROLTYPE_PAD)
//	{// パッド操作の場合
//		TextureName = Client::UI_STARTBUTTON;
//	}
//
//	// 外出タスク用UIと矢印の生成
//	for (int nCount = 0; nCount < Client::Num; nCount++)
//	{
//		// 位置
//		D3DXVECTOR3 pos = Offset_pos[nCount];
//
//		// UI分の高さを加算する
//		pos.y = Offset_pos[nCount].y + Client::UI_HEIGHT;
//
//		// UIの生成
//		COutsideUI::Create(pos, VECTOR3_NULL, TextureName);
//
//		// 矢印分の高さを加算する
//		pos.y = Offset_pos[nCount].y + Client::ARROW_HEIGHT;
//
//		//// クライアントの位置を示す矢印の生成
//		//m_pPointObject = CPointObject::Create(D3DXVECTOR3(1245.20f, 130.0f, 461.35f),
//		//	D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f),
//		//	D3DXVECTOR3(HALF, HALF, HALF),
//		//	"STAGEOBJ/yajirusi.x");
//
//	}
//	
//	return S_OK;
//}
//
////========================================================
//// 終了処理
////========================================================
//void CClientManager::Uninit(void)
//{
//	// ポイントのクリア
//	m_pClient.clear();
//}
//
////========================================================
//// 更新処理
////========================================================
//void CClientManager::Update(void)
//{
//
//}
//
////========================================================
//// 生成処理
////========================================================
//void CClientManager::CreateInManager(const D3DXVECTOR3& POS, const D3DXVECTOR3& ROT)
//{
//	// 新規生成
//	m_pClient.push_back(CClient::Create(POS, ROT));
//}