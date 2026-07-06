//=========================================================
//
// 会議室同僚処理 [ meetingfriend.cpp ]
// Author: Shouya Chikada
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "meetingfriend.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "motion.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace FRIENDINFO
{
	constexpr int CHANGETIME_MAX = 600; // モーション変更カウント 
	constexpr float OUTLINESIZE = 0.96f; // アウトラインサイズ
};

//=========================================================
// コンストラクタ
//=========================================================
CMeetingFriend::CMeetingFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeTime(NULL),
m_MotionPath{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMeetingFriend::~CMeetingFriend()
{

}
//=========================================================
// 生成処理
//=========================================================
CMeetingFriend* CMeetingFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const std::string& Path)
{
	// インスタンス生成
	CMeetingFriend* pFriend = new CMeetingFriend;
	if (pFriend == nullptr)return nullptr;

	// オブジェクト設定
	pFriend->SetPos(pos);
	pFriend->SetRot(rot);
	pFriend->SetMotionPath(Path);
	pFriend->SetUseOutLine(true);
	pFriend->SetOutLineSize(FRIENDINFO::OUTLINESIZE);
	pFriend->SetOutLineColor();

	// 初期化失敗時
	if (FAILED(pFriend->Init()))return nullptr;

	return pFriend;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMeetingFriend::Init(void)
{
	// 親クラスの初期化処理
	CNoMoveCharactor::Init();

	// モーション読み込み
	MotionLoad(m_MotionPath.c_str(), MAX, false);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMeetingFriend::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CMeetingFriend::Update(void)
{
	// 一定間隔でモーションのランダム更新
	m_nChangeTime++;

	// 10秒たったら更新
	if (m_nChangeTime >= FRIENDINFO::CHANGETIME_MAX)
	{
		// ローカルランダム数
		int nRand = rand() % MOTION::MAX;

		// モーションセット
		GetMotion()->SetMotion(nRand, true, 10);

		// 変更時間を初期化する
		m_nChangeTime = NULL;
	}

	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CMeetingFriend::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}