//=============================================================================
//
// メッシュ地面の処理 [meshcloud.cpp]
// Author :  GP12A295 25 人見友基
//
//=============================================================================
#include "meshcloud.h"
#include "input.h"
#include "calculate.h"
#include "model.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				g_pD3DTextureMeshCloud;		// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9			g_pD3DVtxBuffMeshCloud;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DINDEXBUFFER9			g_pD3DIdxBuffMeshCloud;		// インデックスバッファインターフェースへのポインタ

MESHCLOUD				meshcloudWk[MESHCLOUD_MAX];

int g_nNumBlockXCloud,			g_nNumBlockZCloud;		// ブロック数
int g_nNumVertexCloud;									// 総頂点数	
int g_nNumVertexIndexCloud;								// 総インデックス数
int g_nNumPolygonCloud;									// 総ポリゴン数
float g_fBlockSizeXCloud,			g_fBlockSizeZCloud;		// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshcloud(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ, int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHCLOUD *meshcloud = &meshcloudWk[0];

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// ポリゴン表示位置の中心座標を設定
		meshcloud->vecPos = pos;
		if (i == 1)
		{
			meshcloud->vecRot = rot + D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		}
		else
		{
			meshcloud->vecRot = rot;
		}
		meshcloud->fVAnglel;
	}

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_FILENAME,		// ファイルの名前
			&g_pD3DTextureMeshCloud);	// 読み込むメモリー
	}

	// ブロック数の設定
	g_nNumBlockXCloud = nNumBlockX;
	g_nNumBlockZCloud = nNumBlockZ;

	// 頂点数の設定
	g_nNumVertexCloud = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	g_nNumVertexIndexCloud = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	g_nNumPolygonCloud = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	g_fBlockSizeXCloud = fBlockSizeX;
	g_fBlockSizeZCloud = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCloud,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffMeshCloud,				// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexCloud,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												D3DFMT_INDEX16,						// 使用するインデックスフォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DIdxBuffMeshCloud,				// インデックスバッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffMeshCloud->Lock( 0, 0, (void**)&meshcloud->vertexWK, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZCloud + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXCloud / 2.0f) * g_fBlockSizeXCloud + nCntVtxX * g_fBlockSizeXCloud;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = 0.0f;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.z = (g_nNumBlockZCloud / 2.0f) * g_fBlockSizeZCloud - nCntVtxZ * g_fBlockSizeZCloud;

				// 法線の設定
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// 反射光の設定
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].diffuse = D3DXCOLOR(MESHCLOUD_DIFFUSE_R, MESHCLOUD_DIFFUSE_G, MESHCLOUD_DIFFUSE_B, MESHCLOUD_DIFFUSE_A);

				// テクスチャ座標の設定
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX / 5;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ / 5;
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffMeshCloud->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DIdxBuffMeshCloud->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZCloud; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXCloud + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXCloud + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (g_nNumBlockZCloud - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXCloud + 1) + g_nNumBlockXCloud;
				nCntIdx++;
			}
		}

		// インデックスデータをアンロックする
		g_pD3DIdxBuffMeshCloud->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 座標初期化処理
//=============================================================================
HRESULT InitMeshcloudPos(void)
{
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	meshcloud->vecPos = D3DXVECTOR3(MESHCLOUD_POS_X, 0.0f, MESHCLOUD_POS_Z);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshcloud(void)
{
	if(g_pD3DVtxBuffMeshCloud)
	{// 頂点バッファの開放
		g_pD3DVtxBuffMeshCloud->Release();
		g_pD3DVtxBuffMeshCloud = NULL;
	}

	if(g_pD3DIdxBuffMeshCloud)
	{// インデックスバッファの開放
		g_pD3DIdxBuffMeshCloud->Release();
		g_pD3DIdxBuffMeshCloud = NULL;
	}

	if(g_pD3DTextureMeshCloud)
	{// テクスチャの開放
		g_pD3DTextureMeshCloud->Release();
		g_pD3DTextureMeshCloud = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshcloud(void)
{
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	MODEL *model = GetModel(0);

	if (model->posModel.x - meshcloud->vecPos.x > MESHCLOUD_RESET_X || meshcloud->vecPos.x - model->posModel.x > MESHCLOUD_RESET_X)
	{
		meshcloud->vecPos.x = model->posModel.x;
	}
	if (model->posModel.z - meshcloud->vecPos.z > MESHCLOUD_RESET_Z || meshcloud->vecPos.z - model->posModel.z> MESHCLOUD_RESET_Z)
	{
		meshcloud->vecPos.z = model->posModel.z;
	}

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffMeshCloud->Lock(0, 0, (void**)&meshcloud->vertexWK, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZCloud + 1); nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXCloud + 1); nCntVtxX++)
			{
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXCloud / 2.0f) * g_fBlockSizeXCloud + nCntVtxX * g_fBlockSizeXCloud;
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = 0.0f;
				meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.y = sinf(meshcloud->fVAnglel) *MESHCLOUD_WAVE;
				//meshcloud->vertexWK[nCntVtxZ * (g_nNumBlockXCloud + 1) + nCntVtxX].vtx.z = (g_nNumBlockZCloud / 2.0f) * g_fBlockSizeZCloud - nCntVtxZ * g_fBlockSizeZCloud;
				meshcloud->fVAnglel += MESHCLOUD_WAVE_SPEED;
				meshcloud->fVAnglel = PiCalculate180(meshcloud->fVAnglel);
			}
			meshcloud->fVAnglel += MESHCLOUD_WAVE_SPEED;
			meshcloud->fVAnglel = PiCalculate180(meshcloud->fVAnglel);
			// 反射光の設定

		}
		//for (int i = 0; i < (MESHCLOUD_X + 1)*(MESHCLOUD_Z + 1); i++)
		//{
		//	g_pVtx[i].vtx.y = sinf(fVAnglel) *MESHCLOUD_WAVE_UPDATE;
		//	fVAnglel += MESHCLOUD_WAVE_SPEED_UPDATE;
		//	fVAnglel = PiCalculate360(fVAnglel);
		//}

		// 頂点データをアンロックする
		g_pD3DVtxBuffMeshCloud->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshcloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHCLOUD *meshcloud = &meshcloudWk[0];
	D3DXMATRIX mtxRot, mtxTranslate;

	//// ラインティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MESHCLOUD_MAX; i++, meshcloud++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&meshcloud->mtxWorldCloud);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, meshcloud->vecRot.y, meshcloud->vecRot.x, meshcloud->vecRot.z);
		D3DXMatrixMultiply(&meshcloud->mtxWorldCloud, &meshcloud->mtxWorldCloud, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, meshcloud->vecPos.x, meshcloud->vecPos.y, meshcloud->vecPos.z);
		D3DXMatrixMultiply(&meshcloud->mtxWorldCloud, &meshcloud->mtxWorldCloud, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &meshcloud->mtxWorldCloud);

		// 頂点バッファをレンダリングパイプラインに設定
		pDevice->SetStreamSource(0, g_pD3DVtxBuffMeshCloud, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		pDevice->SetIndices(g_pD3DIdxBuffMeshCloud);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureMeshCloud);

		// ポリゴンの描画
		//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_pVtx, sizeof(VERTEX_3D));
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexCloud, 0, g_nNumPolygonCloud);
	}

	//// ラインティングを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

