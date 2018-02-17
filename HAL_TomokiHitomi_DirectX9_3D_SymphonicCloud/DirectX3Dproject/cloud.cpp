//=============================================================================
//
// Cloud処理 [cloud.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "camera.h"
#include "cloud.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCloud(LPDIRECT3DDEVICE9 pDevice);
void SetVtxCloud(int nCloud, float fSizeX, float fSizeY);
void SetDiffuseCloud(int nCloud, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureCloud = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffCloud = NULL;	// 頂点バッファへのポインタ

CLOUD					cloudWk[CLOUD_MAX];
D3DXMATRIX				g_mtxWorldCloud;			// ワールドマトリックス


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCloud(int nType)
{
	CLOUD *cloud = &cloudWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_CLOUD,
			&g_pD3DTextureCloud);
	}

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		// 位置・回転・スケールの設定
		cloud->posCloud = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cloud->rotCloud = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		cloud->sclCloud = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		InitStatusCloud(i);

	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexCloud(pDevice);


	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusCloud(int nCloud)
{
	CLOUD *cloud = &cloudWk[nCloud];

	cloud->bUse = false;
	cloud->vec2Size = D3DXVECTOR2(CLOUD_SIZE_X, CLOUD_SIZE_Y);
	cloud->colorCloud = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	cloud->nTex = 0;
	cloud->fSizeChange = 0.0f;
	cloud->fAlphaChange = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCloud(void)
{
	if (g_pD3DTextureCloud != NULL)
	{// テクスチャの開放
		g_pD3DTextureCloud->Release();
		g_pD3DTextureCloud = NULL;
	}

	if (g_pD3DVtxBuffCloud != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffCloud->Release();
		g_pD3DVtxBuffCloud = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCloud(void)
{
	CLOUD *cloud = &cloudWk[0];
	MODEL *model = GetModel(0);

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (cloud->bUse)
		{
			cloud->colorCloud.a -= CLOUD_ALPHA_CHANGE;
			cloud->vec2Size -= D3DXVECTOR2(CLOUD_SIZE_CHANGE, CLOUD_SIZE_CHANGE);

			if (cloud->vec2Size.x <= 0.0f || cloud->vec2Size.y <= 0.0f)
			{
				cloud->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
			}
			if (cloud->colorCloud.a <= 0.0f)
			{
				cloud->colorCloud.a = 0.0f;
			}

			if (cloud->colorCloud.a <= 0.0f)
			{
				InitStatusCloud(i);
			}

			SetVtxCloud(i, cloud->vec2Size.x, cloud->vec2Size.y);
			SetDiffuseCloud(i, cloud->colorCloud);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// スケール, 回転, 平行移動

	CLOUD *cloud = &cloudWk[0];

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (cloud->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldCloud);

			// ビューマトリックスを取得
			mtxView = GetMtxView();
			// ビルボード化
			g_mtxWorldCloud._11 = mtxView._11;
			g_mtxWorldCloud._12 = mtxView._21;
			g_mtxWorldCloud._13 = mtxView._31;
			g_mtxWorldCloud._21 = mtxView._12;
			g_mtxWorldCloud._22 = mtxView._22;
			g_mtxWorldCloud._23 = mtxView._32;
			g_mtxWorldCloud._31 = mtxView._13;
			g_mtxWorldCloud._32 = mtxView._23;
			g_mtxWorldCloud._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, cloud->sclCloud.x, cloud->sclCloud.y, cloud->sclCloud.z);
			D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, cloud->rotCloud.y, cloud->rotCloud.x, cloud->rotCloud.z);
			//D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &GetInvRotateMat(cloud->posCloud));
			////D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, cloud->posCloud.x, cloud->posCloud.y, cloud->posCloud.z);
			D3DXMatrixMultiply(&g_mtxWorldCloud, &g_mtxWorldCloud, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCloud);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffCloud, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureCloud);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCloud(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * CLOUD_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffCloud,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < CLOUD_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-CLOUD_SIZE_X, -CLOUD_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-CLOUD_SIZE_X, CLOUD_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(CLOUD_SIZE_X, -CLOUD_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(CLOUD_SIZE_X, CLOUD_SIZE_Y, 0.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_CLOUD;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_CLOUD;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffCloud->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxCloud(int nCloud, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCloud * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffCloud->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseCloud(int nCloud, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffCloud->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCloud * 4);

		// 頂点座標の設定
			pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffCloud->Unlock();
	}
}

//=============================================================================
// 対象に設置
//=============================================================================
void SetCloud(D3DXVECTOR3 vecPos)
{
	CLOUD *cloud = &cloudWk[0];

	// 未使用を探す
	for (int i = 0; i < CLOUD_MAX; i++, cloud++)
	{
		if (!cloud->bUse)
		{
			cloud->bUse = true;
			cloud->posCloud = vecPos;
			return;
		}
	}
}
//=============================================================================
// 取得関数
//=============================================================================
CLOUD *GetCloud(int no)
{
	return(&cloudWk[no]);
}