//=============================================================================
//
// GAGEFREAM処理 [gagefream.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "gagefream.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGagefream(int no);

void SetVertexGagefream(int no);
void SetDiffuseGagefream(int no, D3DXCOLOR color);
void SetTextureGagefream(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAGEFREAM					gagefreamWk[GAGEFREAM_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureGagefream[GAGEFREAM_TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGagefream(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEFREAM00,				// ファイルの名前
			&pD3DTextureGagefream[GAGEFREAM_MAGIC]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEFREAM01,				// ファイルの名前
			&pD3DTextureGagefream[GAGEFREAM_DEFAULT]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEFREAM02,				// ファイルの名前
			&pD3DTextureGagefream[GAGEFREAM_BULLET]);			// 読み込むメモリのポインタ

	}

	// GAGEFREAMの初期化
	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{

		switch (i)
		{
		case GAGEFREAM_TYPE_MAGIC:
			gagefream->bUse = true;
			gagefream->bPatternFlag = false;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_POS_X, TEXTURE_GAGEFREAM_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_SIZE_X, TEXTURE_GAGEFREAM_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_MAGIC];			// テクスチャへのGAGEFREAM

			break;
		case GAGEFREAM_TYPE_BOSS:
			gagefream->bUse = false;
			gagefream->bPatternFlag = true;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_BOSS_POS_X, TEXTURE_GAGEFREAM_BOSS_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_BOSS_SIZE_X, TEXTURE_GAGEFREAM_BOSS_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_DEFAULT];		// テクスチャへのGAGEFREAM
			break;
		case GAGEFREAM_TYPE_MODEL:
			gagefream->bUse = true;
			gagefream->bPatternFlag = true;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_MODEL_POS_X, TEXTURE_GAGEFREAM_MODEL_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_MODEL_SIZE_X, TEXTURE_GAGEFREAM_MODEL_SIZE_Y);
			gagefream->nPatternAnim = 1;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_DEFAULT];		// テクスチャへのGAGEFREAM
			break;
		case GAGEFREAM_TYPE_BULLET:
			gagefream->bUse = true;
			gagefream->bPatternFlag = false;
			gagefream->pos = D3DXVECTOR3(TEXTURE_GAGEFREAM_BULLET_POS_X, TEXTURE_GAGEFREAM_BULLET_POS_Y, 0.0f);
			gagefream->size = D3DXVECTOR2(TEXTURE_GAGEFREAM_BULLET_SIZE_X, TEXTURE_GAGEFREAM_BULLET_SIZE_Y);
			gagefream->nPatternAnim = 0;
			gagefream->Texture = pD3DTextureGagefream[GAGEFREAM_BULLET];		// テクスチャへのGAGEFREAM
			break;
		}

		gagefream->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点情報の作成
		MakeVertexGagefream(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGagefream(void)
{
	for (int i = 0; i < GAGEFREAM_TEXTURE_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureGagefream[i] != NULL)
		{
			pD3DTextureGagefream[i]->Release();
			pD3DTextureGagefream[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGagefream(void)
{
	GAGEFREAM *gagefream = &gagefreamWk[0];
	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{
		if (gagefream->bUse)
		{
			SetDiffuseGagefream(i, GetMagicColor());
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGagefream(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGEFREAM_MAX; i++, gagefream++)
	{
		if (gagefream->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, gagefream->Texture);

			// GAGEFREAMの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGEFREAM, gagefream->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGagefream(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// 頂点座標の設定
	SetVertexGagefream(no);

	// rhwの設定
	gagefream->vertexWk[0].rhw =
	gagefream->vertexWk[1].rhw =
	gagefream->vertexWk[2].rhw =
	gagefream->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	gagefream->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);
	gagefream->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGEFREAM_R, TEXTURE_GAGEFREAM_G, TEXTURE_GAGEFREAM_B, 255);


	if (gagefream->bPatternFlag)
	{
		SetTextureGagefream(no, no - 1);
	}
	else
	{
		// テクスチャ座標の設定
		gagefream->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		gagefream->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		gagefream->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		gagefream->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexGagefream(int no)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	gagefream->vertexWk[0].vtx.x = gagefream->pos.x - gagefream->size.x;
	gagefream->vertexWk[0].vtx.y = gagefream->pos.y - gagefream->size.y;
	gagefream->vertexWk[0].vtx.z = 0.0f;

	gagefream->vertexWk[1].vtx.x = gagefream->pos.x + gagefream->size.x;
	gagefream->vertexWk[1].vtx.y = gagefream->pos.y - gagefream->size.y;
	gagefream->vertexWk[1].vtx.z = 0.0f;

	gagefream->vertexWk[2].vtx.x = gagefream->pos.x - gagefream->size.x;
	gagefream->vertexWk[2].vtx.y = gagefream->pos.y + gagefream->size.y;
	gagefream->vertexWk[2].vtx.z = 0.0f;

	gagefream->vertexWk[3].vtx.x = gagefream->pos.x + gagefream->size.x;
	gagefream->vertexWk[3].vtx.y = gagefream->pos.y + gagefream->size.y;
	gagefream->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseGagefream(int no, D3DXCOLOR color)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// 反射光の設定
	gagefream->vertexWk[0].diffuse =
		gagefream->vertexWk[1].diffuse =
		gagefream->vertexWk[2].diffuse =
		gagefream->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGagefream(int no, int cntPattern)
{
	GAGEFREAM *gagefream = &gagefreamWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM;
	gagefream->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gagefream->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gagefream->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gagefream->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 有効設定
//=============================================================================
void SetGagefream(int nGage)
{
	GAGEFREAM *gagefream = &gagefreamWk[nGage];
	gagefream->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseGagefream(void)
{
	GAGEFREAM *gagefream = &gagefreamWk[0];
	gagefream->bUse = false;
}

//=============================================================================
// GAGEFREAM取得関数
//=============================================================================
GAGEFREAM *GetGagefream(int no)
{
	return(&gagefreamWk[no]);
}

