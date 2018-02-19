//=============================================================================
//
// バレット処理 [enemybullet.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "camera.h"
#include "enemybullet.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "calculate.h"

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

void InitStatusEnemybulletSys(int nEnemybulletSys);
HRESULT MakeVertexEnemybullet(LPDIRECT3DDEVICE9 pDevice);
void SetVtxEnemybullet(int nEnemybullet, float fSizeX, float fSizeY);
void SetDiffuseEnemybullet(int nEnemybullet, D3DXCOLOR col);
void SetTexEnemybullet(int nEnemybullet, int nTex);
void SetSysEnemybulletFire(int nSys);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemybullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEnemybullet = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldEnemybullet;			// ワールドマトリックス

ENEMYBULLET					enemybulletWk[ENEMYBULLET_MAX];
ENEMYBULLET_SYS				enemybulletSysWk[ENEMYBULLET_SYS_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemybullet(int nType)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ENEMYBULLET,
			&g_pD3DTextureEnemybullet);
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		// 位置・回転・スケールの設定
		enemybullet->posEnemybullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->rotEnemybullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->sclEnemybullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemybullet->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->colorEnemybullet = D3DXCOLOR(ENEMYBULLET_DIF_R, ENEMYBULLET_DIF_G, ENEMYBULLET_DIF_B, ENEMYBULLET_DIF_A);
		InitStatusEnemybullet(i);
	}

	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++,enemybulletSys)
	{
		enemybulletSys->nCoolDown = 0;
		InitStatusEnemybulletSys(i);
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexEnemybullet(pDevice);

	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusEnemybullet(int nEnemybullet)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[nEnemybullet];

	enemybullet->vec2Size = D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);
	enemybullet->colorEnemybullet = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	enemybullet->nUseCount = 0;
	enemybullet->nSysNum = 0;
	enemybullet->nTex = 0;
	enemybullet->nEffectCount = 0;
	enemybullet->nAnimeCount = 0;
	enemybullet->nAnimePattern = 0;
	enemybullet->nType = 0;

	enemybullet->fMoveSpeed = 0.0f;
	enemybullet->fHAngle = 0.0f;
	enemybullet->fVAngle = 0.0f;
	enemybullet->fVecPower = 0.0f;

	enemybullet->bUse = false;
	enemybullet->bEnemy = false;
	enemybullet->bVertex = false;
	enemybullet->bHoming = true;

	enemybullet->nIdxShadow = 0;
	enemybullet->fSizeShadow = 0.0f;
	enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// 再利用処理SYS
