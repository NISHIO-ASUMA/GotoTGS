//=========================================================
//
// モーション処理 [ motion.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "motion.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "template.h"
#include "debugproc.h"
#include "motionmanager.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CMotion::CMotion() : m_nCounterMotion(NULL),
m_nKey(NULL),
m_motiontype(NULL),
m_nNextKey(NULL),
m_nAllFrameCount(NULL),
m_nNumAllFrame(NULL),
m_motiontypeBlend(NULL),
m_nFrameBlend(NULL),
m_nKeyBlend(NULL),
m_nNextKeyBlend(NULL),
m_nCounterBlend(NULL),
m_nMotionIdx(-1),
m_isFinishMotion(false),
m_isFirstMotion(false),
m_isBlendMotion(false)
{
}
//=========================================================
// デストラクタ
//=========================================================
CMotion::~CMotion()
{
	
}
//=========================================================
// モーション読み込み関数
//=========================================================
std::unique_ptr<CMotion> CMotion::Load
(
	const char* pFilename, 
	std::vector<CModel*>& pModel, 
	int nDestMotions,
	const bool isShadow
)
{
	// モーションのユニークポインタ生成
	auto pMotion = std::make_unique<CMotion>();

	// 登録処理
	pMotion->RegisterPath(pFilename, pModel, nDestMotions, isShadow);

	// 生成されたポインタを返す
	return pMotion;
}
//=========================================================
// モーションインデックス登録関数
//=========================================================
void CMotion::RegisterPath(const char* pMotionName, std::vector<CModel*>& pModel, int nDestMotions, const bool isShadow)
{
	// モーションマネージャーを取得する
	auto MotionManager = CManager::GetInstance()->GetMotionManager();
	if (MotionManager == nullptr) return;

	// インデックスに登録し,その情報を取得する
	m_nMotionIdx = MotionManager->Register(pMotionName, pModel, nDestMotions, isShadow);
}
//============================================================
// モーションセット
//============================================================
void CMotion::SetMotion(int motiontype)
{
	// 一致していたら
	if (m_motiontype == motiontype) return;

	// 代入
	m_motiontype = motiontype;
	m_nKey = 0;
	m_nCounterMotion = 0;
	m_nAllFrameCount = 0;
	m_isFinishMotion = false;
}
//=================================================================
// ブレンドモーションセット情報
//=================================================================
void CMotion::SetMotion(int nMotionType, bool isBlend, int nBlendFrame)
{
	// 一致していたら
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
		// 一致していたら
		if (m_motiontype == nMotionType) return;

		// 代入
		m_motiontype = nMotionType;
		m_nKey = 0;
		m_nCounterMotion = 0;
		m_nAllFrameCount = 0;
		m_isFinishMotion = false;
	}
}
//=================================================================
// モーション全体更新処理
//=================================================================
void CMotion::Update(const std::vector<CModel*>& pModel)
{
	// データ取得
	const auto& Manager = CManager::GetInstance()->GetMotionManager();
	const auto& Info = Manager->GetFileDataIdx(m_nMotionIdx);

	// モデル数取得
	int nModelNum = Info.nNumModel;
	if (nModelNum <= 0) return;

	// 現在のモーション情報
	const auto& CurrentMotionInfo = Info.m_aMotionInfo[m_motiontype];

	// 補間係数の事前計算
	float fRate = static_cast<float>(m_nCounterMotion) / CurrentMotionInfo.aKeyInfo[m_nKey].nFrame;
	float fBlendRate = 0.0f;

	// ブレンドの補完計算
	if (m_isFirstMotion && m_nFrameBlend > 0)
	{
		fBlendRate = static_cast<float>(m_nCounterBlend) / m_nFrameBlend;
	}

	// 次のキー番号の確定
	m_nNextKey = (m_nKey + 1) % CurrentMotionInfo.nNumKey;

	// モデル更新ループ
	for (int nCnt = 0; nCnt < nModelNum; ++nCnt)
	{
		if (m_isFirstMotion)
		{
			// ブレンド更新
			UpdateBlendOptimized(Info, pModel[nCnt], nCnt, fRate, fBlendRate);
		}
		else
		{
			// 現在更新
			UpdateCurrentMotionOptimized(CurrentMotionInfo, pModel[nCnt], nCnt, fRate);
		}

		// モデル更新
		pModel[nCnt]->Update();
	}

	if (m_isFirstMotion)
	{
		// ブレンド中
		m_nCounterBlend++;

		// ブレンド終了判定
		if (m_nCounterBlend >= m_nFrameBlend)
		{
			m_isFirstMotion = false;
			m_motiontype = m_motiontypeBlend;
			m_nKey = 0;
			m_nCounterMotion = 0;
			m_nCounterBlend = 0;
			m_nFrameBlend = 0;
		}
	}
	else
	{
		// 通常再生中：カウンタ加算
		m_nCounterMotion++;
		m_nAllFrameCount++;

		// キー進行判定
		if (m_nCounterMotion >= CurrentMotionInfo.aKeyInfo[m_nKey].nFrame)
		{
			m_nCounterMotion = 0;
			m_nKey++;

			// モーションループ判定
			if (m_nKey >= CurrentMotionInfo.nNumKey)
			{
				m_nKey = 0;
				m_nAllFrameCount = 0;
			}
		}
	}

	// モーション終了判定
	int nLastKey = CurrentMotionInfo.nNumKey - 1;

	if (m_nKey >= nLastKey && m_nCounterMotion >= CurrentMotionInfo.aKeyInfo[m_nKey].nFrame)
	{
		m_isFinishMotion = true;
	}

	// 全体フレームカウント計算用
	int nFrame = 0;

	// キーごとのフレームで回す
	for (int nCnt = 0; nCnt < Info.m_aMotionInfo[m_motiontype].nNumKey; nCnt++)
	{
		// 全体計算用に加算
		nFrame += Info.m_aMotionInfo[m_motiontype].aKeyInfo[nCnt].nFrame;
	}

	// 全体カウント
	m_nAllFrameCount = nFrame;
}
//=================================================================
// 現在のモーションの更新関数
//=================================================================
void CMotion::UpdateCurrentMotionOptimized(const CMotionManager::INFO& info, CModel* pModel, int idx, float fRate)
{
	// 現在モーションのキー情報
	const auto& now = info.aKeyInfo[m_nKey].aKey[idx];

	// 次のモーションののキー情報
	const auto& next = info.aKeyInfo[m_nNextKey].aKey[idx];

	// 格納変数
	D3DXVECTOR3 pos, rot;

	// 座標差分を計算
	pos.x = now.fPosX + (next.fPosX - now.fPosX) * fRate;
	pos.y = now.fPosY + (next.fPosY - now.fPosY) * fRate;
	pos.z = now.fPosZ + (next.fPosZ - now.fPosZ) * fRate;

	// 角度の差分計算
	rot.x = NormalAngle(now.fRotX + (NormalAngle(next.fRotX - now.fRotX)) * fRate);
	rot.y = NormalAngle(now.fRotY + (NormalAngle(next.fRotY - now.fRotY)) * fRate);
	rot.z = NormalAngle(now.fRotZ + (NormalAngle(next.fRotZ - now.fRotZ)) * fRate);

	// モデルに適用
	pModel->SetPos(pos);
	pModel->SetRot(rot);
}

