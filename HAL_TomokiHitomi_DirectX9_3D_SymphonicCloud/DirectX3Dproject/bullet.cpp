//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "bullet.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "calculate.h"
#include "lockon.h"


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

void InitStatusBulletSys(int nBulletSys);
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVtxBullet(int nBullet);
void SetDiffuseBullet(int nBullet, D3DXCOLOR col);
void SetTexBullet(int nBullet, int nTex);
void SetSpecialBullet(int nSys);
void UpdateSpecialBullet(int nBullet);

void SetSpecialBulletFireSys(int nSys);
void SetSpecialBulletThunderSys(int nSys);
void SetSpecialBulletIceSys(int nSys);
void SetSpecialBulletWindSys(int nSys);
void SetSpecialBulletWaterSys(int nSys);
void SetSpecialBulletEarthSys(int nSys);

void UpdateSpecialBulletFire(int nBullet);
void UpdateSpecialBulletThunder(int nBullet);
void UpdateSpecialBulletIce(int nBullet);
void UpdateSpecialBulletWind(int nBullet);
void UpdateSpecialBulletWater(int nBullet);
void UpdateSpecialBulletEarth(int nBullet);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldBullet;			// ワールドマトリックス

BULLET					bulletWk[BULLET_MAX];
BULLET_SYS				bulletSysWk[BULLET_SYS_MAX];

float					g_fBaseAngleBullet;			// 角度（θ）
float					g_fRadiusBullet;			// 半径

// シェーダー関連
ID3DXBuffer *errorBullet = 0;
ID3DXEffect *bulletshader;
UINT numPassBullet;
HRESULT hrBullet;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(int nType)
{
	BULLET *bullet = &bulletWk[0];
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* path = SHADER_BULLET;
	// ファイル( const char* path )からシェーダーファイルを読み込み読み込み
	hrBullet = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &bulletshader, &errorBullet);
	if (FAILED(hrBullet))
	{
		// エラー
		return S_FALSE;
	}
	// 使用するテクニックを定義
	hrBullet = bulletshader->SetTechnique("Tec01");
	if (FAILED(hrBullet)) {

		// エラー
		return S_FALSE;
	}


	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BULLET,
			&g_pD3DTextureBullet);
	}

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{	// バレットの初期化
		InitStatusBullet(i);
	}

	for (int i = 0; i < BULLET_SYS_MAX; i++,bulletSys)
	{	// バレットシステムの初期化
		bulletSys->nCoolDown = 0;
		InitStatusBulletSys(i);
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexBullet(pDevice);

	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	// 位置・回転・スケールの設定
	bullet->posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->rotBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	bullet->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	bullet->vec2Size = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);
	bullet->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bullet->nUseCount = 0;
	bullet->nSysNum = 0;
	bullet->nTex = 0;
	bullet->nEffectCount = 0;
	bullet->nAnimeCount = 0;
	bullet->nAnimePattern = 0;
	bullet->nType = 0;
	bullet->nNum = 0;
	bullet->nEffect = 0;

	bullet->fSpeed = 0.0f;
	bullet->fHAngle = 0.0f;
	bullet->fVAngle = 0.0f;
	bullet->fAngle = 0.0f;
	bullet->fBaseAngle = 0.0f;
	bullet->fRadius = 0.0f;
	bullet->fEffectSizeChange = 0.0f;
	bullet->fEffectAlphaChange = 0.0f;

	bullet->bUse = false;
	bullet->bEnemy = false;
	bullet->bVertex = false;
	bullet->bSys = false;
	bullet->bEffect = true;

	bullet->nIdxShadow = 0;
	bullet->fSizeShadow = 0.0f;
	bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// 再利用処理SYS
