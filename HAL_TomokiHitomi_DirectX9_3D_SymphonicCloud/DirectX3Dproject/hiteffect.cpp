//=============================================================================
//
// HITEFFECT処理 [hiteffect.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "hiteffect.h"
#include "main.h"

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
HRESULT	MakeVertexHiteffect(LPDIRECT3DDEVICE9 pDevice);
void	SetVtxHiteffect(int nHiteffect, float fSizeX, float fSizeY);
void	SetDiffuseHiteffect(int nHiteffect, D3DXCOLOR col);
void	SetTexHiteffect(int nHiteffect, int nTex);
void	InitStatusHiteffect(int nHiteffect);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHiteffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffHiteffect = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldHiteffect;			// ワールドマトリックス

HITEFFECT					hiteffectWk[HITEFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHiteffect(int nType)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HITEFFECT,
			&g_pD3DTextureHiteffect);
	}

	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		// 位置・回転・スケールの設定
		hiteffect->posHiteffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		hiteffect->rotHiteffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		hiteffect->sclHiteffect = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		hiteffect->vec2Size = D3DXVECTOR2(HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y);
		InitStatusHiteffect(i);
	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexHiteffect(pDevice);

	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusHiteffect(int nHiteffect)
{
	HITEFFECT *hiteffect = &hiteffectWk[nHiteffect];

	hiteffect->bUse = false;
	hiteffect->nTex = 0;
	hiteffect->nAnimeCount = 0;
	hiteffect->nAnimePattern = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHiteffect(void)
{
	if (g_pD3DTextureHiteffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureHiteffect->Release();
		g_pD3DTextureHiteffect = NULL;
	}

	if (g_pD3DVtxBuffHiteffect != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffHiteffect->Release();
		g_pD3DVtxBuffHiteffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHiteffect(void)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];

#ifdef _DEBUG
	PrintDebugProc("【 HITEFFECT 】\n");
#endif
	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{	// 使用中のオブジェクトのみ処理
		if (hiteffect->bUse)
		{
			hiteffect->nAnimeCount++;
			if (hiteffect->nAnimeCount % TIME_ANIMATION_HITEFFECT == 0)
			{
				// パターンの切り替え
				hiteffect->nAnimePattern = (hiteffect->nAnimePattern + 1)
					% TEXTURE_PATTERN_DIVIDE_X_HITEFFECT
					+ (TEXTURE_PATTERN_DIVIDE_X_HITEFFECT* hiteffect->nTex);
				SetTexHiteffect(i, hiteffect->nAnimePattern);
				if (hiteffect->nAnimePattern == hiteffect->nTex * TEXTURE_PATTERN_DIVIDE_X_HITEFFECT)
				{
					InitStatusHiteffect(i);
				}

				//// アニメーション
				//player->nCountAnim++;
				//if ((player->nCountAnim % TIME_ANIMATION_PLAYER) == 0)
				//{
				//	// パターンの切り替え
				//	player->nPatternAnim = (player->nPatternAnim + 1) % (TEXTURE_PATTERN_DIVIDE_X_PLAYER)+(TEXTURE_PATTERN_DIVIDE_X_PLAYER * player->nDirection);
				//}

				//// アニメーション
				//effect_knockdown->nCountAnim++;
				//if ((effect_knockdown->nCountAnim % TIME_ANIMATION_EFFECT_KNOCKDOWN) == 0)
				//{
				//	// パターンの切り替え
				//	effect_knockdown->nPatternAnim++;
				//}

				//// アニメーション終了でfalse
				//if (effect_knockdown->nPatternAnim >= TEXTURE_PATTERN_DIVIDE_X_EFFECT_KNOCKDOWN * (effect_knockdown->nTypeAnim + 1))
				//{
				//	effect_knockdown->bUse = false;
				//}
			}
		}
	}
#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHiteffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	HITEFFECT *hiteffect = &hiteffectWk[0];

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, HITEFFECT_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		if (hiteffect->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldHiteffect);

			// ビューマトリックスを取得
			mtxView = GetMtxView();
			// ビルボード化
			g_mtxWorldHiteffect._11 = mtxView._11;
			g_mtxWorldHiteffect._12 = mtxView._21;
			g_mtxWorldHiteffect._13 = mtxView._31;
			g_mtxWorldHiteffect._21 = mtxView._12;
			g_mtxWorldHiteffect._22 = mtxView._22;
			g_mtxWorldHiteffect._23 = mtxView._32;
			g_mtxWorldHiteffect._31 = mtxView._13;
			g_mtxWorldHiteffect._32 = mtxView._23;
			g_mtxWorldHiteffect._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, hiteffect->sclHiteffect.x, hiteffect->sclHiteffect.y, hiteffect->sclHiteffect.z);
			D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, hiteffect->rotHiteffect.y, hiteffect->rotHiteffect.x, hiteffect->rotHiteffect.z);
			//D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &GetInvRotateMat(hiteffect->posHiteffect));
			////D3DXMatrixMultiply(&hiteffect->mtxWorld, &hiteffect->mtxWorld, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, hiteffect->posHiteffect.x, hiteffect->posHiteffect.y, hiteffect->posHiteffect.z);
			D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldHiteffect);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffHiteffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureHiteffect);

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
HRESULT MakeVertexHiteffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * HITEFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffHiteffect,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < HITEFFECT_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X, -HITEFFECT_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X, -HITEFFECT_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffHiteffect->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxHiteffect(int nHiteffect, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseHiteffect(int nHiteffect, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexHiteffect(int nHiteffect, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// バレットを対象に設置
//=============================================================================
void SetHiteffect(D3DXVECTOR3 pos,int nTex)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];

	// 未使用を探す
	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		if (!hiteffect->bUse)
		{
			hiteffect->bUse = true;
			hiteffect->posHiteffect = pos;
			hiteffect->nTex = nTex;
			return;
		}
	}
}

//=============================================================================
// 取得関数
//=============================================================================
HITEFFECT *GetHiteffect(int no)
{
	return(&hiteffectWk[no]);
}