//=========================================================
//
// 自動ドア当たり判定管理処理 [ autodoor_collision.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "autodoor_collision.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

//*********************************************************
// 名前空間を定義
//*********************************************************
namespace CollisionDoor
{
	using json = nlohmann::json;										 // 名前空間
	constexpr const char* LoadName = "data/JSON/AutoDoor_Collider.json"; // 読み込むファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CAutoMaticDoorCollision::CAutoMaticDoorCollision() : m_AutoDoorCollider{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CAutoMaticDoorCollision::~CAutoMaticDoorCollision()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CAutoMaticDoorCollision::Init(void)
{
	// 配列のクリア
	m_AutoDoorCollider.clear();

	// 外部ファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CAutoMaticDoorCollision::Uninit(void)
{
	m_AutoDoorCollider.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CAutoMaticDoorCollision::Update(void)
{

}
//=========================================================
// コライダー追加処理
//=========================================================
void CAutoMaticDoorCollision::AddCollider(const D3DXVECTOR3& pos, const float& fRadius,const int& nIdx)
{
	// インスタンス生成
	auto CollisionPoint = std::make_unique<CollisionData>();
	if (!CollisionPoint) return;

	// 構造体変数の設定
	CollisionPoint->pos = pos;
	CollisionPoint->fRadius = fRadius;
	CollisionPoint->nIdx = nIdx;

	// 球の当たり判定コライダー生成
	CollisionPoint->pCollider = CSphereCollider::Create(pos, fRadius);

	// 配列内のオブジェクトに登録する
	m_AutoDoorCollider.push_back(std::move(CollisionPoint));
}
//=========================================================
// ファイル読み込み処理
//=========================================================
void CAutoMaticDoorCollision::LoadJson(void)
{
	// ファイルストリームを開く
	std::ifstream file(CollisionDoor::LoadName);

	// ファイルが正常に開けなかった場合
	if (!file.is_open())
	{
		return;
	}

	// JSONデータを格納するオブジェクト
	CollisionDoor::json sphereData;

	// ファイルからJSONをパース
	file >> sphereData;

	if (sphereData.is_array())
	{
		// 配列の要素を1つずつループ処理
		for (const auto& item : sphereData)
		{
			// 必要なキーチェック
			if (item.contains("Pos") && item.contains("Radius") && item.contains("DoorNumber"))
			{
				// 座標情報
				D3DXVECTOR3 pos;
				pos.x = item["Pos"][0].get<float>();
				pos.y = item["Pos"][1].get<float>();
				pos.z = item["Pos"][2].get<float>();

				// 半径情報
				float fRadius = item["Radius"].get<float>();

				// インデックス
				int nIdx = item["DoorNumber"].get<int>();

				// 登録処理でリストに追加
				CAutoMaticDoorCollision::AddCollider(pos, fRadius, nIdx);
			}
		}
	}

	// ファイルを閉じる
	file.close();
}