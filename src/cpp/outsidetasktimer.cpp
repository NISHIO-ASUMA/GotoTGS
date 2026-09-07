//=========================================================
//
// 外タスクのタイマー処理 [ outsidetasktimer.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "outsidetasktimer.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "number.h"
#include "template.h"
#include "camera.h"
#include "easing.h"
#include "player.h"
#include "ui.h"
#include "returnofficeui.h"

//=========================================================
// コンストラクタ
//=========================================================
COutSideTaskTimer::COutSideTaskTimer(int nPriority) : CObject(nPriority),
m_nAllTime(NULL),
m_nDecTime(NULL),
m_nCounter(NULL),
m_nMaxTime(NULL),
m_fWidth(NULL),
m_fHeight(NULL),
m_nEffectFrame(NULL),
m_pos(VECTOR3_NULL),
m_pPlayerOwner(nullptr),
m_pReturnUi(nullptr),
m_pUi(nullptr),
m_event{}
{
	// ポインタ初期化
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		m_pNumberMinutes[nDigit] = nullptr;
	}
}
//=========================================================
// デストラクタ
//=========================================================
COutSideTaskTimer::~COutSideTaskTimer()
{

}
//=========================================================
// 生成処理処理
//=========================================================
COutSideTaskTimer* COutSideTaskTimer::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// インスタンス生成
	COutSideTaskTimer* pOutSidetime = new COutSideTaskTimer;
	if (pOutSidetime == nullptr) return nullptr;

	// 引数を設定
	pOutSidetime->SetPos(pos);
	pOutSidetime->SetWidth(fWidth);
	pOutSidetime->SetHeight(fHeight);

	// 初期化が失敗した場合
	if (FAILED(pOutSidetime->Init())) return nullptr;

	return pOutSidetime;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT COutSideTaskTimer::Init(void)
{
	// 開始時の時間を格納
	m_nMaxTime = Config::NUMTIME;

	// 全体の時間を設定
	m_nAllTime = Config::NUMTIME;

	// 背景ui生成
	m_pUi = CUi::Create({640.0f,60.0f,0.0f,}, 0, 115.0f, 55.0f, "Limit_frame.png");
	m_pUi->SetUse(false);

	// 警告ui生成
	m_pReturnUi = CReturnOfficeUi::Create({ 640.0f,60.0f,0.0f, },10, 115.0f, 55.0f, "return_office.png");
	m_pReturnUi->SetUse(false);

	// 一桁の横幅
	float fTexpos = m_fWidth / Config::DIGIT_TIME;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		// 数値生成
		m_pNumberMinutes[nDigit] = new CNumber;

		// ナンバーの初期化処理
		m_pNumberMinutes[nDigit]->Init
		(D3DXVECTOR3(m_pos.x + (fTexpos * Config::VALUE_FLOAT * nDigit), m_pos.y, 0.0f),// 位置
			fTexpos,																	// 一桁分の横幅
			m_fHeight);																	// 縦幅

		// サイズ設定
		m_pNumberMinutes[nDigit]->SetSize(fTexpos, m_fHeight);

		// カラー設定
		m_pNumberMinutes[nDigit]->SetCol(COLOR_WHITE);

		// テクスチャ設定
		m_pNumberMinutes[nDigit]->SetTexture(Config::TEXNAME);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void COutSideTaskTimer::Uninit(void)
{
	// 桁数分破棄
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		if (m_pNumberMinutes[nDigit] != nullptr)
		{
			// 分数の終了処理
			m_pNumberMinutes[nDigit]->Uninit();
			delete m_pNumberMinutes[nDigit];
			m_pNumberMinutes[nDigit] = nullptr;
		}
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void COutSideTaskTimer::Update(void)
{
	// フラグがoffなら
	if (!m_isActive) return;

	// アニメーション中なら
	if (CManager::GetInstance()->GetCamera()->GetIsAnimTime() || 
		CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_BOSS_SYSTEM) 
		return;

	// 最大時間が0なら処理を通さない
	if (m_nAllTime <= 0)
	{
		// 状態リセット関数
		End();
		UpdateState();

		// 最大時間のクリア
		m_nAllTime = 0;
		return;
	}

	if (m_nCounter < 0)
	{
		// カウンターを0にする
		m_nCounter = 0;
		return;
	}

	// カウンターを1つ増やす
	m_nCounter++;

	if (m_nCounter >= Config::CARVETIME)
	{// カウンターが1Fを超えたとき

		// カウンターを0にする
		m_nCounter = 0;

		// 全体の時間を減らす
		m_nAllTime--;

		if (m_nAllTime <= 0)
		{// 現在の時間が0以下になった時
			// カウンターを0にする
			m_nCounter = 0;
		}
	}

	// 状態更新
	UpdateState();

	// 桁数更新処理
	UpdateDigitNumbers();
}
//=========================================================
// 描画処理
//=========================================================
void COutSideTaskTimer::Draw(void)
{
	// フラグがoffなら
	if (!m_isActive) return;

	// 桁数分表示
	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		// ナンバーの描画処理
		m_pNumberMinutes[nDigit]->Draw();
	}
}
//=========================================================
// 開始関数
//=========================================================
void COutSideTaskTimer::Start(void)
{
	// フラグ起動
	m_isActive = true;

	// 状態変更
	m_State = TIMESTATE_START;

	// 最大時間の再設定
	m_nAllTime = Config::NUMTIME;
}
//=========================================================
// 終了関数
//=========================================================
void COutSideTaskTimer::End(void)
{
	// 状態変更
	m_State = TIMESTATE_END;

	// 最大時間をリセット
	m_nAllTime = 0;
}
//=========================================================
// 桁数の更新関数
//=========================================================
void COutSideTaskTimer::UpdateDigitNumbers(void)
{
	// 桁数ごとの分割値
	int nTimeDate = Config::DIVIDE * Config::DIVIDE;
	int nTimeDateBase = Config::DIVIDE;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		int nPosTexU = m_nAllTime % nTimeDate / nTimeDateBase;
		nTimeDate /= Config::DIVIDE;
		nTimeDateBase /= Config::DIVIDE;

		if (m_pNumberMinutes[nDigit] != nullptr)
		{// 分数の更新処理
			m_pNumberMinutes[nDigit]->Update();
			m_pNumberMinutes[nDigit]->SetDigit(nPosTexU);
		}
	}
}
//=========================================================
// 状態ごとの更新処理
//=========================================================
void COutSideTaskTimer::UpdateState(void)
{
	// 状態ごとに動く処理
	switch (m_State)
	{
	case COutSideTaskTimer::TIMESTATE_NONE:		// 初期
		break;

	case COutSideTaskTimer::TIMESTATE_START:	// 開始時

		// 下側に移動する
		m_pos.y += Config::MOVE_Y;
		SetPos(m_pos);

		// 上限値に達したら
		if (m_pos.y >= Config::MAX_POS_Y)
		{
			m_pos.y = Config::MAX_POS_Y;
			SetPos(m_pos);
			m_State = TIMESTATE_STOP;

			// 背景ui描画起動
			m_pUi->SetUse(true);

			break;
		}
		break;

	case COutSideTaskTimer::TIMESTATE_STOP:		// 停止継続中
		SetPos(m_pos);
		break;

	case COutSideTaskTimer::TIMESTATE_END:		// 状態終了

		// 上側に移動する
		m_pos.y -= Config::MOVE_Y;
		SetPos(m_pos);

		// 上限値に達したら
		if (m_pos.y <= Config::MAX_POS_RETURN_Y)
		{
			m_pos.y = Config::MAX_POS_RETURN_Y;
			SetPos(m_pos);
			m_State = TIMESTATE_NONE;
			m_isActive = false;
			m_pUi->SetUse(false);

			// イベントがある かつ プレイヤーがまだ外タスクをしているのなら
			if (m_event)
			{
				if (m_pPlayerOwner->GetIsTaskOutSide() == true)
				{
					m_event();

					// ui起動
					m_pReturnUi->SetUse(true);
				}
				else
				{
					// 警告ui起動
					m_pReturnUi->SetUse(false);
				}
			}
			break;
		}
		break;

	default:
		break;
	}

	// 座標が反映された後に数字の位置を更新
	float fTexpos = m_fWidth / Config::DIGIT_TIME;

	for (int nDigit = 0; nDigit < Config::DIGIT_TIME; nDigit++)
	{
		if (m_pNumberMinutes[nDigit] != nullptr)
		{
			// 座標の更新設定
			m_pNumberMinutes[nDigit]->SetPos
			(
				D3DXVECTOR3
				(
				m_pos.x + (fTexpos * Config::VALUE_FLOAT * nDigit),
				m_pos.y,
				0.0f
				)
			);

			// もし10秒以下なら色を変更する
			if (m_nAllTime <= 10)
			{
				// カウンターを更新
				m_nEffectFrame++;
				if (m_nEffectFrame >= Config::EASE_MAX_FRAME)
				{
					m_nEffectFrame = 0;
				}

				// イージング設定
				float t = CEasing::SetEase(m_nEffectFrame, Config::EASE_MAX_FRAME);

				float effectEase = CEasing::EaseOutCubic(t);

				float effectScale = 1.0f + (Config::EFFECT_MAX_SCALE - 1.0f) * effectEase;
				float effectWidth = fTexpos * effectScale;
				float effectHeight = m_fHeight * effectScale;

				float alpha = 1.0f;
				float fadeStartT = 0.5f;

				if (t > fadeStartT)
				{
					float fadeProgress = (t - fadeStartT) / (1.0f - fadeStartT);
					alpha = 1.0f - fadeProgress;
				}

				float bodyPulseT = sinf(t * D3DX_PI);
				float bodyEase = CEasing::EaseOutQuad(bodyPulseT);
				float bodyScale = 1.0f + (Config::BODY_MAX_SCALE - 1.0f) * bodyEase;

				// 描画設定
				m_pNumberMinutes[nDigit]->SetSize(fTexpos * bodyScale, m_fHeight * bodyScale);

				// カラー設定
				D3DXCOLOR colRed = COLOR_RED;
				colRed.a = alpha;
				m_pNumberMinutes[nDigit]->SetCol(colRed);
			}
			else
			{
				// カラー更新設定
				m_pNumberMinutes[nDigit]->SetCol(COLOR_WHITE);
				m_pNumberMinutes[nDigit]->SetSize(fTexpos, m_fHeight);
				m_nEffectFrame = 0;
			}

			// 数字の更新
			m_pNumberMinutes[nDigit]->Update();
		}
	}
}