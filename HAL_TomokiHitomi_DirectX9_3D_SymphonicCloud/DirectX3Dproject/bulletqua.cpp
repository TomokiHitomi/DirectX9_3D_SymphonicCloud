
//=============================================================================
//
// バレット処理 [bulletqua.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "camera.h"
#include "bulletqua.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "calculate.h"
#include "camera.h"

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

void InitStatusBulletQuaSys(int nBulletQuaSys);
HRESULT MakeVertexBulletQua(LPDIRECT3DDEVICE9 pDevice);
void SetVtxBulletQua(int nBulletQua, float fSizeX, float fSizeY);
void SetDiffuseBulletQua(int nBulletQua, D3DXCOLOR col);
void SetTexBulletQua(int nBulletQua, int nTex);
void SetSysBulletQuaFire(int nSys);
void UpdateSysBulletQua(int nSys);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletQua = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletQua = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldBulletQua;			// ワールドマトリックス

BULLETQUA					bulletquaWk[BULLETQUA_MAX];
BULLETQUA_SYS				bulletquaSysWk[BULLETQUA_SYS_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBulletQua(int nType)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BULLETQUA,
			&g_pD3DTextureBulletQua);
	}

	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		// 位置・回転・スケールの設定
		bulletqua->posBulletQua = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->rotBulletQua = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->sclBulletQua = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		bulletqua->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->colorBulletQua = D3DXCOLOR(BULLETQUA_DIF_R, BULLETQUA_DIF_G, BULLETQUA_DIF_B, BULLETQUA_DIF_A);
		InitStatusBulletQua(i);
	}

	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		bulletquaSys->nCoolDown = 0;
		InitStatusBulletQuaSys(i);
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexBulletQua(pDevice);

	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusBulletQua(int nBulletQua)
{
	BULLETQUA *bulletqua = &bulletquaWk[nBulletQua];

	bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y);
	bulletqua->colorBulletQua = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletqua->nUseCount = 0;
	bulletqua->nSysNum = 0;
	bulletqua->nTex = 0;
	bulletqua->nEffectCount = 0;
	bulletqua->nAnimeCount = 0;

	bulletqua->fMoveSpeed = 0.0f;
	bulletqua->fHAngle = 0.0f;
	bulletqua->fVAngle = 0.0f;

	bulletqua->bUse = false;
	bulletqua->bEnemy = false;
	bulletqua->bVertex = false;
	bulletqua->bDraw = true;

	// 影初期化
	bulletqua->nIdxShadow = 0;
	bulletqua->fSizeShadow = 0.0f;
	bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// 再利用処理SYS
