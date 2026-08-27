//=========================================================
//
// キー入力で開く両開きドアの処理 [ sideopendoor.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "sideopendoor.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "renderer.h"
#include "boxcollider.h"
#include "collisionbox.h"
#include "xfilemanager.h"
#include "texture.h"
#include "input.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace SIDEDOOR_INFO
{
	constexpr float MAX_ROTATION = 90.0f; // 最大角度
	constexpr int MAX_DOOR_CLOSETIME = 240; // 閉まる最大時間
};

//=========================================================
// コンストラクタ
//=========================================================
CSideOpenDoor::CSideOpenDoor(int nPriority) : CObjectX(nPriority),
m_isZTestEneble(false),
m_nOpenType(OPENTYPE_LEFT),
m_nState(STATE_CLOSE_WAIT),
m_pCollider(nullptr),
m_Size(INITSCALE),
m_fAngle(NULL),
m_fOffsetX(1.0f),
m_nCloseTime(NULL),
m_nReturnCount(NULL)
{
}
//=========================================================
// デストラクタ
//=========================================================
CSideOpenDoor::~CSideOpenDoor()
{

}
//=========================================================
// 生成処理
//=========================================================
CSideOpenDoor* CSideOpenDoor::Create
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale,
	const char* pModelName,
	const OPENTYPE& nType
)
{
	// インスタンス生成
	CSideOpenDoor* pSide = new CSideOpenDoor;
	if (pSide == nullptr) return nullptr;

	// オブジェクト設定
	pSide->SetPos(pos);
	pSide->SetRot(rot);
	pSide->SetScale(scale);
	pSide->SetFilePass(pModelName);
	pSide->SetType(nType);

	// 初期化失敗時
	if (FAILED(pSide->Init())) return nullptr;

	return pSide;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSideOpenDoor::Init(void)
{
	// 親クラスの初期化
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号のモデルを取得
	int nModelIdx = GetModelIdx();

	// モデルの拡大率と既存の拡大率を合わせる
	D3DXVECTOR3 Scale = GetScale();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// 元のサイズにスケールを掛け合わせる
	m_Size.x = Size.x * Scale.x;
	m_Size.y = Size.y * Scale.y;
	m_Size.z = Size.z * Scale.z;

	// オブジェクトの回転角度を取得
	D3DXMATRIX matRot;
	D3DXVECTOR3 rot = GetRot();

	// 回転を合成して回転行列を作成
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 矩形コライダー生成処理
	m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), m_Size, matRot);

	// 回転する際のオフセットを作成
	m_fOffsetX = (m_Size.x / 2.0f) * Scale.x;

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSideOpenDoor::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CSideOpenDoor::Update(void)
{
	// 状態ごとの切り替え
	switch (m_nState)
	{
	case STATE_OPENING:
		m_fAngle += 0.03f; // 開くスピード

		if (m_fAngle >= D3DXToRadian(SIDEDOOR_INFO::MAX_ROTATION))
		{
			// 最大角度を設定
			m_fAngle = D3DXToRadian(SIDEDOOR_INFO::MAX_ROTATION);

			// タイマーをリセットして待機ステートへ
			m_nCloseTime = 0;
			m_nState = STATE_OPENWAIT;
		}
		break;

	case STATE_OPENWAIT: // 開いた状態をキープ

		m_nCloseTime++; // 毎フレームカウントアップ

		// 最大時間経ったら閉じ始める
		if (m_nCloseTime >= SIDEDOOR_INFO::MAX_DOOR_CLOSETIME)
		{
			m_nState = STATE_RETURN;
		}
		break;

	case STATE_RETURN:
		m_fAngle -= 0.03f; // 閉じるスピード

		if (m_fAngle <= 0.0f)
		{
			m_fAngle = 0.0f;
			m_nState = STATE_CLOSE_WAIT;
		}
		break;

	case STATE_CLOSE_WAIT:
		
		m_nReturnCount++;

		if (m_nReturnCount >= 5)
		{
			m_nReturnCount = 0;
			m_nState = STATE_CLOSE;
		}
		break;

	case STATE_CLOSE:
		break;
	}

	// ローカルマトリックス宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// マトリックスの要素を設定する
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);

	// ドアの開く角度を設定する
	float finalAngle = (m_nOpenType == OPENTYPE_LEFT) ? -m_fAngle : m_fAngle;

	// Y軸回転の値をマトリックスに設定する
	D3DXMATRIX mtxDoorRot;
	D3DXMatrixRotationY(&mtxDoorRot, finalAngle);

	// 基準となる回転の位置を設定する
	float pivotX = (m_nOpenType == OPENTYPE_LEFT) ? m_fOffsetX : -m_fOffsetX;

	// ピボット用の変数宣言
	D3DXMATRIX mtxPivotIn, mtxPivotOut;

	// 種類によって回転の基準の位置を変更
	if (m_nOpenType == OPENTYPE_LEFT)
	{
		D3DXMatrixTranslation(&mtxPivotIn, -pivotX, 0.0f, 0.0f);
		D3DXMatrixTranslation(&mtxPivotOut, pivotX, 0.0f, 0.0f);
	}
	else if (m_nOpenType == OPENTYPE_RIGHT)
	{
		D3DXMatrixTranslation(&mtxPivotIn, pivotX, 0.0f, 0.0f);
		D3DXMatrixTranslation(&mtxPivotOut, -pivotX, 0.0f, 0.0f);
	}

	// ワールド座標に設定する
	D3DXMATRIX mtxFinalWorld = mtxScale * mtxPivotIn * mtxDoorRot * mtxPivotOut * mtxRot * mtxTrans;

	// オブジェクトのマトリックスに設定
	SetMtxWorld(mtxFinalWorld);

	// コライダー更新処理
	if (m_pCollider != nullptr)
	{
		// 最終座標を変換する
		auto FinalPos = D3DXVECTOR3(mtxFinalWorld._41, mtxFinalWorld._42, mtxFinalWorld._43);

		// コライダーをマトリックスから計算する
		m_pCollider->SetPosOld(m_pCollider->GetPos());
		m_pCollider->SetPos(FinalPos);
	}

	// 親クラスの更新処理
	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CSideOpenDoor::Draw(void)
{
	// ファイルマネージャーから取得
	CXfileManager* pXMgr = CManager::GetInstance()->GetXManager();
	if (!pXMgr) return;

	// モデルのリストを取得
	auto& fileData = pXMgr->GetList();
	if (GetModelIdx() >= static_cast<int>(fileData.size())) return;

	// モデルのインデックスチェック
	auto& model = fileData[GetModelIdx()];
	if (!model.pMesh) return;

	// デバイスとカラーマトリックスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;

	// Zテストの制御
	if (m_isZTestEneble) 
	{
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// ワールド座標を取得
	D3DXMATRIX mtxWorld = GetMtxWorld();
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// デバイスのマテリアルの設定
	pDevice->GetMaterial(&matDef);

	if (model.pBuffMat)
	{
		// マテリアルのポインタを取得する
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

		// テクスチャ取得
		CTexture* pTex = CManager::GetInstance()->GetTexture();

		// マテリアルループ
		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// カラーの乗算設定
			D3DXMATERIAL Col = pMat[nCnt];

			Col.MatD3D.Diffuse.a *= GetCol().a;
			Col.MatD3D.Diffuse.r *= GetCol().r;
			Col.MatD3D.Diffuse.g *= GetCol().g;
			Col.MatD3D.Diffuse.b *= GetCol().b;

			// マテリアルをセット
			pDevice->SetMaterial(&Col.MatD3D);

			// テクスチャインデックスを取得
			int nTexIdx = model.pTexture[nCnt];

			// デバイスにテクスチャを設定
			pDevice->SetTexture(0, (nTexIdx >= NULL) ? pTex->GetAddress(nTexIdx) : nullptr);

			// メッシュの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}

	// マテリアルを変更
	pDevice->SetMaterial(&matDef);

	// Zテスト設定を戻す
	if (m_isZTestEneble) 
	{
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}
//=========================================================
// 当たり判定処理
//=========================================================
bool CSideOpenDoor::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullなら
	if (m_pCollider == nullptr) return false;

	// 判定式を使う
	return CCollisionBox::CollisionEx(m_pCollider.get(),pOther,OutPos);
}
//=========================================================
// 回転するドアのフラグ起動
//=========================================================
void CSideOpenDoor::RotationDoorFlag(void)
{
	if (m_nState == STATE_CLOSE_WAIT || m_nState == STATE_CLOSE)
	{
		// 開き始めるフラグにする
		m_nState = STATE_OPENING;
	}
}
//=========================================================
// ドアを強制的に閉じる関数
//=========================================================
void CSideOpenDoor::CloseDoor(void)
{
	// 状態を"閉じはじめ開始"に変更する
	m_nState = STATE_RETURN;
}