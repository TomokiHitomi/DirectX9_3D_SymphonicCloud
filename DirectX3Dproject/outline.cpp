//=============================================================================
//
// OUTLINE処理 [outline.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "outline.h"
#include "main.h"
#include "minimap.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexOutline(int no);
void SetVertexOutline(int no);
void SetDiffuseOutline(int no, D3DXCOLOR color);
void SetTextureOutline(int no, int cntPattern);
void SetChangeCollarOutline(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
OUTLINE					outlineWk[OUTLINE_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureOutline = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitOutline(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_OUTLINE,				// ファイルの名前
			&pD3DTextureOutline);			// 読み込むメモリのポインタ
	}

	// OUTLINEの初期化
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		outline->bUse = false;
		outline->pos = D3DXVECTOR3(TEXTURE_OUTLINE_POS_X, TEXTURE_OUTLINE_POS_Y, 0.0f);
		outline->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		outline->colorOutline = D3DXCOLOR(TEXTURE_OUTLINE_R, TEXTURE_OUTLINE_G, TEXTURE_OUTLINE_B, TEXTURE_OUTLINE_A);
		outline->fBaseAngle = 0.0f;
		outline->fRadius = 0.0f;

		outline->nCollarFlag = 0;

		outline->Texture = pD3DTextureOutline;			// テクスチャへのOUTLINE
		outline->size = D3DXVECTOR2(TEXTURE_OUTLINE_SIZE_X, TEXTURE_OUTLINE_SIZE_Y);

		// 頂点情報の作成
		MakeVertexOutline(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOutline(void)
{
	OUTLINE *outline = &outlineWk[0];

	// メモリ解放
	if (pD3DTextureOutline != NULL)
	{
		pD3DTextureOutline->Release();
		pD3DTextureOutline = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOutline(void)
{
	OUTLINE *outline = &outlineWk[0];
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		if (outline->bUse)
		{
			// 回転テスト
			outline->rot.z += 0.003f;

			//SetChangeCollarOutline(i);				// カラー変更処理
			SetVertexOutline(i);						// 移動後の座標で頂点を設定
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOutline(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{
		if (outline->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, outline->Texture);

			// OUTLINEの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_OUTLINE, outline->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexOutline(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	OUTLINE *outline = &outlineWk[no];

	// 角度と半径を求める
	outline->fBaseAngle = atan2f(TEXTURE_OUTLINE_SIZE_Y, TEXTURE_OUTLINE_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_OUTLINE_SIZE_X, TEXTURE_OUTLINE_SIZE_Y);
	outline->fRadius = D3DXVec2Length(&temp);

	// 頂点座標の設定
	outline->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	outline->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_OUTLINE_SIZE_X, 100.0f, 0.0f);
	outline->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_OUTLINE_SIZE_Y, 0.0f);
	outline->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_OUTLINE_SIZE_X, 100.0f + TEXTURE_OUTLINE_SIZE_Y, 0.0f);
	//SetVertexOutline();

	// rhwの設定
	outline->vertexWk[0].rhw =
	outline->vertexWk[1].rhw =
	outline->vertexWk[2].rhw =
	outline->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	outline->vertexWk[0].diffuse =
	outline->vertexWk[1].diffuse =
	outline->vertexWk[2].diffuse =
	outline->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	outline->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	outline->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE, 0.0f);
	outline->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE);
	outline->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexOutline(int no)
{
	OUTLINE *outline = &outlineWk[no];
	// 頂点0:回転あり
	outline->vertexWk[0].vtx.x = outline->pos.x - cosf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[0].vtx.y = outline->pos.y - sinf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	outline->vertexWk[1].vtx.x = outline->pos.x + cosf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[1].vtx.y = outline->pos.y - sinf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	outline->vertexWk[2].vtx.x = outline->pos.x - cosf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[2].vtx.y = outline->pos.y + sinf(outline->fBaseAngle - outline->rot.z) * outline->fRadius;
	outline->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	outline->vertexWk[3].vtx.x = outline->pos.x + cosf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[3].vtx.y = outline->pos.y + sinf(outline->fBaseAngle + outline->rot.z) * outline->fRadius;
	outline->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseOutline(int no, D3DXCOLOR color)
{
	OUTLINE *outline = &outlineWk[no];

	// 反射光の設定
	outline->vertexWk[0].diffuse =
	outline->vertexWk[1].diffuse =
	outline->vertexWk[2].diffuse =
	outline->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureOutline(int no, int cntPattern)
{
	OUTLINE *outline = &outlineWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_OUTLINE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_OUTLINE;
	outline->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	outline->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	outline->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	outline->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarOutline(int no)
{
	OUTLINE *outline = &outlineWk[no];

	switch (outline->nCollarFlag)
	{
	case 0:
		outline->colorOutline -= D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		outline->colorOutline += D3DXCOLOR(0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		if (outline->colorOutline.r <= 0.0f)
		{
			outline->colorOutline.r = 0.0f;
			outline->nCollarFlag++;
		}
		if (outline->colorOutline.g >= 1.0f)
		{
			outline->colorOutline.g = 1.0f;
		}
		if (outline->colorOutline.b >= 1.0f)
		{
			outline->colorOutline.b = 1.0f;
		}
		break;
	case 1:
		outline->colorOutline -= D3DXCOLOR(0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f);
		outline->colorOutline += D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		if (outline->colorOutline.r >= 1.0f)
		{
			outline->colorOutline.r = 1.0f;
		}
		if (outline->colorOutline.g <= 0.0f)
		{
			outline->colorOutline.g = 0.0f;
			outline->nCollarFlag++;
		}
		if (outline->colorOutline.b >= 1.0f)
		{
			outline->colorOutline.b = 1.0f;
		}
		break;
	case 2:
		outline->colorOutline -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f);
		outline->colorOutline += D3DXCOLOR(TEXTURE_OUTLINE_CHANGE_COLLAR, TEXTURE_OUTLINE_CHANGE_COLLAR, 0.0f, 0.0f);
		if (outline->colorOutline.r >= 1.0f)
		{
			outline->colorOutline.r = 1.0f;
		}
		if (outline->colorOutline.g >= 1.0f)
		{
			outline->colorOutline.g = 1.0f;
		}
		if (outline->colorOutline.b <= 0.0f)
		{
			outline->colorOutline.b = 0.0f;
			outline->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// 有効設定
//=============================================================================
int SetOutline(D3DXCOLOR colorfloat)
{
	OUTLINE *outline = &outlineWk[0];
	for (int i = 0; i < OUTLINE_MAX; i++, outline++)
	{	// 未使用を探す
		if (!outline->bUse)
		{	// 有効化処理
			outline->bUse = true;
			SetDiffuseOutline(i, colorfloat);	// カラー適用処理
			return i;
		}
	}
}

//=============================================================================
// 更新設定
//=============================================================================
void SetUpdateOutline(int nOutline, D3DXVECTOR3 pos, float fAngle)
{
	OUTLINE *outline = &outlineWk[nOutline];

	// 各情報の更新
	outline->pos = pos;
	outline->rot.z = fAngle;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseOutline(int nOutline)
{
	OUTLINE *outline = &outlineWk[nOutline];
	outline->bUse = false;
}

//=============================================================================
// OUTLINE取得関数
//=============================================================================
OUTLINE *GetOutline(int no)
{
	return(&outlineWk[no]);
}