//=============================================================================
void InitStatusBulletQuaSys(int nBulletQuaSys)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nBulletQuaSys];
	bulletquaSys->vecTagPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletquaSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	bulletquaSys->colorBulletQua = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletquaSys->fHAngle = 0.0f;
	bulletquaSys->fVAngle = 0.0f;
	bulletquaSys->fMoveSpeed = 0.0f;
	bulletquaSys->fWave = 0.0f;
	bulletquaSys->fLength = 1.0f;

	bulletquaSys->nType = 0;
	bulletquaSys->nFireTime = 0;
	bulletquaSys->nSplitCount = 0;
	bulletquaSys->nTex = 0;
	bulletquaSys->nBulletCount = 0;

	bulletquaSys->bUse = false;
	bulletquaSys->bProcess = false;

	// クォータニオン用変数
	// 回転半径を設定
	bulletquaSys->vecTa = D3DXVECTOR3(0.0f, bulletquaSys->fLength, 0.0f);
	// 現在の角度に垂直な回転軸ベクトルを設定
	bulletquaSys->vecAxis = D3DXVECTOR3(cosf(bulletquaSys->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(bulletquaSys->fHAngle + D3DX_PI * 0.5f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBulletQua(void)
{
	if(g_pD3DTextureBulletQua != NULL)
	{// テクスチャの開放
		g_pD3DTextureBulletQua->Release();
		g_pD3DTextureBulletQua = NULL;
	}

	if(g_pD3DVtxBuffBulletQua != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBulletQua->Release();
		g_pD3DVtxBuffBulletQua = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBulletQua(void)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// 使用中のシステムを探す
	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		if (bulletquaSys->bProcess)
		{
			SetSysBulletQuaFire(i);
		}
		if (bulletquaSys->bUse)
		{
			if (bulletquaSys->nBulletCount > 0)
			{
				UpdateSysBulletQua(i);
			}
			else
			{
				InitStatusBulletQuaSys(i);
			}

#ifdef _DEBUG
			//PrintDebugProc("Sys[%d]  Tex[%d]  Type[%d]  Length[%f]  Angle[V:%f H:%f Add:%f]\n", i, bulletquaSys->nTex, bulletquaSys->nType, bulletquaSys->fLength, bulletquaSys->fVAngle, bulletquaSys->fHAngle, bulletquaSys->fAddAngle);
			//			PrintDebugProc("BulletQuaNum[%d]  BulletQuaPos[X:%f Z:%f]  BulletQuaSize[X:%f]  BulletQuaColorR:[%f]\n", i, bulletqua->posBulletQua.x, bulletqua->posBulletQua.z, bulletqua->vec2Size.x, bulletqua->colorBulletQua.r);
#endif

		}
		//if (bulletquaSys->nCoolDown > 0)
		//{
		//	bulletquaSys->nCoolDown--;
		//	if (bulletquaSys->nCoolDown <= 0)
		//	{
		//		InitStatusBulletQuaSys(i);
		//	}
		//}
	}

	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{	// 使用中のオブジェクトのみ処理
		if (bulletqua->bUse)
		{
			bulletqua->nAnimeCount++;
			if (bulletqua->nAnimeCount % TIME_ANIMATION_TEX == 0)
			{
				// パターンの切り替え
				bulletqua->nTex = (bulletqua->nTex + 1) % TEXTURE_PATTERN_NU_BULLETQUA;
				SetTexBulletQua(i, bulletqua->nTex);
			}
			if (!bulletqua->bVertex)
			{
				SetVtxBulletQua(i, bulletqua->vec2Size.x, bulletqua->vec2Size.y);		// 頂点座標の変更処理
				SetDiffuseBulletQua(i, bulletqua->colorBulletQua);	// 反射光変更の変更処理
				SetTexBulletQua(i, bulletqua->nTex);		// テクスチャ座標の設定
				bulletqua->bVertex = true;
			}


			bulletquaSys = &bulletquaSysWk[bulletqua->nSysNum];

			bulletquaSys->fVAngle += bulletquaSys->fSplitAngle;
			PiCalculate180(bulletquaSys->fVAngle);

			if(bulletquaSys->nType <= 3)
			{ 
				bulletquaSys->vecTa = D3DXVECTOR3(0.0f, 0.0f, bulletquaSys->fLength);
				bulletquaSys->vecAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				bulletquaSys->vecTa = D3DXVECTOR3(0.0f, bulletquaSys->fLength, 0.0f);
				bulletquaSys->vecAxis = D3DXVECTOR3(cosf(bulletquaSys->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(bulletquaSys->fHAngle + D3DX_PI * 0.5f));
			}

			// クォータニオン処理
			QuaternionCalculate(&bulletquaSys->vecTa, &bulletquaSys->vecAxis, bulletquaSys->fVAngle,&bulletqua->posBulletQua);
			// 回転軸に設置
			bulletqua->posBulletQua += bulletquaSys->vecTagPos;

			// シャドウ管理
			SetPositionShadow(bulletqua->nIdxShadow, D3DXVECTOR3(bulletqua->posBulletQua.x, 0.2f, bulletqua->posBulletQua.z));
			SetVertexShadow(bulletqua->nIdxShadow, bulletqua->fSizeShadow, bulletqua->fSizeShadow);
			SetColorShadow(bulletqua->nIdxShadow, bulletqua->colShadow);

			// エフェクト設置
			//SetEffect(bulletqua->nTex, bulletqua->vec2Size, bulletqua->colorBulletQua, bulletqua->posBulletQua, bulletqua->vec2Size.x / 60, bulletqua->vec2Size.x / 80);

			// 使用カウントで透過→初期化
			if (bulletqua->nUseCount > BULLETQUA_FALSE_COUNT)
			{
				bulletqua->colorBulletQua.a -= BULLETQUA_ALPHA_CHANGE;
				bulletqua->colShadow.a -= BULLETQUA_ALPHA_CHANGE;
				if (bulletqua->colorBulletQua.a >= 0.0f)
				{
					SetDiffuseBulletQua(i, bulletqua->colorBulletQua);	// 反射光変更の変更処理
				}
				else
				{
					bulletquaSys->nBulletCount--;
					// シャドウの無効化
					ReleaseShadow(bulletqua->nIdxShadow);
					InitStatusBulletQua(i);
				}
			}
		// 使用カウント増加
		bulletqua->nUseCount++;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBulletQua(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// スケール, 回転, 平行移動

	BULLETQUA *bulletqua = &bulletquaWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, BULLETQUA_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (bulletqua->bUse && bulletqua->bDraw)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBulletQua);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ビルボード化
			g_mtxWorldBulletQua._11 = mtxView._11;
			g_mtxWorldBulletQua._12 = mtxView._21;
			g_mtxWorldBulletQua._13 = mtxView._31;
			g_mtxWorldBulletQua._21 = mtxView._12;
			g_mtxWorldBulletQua._22 = mtxView._22;
			g_mtxWorldBulletQua._23 = mtxView._32;
			g_mtxWorldBulletQua._31 = mtxView._13;
			g_mtxWorldBulletQua._32 = mtxView._23;
			g_mtxWorldBulletQua._33 = mtxView._33;


			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, bulletqua->sclBulletQua.x, bulletqua->sclBulletQua.y, bulletqua->sclBulletQua.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletQua, &g_mtxWorldBulletQua, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bulletqua->rotBulletQua.y, bulletqua->rotBulletQua.x, bulletqua->rotBulletQua.z);
			//D3DXMatrixMultiply(&bulletqua->mtxWorld, &bulletqua->mtxWorld, &GetInvRotateMat(bulletqua->posBulletQua));
			////D3DXMatrixMultiply(&bulletqua->mtxWorld, &bulletqua->mtxWorld, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, bulletqua->posBulletQua.x, bulletqua->posBulletQua.y, bulletqua->posBulletQua.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletQua, &g_mtxWorldBulletQua, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletQua);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletQua, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureBulletQua);

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
HRESULT MakeVertexBulletQua(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLETQUA_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffBulletQua,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLETQUA_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLETQUA_SIZE_X, -BULLETQUA_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETQUA_SIZE_X, -BULLETQUA_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletQua->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxBulletQua(int nBulletQua, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseBulletQua(int nBulletQua, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexBulletQua(int nBulletQua, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// バレットを対象に設置
//=============================================================================
void SetBulletQua(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];

	// 未使用の弾を探す
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			bulletqua->colorBulletQua = SetColorPallet(5);							// カラーを設定
			bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y);	// サイズを設定
			bulletqua->fMoveSpeed = BULLETQUA_SPEED;								// 移動スピードを設定
			bulletqua->nUseCount = 0;

			bulletqua->bUse = true;
			bulletqua->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bulletqua->vecTag, &bulletqua->vecTag);
			bulletqua->posBulletQua = vecPos2;

			// 影の生成
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// 通常エネミーバレットを対象に設置
//=============================================================================
void SetEnemyBulletQua00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			// カラー
			bulletqua->colorBulletQua = SetColorPallet(3);
			// サイズ
			bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_ENEMY_SIZE_X, BULLETQUA_ENEMY_SIZE_Y);
			bulletqua->fMoveSpeed = BULLETQUA_ENEMY_SPEED;			// 移動スピード
			bulletqua->nTex = 0;									// テクスチャー
			bulletqua->nUseCount = 0;
			bulletqua->bUse = true;
			bulletqua->bEnemy = true;

			bulletqua->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bulletqua->vecTag, &bulletqua->vecTag);
			bulletqua->posBulletQua = vecPos2;

			// 影の生成
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// システムバレットを対象に設定
//=============================================================================
void SetSysBulletQua(int nType, int nTex, int nSize, int nColor, int nSplit, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	int *nTotalCount = GetTotalCount();

	// 未使用のシステムを探す
	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		if (!bulletquaSys->bUse)
		{
			bulletquaSys->bUse = true;								// 有効化
			bulletquaSys->bProcess = true;							// プロセス有効化
			bulletquaSys->vecTagPos = vecPos;						// 対象POSを記憶
			bulletquaSys->colorBulletQua = SetColorPallet(nColor);
			bulletquaSys->vec2Size = D3DXVECTOR2(nSize, nSize);		// サイズ

			bulletquaSys->fMoveSpeed = fSpeed;						// 移動スピード
			bulletquaSys->fVAngle = 0.0f;
			bulletquaSys->fWave = fWave;
			bulletquaSys->nType = nType;
			bulletquaSys->nFireTime = 0;
			bulletquaSys->nCoolDown = 0;
			bulletquaSys->nTex = nTex;
			bulletquaSys->nSplitCount = nSplit;

			bulletquaSys->fSplitAngle = D3DX_PI * 2 / nSplit;
			bulletquaSys->fAddAngle = fAngle;
			bulletquaSys->fHAngle = 0.0f;

			return;
		}
	}
}

//=============================================================================
// システムバレット初期化処理
//=============================================================================
void SetSysBulletQuaFire(int nSys)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nSys];
	int *nTotalCount = GetTotalCount();

	// 未使用の弾を探す
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			bulletqua->colorBulletQua = bulletquaSys->colorBulletQua;		// カラー
			bulletqua->vec2Size = bulletquaSys->vec2Size;					// サイズ
			bulletqua->fMoveSpeed = bulletquaSys->fMoveSpeed;				// 移動スピード
			bulletqua->nTex = bulletquaSys->nTex;							// テクスチャー
			bulletqua->nUseCount = 0;
			bulletqua->bUse = true;											// 有効化
			bulletqua->bEnemy = true;										// エネミーフラグを有効化
			bulletqua->nSysNum = nSys;									// システムフラグを有効化

			SetVtxBulletQua(i, bulletqua->vec2Size.x, bulletqua->vec2Size.y);

			// 影の生成
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

			bulletquaSys->nBulletCount++;

			// 360度発射が完了したらプロセスを終了
			bulletquaSys->nSplitCount--;
			if (bulletquaSys->nSplitCount <= 0)
			{
				bulletquaSys->bProcess = false;
				bulletquaSys->nCoolDown = *nTotalCount;
				return;
			}
		}
	}
}
//=============================================================================
// システムバレット更新処理
//=============================================================================
void UpdateSysBulletQua(int nSys)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nSys];
	ENEMY *enemy = GetEnemy(0);

	switch (bulletquaSys->nType)
	{
	case BULLET_QUA_ROT_L:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_R:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_LL:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_RR:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_LV:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_RV:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	}
}

//=============================================================================
// 取得関数
//=============================================================================
BULLETQUA *GetBulletQua(int no)
{
	return(&bulletquaWk[no]);
}