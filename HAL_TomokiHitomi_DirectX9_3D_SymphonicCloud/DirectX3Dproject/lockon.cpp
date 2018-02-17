//=============================================================================
//
// Lockon処理 [lockon.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "lockon.h"
#include "model.h"
#include "enemy.h"
#include "camera.h"
#include "calculate.h"
#include "checkhit.h"
#include "magic.h"

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
HRESULT MakeVertexLockon(LPDIRECT3DDEVICE9 pDevice);
void SetVtxLockon(int nLockon);
void SetDiffuseLockon(int nLockon, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureLockon = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffLockon = NULL;	// 頂点バッファへのポインタ

LOCKON					lockonWk[LOCKON_MAX];
D3DXMATRIX				g_mtxWorldLockon;			// ワールドマトリックス

float					g_fBaseAngleLockon;			// 角度（θ）
float					g_fRadiusLockon;			// 半径
int						g_nLockonMagicType;			// マジックタイプのログ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitLockon(int nType)
{
	LOCKON *lockon = &lockonWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_LOCKON,
			&g_pD3DTextureLockon);
	}

	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		InitStatusLockon(i);
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexLockon(pDevice);


	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusLockon(int nLockon)
{
	LOCKON *lockon = &lockonWk[nLockon];

	// 位置・回転・スケールの設定
	lockon->posLockon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lockon->rotLockon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	lockon->bUse = false;
	lockon->bRelease = false;
	lockon->vec2Size = D3DXVECTOR2(LOCKON_SIZE_X, LOCKON_SIZE_Y);
	lockon->colorLockon = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	lockon->nTex = 0;
	lockon->nIndex = 0;
	lockon->nUseCount = 0;
	lockon->nReleaseCount = 0;
	lockon->nCoolDown = 0;
	lockon->fSizeChange = 1.0f;
	lockon->fAlphaChange = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLockon(void)
{
	if (g_pD3DTextureLockon != NULL)
	{// テクスチャの開放
		g_pD3DTextureLockon->Release();
		g_pD3DTextureLockon = NULL;
	}

	if (g_pD3DVtxBuffLockon != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffLockon->Release();
		g_pD3DVtxBuffLockon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLockon(void)
{
	LOCKON *lockon = &lockonWk[0];

#ifdef _DEBUG
	PrintDebugProc("【 LOCKON 】\n");
#endif

	// マジックタイプ変更時はロックオンを一度リリース
	//int nMagicType = GetTypeMagic();
	//bool bRelease;
	//if (g_nLockonMagicType == nMagicType)
	//{
	//	bRelease = false;
	//}
	//else
	//{
	//	bRelease = true;
	//}
	//g_nLockonMagicType = nMagicType;



	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		lockon->nCoolDown--;
		if (lockon->bUse)
		{
			if (lockon->nReleaseCount == LOCKON_RELEASE_COUNT)
			{
				ReleaseLockon(i);
			}

			if (lockon->bRelease)
			{
				lockon->fSizeChange += LOCKON_SCALE_CHANGE;
				lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * lockon->fSizeChange;
				if (lockon->fSizeChange > LOCKON_SCALE_MAX)
				{
					InitStatusLockon(i);
					lockon->nCoolDown = LOCKON_COOLDOWN;
				}
			}
			else
			{
				if (lockon->fSizeChange > LOCKON_SCALE_MIN)
				{
					lockon->fSizeChange -= LOCKON_SCALE_CHANGE;
					lockon->sclLockon = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * lockon->fSizeChange;
				}
			}

			lockon->rotLockon.z += 0.02f;
			lockon->rotLockon.z = PiCalculate360(lockon->rotLockon.z);
			SetVtxLockon(i);
			SetDiffuseLockon(i, GetMagicColor());			// マジックのカラーを取得して適用

			// デバッグ用
#ifdef _DEBUG
			PrintDebugProc("Lockon[%d]  rotZ:%f  Angle:%f  Len:%f\n", i,lockon->rotLockon.z, g_fBaseAngleLockon, g_fRadiusLockon);
#endif
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLockon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// スケール, 回転, 平行移動

	LOCKON *lockon = &lockonWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, LOCKON_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		if (lockon->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldLockon);

			// ビューマトリックスを取得
			mtxView = GetMtxView();
			// ビルボード化
			g_mtxWorldLockon._11 = mtxView._11;
			g_mtxWorldLockon._12 = mtxView._21;
			g_mtxWorldLockon._13 = mtxView._31;
			g_mtxWorldLockon._21 = mtxView._12;
			g_mtxWorldLockon._22 = mtxView._22;
			g_mtxWorldLockon._23 = mtxView._32;
			g_mtxWorldLockon._31 = mtxView._13;
			g_mtxWorldLockon._32 = mtxView._23;
			g_mtxWorldLockon._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, lockon->sclLockon.x, lockon->sclLockon.y, lockon->sclLockon.z);
			D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, lockon->rotLockon.y, lockon->rotLockon.x, lockon->rotLockon.z);
			//D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &GetInvRotateMat(lockon->posLockon));
			////D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, lockon->posLockon.x, lockon->posLockon.y, lockon->posLockon.z);
			D3DXMatrixMultiply(&g_mtxWorldLockon, &g_mtxWorldLockon, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLockon);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffLockon, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureLockon);

			// ポリゴンの描画
			//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, lockon->vertexWK, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
			//pDevice->DrawPrimitive(D3DPT_POINTLIST, (i * 4), NUM_POLYGON);


		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexLockon(LPDIRECT3DDEVICE9 pDevice)
{
	// 角度と半径を求める
	g_fBaseAngleLockon = atan2f(LOCKON_SIZE_Y, LOCKON_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(LOCKON_SIZE_X, LOCKON_SIZE_Y);
	g_fRadiusLockon = D3DXVec2Length(&temp);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * LOCKON_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffLockon,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < LOCKON_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-LOCKON_SIZE_X, -LOCKON_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-LOCKON_SIZE_X, LOCKON_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(LOCKON_SIZE_X, -LOCKON_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(LOCKON_SIZE_X, LOCKON_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_LOCKON;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_LOCKON;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffLockon->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxLockon(int nLockon)
{
	LOCKON *lockon = &lockonWk[nLockon];

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nLockon * 4);

		// 頂点0:回転あり
		pVtx[0].vtx.x = 0.0f - cosf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[0].vtx.y = 0.0f - sinf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[0].vtx.z = 0.0f;

		// 頂点1:回転あり
		pVtx[1].vtx.x = 0.0f - cosf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[1].vtx.y = 0.0f + sinf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[1].vtx.z = 0.0f;

		// 頂点2:回転あり
		pVtx[2].vtx.x = 0.0f + cosf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[2].vtx.y = 0.0f - sinf(g_fBaseAngleLockon - lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[2].vtx.z = 0.0f;

		// 頂点3:回転あり
		pVtx[3].vtx.x = 0.0f + cosf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[3].vtx.y = 0.0f + sinf(g_fBaseAngleLockon + lockon->rotLockon.z) * g_fRadiusLockon;
		pVtx[3].vtx.z = 0.0f;

		// 頂点データをアンロックする
		g_pD3DVtxBuffLockon->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseLockon(int nLockon, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffLockon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nLockon * 4);

		// 頂点座標の設定
			pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffLockon->Unlock();
	}
}

//=============================================================================
// 対象に設置
//=============================================================================
int SetLockon(D3DXVECTOR3 vecPos)
{
	LOCKON *lockon = &lockonWk[0];

	// 未使用を探す
	for (int i = 0; i < LOCKON_MAX; i++, lockon++)
	{
		if (!lockon->bUse && lockon->nCoolDown <= 0)
		{
			lockon->bUse = true;
			lockon->posLockon = vecPos;
			return i;
		}
	}
}

//=============================================================================
// 対象座標に移動
//=============================================================================
void SetPosLockon(int nIndex, D3DXVECTOR3 vecPos)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->posLockon = vecPos;
}

//=============================================================================
// 未使用処理
//=============================================================================
void ReleaseLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->bRelease = true;
}

//=============================================================================
// リリースカウント加算処理
//=============================================================================
bool AddReleaseCountLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->nReleaseCount++;
	if (lockon->nReleaseCount > LOCKON_RELEASE_COUNT)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//=============================================================================
// リリースカウント初期化処理
//=============================================================================
void ResetReleaseCountLockon(int nIndex)
{
	LOCKON *lockon = &lockonWk[nIndex];
	lockon->nReleaseCount = 0;
}

//=============================================================================
// 取得関数
//=============================================================================
LOCKON *GetLockon(int no)
{
	return(&lockonWk[no]);
}