//=========================================================
//
// サイドに回転して開くドア当たり判定管理処理 [ sideopendoorcollision.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "sideopendoorcollision.h"
#include "spherecollider.h"

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
namespace CollisionSide
{
	using json = nlohmann::json;									// 名前空間
	constexpr const char* LoadName = "data/JSON/SideCollision.json"; // 読み込むファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CSideOpenDoorCollision::CSideOpenDoorCollision() : m_SideDoorCollider{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CSideOpenDoorCollision::~CSideOpenDoorCollision()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSideOpenDoorCollision::Init(void)
{
	// 配列クリア
	m_SideDoorCollider.clear();

	// 外部ファイル読み込み
	LoadJson();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSideOpenDoorCollision::Uninit(void)
{
	// 配列クリア
	m_SideDoorCollider.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CSideOpenDoorCollision::Update(void)
{

}
//=========================================================
// コライダー追加関数
//=========================================================
void CSideOpenDoorCollision::AddCollider(const D3DXVECTOR3& pos, const float& fRadius, const std::vector<int>& nIdx)
{
	// インスタンス生成
	auto CollisionPoint = std::make_unique<CollisionData>();
	if (!CollisionPoint) return;

	// 構造体変数の設定
	CollisionPoint->pos = pos;
	CollisionPoint->fRadius = fRadius;
	CollisionPoint->targetDoorIndices = nIdx;

	// 球の当たり判定コライダー生成
	CollisionPoint->pCollider = CSphereCollider::Create(pos, fRadius);

	// 配列内のオブジェクトに登録する
	m_SideDoorCollider.push_back(std::move(CollisionPoint));
}
//=========================================================
// JSONファイル読み込み
//=========================================================
void CSideOpenDoorCollision::LoadJson(void)
{
	// ファイルストリームを開く
	std::ifstream file(CollisionSide::LoadName);

	// ファイルが正常に開けなかった場合
	if (!file.is_open()) return;

	// JSONデータを格納するオブジェクト
	CollisionSide::json sphereData;

	// ファイルからJSONをパース
	file >> sphereData;

	if (sphereData.is_array())
	{
		// 配列の要素を1つずつループ処理
		for (const auto& item : sphereData)
		{
			// 必要なキーチェック
			if (item.contains("Pos") && item.contains("Radius") && item.contains("SideDoorNumber"))
			{
				// 座標情報
				D3DXVECTOR3 pos;
				pos.x = item["Pos"][0].get<float>();
				pos.y = item["Pos"][1].get<float>();
				pos.z = item["Pos"][2].get<float>();

				// 半径情報
				float fRadius = item["Radius"].get<float>();

				// 開く数
				std::vector<int> doorIndices;

				if (item["SideDoorNumber"].is_array())
				{
					// 配列の要素
					doorIndices = item["SideDoorNumber"].get<std::vector<int>>();
				}
				else if (item["SideDoorNumber"].is_number())
				{
					// 単体
					doorIndices.push_back(item["SideDoorNumber"].get<int>());
				}

				// 登録処理でリストに追加
				AddCollider(pos, fRadius, doorIndices);
			}
		}
	}

	// ファイルを閉じる
	file.close();
}