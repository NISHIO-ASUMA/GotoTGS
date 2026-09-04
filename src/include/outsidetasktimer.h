//=========================================================
//
// 外タスクのタイマー処理 [ outsidetasktimer.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <functional>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;
class CPlayer;
class CUi;
class CReturnOfficeUi;

//*********************************************************
// 外タスクのタイマークラスを定義
//*********************************************************
class COutSideTaskTimer : public CObject
{
public:

	//************************************
	// 状態列挙型宣言
	//************************************
	enum TIMESTATE
	{
		TIMESTATE_NONE,
		TIMESTATE_START,	// 開始
		TIMESTATE_STOP,		// 停止継続
		TIMESTATE_END,		// 終了
		TIMESTATE_MAX
	};

	COutSideTaskTimer(int nPriority = static_cast<int>(CObject::PRIORITY::NUMBERSET));
	~COutSideTaskTimer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Start(void);
	void End(void);
	void SetState(TIMESTATE state) { m_State = state; }
	void SetPlayerOwner(CPlayer* pPlayer = nullptr) { m_pPlayerOwner = pPlayer; }

	/// <summary>
	/// イベントの登録関数
	/// </summary>
	/// <param name="Event"></param>
	void RegisterEvent(std::function<void(void)> Event) { m_event = Event; }

	CReturnOfficeUi* GetReturnUi(void) { return m_pReturnUi; }

	inline void SetActiveFlag(bool isFlags) { m_isActive = isFlags; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static COutSideTaskTimer* Create
	(
		const D3DXVECTOR3& pos, 
		const float& fWidth, 
		const float& fHeight
	);

private:
	void UpdateDigitNumbers(void);
	void UpdateState(void);

private:
	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr const char* TEXNAME = "number_k.png";	// テクスチャ名
		static constexpr float VALUE_WIDTH = 150.0f;			// 横幅の増加分
		static constexpr float VALUE_FLOAT = 2.0f;				// ずらす基準値
		static constexpr float MAX_POS_Y = 60.0f;				// 最大幅
		static constexpr float MAX_POS_RETURN_Y = -30.0f;		// 最大幅 ( 帰るとき)

		static constexpr float MOVE_Y = 2.0f;					// 移動速度
		static constexpr int DIGIT_TIME = 2;					// 桁数
		static constexpr int DIVIDE = 10;						// 桁分割の値
		static constexpr int CARVETIME = 60;					// カウント上限
		static constexpr int NUMTIME = 15;						// 最大タイマー

		static constexpr int EASE_MAX_FRAME = 120;			// 1回にかかるフレーム数
		static constexpr float EFFECT_MAX_SCALE = 1.8f;		// エフェクトが広がる最大倍率
		static constexpr float BODY_MAX_SCALE = 1.2f;		// 本体が少し膨らむ倍率
	};

private:
	int m_nMaxTime;		// 開始時の最大時間
	int m_nAllTime;		// 全体の時間
	int m_nDecTime;		// 経過時間
	int m_nCounter;		// カウンター
	float m_fWidth;		// 横幅
	int m_nEffectFrame; // エフェクト用のフレームカウンター
	float m_fHeight;	// 縦幅
	bool m_isActive;	// アクティブフラグ

private:
	CNumber* m_pNumberMinutes[Config::DIGIT_TIME];		// 分数のポインタ
	CPlayer* m_pPlayerOwner;							// プレイヤーポインタ
	CUi* m_pUi;											// uiクラス
	CReturnOfficeUi* m_pReturnUi;						// 帰ってくる警告uiクラス
	D3DXVECTOR3 m_pos;									// 位置
	TIMESTATE m_State;									// 状態変更

	std::function<void(void)> m_event;					// 関数イベント
};