//=============================================================================
void InitStatusBulletSys(int nBulletSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nBulletSys];
	bulletSys->posBulletSys = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->posBulletSysTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->vecBulletSys = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->posTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	bulletSys->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletSys->fHAngle = 0.0f;
	bulletSys->fVAngle = 0.0f;
	bulletSys->fAngle = 0.0f;
	bulletSys->fSpeed = 0.0f;
	bulletSys->fRotSpeed = 0.0f;
	bulletSys->fRotLength = 0.0f;
	bulletSys->fWave = 0.0f;
	bulletSys->fLength = 0.0f;

	bulletSys->nType = 0;
	bulletSys->nChain = 0;
	bulletSys->nFireTime = 0;
	bulletSys->nSplit = 0;
	bulletSys->nTex = 0;
	bulletSys->nBulletCount = 0;
	bulletSys->nUseCount = 0;
	bulletSys->nLockon = 0;

	bulletSys->bUse = false;
	bulletSys->bSet = false;
	bulletSys->bProcess = false;
	bulletSys->bTrack = false;
	bulletSys->bMove = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{

	// シェーダーを解放
	bulletshader->Release();

	if(g_pD3DTextureBullet != NULL)
	{// テクスチャの開放
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if(g_pD3DVtxBuffBullet != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBullet->Release();
		g_pD3DVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// 使用中のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (bulletSys->bUse)
		{
			SetSpecialBullet(i);
		}
	}

#ifdef _DEBUG
	PrintDebugProc("【 BULLET 】\n");
	int nBulletCountDebug = 0;
#endif

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{	// 使用中のオブジェクトのみ処理
		if (bullet->bUse)
		{
#ifdef _DEBUG
			nBulletCountDebug++;
#endif
			if (bullet->nType == BULLET_NORMAL)
			{	// 通常バレット処理
				bullet->posBullet += bullet->vecTag * bullet->fSpeed;
				bullet->rotBullet.z += BULLET_ROT_SPEED;			// ビルボード回転

			}
			else
			{	// 特殊バレット処理
				UpdateSpecialBullet(i);
				bullet->rotBullet.z -= BULLET_ROT_SPEED;			// ビルボード回転
			}

			// 頂点座標の変更処理
			SetVtxBullet(i);

			// シャドウ管理
			SetPositionShadow(bullet->nIdxShadow, D3DXVECTOR3(bullet->posBullet.x, bullet->posBullet.y, bullet->posBullet.z));
			SetVertexShadow(bullet->nIdxShadow, bullet->fSizeShadow, bullet->fSizeShadow);
			SetColorShadow(bullet->nIdxShadow, bullet->colShadow);

			if (bullet->bEffect)
			{
				// エフェクト設置
				SetEffect(bullet->nTex,
					bullet->vec2Size * bullet->sclBullet.x,
					bullet->xColor,
					bullet->posBullet,
					bullet->fEffectSizeChange,
					bullet->fEffectAlphaChange);

			}

			// 使用カウントで透過→初期化
			if (bullet->nUseCount > BULLET_FALSE_COUNT)
			{

				bullet->xColor.a -= BULLET_ALPHA_CHANGE;
				if (bullet->xColor.a < 0.0f)
				{
					bullet->xColor.a = 0.0f;
				}

				bullet->colShadow.a -= BULLET_ALPHA_CHANGE;
				if (bullet->colShadow.a < 0.0f)
				{
					bullet->colShadow.a = 0.0f;
				}

				if (bullet->xColor.a > 0.0f)
				{
					SetDiffuseBullet(i, bullet->xColor);	// 反射光変更の変更処理
				}
				else
				{
					if (bullet->nSysNum != BULLET_NORMAL)
					{
						bulletSys = &bulletSysWk[bullet->nSysNum];
						bulletSys->nBulletCount--;
					}
					// シャドウの無効化
					ReleaseShadow(bullet->nIdxShadow);
					InitStatusBullet(i);
				}
			}
			bullet->nUseCount++;
		}
	}
#ifdef _DEBUG
	PrintDebugProc("bulletCount[%d]\n",nBulletCountDebug);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	BULLET *bullet = &bulletWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	D3DXMATRIX mtxProjection = GetMtxProjection();

	//// 加算合成に設定
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, BULLET_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBullet);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ビルボード化
			g_mtxWorldBullet._11 = mtxView._11;
			g_mtxWorldBullet._12 = mtxView._21;
			g_mtxWorldBullet._13 = mtxView._31;
			g_mtxWorldBullet._21 = mtxView._12;
			g_mtxWorldBullet._22 = mtxView._22;
			g_mtxWorldBullet._23 = mtxView._32;
			g_mtxWorldBullet._31 = mtxView._13;
			g_mtxWorldBullet._32 = mtxView._23;
			g_mtxWorldBullet._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, bullet->sclBullet.x, bullet->sclBullet.y, bullet->sclBullet.z);
			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bullet->rotBullet.y, bullet->rotBullet.x, bullet->rotBullet.z);
			//D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &GetInvRotateMat(bullet->posBullet));
			////D3DXMatrixMultiply(&bullet->mtxWorld, &bullet->mtxWorld, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, bullet->posBullet.x, bullet->posBullet.y, bullet->posBullet.z);
			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTranslate);

			//// ワールドマトリクスの設定
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 必要なグローバル情報を与える
			bulletshader->SetMatrix("proj", &mtxProjection);
			bulletshader->SetMatrix("view", &mtxView);
			bulletshader->SetMatrix("world", &g_mtxWorldBullet);

			//// テクスチャの設定
			//pDevice->SetTexture(0, g_pD3DTextureBullet);

			// シェーダーの開始、numPassには指定してあるテクニックに定義してあるpassの数が変える
			bulletshader->Begin(&numPassBullet, 0);

			// パスを指定して開始
			bulletshader->BeginPass(0);

			// テクスチャをセット
			bulletshader->SetTexture("tex", g_pD3DTextureBullet);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);

			// シェーダ終了
			bulletshader->EndPass();
			bulletshader->End();
		}
	}

	//// 通常ブレンドに戻す
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// 角度と半径を求める
	g_fBaseAngleBullet = atan2f(BULLET_SIZE_Y, BULLET_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);
	g_fRadiusBullet = D3DXVec2Length(&temp);

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffBullet,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
		 return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLET_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_BULLET;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_BULLET;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffBullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// 頂点0:回転あり
		pVtx[0].vtx.x = 0.0f - cosf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[0].vtx.y = 0.0f - sinf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[0].vtx.z = 0.0f;

		// 頂点1:回転あり
		pVtx[1].vtx.x = 0.0f - cosf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[1].vtx.y = 0.0f + sinf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[1].vtx.z = 0.0f;

		// 頂点2:回転あり
		pVtx[2].vtx.x = 0.0f + cosf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[2].vtx.y = 0.0f - sinf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[2].vtx.z = 0.0f;

		// 頂点3:回転あり
		pVtx[3].vtx.x = 0.0f + cosf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[3].vtx.y = 0.0f + sinf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[3].vtx.z = 0.0f;

		// 頂点データをアンロックする
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseBullet(int nBullet, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexBullet(int nBullet, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_BULLET;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_BULLET;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// バレットを対象に設置
//=============================================================================
void SetBullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor, float fScl, int nEffect)
{
	BULLET *bullet = &bulletWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (!bullet->bUse)
		{
			bullet->xColor = xColor;										// カラーを設定
			bullet->vec2Size = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);	// サイズを設定
			bullet->fSpeed = BULLET_SPEED;									// 移動スピードを設定

			// 消滅用カウント
			bullet->nUseCount = 0;
			bullet->nEffect = nEffect;
			bullet->fEffectSizeChange = BULLET_NORMAL_EFFECT_SIZE_CHANGE;
			bullet->fEffectAlphaChange = BULLET_NORMAL_EFFECT_ALPHA_CHANGE;
			bullet->bUse = true;
			bullet->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bullet->vecTag, &bullet->vecTag);
			bullet->posBullet = vecPos2;
			bullet->sclBullet *= fScl;
			// Vtx設定
			SetVtxBullet(i);// 頂点座標の変更処理
			SetDiffuseBullet(i, bullet->xColor);				// 反射光変更の変更処理
			SetTexBullet(i, bullet->nTex);							// テクスチャ座標の設定

			// 影の生成
			bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
			bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
			bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}


