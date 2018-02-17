//=============================================================================
//
// GAGEBACK処理 [gageback.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "gageback.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGageback(int no);

void SetVertexGageback(int no);
void SetDiffuseGageback(int no, D3DXCOLOR color);
void SetTextureGageback(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAGEBACK					gagebackWk[GAGEBACK_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureGageback[GAGEBACK_TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGageback(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEBACK00,				// ファイルの名前
			&pD3DTextureGageback[GAGEBACK_MAGIC]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEBACK01,				// ファイルの名前
			&pD3DTextureGageback[GAGEBACK_DEFAULT]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGEBACK02,				// ファイルの名前
			&pD3DTextureGageback[GAGEBACK_BULLET]);			// 読み込むメモリのポインタ
	}

	// GAGEBACKの初期化
	for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	{

		switch (i)
		{
		case GAGEBACK_TYPE_MAGIC:
			gageback->bUse = true;
			gageback->bPatternFlag = false;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_POS_X, TEXTURE_GAGEBACK_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_SIZE_X, TEXTURE_GAGEBACK_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_MAGIC];			// テクスチャへのGAGEBACK

			break;
		case GAGEBACK_TYPE_BOSS:
			gageback->bUse = false;
			gageback->bPatternFlag = true;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_BOSS_POS_X, TEXTURE_GAGEBACK_BOSS_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_BOSS_SIZE_X, TEXTURE_GAGEBACK_BOSS_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_DEFAULT];		// テクスチャへのGAGEBACK
			break;
		case GAGEBACK_TYPE_MODEL:
			gageback->bUse = true;
			gageback->bPatternFlag = true;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_MODEL_POS_X, TEXTURE_GAGEBACK_MODEL_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_MODEL_SIZE_X, TEXTURE_GAGEBACK_MODEL_SIZE_Y);
			gageback->nPatternAnim = 1;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_DEFAULT];		// テクスチャへのGAGEBACK
			break;
		case GAGEBACK_TYPE_BULLET:
			gageback->bUse = true;
			gageback->bPatternFlag = false;
			gageback->pos = D3DXVECTOR3(TEXTURE_GAGEBACK_BULLET_POS_X, TEXTURE_GAGEBACK_BULLET_POS_Y, 0.0f);
			gageback->size = D3DXVECTOR2(TEXTURE_GAGEBACK_BULLET_SIZE_X, TEXTURE_GAGEBACK_BULLET_SIZE_Y);
			gageback->nPatternAnim = 0;
			gageback->Texture = pD3DTextureGageback[GAGEBACK_BULLET];		// テクスチャへのGAGEFREAM
			break;
		}

		gageback->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点情報の作成
		MakeVertexGageback(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGageback(void)
{
	for (int i = 0; i < GAGEBACK_TEXTURE_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureGageback[i] != NULL)
		{
			pD3DTextureGageback[i]->Release();
			pD3DTextureGageback[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGageback(void)
{
	//GAGEBACK *gageback = &gagebackWk[0];
	//for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	//{
	//	if (gageback->bUse)
	//	{
	//		SetDiffuseGageback(i, GetMagicColor());
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGageback(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGEBACK_MAX; i++, gageback++)
	{
		if (gageback->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, gageback->Texture);

			// GAGEBACKの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGEBACK, gageback->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGageback(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGEBACK *gageback = &gagebackWk[no];

	// 頂点座標の設定
	SetVertexGageback(no);

	// rhwの設定
	gageback->vertexWk[0].rhw =
	gageback->vertexWk[1].rhw =
	gageback->vertexWk[2].rhw =
	gageback->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	gageback->vertexWk[0].diffuse =
	gageback->vertexWk[1].diffuse =
	gageback->vertexWk[2].diffuse =
	gageback->vertexWk[3].diffuse = D3DXCOLOR(TEXTURE_GAGEBACK_R, TEXTURE_GAGEBACK_G, TEXTURE_GAGEBACK_B, TEXTURE_GAGEBACK_A);


	if (gageback->bPatternFlag)
	{
		SetTextureGageback(no, no - 1);
	}
	else
	{
		// テクスチャ座標の設定
		gageback->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		gageback->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		gageback->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		gageback->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexGageback(int no)
{
	GAGEBACK *gageback = &gagebackWk[no];

	gageback->vertexWk[0].vtx.x = gageback->pos.x - gageback->size.x;
	gageback->vertexWk[0].vtx.y = gageback->pos.y - gageback->size.y;
	gageback->vertexWk[0].vtx.z = 0.0f;

	gageback->vertexWk[1].vtx.x = gageback->pos.x + gageback->size.x;
	gageback->vertexWk[1].vtx.y = gageback->pos.y - gageback->size.y;
	gageback->vertexWk[1].vtx.z = 0.0f;

	gageback->vertexWk[2].vtx.x = gageback->pos.x - gageback->size.x;
	gageback->vertexWk[2].vtx.y = gageback->pos.y + gageback->size.y;
	gageback->vertexWk[2].vtx.z = 0.0f;

	gageback->vertexWk[3].vtx.x = gageback->pos.x + gageback->size.x;
	gageback->vertexWk[3].vtx.y = gageback->pos.y + gageback->size.y;
	gageback->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseGageback(int no, D3DXCOLOR color)
{
	GAGEBACK *gageback = &gagebackWk[no];

	// 反射光の設定
	gageback->vertexWk[0].diffuse =
		gageback->vertexWk[1].diffuse =
		gageback->vertexWk[2].diffuse =
		gageback->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGageback(int no, int cntPattern)
{
	GAGEBACK *gageback = &gagebackWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGEBACK;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK;
	gageback->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gageback->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gageback->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gageback->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 有効設定
//=============================================================================
void SetGageback(int nGage)
{
	GAGEBACK *gageback = &gagebackWk[nGage];
	gageback->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseGageback(void)
{
	GAGEBACK *gageback = &gagebackWk[0];
	gageback->bUse = false;
}

//=============================================================================
// GAGEBACK取得関数
//=============================================================================
GAGEBACK *GetGageback(int no)
{
	return(&gagebackWk[no]);
}

