//=============================================================================
//
// RETICLE処理 [reticle.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "reticle.h"
#include "calculate.h"
#include "main.h"
#include "magic.h"
#include "ranking.h"
#include "rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexReticle(int no);

void SetVertexReticle(int no);
void SetDiffuseReticle(int no, D3DXCOLOR color);
void SetTextureReticle(int no, int cntPattern);
void SetChangeCollarReticle(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
RETICLE					reticleWk[RETICLE_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureReticle[RETICLE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitReticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RETICLE00,				// ファイルの名前
			&pD3DTextureReticle[0]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RETICLE01,				// ファイルの名前
			&pD3DTextureReticle[1]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RETICLE_CENTER,				// ファイルの名前
			&pD3DTextureReticle[2]);			// 読み込むメモリのポインタ
	}

	// RETICLEの初期化
	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		reticle->bUse = true;
		reticle->pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
		reticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		reticle->colorMagic = D3DXCOLOR(TEXTURE_RETICLE_R, TEXTURE_RETICLE_G, TEXTURE_RETICLE_B, TEXTURE_RETICLE_A);
		reticle->fBaseAngle = 0.0f;
		reticle->fRadius = 0.0f;
		reticle->fScale = 1.0f;
		reticle->nCollarFlag = 0;


		reticle->Texture = pD3DTextureReticle[i];			// テクスチャへのRETICLE
		if (i == RETICLE_CENTER)
		{
			reticle->size = D3DXVECTOR2(TEXTURE_RETICLE_SIZE_X, TEXTURE_RETICLE_SIZE_Y);
		}
		else
		{
			reticle->size = D3DXVECTOR2(TEXTURE_RETICLE_CENTER_SIZE_X, TEXTURE_RETICLE_CENTER_SIZE_Y);
		}

		// 頂点情報の作成
		MakeVertexReticle(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitReticle(void)
{
	RETICLE *reticle = &reticleWk[0];

	for (int i = 0; i < RETICLE_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureReticle[i] != NULL)
		{
			pD3DTextureReticle[i]->Release();
			pD3DTextureReticle[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		if (reticle->bUse)
		{
			// 回転処理

			switch (i)
			{
			case RETICLE_1:
				reticle->rot.z -= RETICLE_ROTATION_SPEED;
				break;
			case RETICLE_2:
				reticle->rot.z += RETICLE_ROTATION_SPEED;
				break;
			}

			reticle->rot.z = PiCalculate360(reticle->rot.z);// PIの誤差修正
			if (GetStage() == STAGE_GAME)
			{
				reticle->colorMagic = GetMagicColor();			// マジックのカラーを取得して適用
			}
			else
			{
				reticle->colorMagic = GetRankColor();			// マジックのカラーを取得して適用
				reticle->fScale = RETICLE_RESULT_SCALE;
			}
			SetVertexReticle(i);							// 移動後の座標で頂点を設定
			SetDiffuseReticle(i, reticle->colorMagic);		// カラー適用処理
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawReticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RETICLE_MAX; i++, reticle++)
	{
		if (reticle->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, reticle->Texture);

			// RETICLEの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RETICLE, reticle->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexReticle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RETICLE *reticle = &reticleWk[no];

	// 角度と半径を求める
	if (no == RETICLE_CENTER)
	{
		reticle->fBaseAngle = atan2f(TEXTURE_RETICLE_CENTER_SIZE_Y, TEXTURE_RETICLE_CENTER_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_RETICLE_CENTER_SIZE_X, TEXTURE_RETICLE_CENTER_SIZE_Y);
		reticle->fRadius = D3DXVec2Length(&temp);
	}
	else
	{
		reticle->fBaseAngle = atan2f(TEXTURE_RETICLE_SIZE_Y, TEXTURE_RETICLE_SIZE_X);
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_RETICLE_SIZE_X, TEXTURE_RETICLE_SIZE_Y);
		reticle->fRadius = D3DXVec2Length(&temp);
	}

	// 頂点座標の設定
	reticle->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	reticle->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_RETICLE_SIZE_X, 100.0f, 0.0f);
	reticle->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_RETICLE_SIZE_Y, 0.0f);
	reticle->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_RETICLE_SIZE_X, 100.0f + TEXTURE_RETICLE_SIZE_Y, 0.0f);
	//SetVertexReticle();

	// rhwの設定
	reticle->vertexWk[0].rhw =
	reticle->vertexWk[1].rhw =
	reticle->vertexWk[2].rhw =
	reticle->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	reticle->vertexWk[0].diffuse =
	reticle->vertexWk[1].diffuse =
	reticle->vertexWk[2].diffuse =
	reticle->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	reticle->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	reticle->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE, 0.0f);
	reticle->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE);
	reticle->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE);

	return S_OK;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseReticle(int no, D3DXCOLOR color)
{
	RETICLE *reticle = &reticleWk[no];

	// 反射光の設定
	reticle->vertexWk[0].diffuse =
	reticle->vertexWk[1].diffuse =
	reticle->vertexWk[2].diffuse =
	reticle->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureReticle(int no, int cntPattern)
{
	RETICLE *reticle = &reticleWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RETICLE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RETICLE;
	reticle->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	reticle->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	reticle->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	reticle->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexReticle(int no)
{
	RETICLE *reticle = &reticleWk[no];

	// 頂点0:回転あり
	reticle->vertexWk[0].vtx.x = reticle->pos.x - cosf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[0].vtx.y = reticle->pos.y - sinf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	reticle->vertexWk[1].vtx.x = reticle->pos.x + cosf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[1].vtx.y = reticle->pos.y - sinf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	reticle->vertexWk[2].vtx.x = reticle->pos.x - cosf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[2].vtx.y = reticle->pos.y + sinf(reticle->fBaseAngle - reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	reticle->vertexWk[3].vtx.x = reticle->pos.x + cosf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[3].vtx.y = reticle->pos.y + sinf(reticle->fBaseAngle + reticle->rot.z) * (reticle->fRadius * reticle->fScale);
	reticle->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarReticle(int no)
{
	RETICLE *reticle = &reticleWk[no];

	switch (reticle->nCollarFlag)
	{
	case 0:
		reticle->colorMagic -= D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		reticle->colorMagic += D3DXCOLOR(0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		if (reticle->colorMagic.r <= 0.0f)
		{
			reticle->colorMagic.r = 0.0f;
			reticle->nCollarFlag++;
		}
		if (reticle->colorMagic.g >= 1.0f)
		{
			reticle->colorMagic.g = 1.0f;
		}
		if (reticle->colorMagic.b >= 1.0f)
		{
			reticle->colorMagic.b = 1.0f;
		}
		break;
	case 1:
		reticle->colorMagic -= D3DXCOLOR(0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f);
		reticle->colorMagic += D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		if (reticle->colorMagic.r >= 1.0f)
		{
			reticle->colorMagic.r = 1.0f;
		}
		if (reticle->colorMagic.g <= 0.0f)
		{
			reticle->colorMagic.g = 0.0f;
			reticle->nCollarFlag++;
		}
		if (reticle->colorMagic.b >= 1.0f)
		{
			reticle->colorMagic.b = 1.0f;
		}
		break;
	case 2:
		reticle->colorMagic -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f);
		reticle->colorMagic += D3DXCOLOR(TEXTURE_RETICLE_CHANGE_COLLAR, TEXTURE_RETICLE_CHANGE_COLLAR, 0.0f, 0.0f);
		if (reticle->colorMagic.r >= 1.0f)
		{
			reticle->colorMagic.r = 1.0f;
		}
		if (reticle->colorMagic.g >= 1.0f)
		{
			reticle->colorMagic.g = 1.0f;
		}
		if (reticle->colorMagic.b <= 0.0f)
		{
			reticle->colorMagic.b = 0.0f;
			reticle->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// 有効設定
//=============================================================================
void SetReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	reticle->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseReticle(void)
{
	RETICLE *reticle = &reticleWk[0];
	//reticle->bUse = false;
}

//=============================================================================
// RETICLE取得関数
//=============================================================================
RETICLE *GetReticle(int no)
{
	return(&reticleWk[no]);
}