//=============================================================================
// 特殊バレット振り分け処理
//=============================================================================
void SetSpecialBullet(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	
	switch (bulletSys->nType)
	{
	case BULLET_SPECIAL_FIRE:
		SetSpecialBulletFireSys(nSys);
		break;
	case BULLET_SPECIAL_THUNDER:
		SetSpecialBulletThunderSys(nSys);
		break;
	case BULLET_SPECIAL_ICE:
		SetSpecialBulletIceSys(nSys);
		break;
	case BULLET_SPECIAL_WIND:
		SetSpecialBulletWindSys(nSys);
		break;
	case BULLET_SPECIAL_WATER:
		SetSpecialBulletWaterSys(nSys);
		break;
	case BULLET_SPECIAL_EARTH:
		SetSpecialBulletEarthSys(nSys);
		break;
	}
}

//=============================================================================
// 特殊バレット更新処理
//=============================================================================
void UpdateSpecialBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	switch (bullet->nType)
	{
	case BULLET_SPECIAL_FIRE:
		UpdateSpecialBulletFire(nBullet);
		break;
	case BULLET_SPECIAL_THUNDER:
		UpdateSpecialBulletThunder(nBullet);
		break;
	case BULLET_SPECIAL_ICE:
		UpdateSpecialBulletIce(nBullet);
		break;
	case BULLET_SPECIAL_WIND:
		UpdateSpecialBulletWind(nBullet);
		break;
	case BULLET_SPECIAL_WATER:
		UpdateSpecialBulletWater(nBullet);
		break;
	case BULLET_SPECIAL_EARTH:
		UpdateSpecialBulletEarth(nBullet);
		break;
	}
}

