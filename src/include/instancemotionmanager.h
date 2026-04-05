//===================================================================
//
// インスタンシング用モーション管理クラス [ instancemotionmanager.h ]
// Author: Asuma Nishio
// 
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once

//*******************************************************************
// システムインクルードファイル
//*******************************************************************
#include <string>
#include <vector>
#include <memory>

//*******************************************************************
// 前方宣言
//*******************************************************************
class CMotionInstancing;
class CInstanceModel;

//*******************************************************************
// インスタンシングモデルのモーションファイル管理クラスを定義
//*******************************************************************
class CInstanceMotionManager
{
public:

	//*********************************
	// キー構造体宣言
	//*********************************
	struct KEY
	{
		float fPosX;	// 位置X
		float fPosY;	// 位置Y
		float fPosZ;	// 位置Z
		float fRotX;	// 角度X
		float fRotY;	// 角度Y
		float fRotZ;	// 角度Z
	};

	//*********************************
	// キー情報の構造体宣言
	//*********************************
	struct KEY_INFO
	{
		int nFrame;				// フレーム数
		std::vector<KEY> aKey;  // 動的なキー数
	};

	//*********************************
	// モーション情報の構造体宣言
	//*********************************
	struct INFO
	{
		bool bLoop;						// ループするかしないか
		int nNumKey;					// キーの総数
		std::vector<KEY_INFO> aKeyInfo; // モーションの動的キーフレーム
	};

	//*********************************
	// フレーム間結果を保持する構造体
	//*********************************
	struct MATH_MOTIONFRAME
	{
		std::vector<D3DXVECTOR3> pos; // 座標
		std::vector<D3DXVECTOR3> rot; // 角度
		std::vector<D3DXMATRIX> local; // ローカル
		std::vector<D3DXMATRIX> world; //
	};

	//*********************************
	// モーション一個分の情報
	//*********************************
	struct MOTION_CACHEDATA
	{
		int totalFrame = 0;						// 合計フレーム
		std::vector<MATH_MOTIONFRAME> Frames;	// 保持構造体
	};

	//*********************************
	// モーション情報を保存する構造体
	//*********************************
	struct MOTIONFILE
	{
		std::string FilePath;				// ファイル名
		std::vector<CInstanceModel*>apModel; // モデルインデックス
		std::vector<std::string>Modelpath;	// モデルパス
		std::vector<int>nParentId;			// idセット
		std::vector<D3DXVECTOR3>offpos;		// オフセット座標
		std::vector<D3DXVECTOR3>offrot;		// オフセット角度
		std::vector<INFO> m_aMotionInfo;	// モーション情報
		std::vector<MOTION_CACHEDATA> cache; // モーションキャッシュ用データ配列
		int nNumModel;						 // 最大モデル数
		int nNumMotion;						 // 最大モーション数
	};

	CInstanceMotionManager();
	~CInstanceMotionManager();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// モーションファイルの登録処理
	/// </summary>
	/// <param name="pFileName">モーションファイル名</param>
	/// <param name="pModel">参照モデル</param>
	/// <param name="nDestMotion">モーション最大数</param>
	/// <param name="isShadow">影の描画フラグ</param>
	/// <returns></returns>
	int Register(const char* pFileName, std::vector<CInstanceModel*>& pModel, int nDestMotion, bool isShadow);

	/// <summary>
	/// 実際のモーションロードを行う
	/// </summary>
	/// <param name="pFileName">モーションファイル名</param>
	/// <param name="pModel">参照モデル</param>
	/// <param name="nDestMotion">モーション最大数</param>
	/// <param name="isShadow">影の描画フラグ</param>
	void LoadMotion(const char* pFileName, std::vector<CInstanceModel*>& pModel, int nDestMotion, bool isShadow);

	/// <summary>
	/// 動的配列からデータを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<MOTIONFILE>& GetFileData(void) { return m_FileData; }

	/// <summary>
	/// インデックスで取得
	/// </summary>
	/// <param name="nIdx">取得するモーション番号</param>
	/// <returns></returns>
	const MOTIONFILE& GetFileDataIdx(const int nIdx) { return m_FileData[nIdx]; }

private:

	//***********************************
	// クラス内メンバ関数
	//***********************************
	void SetModelFile(std::istringstream& iss, std::vector<CInstanceModel*>& pModel, int nCnt, const bool isShadow);
	void SetParts(std::ifstream& file, std::vector<CInstanceModel*>& pModel);
	void SetPartsMotion(std::ifstream& file, int nCntMotion);
	void SetKey(std::ifstream& file, int nCntMotion, int nCntKey);
	void SetKeyDate(std::istringstream& ss, const std::string& param, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex);
	void BuildMotionCache(MOTIONFILE& file);
	int SetModels(std::istringstream& iss);

	// 静的メンバ変数
	static int m_nNumAll;

	// 構造体の動的配列
	std::vector<MOTIONFILE>m_FileData;
};