//=================================================================
// ブレンドモーションの更新関数
//=================================================================
void CMotion::UpdateBlendOptimized(const CMotionManager::MOTIONFILE& info, CModel* pModel, int idx, float fRate, float fBlendRate)
{
	// 現在のモーションの補間
	const auto& curInfo = info.m_aMotionInfo[m_motiontype];
	const auto& curNow = curInfo.aKeyInfo[m_nKey].aKey[idx];
	const auto& curNext = curInfo.aKeyInfo[m_nNextKey].aKey[idx];

	D3DXVECTOR3 posA, rotA;
	posA.x = curNow.fPosX + (curNext.fPosX - curNow.fPosX) * fRate;
	posA.y = curNow.fPosY + (curNext.fPosY - curNow.fPosY) * fRate;
	posA.z = curNow.fPosZ + (curNext.fPosZ - curNow.fPosZ) * fRate;

	rotA.x = curNow.fRotX + (curNext.fRotX - curNow.fRotX) * fRate;
	rotA.y = curNow.fRotY + (curNext.fRotY - curNow.fRotY) * fRate;
	rotA.z = curNow.fRotZ + (curNext.fRotZ - curNow.fRotZ) * fRate;

	// ブレンド先モーションの補間 (Next/Blend)
	const auto& bldInfo = info.m_aMotionInfo[m_motiontypeBlend];

	// ブレンド先もキーとキーの間を補間する必要がある (m_nKeyBlendを使用)
	const auto& bldNow = bldInfo.aKeyInfo[m_nKeyBlend].aKey[idx];
	const auto& bldNext = bldInfo.aKeyInfo[m_nNextKeyBlend].aKey[idx];

	// ブレンド先モーション内の補間係数 (もしブレンド先も動かすなら必要)
	float fRateB = 0.0f;

	if (bldInfo.aKeyInfo[m_nKeyBlend].nFrame > 0)
		fRateB = static_cast<float>(m_nCounterMotion) / bldInfo.aKeyInfo[m_nKeyBlend].nFrame;

	D3DXVECTOR3 posB, rotB;

	posB.x = bldNow.fPosX + (bldNext.fPosX - bldNow.fPosX) * fRateB;
	posB.y = bldNow.fPosY + (bldNext.fPosY - bldNow.fPosY) * fRateB;
	posB.z = bldNow.fPosZ + (bldNext.fPosZ - bldNow.fPosZ) * fRateB;

	rotB.x = bldNow.fRotX + (bldNext.fRotX - bldNow.fRotX) * fRateB;
	rotB.y = bldNow.fRotY + (bldNext.fRotY - bldNow.fRotY) * fRateB;
	rotB.z = bldNow.fRotZ + (bldNext.fRotZ - bldNow.fRotZ) * fRateB;

	// 最終ブレンド
	D3DXVECTOR3 finalPos, finalRot;

	finalPos.x = posA.x + (posB.x - posA.x) * fBlendRate;
	finalPos.y = posA.y + (posB.y - posA.y) * fBlendRate;
	finalPos.z = posA.z + (posB.z - posA.z) * fBlendRate;

	// 角度は成功時の計算に合わせつつ、NormalAngleで補正
	finalRot.x = NormalAngle(rotA.x + (NormalAngle(rotB.x - rotA.x)) * fBlendRate);
	finalRot.y = NormalAngle(rotA.y + (NormalAngle(rotB.y - rotA.y)) * fBlendRate);
	finalRot.z = NormalAngle(rotA.z + (NormalAngle(rotB.z - rotA.z)) * fBlendRate);

	// モデル適用
	pModel->SetPos(finalPos);
	pModel->SetRot(finalRot);
}


//=================================================================
// デバッグフォント関数
//=================================================================
void CMotion::Debug(void)
{
#ifdef _DEBUG
	CDebugproc::Print("[現在フレームカウント] %d /  [ 最大モーションフレーム ] %d", m_nAllFrameCount, m_nNumAllFrame);
	CDebugproc::Draw(800, 320);

	CDebugproc::Print("[ブレンドフレーム] %d / [ブレンドカウント] %d", m_nFrameBlend, m_nCounterBlend);
	CDebugproc::Draw(800, 340);
#endif // _DEBUG
}
//=================================================================
// モーションフレーム判定
//=================================================================
bool CMotion::CheckFrame(int nStartMotion, int nEndMotion, int nMotionType)
{
	// StartとEndの範囲内なら
	if (m_nAllFrameCount >= nStartMotion && m_nAllFrameCount <= nEndMotion && m_motiontype == nMotionType)
		return true;

	// それ以外の時
	return false;
}