//=============================================================================
// ファイアバレットセット
//=============================================================================
void SetSpecialBulletFire(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// 未使用のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// 有効化
			bulletSys->bTrack = true;								// 追尾フラグ
			bulletSys->posBulletSys = pos1;							// セットPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// 移動ベクトル
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_FIRE;					// タイプ
			bulletSys->nTex = BULLET_NORMAL;						// テクスチャー
			bulletSys->nLockon = nLockon;							// ロックオンフラグ
			bulletSys->vec2Size = D3DXVECTOR2(						// サイズ
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_FIRE_SPEED;			// スピード
			bulletSys->fRotSpeed = BULLET_SPECIAL_FIRE_ROT_SPEED;	// 回転スピード
			bulletSys->fRotLength = 0.0f;							// 回転展開距離
			bulletSys->nSplit = BULLET_SPECIAL_FIRE_SPLIT + BULLET_SPECIAL_FIRE_SPLIT * nCharge;// 分割角度
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_FIRE_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_ORANGE);	// カラー
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// 回転軸を設定
			bulletSys->vecTa = vecTa;								// 回転軸に垂直なベクトル
			return;
		}
	}
}

//=============================================================================
// ファイアバレット処理
//=============================================================================
void SetSpecialBulletFireSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];
	MODEL *model = GetModel(0);
	CAMERA *camera = GetCamera(CAMERA_GAME);

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}

		bulletSys->fRotLength += BULLET_SPECIAL_FIRE_ROT_LENGTH;
		if (bulletSys->fRotLength > BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX;
		}

		D3DXVECTOR3 vecTag = camera->posCameraAt - camera->posCameraEye;
		D3DXVec3Normalize(&vecTag, &vecTag);
		bulletSys->posBulletSysTag = camera->posCameraAt + vecTag * BULLET_TAG_VECTOR;

		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->posBulletSys.x = model->posModel.x;
		bulletSys->posBulletSys.y = model->posModel.y + MODEL_CENTER;
		bulletSys->posBulletSys.z = model->posModel.z;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->bEffect = false;										// エフェクトなし
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->nNum = i;
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_FIRE_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_FIRE_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_FIRE_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// ファイアバレット更新処理
//=============================================================================
void UpdateSpecialBulletFire(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];
	MODEL *model = GetModel(0);

	bulletSys->vecAxis = D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI), 0.0f, sinf(model->fHAngle + D3DX_PI));
	bulletSys->vecTa = D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f));

	if (bulletSys->fRotLength >= BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX)
	{
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_FIRE_RAPID == 0)
		//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
		//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			SetBullet(
				bulletSys->posBulletSysTag,
				bullet->posBullet,
				SetColorPallet(COLOR_PALLET_ORANGE),
				BULLET_SPECIAL_FIRE_CHILD__SCALE,
				BULLET_SPECIAL_FIRE);
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// サンダーバレットセット
//=============================================================================
void SetSpecialBulletThunder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// 未使用のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// 有効化
			bulletSys->bTrack = true;								// 追尾フラグ
			bulletSys->posBulletSys = pos1;							// セットPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// 移動ベクトル
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_THUNDER;				// タイプ
			bulletSys->nTex = BULLET_NORMAL;						// テクスチャー
			bulletSys->nLockon = nLockon;							// ロックオンフラグ
			bulletSys->vec2Size = D3DXVECTOR2(						// サイズ
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_THUNDER_SPEED;		// スピード
			bulletSys->fRotSpeed = BULLET_SPECIAL_THUNDER_ROT_SPEED;// 回転スピード
			bulletSys->fRotLength = 0.0f;							// 回転展開距離
			bulletSys->nSplit = BULLET_SPECIAL_THUNDER_SPLIT + BULLET_SPECIAL_THUNDER_SPLIT * nCharge;// 分割角度
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_THUNDER_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_YELLOW);// カラー
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// 回転軸を設定
			bulletSys->vecTa = vecTa;								// 回転軸に垂直なベクトル
			return;
		}
	}
}

