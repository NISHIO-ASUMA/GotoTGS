//=========================================================
//
// インスタンシングモーション処理 [ motioninstancing.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "motioninstancing.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "template.h"
#include "debugproc.h"
#include "motionmanager.h"
#include "manager.h"
#include "instancemodel.h"

//=========================================================
// コンストラクタ
//=========================================================
CMotionInstancing::CMotionInstancing() : m_cache{},
m_cacheDirty(true),
m_nCounterMotion(NULL),
m_nKey(NULL),
m_motiontype(NULL),
m_nAllFrameCount(NULL),
m_motiontypeBlend(NULL),
m_nFrameBlend(NULL),
m_nCounterBlend(NULL),
m_nKeyBlend(NULL),
m_isBlendMotion(false),
m_isFinishMotion(false),
m_nMotionIdx(-1)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CMotionInstancing::~CMotionInstancing()
{
	
}
//=========================================================
// モーション読み込み関数
//=========================================================
std::unique_ptr<CMotionInstancing> CMotionInstancing::Load
(
	const char* pFilename,
	std::vector<CInstanceModel*>& pModel, 
	int nDestMotions, 
	const bool isShadow
)
{
	// モーションのユニークポインタ生成
	auto pMotionInst = std::make_unique<CMotionInstancing>();

	// モーション情報の登録処理
	pMotionInst->RegisterPath(pFilename, pModel, nDestMotions, isShadow);

	// 生成されたポインタを返す
	return pMotionInst;
}
//=========================================================
// モーションインデックス登録関数
//=========================================================
void CMotionInstancing::RegisterPath(const char* pMotionName, std::vector<CInstanceModel*>& pModel, int nDestMotions, const bool isShadow)
{
	// モーションマネージャーを取得する
	auto MotionManager = CManager::GetInstance()->GetInstMotionManager();
	if (MotionManager == nullptr) return;

	// インデックスに登録し,その情報を取得する
	m_nMotionIdx = MotionManager->Register(pMotionName, pModel, nDestMotions, isShadow);
}
//============================================================
// モーションセット
//============================================================
void CMotionInstancing::SetMotion(int motiontype)
{
	// 一緒の番号だったら
	if (m_motiontype == motiontype) return;

	// 値をセットする
	m_motiontype = motiontype;
	m_nKey = 0;
	m_nCounterMotion = 0;
	m_nAllFrameCount = 0;
	m_isFinishMotion = false;
}
//=================================================================
// モーションセット情報
//=================================================================
void CMotionInstancing::SetMotion(int nMotionType, bool isBlend, int nBlendFrame)
{
	// 一緒の番号だったら
	if (m_motiontype == nMotionType) return;

	// ブレンドが有効化
	if (isBlend == true)
	{
		// 最初のモーションブレンドが終わってたら
		if (!m_isFirstMotion)
		{
			m_isFirstMotion = true;
			m_nKeyBlend = 0;				// 0から始める
			m_nCounterBlend = 0;			// 0から始める
		}

		m_isBlendMotion = isBlend;			// ブレンドがあるかどうか
		m_nFrameBlend = nBlendFrame;		// ブレンドのフレームを代入
		m_motiontypeBlend = nMotionType;	// ブレンドするモーションのタイプを代入
	}
	// モーションブレンドがない
	else
	{
		// ここはmotiontypeに渡された番号を取得する
		if (m_motiontype == nMotionType) return;

		// 代入
		m_motiontype = nMotionType;
		m_nKey = 0;
		m_nCounterMotion = 0;
		m_nAllFrameCount = 0;
		m_isFinishMotion = false;
	}
}
//=========================================================
// モーション全体更新処理
//=========================================================
void CMotionInstancing::Update(std::vector<CInstanceModel*> pModel)
{
	// モーションマネージャークラスからモーションデータ取得
	const auto& manager = CManager::GetInstance()->GetInstMotionManager();
	const auto& info = manager->GetFileDataIdx(m_nMotionIdx);

	// モーションフレーム計算処理
	MathMotionFrame(info);

	// モデルに適用する
	ApplyCache(pModel,info);
}
//=================================================================
// フレーム計算
//=================================================================
void CMotionInstancing::MathMotionFrame(const CInstanceMotionManager::MOTIONFILE& info)
{
	// モーションキャッシュ情報を取得
	const auto& motioncache = info.cache[m_motiontype];

	// フレームを加算
	m_nAllFrameCount++;

	// 最大カウントに達したらリセット
	if (m_nAllFrameCount >= motioncache.totalFrame)
		m_nAllFrameCount = 0;
}
//=========================================================
// 結果の適用
//=========================================================
void CMotionInstancing::ApplyCache
(
	std::vector<CInstanceModel*>& pModel, 
	const CInstanceMotionManager::MOTIONFILE& info
)
{
	// モーションの種類のフレーム取得
	const auto& frame = info.cache[m_motiontype].Frames[m_nAllFrameCount];

#ifdef _DEBUG
	// モデルに適用
	for (int nModelCount = 0; nModelCount < info.nNumModel; ++nModelCount)
	{
		pModel[nModelCount]->SetTargetPos(frame.pos[nModelCount]);
		pModel[nModelCount]->SetTargetRot(frame.rot[nModelCount]);
	}
#else
	// モデルに適用
	for (int nModelCount = 0; nModelCount < info.nNumModel; ++nModelCount)
	{
		pModel[nModelCount]->SetPos(frame.pos[nModelCount]);
		pModel[nModelCount]->SetRot(frame.rot[nModelCount]);
	}
#endif // _DEBUG
}
//=================================================================
// デバッグフォント関数
//=================================================================
void CMotionInstancing::Debug(void)
{
#if 0
	CDebugproc::Print("[現在フレームカウント] %d /  [ 最大モーションフレーム ] %d", m_nAllFrameCount, m_nNumAllFrame);
	CDebugproc::Draw(800, 320);

	CDebugproc::Print("[ブレンドフレーム] %d / [ブレンドカウント] %d", m_nFrameBlend, m_nCounterBlend);
	CDebugproc::Draw(800, 340);
#endif // _DEBUG
}
//=================================================================
// モーションフレーム判定
//=================================================================
bool CMotionInstancing::CheckFrame(int nStartMotion, int nEndMotion, int nMotionType)
{
	// StartとEndの範囲内なら
	if (m_nAllFrameCount >= nStartMotion && m_nAllFrameCount <= nEndMotion && m_motiontype == nMotionType)
		return true;

	// それ以外の時
	return false;
}