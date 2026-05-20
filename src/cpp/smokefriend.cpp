//========================================================
//
// 煙草吸っている同僚の処理 [ smokefriend.cpp ]
// Author: Asuma Nishio
// 
// 
// 煙草のアクションモーションを繰り返す回数でモーションを変更するか決めたい
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
	constexpr int MIN_CHANGETIME = 6;
	constexpr int MAX_CHANGETIME = 8;
	constexpr int MAX_COUNT = 120;
};

//========================================================
// コンストラクタ
//========================================================
CSmokeFriend::CSmokeFriend(int nPriority) : CNoMoveCharactor(nPriority),
m_nChangeCount(NULL),
m_nNextChangeCount(NULL),
m_nLoopCount(NULL),
m_nTargetLoopCount(NULL)
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

		if (m_nChangeCount >= SMOKEFRIEND::MAX_COUNT)
		{
			// タバコモーションへ移行
			GetMotion()->SetMotion(MOTION::ACTION, true, SMOKEFRIEND::BLEND);

			// 変数の初期化
			m_nChangeCount = 0;
			m_nLoopCount = 0;

			// タバコを何回繰り返すかをランダムで決定
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(SMOKEFRIEND::MIN_CHANGETIME, SMOKEFRIEND::MAX_CHANGETIME);
			m_nTargetLoopCount = dis(gen);
		}
	}
	else if (nMotionType == MOTION::ACTION)
	{
		if (GetMotion()->GetFinishMotion())
		{
			m_nLoopCount++; // ループ回数を加算

			// リセット
			GetMotion()->SetFinishMotion(false);

			// 目標回数に達したらニュートラルに戻る
			if (m_nLoopCount >= m_nTargetLoopCount)
			{
				// モーション変更
				GetMotion()->SetMotion(MOTION::NEUTRAL, true, SMOKEFRIEND::BLEND);
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