//=============================================================================
// サンダーバレット処理
//=============================================================================
void SetSpecialBulletThunderSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}

		// 回転半径を加算
		bulletSys->fRotLength += BULLET_SPECIAL_THUNDER_ROT_LENGTH;
		// 最大半径を越えたら
		if (bulletSys->fRotLength > BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{
				if (bulletSys->bMove)
				{
					D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
					vecTemp.y += BULLET_SPECIAL_THUNDER_HEIGHT;

					D3DXVec3Normalize(&vecTemp, &vecTemp);
					bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
					bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
					if ((bulletSys->posBulletSys.x - lockon->posLockon.x < BULLET_SPECIAL_THUNDER_STOP
						&& bulletSys->posBulletSys.x - lockon->posLockon.x > -BULLET_SPECIAL_THUNDER_STOP)
						&& (bulletSys->posBulletSys.z - lockon->posLockon.z < BULLET_SPECIAL_THUNDER_STOP
							&& bulletSys->posBulletSys.z - lockon->posLockon.z > -BULLET_SPECIAL_THUNDER_STOP))
					{
						bulletSys->bMove = false;
					}
				}
				else
				{
					bulletSys->posBulletSys = lockon->posLockon;
					bulletSys->posBulletSys.y += BULLET_SPECIAL_THUNDER_HEIGHT;
				}
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		if(bulletSys->nLockon < 0 && bulletSys->bMove)
		{
			MODEL *model = GetModel(0);
			D3DXVECTOR3 vecTemp = model->posModel - bulletSys->posBulletSys;
			vecTemp.y += BULLET_SPECIAL_THUNDER_HEIGHT;

			D3DXVec3Normalize(&vecTemp, &vecTemp);
			bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
			bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
			if (bulletSys->posBulletSys.y >= model->posModel.y + BULLET_SPECIAL_THUNDER_HEIGHT)
			{
				bulletSys->bMove = false;
			}
		}
		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->fAngle -= bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->nNum = i;												// ナンバー
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_THUNDER_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_THUNDER_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_THUNDER_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// サンダーバレット更新処理
//=============================================================================
void UpdateSpecialBulletThunder(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	// クォータニオン軸を変更
	if (bulletSys->vecAxis.y < BULLET_SPECIAL_THUNDER_AXIS_Y_MAX)
	{
		bulletSys->vecAxis.y += BULLET_SPECIAL_THUNDER_AXIS_Y;
		D3DXVec3Normalize(&bulletSys->vecAxis, &bulletSys->vecAxis);
	}

	// 最大半径を越えたら
	if (bulletSys->fRotLength >= BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX)
	{
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_THUNDER_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_THUNDER_RAPID == 0)
			//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
			//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse && bulletSys->bTrack)
			{
				SetBullet(
					lockon->posLockon,
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_YELLOW),
					BULLET_SPECIAL_THUNDER_CHILD__SCALE,
					BULLET_SPECIAL_THUNDER);
			}
			else
			{
				SetBullet(
					bullet->posBullet - D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_YELLOW),
					BULLET_SPECIAL_THUNDER_CHILD__SCALE,
					BULLET_SPECIAL_THUNDER);
			}
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// アイスバレットセット
//=============================================================================
void SetSpecialBulletIce(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// 未使用のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// 有効化
			bulletSys->bTrack = true;								// 追尾フラグ
			bulletSys->posBulletSys = pos1;							// セットPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// 移動ベクトル
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_ICE;					// タイプ
			bulletSys->nTex = BULLET_NORMAL;						// テクスチャー
			bulletSys->nLockon = nLockon;							// ロックオンフラグ
			bulletSys->vec2Size = D3DXVECTOR2(						// サイズ
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_ICE_SPEED;			// スピード
			bulletSys->fRotSpeed = BULLET_SPECIAL_ICE_ROT_SPEED;	// 回転スピード
			bulletSys->fRotLength = 0.0f;							// 回転展開距離
			bulletSys->nSplit = BULLET_SPECIAL_ICE_SPLIT + BULLET_SPECIAL_ICE_SPLIT * nCharge;// 分割角度
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_ICE_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_CYAN);	// カラー
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// 回転軸を設定
			bulletSys->vecTa = vecTa;								// 回転軸に垂直なベクトル
			return;
		}
	}
}

