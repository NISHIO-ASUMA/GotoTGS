//========================================================
//
// 煙草吸っている同僚の処理 [ smokefriend.cpp ]
// Author: Asuma Nishio
// 
//========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "smokefriend.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <random>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "motion.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SMOKEFRIEND
{
	constexpr const char* SCRIPT = "data/MOTION/Smoke/SmokeMotion.txt"; // モーションファイル
	constexpr int MIN_CHANGETIME = 600;									// 10秒
	constexpr int MAX_CHANGETIME = 1200;								// 20秒
	constexpr int BLEND = 5;											// ブレンド
};

//========================================================
// コンストラクタ
//========================================================
CSmokeFriend::CSmokeFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeCount(NULL),
m_nNextChangeCount(NULL)
{

}
//========================================================
// デストラクタ
//========================================================
CSmokeFriend::~CSmokeFriend()
{

}
//========================================================
// 生成処理
//========================================================
CSmokeFriend* CSmokeFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CSmokeFriend* pSmoke = new CSmokeFriend;
	if (pSmoke == nullptr) return nullptr;

	// オブジェクト設定
	pSmoke->SetPos(pos);
	pSmoke->SetRot(rot);
	pSmoke->SetUseOutLine(true);
	pSmoke->SetOutLineColor(D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f));

	// 初期化失敗時
	if (FAILED(pSmoke->Init())) return nullptr;

	return pSmoke;
}
//========================================================
// 初期化処理
//========================================================
HRESULT CSmokeFriend::Init(void)
{
	// 親クラスの初期化
	CNoMoveCharactor::Init();

	// モーションロード
	MotionLoad(SMOKEFRIEND::SCRIPT, MOTION::MAX,false);

	// 初期ランダム関数
	RandomChange();

	return S_OK;
}
//========================================================
// 終了処理
//========================================================
void CSmokeFriend::Uninit(void)
{
	// 親クラスの終了処理
	CNoMoveCharactor::Uninit();
}
//========================================================
// 更新処理
//========================================================
void CSmokeFriend::Update(void)
{
	// カウント加算
	m_nChangeCount++;

	// 超過したら
	if (m_nChangeCount >= m_nNextChangeCount)
	{
		// モーションの番号を取得
		int nMotionType = GetMotion()->GetMotionType();
		int nNextMotion = (nMotionType == MOTION::NEUTRAL) ? MOTION::ACTION : MOTION::NEUTRAL;

		// モーション変更
		GetMotion()->SetMotion(nNextMotion,true,SMOKEFRIEND::BLEND);

		// 変数初期化
		m_nChangeCount = 0;
		RandomChange();
	}

	// 親クラスの更新処理
	CNoMoveCharactor::Update();
}
//========================================================
// 描画処理
//========================================================
void CSmokeFriend::Draw(void)
{
	// 親クラスの描画処理
	CNoMoveCharactor::Draw();
}
//========================================================
// ランダム変更計算
//========================================================
void CSmokeFriend::RandomChange(void)
{
	// ランダムを生成する
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(SMOKEFRIEND::MIN_CHANGETIME, SMOKEFRIEND::MAX_CHANGETIME);
	
	// 変更カウントに設定
	m_nNextChangeCount = dis(gen);
}