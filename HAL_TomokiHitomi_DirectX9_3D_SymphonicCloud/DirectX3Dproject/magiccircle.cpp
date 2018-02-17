//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : GP11B243 24 人見友基 
//
//=============================================================================
#include "enemy.h"
#include "magiccircle.h"
#include "magic.h"
#include "main.h"
#include "calculate.h"
#include "model.h"
#include "sound.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT ReInitMagiccircle(int nType);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureMagiccircle;		// テクスチャへのポインタ
D3DMATERIAL9		*g_pD3DMatMeshMagiccircle;

LPD3DXMESH			g_pD3DXMeshMagiccircle;			// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatMagiccircle;		// マテリアル情報へのポインタ
DWORD				g_nNumMatMagiccircle;			// マテリアル情報の数
D3DXMATRIX			g_mtxWorldMagiccircle;			// ワールドマトリックス

MAGICCIRCLE		magiccircleWk[MAGICCIRCLE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMagiccircle(int nType)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		ReInitMagiccircle(i);
	}

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureMagiccircle = NULL;
		g_pD3DXMeshMagiccircle = NULL;
		g_pD3DXBuffMatMagiccircle = NULL;
		g_nNumMatMagiccircle = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(MAGICCIRCLE_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatMagiccircle,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatMagiccircle,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshMagiccircle)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// マテリアル情報を取り出す
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatMagiccircle->GetBufferPointer();
		g_pD3DMatMeshMagiccircle = new D3DMATERIAL9[g_nNumMatMagiccircle];		// メッシュ情報を確保
		g_pD3DTextureMagiccircle = new LPDIRECT3DTEXTURE9[g_nNumMatMagiccircle];// テクスチャを確保
		for (int i = 0; i < g_nNumMatMagiccircle; i++)
		{
			g_pD3DMatMeshMagiccircle[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
			g_pD3DMatMeshMagiccircle[i].Ambient = g_pD3DMatMeshMagiccircle[i].Diffuse;// 環境光初期化
			g_pD3DTextureMagiccircle[i] = NULL;	// テクスチャ初期化

			// 使用しているテクスチャがあれば読み込む
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// テクスチャ読み込み
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureMagiccircle[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 再初期化処理
//=============================================================================
HRESULT ReInitMagiccircle(int nType)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nType];
	magiccircle->bUse = false;
	magiccircle->bSe = false;
	magiccircle->pos = D3DXVECTOR3(0.0, 0.0f, 0.0f);
	magiccircle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//float fRotTemp = D3DX_PI * 2.0f / MAGICCIRCLE_MAX * nType;
	//magiccircle->rot = D3DXVECTOR3(fRotTemp, fRotTemp, fRotTemp);
	magiccircle->scl = D3DXVECTOR3(0.0f, MAGICCIRCLE_SCALE_Y, 0.0f);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagiccircle(void)
{
	for (int i = 0; i < g_nNumMatMagiccircle; i++)
	{	// テクスチャの開放
		if (g_pD3DTextureMagiccircle[i] != NULL)
		{
			g_pD3DTextureMagiccircle[i]->Release();
			g_pD3DTextureMagiccircle[i] = NULL;
		}
	}

	if(g_pD3DXMeshMagiccircle != NULL)
	{// メッシュの開放
		g_pD3DXMeshMagiccircle->Release();
		g_pD3DXMeshMagiccircle = NULL;
	}

	if(g_pD3DXBuffMatMagiccircle != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatMagiccircle->Release();
		g_pD3DXBuffMatMagiccircle = NULL;
	}

	delete[]g_pD3DMatMeshMagiccircle;
	delete[]g_pD3DTextureMagiccircle;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagiccircle(void)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (magiccircle->bUse)
		{
			// スケールチェンジ
			if (magiccircle->scl.x < MAGICCIRCLE_SCALE || magiccircle->scl.z < MAGICCIRCLE_SCALE)
			{
				magiccircle->scl.x += MAGICCIRCLE_SCALE_CHANGE;
				magiccircle->scl.z += MAGICCIRCLE_SCALE_CHANGE;
				if (magiccircle->scl.x > MAGICCIRCLE_SCALE || magiccircle->scl.z > MAGICCIRCLE_SCALE)
				{
					magiccircle->scl = D3DXVECTOR3(MAGICCIRCLE_SCALE, MAGICCIRCLE_SCALE_Y, MAGICCIRCLE_SCALE);
					SetMagicChargeModel(0);
					if (!magiccircle->bSe)
					{
						if (i == MAGICCIRCLE_MAX - 1)
						{
							SetSe(SE_CLAP009, E_DS8_FLAG_NONE, CONTINUITY_ON);
						}
						else
						{
							SetSe(SE_OPENHAT, E_DS8_FLAG_NONE, CONTINUITY_ON);
						}
						magiccircle->bSe = true;
					}
				}
			}

			// 回転処理
			if (i % 2 == 0)
			{
				//magiccircle->rot.x += MAGICCIRCLE_ROT;
				magiccircle->rot.y += MAGICCIRCLE_ROT_Y;
				//magiccircle->rot.z += MAGICCIRCLE_ROT;
			}
			else
			{
				//magiccircle->rot.x -= MAGICCIRCLE_ROT;
				magiccircle->rot.y -= MAGICCIRCLE_ROT_Y;
				//magiccircle->rot.z -= MAGICCIRCLE_ROT;
			}
			// 回転量調整
			//magiccircle->rot.x = PiCalculate180(magiccircle->rot.y);
			magiccircle->rot.y = PiCalculate180(magiccircle->rot.y);
			//magiccircle->rot.z = PiCalculate180(magiccircle->rot.y);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagiccircle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	MAGICCIRCLE *magiccircle = &magiccircleWk[0];

	//// ラインティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, MAGICCIRCLE_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 両面表示
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	D3DXCOLOR colorTemp = GetMagicColor();
	//colorTemp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_pD3DMatMeshMagiccircle[0].Diffuse = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshMagiccircle[0].Ambient = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshMagiccircle[0].Specular = (D3DCOLORVALUE)colorTemp;
	colorTemp *= 0.5f;
	g_pD3DMatMeshMagiccircle[0].Emissive = (D3DCOLORVALUE)colorTemp;

	//g_pD3DMatMeshMagiccircle[0].Power = 3.0f;


	//g_pD3DMatMeshMagiccircle->Emissive.r = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.g = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.b = 0.5f;
	//g_pD3DMatMeshMagiccircle->Emissive.a = 1.0f;

	//g_pD3DMatMeshMagiccircle->Diffuse.r = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.g = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.b = 0.5f;
	//g_pD3DMatMeshMagiccircle->Diffuse.a = 1.0f;

	//DWORD dwTemp[3] = { 0,0,0 };

	//pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwTemp[0]);
	//pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTemp[1]);
	//pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwTemp[2]);

	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (magiccircle->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldMagiccircle);

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, magiccircle->scl.x, magiccircle->scl.y, magiccircle->scl.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxScl);

			// 【R】回1転を反映(YawPitchRollはy,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, magiccircle->rot.y, magiccircle->rot.x, magiccircle->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, magiccircle->pos.x, magiccircle->pos.y, magiccircle->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldMagiccircle, &g_mtxWorldMagiccircle, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMagiccircle);

			/******************** ビューポート変換 ********************/
			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);
			//// マテリアル情報に対するポインタの取得
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatMagiccircle->GetBufferPointer();

			for (int j = 0; j < g_nNumMatMagiccircle; j++)
			{
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


				// マテリアルの設定
				pDevice->SetMaterial(&g_pD3DMatMeshMagiccircle[j]);

				// テクスチャの設定（NULL:テクスチャ無し）
				//pDevice->SetTexture(0, NULL);

				pDevice->SetTexture(0, g_pD3DTextureMagiccircle[j]);

				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


				
				// 描画
				g_pD3DXMeshMagiccircle->DrawSubset(j);
			}
			// マテリアルを元に戻す
			pDevice->SetMaterial(&matDef);
		}
	}
	//pDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwTemp[0]);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwTemp[1]);
	//pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwTemp[2]);

	// 片面表示
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//// ラインティングを有効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 出現処理
//=============================================================================
int SetMagiccircle(void)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[0];
	for (int i = 0; i < MAGICCIRCLE_MAX; i++, magiccircle++)
	{
		if (!magiccircle->bUse)
		{
			magiccircle->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void SetMagiccirclePos(int nIndex, D3DXVECTOR3 pos, float fRotY)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nIndex];
	magiccircle->pos = pos;
	magiccircle->pos.y += MAGICCIRCLE_HEIGHT + nIndex * MAGICCIRCLE_HEIGHT_INTERVAL;
	magiccircle->rot.y += fRotY;
}

//=============================================================================
// 解放処理
//=============================================================================
void ReleaseMagiccircle(int nIndex)
{
	MAGICCIRCLE *magiccircle = &magiccircleWk[nIndex];
	ReInitMagiccircle(nIndex);
}

//=============================================================================
// 取得関数
//=============================================================================
MAGICCIRCLE *GetMagiccircle(int no)
{
	return(&magiccircleWk[no]);
}