//=============================================================================
// アイスバレット処理
//=============================================================================
void SetSpecialBulletIceSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{

				D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;

				D3DXVec3Normalize(&vecTemp, &vecTemp);
				bulletSys->vecBulletSys =
					(bulletSys->vecBulletSys + vecTemp) / 2;
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
		//bulletSys->fAngle += bulletSys->fRotSpeed;
		//bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_ICE_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_ICE_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_ICE_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// アイスバレット更新処理
//=============================================================================
void UpdateSpecialBulletIce(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
	{
		LOCKON *lockon = GetLockon(bulletSys->nLockon);
		if (lockon->bUse)
		{
			D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
			float fTemp = D3DXVec3Length(&vecTemp);

			if (fTemp < BULLET_SPECIAL_ICE_LENGTH)
			{
				if (fTemp < BULLET_SPECIAL_ICE_LENGTH / 4)
				{
					bulletSys->fRotLength = fTemp * BULLET_SPECIAL_ICE_ROT_LENGTH * 2;
				}
				else
				{
					bulletSys->fRotLength = (BULLET_SPECIAL_ICE_LENGTH - fTemp)
						* BULLET_SPECIAL_ICE_ROT_LENGTH / 2;
				}
			}
		}
	}




	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// ウィンドバレットセット
//=============================================================================
void SetSpecialBulletWind(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	for (int i = 0; i < BULLET_SPECIAL_WIND_MAX; i++)
	{
		// 未使用のシステムを探す
		for (int j = 0; j < BULLET_SYS_MAX; j++, bulletSys++)
		{
			if (!bulletSys->bUse)
			{
				bulletSys->bUse = true;										// 有効化
				bulletSys->bTrack = true;									// 追尾フラグ
				bulletSys->posBulletSys = pos1;								// セットPOS
				bulletSys->posBulletSysTag = pos2 * (i * BULLET_SPECIAL_WIND_INTERVAL);
				bulletSys->vecBulletSys = pos2;								// 移動ベクトル
				bulletSys->nType = BULLET_SPECIAL_WIND;						// タイプ
				bulletSys->nChain = i;										// 連続設置管理
				bulletSys->nTex = BULLET_NORMAL;							// テクスチャー
				bulletSys->nLockon = nLockon;								// ロックオンフラグ
				bulletSys->vec2Size = D3DXVECTOR2(							// サイズ
					BULLET_SIZE_X, BULLET_SIZE_Y);
				bulletSys->fSpeed = BULLET_SPECIAL_WIND_SPEED;				// スピード
				if (i % 2 == 0)
				{
					bulletSys->fRotSpeed = BULLET_SPECIAL_WIND_ROT_SPEED;		// 回転スピード
				}
				else
				{
					bulletSys->fRotSpeed = -BULLET_SPECIAL_WIND_ROT_SPEED;		// 回転スピード
				}
				bulletSys->fRotLength = 0.0f;								// 回転展開距離
				bulletSys->nSplit = BULLET_SPECIAL_WIND_SPLIT + BULLET_SPECIAL_WIND_SPLIT * nCharge;	// 分割角度
				if (nCharge == MAGICCIRCLE_MAX)
				{
					bulletSys->nSplit += BULLET_SPECIAL_WIND_SPLIT_BONUS;
				}
				bulletSys->xColor = SetColorPallet(COLOR_PALLET_LIGHTGREEN);// カラー
				bulletSys->vecAxis = bulletSys->vecBulletSys;				// 回転軸を設定
				bulletSys->vecTa = vecTa;									// 回転軸に垂直なベクトル
				break;
			}
		}
	}
	return;
}

//=============================================================================
// ウィンドバレット処理
//=============================================================================
void SetSpecialBulletWindSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];
	MODEL *model = GetModel(0);

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}
		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->posBulletSysTag.y += bulletSys->vecBulletSys.y * bulletSys->fSpeed;
		bulletSys->posBulletSys.x = model->posModel.x;
		bulletSys->posBulletSys.y = model->posModel.y + bulletSys->posBulletSysTag.y;
		bulletSys->posBulletSys.z = model->posModel.z;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_WIND_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_WIND_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_WIND_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// ウィンドバレット更新処理