//=============================================================================
void InitStatusEnemybulletSys(int nEnemybulletSys)
{
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[nEnemybulletSys];
	enemybulletSys->vecTagPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemybulletSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	enemybulletSys->colorEnemybullet = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	enemybulletSys->fHAngle = 0.0f;
	enemybulletSys->fVAngle = 0.0f;
	enemybulletSys->fMoveSpeed = 0.0f;
	enemybulletSys->fWave = 0.0f;

	enemybulletSys->nType = 0;
	enemybulletSys->nFireTime = 0;
	enemybulletSys->nSplitCount = 0;
	enemybulletSys->nTex = 0;
	enemybulletSys->nEnemybulletCount = 0;

	enemybulletSys->bUse = false;
	enemybulletSys->bProcess = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemybullet(void)
{
	if(g_pD3DTextureEnemybullet != NULL)
	{// テクスチャの開放
		g_pD3DTextureEnemybullet->Release();
		g_pD3DTextureEnemybullet = NULL;
	}

	if(g_pD3DVtxBuffEnemybullet != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEnemybullet->Release();
		g_pD3DVtxBuffEnemybullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemybullet(void)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// 使用中のシステムを探す
	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++, enemybulletSys++)
	{
		if (enemybulletSys->bProcess)
		{
			SetSysEnemybulletFire(i);
		}
		if (enemybulletSys->bUse)
		{
			if (enemybulletSys->nEnemybulletCount > 0)
			{
			}
			else
			{
				InitStatusEnemybulletSys(i);
			}
		}
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{	// 使用中のオブジェクトのみ処理
		if (enemybullet->bUse)
		{
			if (enemybullet->nSysNum != 0)
			{
				enemybulletSys = &enemybulletSysWk[enemybullet->nSysNum - 1];
				enemybullet->fVAngle += enemybulletSys->fAddAngle;
				enemybullet->fVAngle = PiCalculate180(enemybullet->fVAngle);
				enemybullet->posEnemybullet.y += sinf(enemybullet->fVAngle) * enemybulletSys->fWave;
			}

			enemybullet->nAnimeCount++;
			if (enemybullet->nAnimeCount % TIME_ANIMATION_ENEMYBULLET == 0)
			{
				// パターンの切り替え
				enemybullet->nAnimePattern = (enemybullet->nAnimePattern + 1)
					% TEXTURE_PATTERN_ENEMYBULLET
					+ (TEXTURE_PATTERN_ENEMYBULLET* enemybullet->nTex);
				SetTexEnemybullet(i, enemybullet->nAnimePattern);
			}

			// バレットタイプに応じて処理
			if (enemybullet->nUseCount % ENEMYBULLET_HOMING_UPDATE == 0)
			{
				if (enemybullet->bHoming && enemybullet->nType == ENEMYBULLET_HOMING)
				{	// 指定距離内までホーミング
					D3DXVECTOR3 posTemp = model->posModel - enemybullet->posEnemybullet;
					posTemp.y += MODEL_CENTER;
					D3DXVec3Normalize(&posTemp, &posTemp);
					enemybullet->vecTag += posTemp * enemybullet->fVecPower;
					D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
				}
			}

			// 移動処理
			enemybullet->posEnemybullet += enemybullet->vecTag * enemybullet->fMoveSpeed;

			// シャドウ管理
			SetPositionShadow(enemybullet->nIdxShadow, D3DXVECTOR3(enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z));
			SetVertexShadow(enemybullet->nIdxShadow, enemybullet->fSizeShadow, enemybullet->fSizeShadow);
			SetColorShadow(enemybullet->nIdxShadow, enemybullet->colShadow);

			// エフェクト設置
			SetEffect(0,
				enemybullet->vec2Size,
				enemybullet->colorEnemybullet,
				enemybullet->posEnemybullet,
				ENEMYBULLET_SIZE_CHANGE,
				ENEMYBULLET_ALPHA_CHANGE);

			// 使用カウントで透過→初期化
			if (enemybullet->nUseCount > ENEMYBULLET_FALSE_COUNT )
			{
				enemybullet->colorEnemybullet.a -= ENEMYBULLET_ALPHA_CHANGE;
				if (enemybullet->colorEnemybullet.a < 0.0f)
				{
					enemybullet->colorEnemybullet.a = 0.0f;
				}

				enemybullet->colShadow.a -= ENEMYBULLET_ALPHA_CHANGE;
				if (enemybullet->colShadow.a < 0.0f)
				{
					enemybullet->colShadow.a = 0.0f;
				}

				if (enemybullet->colorEnemybullet.a > 0.0f)
				{
					SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);	// 反射光変更の変更処理
				}
				else
				{
					enemybulletSys = &enemybulletSysWk[enemybullet->nSysNum];
					enemybulletSys->nEnemybulletCount--;
					// シャドウの無効化
					ReleaseShadow(enemybullet->nIdxShadow);
					InitStatusEnemybullet(i);
				}
			}
			enemybullet->nUseCount++;
			// デバッグ用
//#ifdef _DEBUG
//			PrintDebugProc("EnemybulletNum[%d]  EnemybulletPos[X:%f Z:%f]  EnemybulletSize[X:%f]  EnemybulletColorR:[%f]\n", i, enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.z, enemybullet->vec2Size.x, enemybullet->colorEnemybullet.r);
//#endif
		}
	}
	//	sprintf(str, "EnemybulletPos[X:%4.2f Y:%4.2f Z:%4.2f]",
	//		enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemybullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, ENEMYBULLET_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (enemybullet->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEnemybullet);

			// ビューマトリックスを取得
			mtxView = GetMtxView();
			// ビルボード化
			g_mtxWorldEnemybullet._11 = mtxView._11;
			g_mtxWorldEnemybullet._12 = mtxView._21;
			g_mtxWorldEnemybullet._13 = mtxView._31;
			g_mtxWorldEnemybullet._21 = mtxView._12;
			g_mtxWorldEnemybullet._22 = mtxView._22;
			g_mtxWorldEnemybullet._23 = mtxView._32;
			g_mtxWorldEnemybullet._31 = mtxView._13;
			g_mtxWorldEnemybullet._32 = mtxView._23;
			g_mtxWorldEnemybullet._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, enemybullet->sclEnemybullet.x, enemybullet->sclEnemybullet.y, enemybullet->sclEnemybullet.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, enemybullet->rotEnemybullet.y, enemybullet->rotEnemybullet.x, enemybullet->rotEnemybullet.z);
			//D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &GetInvRotateMat(enemybullet->posEnemybullet));
			////D3DXMatrixMultiply(&enemybullet->mtxWorld, &enemybullet->mtxWorld, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemybullet);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEnemybullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureEnemybullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
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
HRESULT MakeVertexEnemybullet(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ENEMYBULLET_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffEnemybullet,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
		 return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ENEMYBULLET_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-ENEMYBULLET_SIZE_X, -ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ENEMYBULLET_SIZE_X, -ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxEnemybullet(int nEnemybullet, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseEnemybullet(int nEnemybullet, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexEnemybullet(int nEnemybullet, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// バレットを対象に設置
//=============================================================================
void SetEnemybullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			//enemybullet->colorEnemybullet = SetColorPallet(COLOR_PALLET_RED);
			enemybullet->colorEnemybullet = xColor;						// カラーを設定 
			enemybullet->vec2Size =
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);	// サイズを設定
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;			// 移動スピードを設定
			enemybullet->nUseCount = 0;									// 消滅用カウント
			enemybullet->bUse = true;
			enemybullet->vecTag = vecPos1 - vecPos2;
			enemybullet->nTex = ENEMYBULLET_TEX_1;

			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
			enemybullet->posEnemybullet = vecPos2;

			// Vtx設定
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);// 頂点座標の変更処理
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// 反射光変更の変更処理
			SetTexEnemybullet(i, enemybullet->nTex);							// テクスチャ座標の設定

			// 影の生成
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// 通常エネミーバレットを対象に設置
//=============================================================================
void SetEnemybullet00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->colorEnemybullet = SetColorPallet(3);						// カラー
			enemybullet->vec2Size = 
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);				// サイズ
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;						// 移動スピード
			enemybullet->nTex = 0;													// テクスチャー
			enemybullet->nUseCount = 0;												// 消滅用カウント


			enemybullet->bUse = true;
			enemybullet->bEnemy = true;

			enemybullet->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
			enemybullet->posEnemybullet = vecPos2;

			// Vtx設定
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);	// 頂点座標の変更処理
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// 反射光変更の変更処理
			SetTexEnemybullet(i, enemybullet->nTex);								// テクスチャ座標の設定

			// 影の生成
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// ホーミングバレットを対象に設定
//=============================================================================
void SetHomingEnemybullet(D3DXVECTOR3 posSet, float vecPower, D3DXCOLOR xColor)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->vec2Size =
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);		// サイズを設定
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;				// 移動スピードを設定
			enemybullet->nUseCount = 0;										// 消滅用カウント
			enemybullet->bUse = true;										// 使用フラグ
			enemybullet->nType = ENEMYBULLET_HOMING;
			enemybullet->colorEnemybullet = xColor;							// カラーを設定
			enemybullet->posEnemybullet = posSet;
			enemybullet->fVecPower = ENEMYBULLET_VEC_POWER;
			enemybullet->nTex = ENEMYBULLET_TEX_2;

			// ランダムでベクトルを生成
			for (int i = 0; i < 3; i++)
			{
				if (i == 1)
				{
					enemybullet->vecTag[i] = rand() % 100;	// ランダム
					enemybullet->vecTag[i] *= 0.01f;		// 小数点化
				}
				else
				{
					enemybullet->vecTag[i] = rand() % 200;	// ランダム
					enemybullet->vecTag[i] *= 0.01f;		// 小数点化
					enemybullet->vecTag[i] -= 1.0f;			// -1.0～1.0へ変更
				}

			}

			enemybullet->vecTag = enemybullet->vecTag;
			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);

			// Vtx設定
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);	// 頂点座標の変更処理
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// 反射光変更の変更処理
			SetTexEnemybullet(i, enemybullet->nTex);								// テクスチャ座標の設定

			// 影の生成
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}
//=============================================================================
// システムバレットを対象に設定
//=============================================================================
void SetSysEnemybullet(int nType, int nTex, int nSize, int nColor, int nSplit, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos)
{
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用のシステムを探す
	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++, enemybulletSys++)
	{
		if (!enemybulletSys->bUse)
		{
			enemybulletSys->bUse = true;						// 有効化
			enemybulletSys->bProcess = true;
			enemybulletSys->vecTagPos = vecPos;				// 対象POSを記憶
			enemybulletSys->colorEnemybullet = SetColorPallet(nColor);
			enemybulletSys->vec2Size = D3DXVECTOR2(nSize, nSize);				// サイズ

			enemybulletSys->fMoveSpeed = fSpeed;	// 移動スピード
			enemybulletSys->fVAngle = 0.0f;
			enemybulletSys->fWave = fWave;
			enemybulletSys->nType = nType;
			enemybulletSys->nFireTime = 0;
			enemybulletSys->nCoolDown = 0;
			enemybulletSys->nTex = nTex;
			enemybulletSys->nSplitCount = nSplit;
			if (nType == 4)
			{
				enemybulletSys->fSplitAngle = D3DX_PI * 2 / nSplit;
				enemybulletSys->fAddAngle = fAngle;
				enemybulletSys->fHAngle = 0.0f;
			}
			else
			{
				switch (nType % 2)
				{
				case 0:
					enemybulletSys->fSplitAngle = D3DX_PI * 2 / nSplit;
					enemybulletSys->fAddAngle = fAngle;
					enemybulletSys->fHAngle = enemybulletSys->fSplitAngle / 2;
					break;
				case 1:
					enemybulletSys->fSplitAngle = (D3DX_PI * 2) - (D3DX_PI * 2 / nSplit);
					enemybulletSys->fAddAngle = -fAngle;
					enemybulletSys->fHAngle = enemybulletSys->fSplitAngle / 2;
					break;
				}
			}
			return;
		}
	}
}
//=============================================================================
// システムバレット処理
//=============================================================================
void SetSysEnemybulletFire(int nSys)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[nSys];

	// 未使用の弾を探す
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->colorEnemybullet = enemybulletSys->colorEnemybullet;			// カラー
			enemybullet->vec2Size = enemybulletSys->vec2Size;					// サイズ
			enemybullet->fMoveSpeed = enemybulletSys->fMoveSpeed;				// 移動スピード
			enemybullet->nTex = enemybulletSys->nTex;							// テクスチャー
			enemybullet->nUseCount = 0;						// 消滅用カウント
			enemybullet->bUse = true;									// 有効化
			enemybullet->bEnemy = true;									// エネミーフラグを有効化
			enemybullet->nSysNum = nSys + 1;								// システムフラグを有効化

			// アングルからベクトル計算
			enemybullet->vecTag = (enemybulletSys->vecTagPos + D3DXVECTOR3(cosf(enemybulletSys->fHAngle),
				sinf(enemybulletSys->fVAngle), sinf(enemybulletSys->fHAngle)) - enemybulletSys->vecTagPos);

			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);	// 正規化
			enemybullet->posEnemybullet = enemybulletSys->vecTagPos;				// 発射位置設定
			enemybulletSys->fHAngle += enemybulletSys->fSplitAngle;			// 次弾の角度を変更
			enemybullet->fVAngle = enemybulletSys->fHAngle;					// ウェーブ用

			// Vtx設定
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);// 頂点座標の変更処理
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// 反射光変更の変更処理
			SetTexEnemybullet(i, enemybullet->nTex);							// テクスチャ座標の設定

			// 影の生成
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

			enemybulletSys->nEnemybulletCount++;

			// 360度発射が完了したらシステムを終了
			enemybulletSys->nSplitCount--;
			if (enemybulletSys->nType < 2)
			{
				if (enemybulletSys->nSplitCount <= 0)
				{
					enemybulletSys->bProcess = false;
					return;
				}
			}
			else if (enemybulletSys->nType >= 2)
			{
				if (enemybulletSys->nSplitCount <= 0)
				{
					enemybulletSys->bProcess = false;
				}
				return;
			}
		}
	}
}

//=============================================================================
// 取得関数
//=============================================================================
ENEMYBULLET *GetEnemybullet(int no)
{
	return(&enemybulletWk[no]);
}