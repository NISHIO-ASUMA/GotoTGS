//=========================================================
// 
// インスタンシングシェーダークラス [ instancing.h ]
// Author : Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インスタンシング適用クラスを定義
//*********************************************************
class CInstancing
{
public:

	//******************************
	// インスタンスデータ構造体
	//******************************
	struct INSTANCE_DATA
	{
		D3DXVECTOR4 color;	 // カラー
		D3DXMATRIX mtxWorld; // ワールドマトリックス
	};

	~CInstancing();

	HRESULT Init(const char* pShaderFile);
	void Uninit(void);
	void Begin(void);
	void BeginPass(const int nPass = 0);
	void EndInstancing(void);
	void EndPass(void);
	void End(void);

	/// <summary>
	/// シェーダーパラメーター設定
	/// </summary>
	/// <param name="Texture">テクスチャ</param>
	/// <param name="color">カラー</param>
	void SetInstancingParam
	(
		const LPDIRECT3DTEXTURE9 &Texture = nullptr,
		const D3DXVECTOR4 &color = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f)
	);

	/// <summary>
	/// インスタンス開始設定
	/// </summary>
	/// <param name="instanceCount">インスタンス数</param>
	/// <param name="meshVB">メッシュ頂点VB(stream0)</param>
	/// <param name="meshStride">メッシュ頂点サイズ</param>
	/// <param name="meshIB">インデックスバッファ</param>
	/// <param name="instanceVB">インスタンスVB(stream1)</param>
	/// <param name="instanceStride">インスタンス1要素サイズ</param>
	void BeginInstancing
	(
		int instanceCount,
		LPDIRECT3DVERTEXBUFFER9 meshVB,
		UINT meshStride,
		LPDIRECT3DINDEXBUFFER9 meshIB,
		LPDIRECT3DVERTEXBUFFER9 instanceVB,
		UINT instanceStride
	);

	/// <summary>
	/// DirectDeviceを取得する
	/// </summary>
	/// <param name="pDevice">レンダラーのD3Dデバイス</param>
	inline void SetDeviceToInstancing(const LPDIRECT3DDEVICE9& pDevice)
	{
		m_pInstanceDevice = pDevice;
	}

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CInstancing* GetInstance(void)
	{
		static CInstancing pInstance;
		return &pInstance;
	};

private:

	CInstancing();								 // 格納コンストラクタ

	LPDIRECT3DVERTEXDECLARATION9 m_pDeclaration; // デクレーションバッファ
	LPD3DXEFFECT m_pInstancing;					 // シェーダーエフェクトハンドル
	LPDIRECT3DDEVICE9 m_pInstanceDevice;		 // インスタンシングデバイスポインタ
};