//=============================================================================
void UpdateSpecialBulletWind(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];
	bulletSys->fRotLength += BULLET_SPECIAL_WIND_ROT_LENGTH + (BULLET_SPECIAL_WIND_ROT_LENGTH_ADD * bulletSys->nChain);

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// ウォーターバレットセット
//=============================================================================
void SetSpecialBulletWater(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// 未使用のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// 有効化
			bulletSys->bTrack = true;								// 追尾フラグ
			bulletSys->posBulletSys = pos1;							// セットPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// 移動ベクトル
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_WATER;				// タイプ
			bulletSys->nTex = BULLET_NORMAL;						// テクスチャー
			bulletSys->nLockon = nLockon;							// ロックオンフラグ
			bulletSys->vec2Size = D3DXVECTOR2(						// サイズ
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_WATER_SPEED;			// スピード
			bulletSys->fRotSpeed = BULLET_SPECIAL_WATER_ROT_SPEED;	// 回転スピード
			bulletSys->fRotLength = 0.0f;							// 回転展開距離
			bulletSys->nSplit = BULLET_SPECIAL_WATER_SPLIT + BULLET_SPECIAL_WATER_SPLIT * nCharge;// 分割角度
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_WATER_SPLIT_BONUS;
			}		
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_BLUE);	// カラー
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// 回転軸を設定
			bulletSys->vecTa = vecTa;								// 回転軸に垂直なベクトル
			return;
		}
	}
}

//=============================================================================
// ウォーターバレット処理
//=============================================================================
void SetSpecialBulletWaterSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{

				D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;

				D3DXVec3Normalize(&vecTemp, &vecTemp);
				bulletSys->vecBulletSys =
					(bulletSys->vecBulletSys + vecTemp) / 2;
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_WATER_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_WATER_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_WATER_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// ウォーターバレット更新処理
//=============================================================================
void UpdateSpecialBulletWater(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
	{
		LOCKON *lockon = GetLockon(bulletSys->nLockon);
		if (lockon->bUse)
		{
			D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
			float fTemp = D3DXVec3Length(&vecTemp);

			if (fTemp < BULLET_SPECIAL_WATER_LENGTH)
			{
				if (fTemp < BULLET_SPECIAL_WATER_LENGTH / 2)
				{
					bulletSys->fRotLength = fTemp * BULLET_SPECIAL_WATER_ROT_LENGTH;
				}
				else
				{
					bulletSys->fRotLength = (BULLET_SPECIAL_WATER_LENGTH - fTemp)
						* BULLET_SPECIAL_WATER_ROT_LENGTH;
				}
			}
		}
	}




	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}


//=============================================================================
// アースバレットセット
//=============================================================================
void SetSpecialBulletEarth(D3DXVECTOR3 pos1 , D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa,int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// 未使用のシステムを探す
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// 有効化
			bulletSys->bTrack = true;								// 追尾フラグ
			bulletSys->posBulletSys = pos1;							// セットPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// 移動ベクトル
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_EARTH;				// タイプ
			bulletSys->nTex = BULLET_NORMAL;						// テクスチャー
			bulletSys->nLockon = nLockon;							// ロックオンフラグ
			bulletSys->vec2Size = D3DXVECTOR2(						// サイズ
				BULLET_SIZE_X,BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_EARTH_SPEED;			// スピード
			bulletSys->fRotSpeed = BULLET_SPECIAL_EARTH_ROT_SPEED;	// 回転スピード
			bulletSys->fRotLength = 0.0f;							// 回転展開距離
			bulletSys->nSplit = BULLET_SPECIAL_EARTH_SPLIT + BULLET_SPECIAL_EARTH_SPLIT * nCharge;// 分割角度
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_EARTH_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_BROWN);	// カラー
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// 回転軸を設定
			bulletSys->vecTa = vecTa;								// 回転軸に垂直なベクトル
			return;
		}
	}
}

