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
	constexpr int BLEND = 5;											// ブレンド
	constexpr int MIN_CHANGETIME = 3;									// 最小ループ
	constexpr int MAX_CHANGETIME = 6;									// 最大ループ
	constexpr int MAX_COUNT = 120;
};

//========================================================
// コンストラクタ
//========================================================
CSmokeFriend::CSmokeFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeCount(NULL),
m_nNextChangeCount(NULL),
m_nLoopCount(NULL),
m_nTargetLoopCount(NULL),
m_pMotionName{}
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
CSmokeFriend* CSmokeFriend::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* Path)
{
	// インスタンス生成
	CSmokeFriend* pSmoke = new CSmokeFriend;
	if (pSmoke == nullptr) return nullptr;

	// オブジェクト設定
	pSmoke->SetPos(pos);
	pSmoke->SetRot(rot);
	pSmoke->SetUseOutLine(true);
	pSmoke->SetOutLineColor(D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f));
	pSmoke->m_pMotionName = Path;

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
	MotionLoad(m_pMotionName, MOTION::MAX,false);

	// ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disLoop(SMOKEFRIEND::MIN_CHANGETIME, SMOKEFRIEND::MAX_CHANGETIME);
	m_nTargetLoopCount = disLoop(gen);

	// ランダム設定
	std::uniform_int_distribution<> disTime(60, SMOKEFRIEND::MAX_COUNT);
	m_nNextChangeCount = disTime(gen);

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
	// 現在のモーション番号を取得
	int nMotionType = GetMotion()->GetMotionType();

	// 待機状態
	if (nMotionType == MOTION::NEUTRAL)
	{
		m_nChangeCount++;

		// 変更カウントになったら
		if (m_nChangeCount >= m_nNextChangeCount)
		{
			// タバコモーションへ移行
			GetMotion()->SetMotion(MOTION::ACTION, true, SMOKEFRIEND::BLEND);

			// 変数の初期化
			m_nChangeCount = 0;
			m_nLoopCount = 0;
		}
	}
	else if (nMotionType == MOTION::ACTION)
	{
		if (GetMotion()->GetFinishMotion())
		{
			// ループ回数を加算
			m_nLoopCount++;

			// リセット
			GetMotion()->SetFinishMotion(false);

			// 目標回数に達したらニュートラルに戻る
			if (m_nLoopCount >= m_nTargetLoopCount)
			{
				// モーション変更
				GetMotion()->SetMotion(MOTION::NEUTRAL, true, SMOKEFRIEND::BLEND);

				// 再抽選
				std::random_device rd;
				std::mt19937 gen(rd());

				// ループ数
				std::uniform_int_distribution<> disLoop(SMOKEFRIEND::MIN_CHANGETIME, SMOKEFRIEND::MAX_CHANGETIME);
				m_nTargetLoopCount = disLoop(gen);

				// 待機カウント数
				std::uniform_int_distribution<> disTime(60, SMOKEFRIEND::MAX_COUNT);
				m_nNextChangeCount = disTime(gen);
			}
		}
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