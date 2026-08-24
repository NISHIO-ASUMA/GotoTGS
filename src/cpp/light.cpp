//=========================================================
//
// ライト処理 [ light.cpp ]
// Author: Asuma Nishio
//
// ポイントライトの追加をする
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "light.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CLight::CLight() : m_aLight{}, m_vecDir{}
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CLight::~CLight()
{
	Uninit();
}
//=========================================================
// ライトの初期化処理
//=========================================================
HRESULT CLight::Init(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライトのゼロクリア処理
	ZeroMemory(&m_aLight, sizeof(m_aLight));

	for (int i = 0;i < NUMLIGHT;i++)
	{
		m_aLight[i].Type = D3DLIGHT_DIRECTIONAL;
		m_aLight[i].Diffuse = COLOR_WHITE;
	}

	// ライトの位置
	m_aLight[0].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_aLight[1].Position = D3DXVECTOR3(0.65f, 0.65f, 0.65f);
	m_aLight[2].Position = D3DXVECTOR3(0.15f, 0.15f, 0.15f);

	// ライトの方向設定 ( ベクトル )
	m_vecDir[0] = D3DXVECTOR3(0.13f, -0.53f, 0.76f);
	m_vecDir[1] = D3DXVECTOR3(-0.23f, -0.78f, -0.56f);
	m_vecDir[2] = D3DXVECTOR3(-0.11f, 0.25f, 0.92f);

	// 各ライトの設定
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{	
		// 正規化
		D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]); 

		m_aLight[nCnt].Direction = m_vecDir[nCnt];
	}

	int pIdx = 3; // インデックス
	m_aLight[pIdx].Type = D3DLIGHT_POINT; // ポイントに変更

	// ライトの配置座標
	m_aLight[pIdx].Position = D3DXVECTOR3(0.0f,300.0f, 0.0f);

	// ライトの色
	m_aLight[pIdx].Diffuse = COLOR_WHITE;
	m_aLight[pIdx].Specular = COLOR_WHITE;
	m_aLight[pIdx].Ambient = COLOR_WHITE;

	// 光の届く最大半径
	m_aLight[pIdx].Theta = D3DXToRadian(15);
	m_aLight[pIdx].Phi = D3DXToRadian(45);
	m_aLight[pIdx].Falloff = 0.5f;
	m_aLight[pIdx].Range = 20000.0f;

	// 光の減衰率の設定
	m_aLight[pIdx].Attenuation0 = 1.0f; // 常数減衰
	m_aLight[pIdx].Attenuation1 = 0.0f; // 線形減衰
	m_aLight[pIdx].Attenuation2 = 0.0f; // 2乗減衰

	// すべてのライトをデバイスに登録
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{
		// ライトの設定
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		// ライトを有効化
		pDevice->LightEnable(nCnt, TRUE);
	}

	return S_OK;
}
//=========================================================
// ライトの終了処理
//=========================================================
void CLight::Uninit(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// すべてのライトをデバイスに登録
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{
		// ライトを有効化
		pDevice->LightEnable(nCnt, FALSE);
	}

	ZeroMemory(&m_aLight, sizeof(m_aLight));
}
//=========================================================
// ライトの更新処理
//=========================================================
void CLight::Update(void)
{
	
}
//=========================================================
// ライトの設定処理
//=========================================================
void CLight::SetLight(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// すべてのライトをデバイスに登録
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{
		// ライトを有効化
		pDevice->LightEnable(nCnt, FALSE);
	}

	// ライトのゼロクリア処理
	ZeroMemory(&m_aLight, sizeof(m_aLight));

	for (int i = 0; i < NUMLIGHT; i++)
	{
		m_aLight[i].Type = D3DLIGHT_DIRECTIONAL;
		m_aLight[i].Diffuse = COLOR_BLACK;
	}

	// ライトの位置
	m_aLight[0].Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_aLight[1].Position = D3DXVECTOR3(0.65f, 0.65f, 0.65f);
	m_aLight[2].Position = D3DXVECTOR3(0.15f, 0.15f, 0.15f);

	// ライトの方向設定 ( ベクトル )
	m_vecDir[0] = D3DXVECTOR3(0.13f, -0.53f, 0.76f);
	m_vecDir[1] = D3DXVECTOR3(-0.23f, -0.78f, -0.56f);
	m_vecDir[2] = D3DXVECTOR3(-0.11f, 0.25f, 0.92f);

	// 各ライトの設定
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{
		// 正規化
		D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]);

		m_aLight[nCnt].Direction = m_vecDir[nCnt];
	}

	int pIdx = 3; // インデックス
	m_aLight[pIdx].Type = D3DLIGHT_SPOT; // ポイントに変更

	// ライトの配置座標
	m_aLight[pIdx].Position = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
	m_aLight[pIdx].Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	// ライトの色
	m_aLight[pIdx].Diffuse = COLOR_WHITE;
	m_aLight[pIdx].Specular = COLOR_WHITE;
	m_aLight[pIdx].Ambient = COLOR_WHITE;

	// 光の届く最大半径
	m_aLight[pIdx].Theta = D3DXToRadian(15);
	m_aLight[pIdx].Phi = D3DXToRadian(45);
	m_aLight[pIdx].Falloff = 0.5f;
	m_aLight[pIdx].Range = 2000.0f;

	// 光の減衰率の設定
	m_aLight[pIdx].Attenuation0 = 1.0f; // 常数減衰
	m_aLight[pIdx].Attenuation1 = 0.0f; // 線形減衰
	m_aLight[pIdx].Attenuation2 = 0.0f; // 2乗減衰

	// すべてのライトをデバイスに登録
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{
		// ライトの設定
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		// ライトを有効化
		pDevice->LightEnable(nCnt, TRUE);
	}
}