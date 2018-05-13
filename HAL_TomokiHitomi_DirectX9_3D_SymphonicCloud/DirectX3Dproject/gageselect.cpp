//=============================================================================
//
// GAGESELECT処理 [gageselect.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "gageselect.h"
#include "main.h"
#include "magic.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGageselect(int no);
void SetTextureGageselect(int no, int cntPattern);	//
void SetVertexGageselect(int no);
void SetDiffuseGageselect(int no);
void SetChangeCollarGageselect(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAGESELECT					gageselectWk[GAGESELECT_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureGageselect = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGageselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_GAGESELECT,				// ファイルの名前
			&pD3DTextureGageselect);			// 読み込むメモリのポインタ
	}

	// GAGESELECTの初期化
	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		switch(i)
		{
		case GAGESELECT_MAGIC:
			gageselect->bUse = false;
			gageselect->pos = D3DXVECTOR3(TEXTURE_GAGESELECT_MAGIC_POS_X, TEXTURE_GAGESELECT_MAGIC_POS_Y, 0.0f);
			gageselect->rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI);
			gageselect->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		case GAGESELECT_BULLET:
			gageselect->bUse = true;
			gageselect->pos = D3DXVECTOR3(TEXTURE_GAGESELECT_BULLET_POS_X, TEXTURE_GAGESELECT_BULLET_POS_Y, 0.0f);
			gageselect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			gageselect->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		gageselect->nCountAnim = 0;
		gageselect->nPatternAnim = 0;

		gageselect->Texture = pD3DTextureGageselect;			// テクスチャへのGAGESELECT
		gageselect->size = D3DXVECTOR2(TEXTURE_GAGESELECT_SIZE_X, TEXTURE_GAGESELECT_SIZE_Y);

		// 頂点情報の作成
		MakeVertexGageselect(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];

	// メモリ解放
	if (pD3DTextureGageselect != NULL)
	{
		pD3DTextureGageselect->Release();
		pD3DTextureGageselect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];
	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		if (gageselect->bUse)
		{
			if (i == GAGESELECT_MAGIC)
			{
				float fMove = TEXTURE_GAGESELECT_MAGIC_POS_Y + TEXTURE_GAGESELECT_MAGIC_INTERVAL * GetTypeMagic();

				fMove -= gageselect->pos.y;
				gageselect->pos.y += fMove * TEXTURE_GAGESELECT_MOVESPEED;
			}
			SetChangeCollarGageselect(i);
			SetDiffuseGageselect(i);
			// テクスチャ座標を設定
			SetTextureGageselect(i, gageselect->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexGageselect(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGageselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < GAGESELECT_MAX; i++, gageselect++)
	{
		if (gageselect->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, gageselect->Texture);

			// GAGESELECTの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_GAGESELECT, gageselect->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGageselect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAGESELECT *gageselect = &gageselectWk[no];

	// 角度と半径を求める
	gageselect->fBaseAngle = atan2f(TEXTURE_GAGESELECT_SIZE_Y, TEXTURE_GAGESELECT_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_GAGESELECT_SIZE_X, TEXTURE_GAGESELECT_SIZE_Y);
	gageselect->fRadius = D3DXVec2Length(&temp);

	// 頂点座標の設定
	gageselect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	gageselect->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_GAGESELECT_SIZE_X, 100.0f, 0.0f);
	gageselect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_GAGESELECT_SIZE_Y, 0.0f);
	gageselect->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_GAGESELECT_SIZE_X, 100.0f + TEXTURE_GAGESELECT_SIZE_Y, 0.0f);
	//SetVertexGageselect();

	// rhwの設定
	gageselect->vertexWk[0].rhw =
	gageselect->vertexWk[1].rhw =
	gageselect->vertexWk[2].rhw =
	gageselect->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	gageselect->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);
	gageselect->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_GAGESELECT_R, TEXTURE_GAGESELECT_G, TEXTURE_GAGESELECT_B, 255);

	// テクスチャ座標の設定
	gageselect->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	gageselect->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT, 0.0f);
	gageselect->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT);
	gageselect->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGageselect(int no, int cntPattern)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_GAGESELECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT;
	gageselect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	gageselect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gageselect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	gageselect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// 反射光の設定
	gageselect->vertexWk[0].diffuse =
		gageselect->vertexWk[1].diffuse =
		gageselect->vertexWk[2].diffuse =
		gageselect->vertexWk[3].diffuse = gageselect->xColor;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	// 頂点0:回転あり
	gageselect->vertexWk[0].vtx.x = gageselect->pos.x - cosf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[0].vtx.y = gageselect->pos.y - sinf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	gageselect->vertexWk[1].vtx.x = gageselect->pos.x + cosf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[1].vtx.y = gageselect->pos.y - sinf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	gageselect->vertexWk[2].vtx.x = gageselect->pos.x - cosf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[2].vtx.y = gageselect->pos.y + sinf(gageselect->fBaseAngle - gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	gageselect->vertexWk[3].vtx.x = gageselect->pos.x + cosf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[3].vtx.y = gageselect->pos.y + sinf(gageselect->fBaseAngle + gageselect->rot.z) * gageselect->fRadius;
	gageselect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarGageselect(int no)
{
	GAGESELECT *gageselect = &gageselectWk[no];

	switch (gageselect->nCollarFlag)
	{
	case 0:
		gageselect->xColor -= D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		gageselect->xColor += D3DXCOLOR(0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		if (gageselect->xColor.r <= 0.0f)
		{
			gageselect->xColor.r = 0.0f;
			gageselect->nCollarFlag++;
		}
		if (gageselect->xColor.g >= 1.0f)
		{
			gageselect->xColor.g = 1.0f;
		}
		if (gageselect->xColor.b >= 1.0f)
		{
			gageselect->xColor.b = 1.0f;
		}
		break;
	case 1:
		gageselect->xColor -= D3DXCOLOR(0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f);
		gageselect->xColor += D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		if (gageselect->xColor.r >= 1.0f)
		{
			gageselect->xColor.r = 1.0f;
		}
		if (gageselect->xColor.g <= 0.0f)
		{
			gageselect->xColor.g = 0.0f;
			gageselect->nCollarFlag++;
		}
		if (gageselect->xColor.b >= 1.0f)
		{
			gageselect->xColor.b = 1.0f;
		}
		break;
	case 2:
		gageselect->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f);
		gageselect->xColor += D3DXCOLOR(TEXTURE_GAGESELECT_CHANGE_COLLAR, TEXTURE_GAGESELECT_CHANGE_COLLAR, 0.0f, 0.0f);
		if (gageselect->xColor.r >= 1.0f)
		{
			gageselect->xColor.r = 1.0f;
		}
		if (gageselect->xColor.g >= 1.0f)
		{
			gageselect->xColor.g = 1.0f;
		}
		if (gageselect->xColor.b <= 0.0f)
		{
			gageselect->xColor.b = 0.0f;
			gageselect->nCollarFlag = 0;
		}
		break;
	}
}


//=============================================================================
// 有効設定
//=============================================================================
void SetGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[GAGESELECT_MAGIC];
	gageselect->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseGageselect(void)
{
	GAGESELECT *gageselect = &gageselectWk[0];
	gageselect->bUse = false;
}

//=============================================================================
// GAGESELECT取得関数
//=============================================================================
GAGESELECT *GetGageselect(int no)
{
	return(&gageselectWk[no]);
}