//=============================================================================
// アースバレット処理
//=============================================================================
void SetSpecialBulletEarthSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// セット済みの場合は更新処理
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// 消滅カウントが0未満の場合はシステム初期化
			InitStatusBulletSys(nSys);
			return;
		}
		// 回転半径を加算
		bulletSys->fRotLength += BULLET_SPECIAL_EARTH_ROT_LENGTH;
		// 最大半径を越えたら
		if (bulletSys->fRotLength > BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{
				if (bulletSys->bMove)
				{
					D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
					vecTemp.y -= BULLET_SPECIAL_EARTH_HEIGHT;

					D3DXVec3Normalize(&vecTemp, &vecTemp);
					bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
					bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
					if ((bulletSys->posBulletSys.x - lockon->posLockon.x < BULLET_SPECIAL_EARTH_STOP
						&& bulletSys->posBulletSys.x - lockon->posLockon.x > -BULLET_SPECIAL_EARTH_STOP)
						&& (bulletSys->posBulletSys.z - lockon->posLockon.z < BULLET_SPECIAL_EARTH_STOP
							&& bulletSys->posBulletSys.z - lockon->posLockon.z > -BULLET_SPECIAL_EARTH_STOP))
					{
						bulletSys->bMove = false;
					}
				}
				else
				{
					bulletSys->posBulletSys = lockon->posLockon;
					bulletSys->posBulletSys.y -= BULLET_SPECIAL_EARTH_HEIGHT;
				}
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		if (bulletSys->nLockon < 0 && bulletSys->bMove)
		{
			MODEL *model = GetModel(0);
			D3DXVECTOR3 vecTemp = model->posModel - bulletSys->posBulletSys;
			vecTemp.y -= BULLET_SPECIAL_EARTH_HEIGHT;

			D3DXVec3Normalize(&vecTemp, &vecTemp);
			bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
			bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
			if (bulletSys->posBulletSys.y <= model->posModel.y - BULLET_SPECIAL_EARTH_HEIGHT)
			{
				bulletSys->bMove = false;
			}
		}
		bulletSys->nUseCount++;					// 消滅カウント更新
		bulletSys->fAngle -= bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// 未セットの場合はセットアップ
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// 未使用の弾を探す
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// 有効化
				bullet->nSysNum = nSys;											// システムナンバー
				bullet->nType = bulletSys->nType;								// タイプ
				bullet->nNum = i;												// ナンバー
				bullet->vec2Size = bulletSys->vec2Size;							// サイズ
				bullet->fEffectSizeChange = BULLET_SPECIAL_EARTH_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_EARTH_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_EARTH_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// 初期角度
				bullet->xColor = bulletSys->xColor;								// カラー

																				// Vtx設定
				SetVtxBullet(i);		// 頂点座標の変更処理
				SetDiffuseBullet(i, bullet->xColor);							// 反射光変更の変更処理
				SetTexBullet(i, bullet->nTex);									// テクスチャ座標の設定

																				// 影の生成
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// セット数をシステムに記憶
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// 分割カウント分のセットが終わったら終了
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// アースバレット更新処理
//=============================================================================
void UpdateSpecialBulletEarth(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	// クォータニオン軸を変更
	if (bulletSys->vecAxis.y < BULLET_SPECIAL_EARTH_AXIS_Y_MAX)
	{
		bulletSys->vecAxis.y += BULLET_SPECIAL_EARTH_AXIS_Y;
		D3DXVec3Normalize(&bulletSys->vecAxis, &bulletSys->vecAxis);
	}

	// 最大半径を越えたら
	if (bulletSys->fRotLength >= BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX)
	{
		bulletSys->fRotLength = BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX;
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_EARTH_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_EARTH_RAPID == 0)
			//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
			//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse && bulletSys->bTrack)
			{
				SetBullet(
					lockon->posLockon,
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_BROWN),
					BULLET_SPECIAL_EARTH_CHILD__SCALE,
					BULLET_SPECIAL_EARTH);
			}
			else
			{
				SetBullet(
					bullet->posBullet + D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_BROWN),
					BULLET_SPECIAL_EARTH_CHILD__SCALE,
					BULLET_SPECIAL_EARTH);
			}
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// 回転角度
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// 回転半径を設定
																				// クォータニオン処理
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// 回転軸に設置
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// ダメージ計算関数
//=============================================================================
float GetBulletDamage(int nType)
{
	float fTemp = 0.0f;

	// バレットタイプに応じてダメージ値を格納
	switch (nType)
	{
	case BULLET_NORMAL:
		fTemp = BULLET_NORMAL_DAMAGE;
		break;
	case BULLET_SPECIAL_FIRE:
		fTemp = BULLET_SPECIAL_FIRE_DAMAGE;
		break;
	case BULLET_SPECIAL_THUNDER:
		fTemp = BULLET_SPECIAL_THUNDER_DAMAGE;
		break;
	case BULLET_SPECIAL_ICE:
		fTemp = BULLET_SPECIAL_ICE_DAMAGE;
		break;
	case BULLET_SPECIAL_WIND:
		fTemp = BULLET_SPECIAL_WIND_DAMAGE;
		break;
	case BULLET_SPECIAL_WATER:
		fTemp = BULLET_SPECIAL_WATER_DAMAGE;
		break;
	case BULLET_SPECIAL_EARTH:
		fTemp = BULLET_SPECIAL_EARTH_DAMAGE;
		break;
	}

	// ダメージ値を返却
	return(fTemp);
}

//=============================================================================
// 取得関数
//=============================================================================
BULLET *GetBullet(int no)
{
	return(&bulletWk[no]);
}

//=============================================================================
// システム取得関数
//=============================================================================
BULLET_SYS *GetBulletSys(int no)
{
	return(&bulletSysWk[no]);
}