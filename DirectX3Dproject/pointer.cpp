//=============================================================================
//
// POINTER処理 [pointer.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "pointer.h"
#include "main.h"
#include "minimap.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPointer(int no);
void SetVertexPointer(int no);
void SetDiffusePointer(int no, D3DXCOLOR color);
void SetTexturePointer(int no, int cntPattern);
void SetChangeCollarPointer(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
POINTER					pointerWk[POINTER_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTexturePointer[TEXTURE_GAME_POINTER_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPointer(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_POINTER,				// ファイルの名前
			&pD3DTexturePointer[POINTER_TEXTURE_NORMAIL]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_OUTLINE,				// ファイルの名前
			&pD3DTexturePointer[POINTER_TEXTURE_OUTLINE]);			// 読み込むメモリのポインタ
	}

	// POINTERの初期化
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		pointer->bUse = false;
		pointer->pos = D3DXVECTOR3(TEXTURE_POINTER_POS_X, TEXTURE_POINTER_POS_Y, 0.0f);
		pointer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pointer->colorPointer = D3DXCOLOR(TEXTURE_POINTER_R, TEXTURE_POINTER_G, TEXTURE_POINTER_B, TEXTURE_POINTER_A);
		pointer->fBaseAngle = 0.0f;
		pointer->fRadius = 0.0f;

		pointer->nCollarFlag = 0;
		pointer->nTex = 0;

		pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_NORMAIL];			// テクスチャへのPOINTER
		pointer->size = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);

		// 頂点情報の作成
		MakeVertexPointer(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPointer(void)
{
	POINTER *pointer = &pointerWk[0];

	for (int i = 0; i < TEXTURE_GAME_POINTER_MAX; i++, pointer++)
	{
		// メモリ解放
		if (pD3DTexturePointer[i] != NULL)
		{
			pD3DTexturePointer[i]->Release();
			pD3DTexturePointer[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePointer(void)
{
	POINTER *pointer = &pointerWk[0];
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->bUse)
		{
			// 回転テスト
			pointer->rot.z += 0.003f;

			//SetChangeCollarPointer(i);				// カラー変更処理
			SetVertexPointer(i);						// 移動後の座標で頂点を設定
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPointer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{
		if (pointer->bUse)
		{
			// テクスチャー切り替え
			if (pointer->nTex == POINTER_TEXTURE_NORMAIL)
			{	// ポインター表示
				pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_NORMAIL];
			}
			else if (pointer->nTex == POINTER_TEXTURE_OUTLINE)
			{	// アウトライン表示
				pointer->Texture = pD3DTexturePointer[POINTER_TEXTURE_OUTLINE];
			}
			// テクスチャの設定
			pDevice->SetTexture(0, pointer->Texture);

			// POINTERの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POINTER, pointer->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPointer(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	POINTER *pointer = &pointerWk[no];

	// 角度と半径を求める
	pointer->fBaseAngle = atan2f(TEXTURE_POINTER_SIZE_Y, TEXTURE_POINTER_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_POINTER_SIZE_X, TEXTURE_POINTER_SIZE_Y);
	pointer->fRadius = D3DXVec2Length(&temp);

	// 頂点座標の設定
	pointer->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pointer->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_POINTER_SIZE_X, 100.0f, 0.0f);
	pointer->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_POINTER_SIZE_Y, 0.0f);
	pointer->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_POINTER_SIZE_X, 100.0f + TEXTURE_POINTER_SIZE_Y, 0.0f);
	//SetVertexPointer();

	// rhwの設定
	pointer->vertexWk[0].rhw =
	pointer->vertexWk[1].rhw =
	pointer->vertexWk[2].rhw =
	pointer->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	pointer->vertexWk[0].diffuse =
	pointer->vertexWk[1].diffuse =
	pointer->vertexWk[2].diffuse =
	pointer->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pointer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pointer->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER, 0.0f);
	pointer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER);
	pointer->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexPointer(int no)
{
	POINTER *pointer = &pointerWk[no];
	// 頂点0:回転あり
	pointer->vertexWk[0].vtx.x = pointer->pos.x - cosf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[0].vtx.y = pointer->pos.y - sinf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	pointer->vertexWk[1].vtx.x = pointer->pos.x + cosf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[1].vtx.y = pointer->pos.y - sinf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	pointer->vertexWk[2].vtx.x = pointer->pos.x - cosf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[2].vtx.y = pointer->pos.y + sinf(pointer->fBaseAngle - pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	pointer->vertexWk[3].vtx.x = pointer->pos.x + cosf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[3].vtx.y = pointer->pos.y + sinf(pointer->fBaseAngle + pointer->rot.z) * pointer->fRadius;
	pointer->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffusePointer(int no, D3DXCOLOR color)
{
	POINTER *pointer = &pointerWk[no];

	// 反射光の設定
	pointer->vertexWk[0].diffuse =
	pointer->vertexWk[1].diffuse =
	pointer->vertexWk[2].diffuse =
	pointer->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePointer(int no, int cntPattern)
{
	POINTER *pointer = &pointerWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_POINTER;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_POINTER;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_POINTER;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_POINTER;
	pointer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	pointer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	pointer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	pointer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarPointer(int no)
{
	POINTER *pointer = &pointerWk[no];

	switch (pointer->nCollarFlag)
	{
	case 0:
		pointer->colorPointer -= D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		pointer->colorPointer += D3DXCOLOR(0.0f, TEXTURE_POINTER_CHANGE_COLLAR, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		if (pointer->colorPointer.r <= 0.0f)
		{
			pointer->colorPointer.r = 0.0f;
			pointer->nCollarFlag++;
		}
		if (pointer->colorPointer.g >= 1.0f)
		{
			pointer->colorPointer.g = 1.0f;
		}
		if (pointer->colorPointer.b >= 1.0f)
		{
			pointer->colorPointer.b = 1.0f;
		}
		break;
	case 1:
		pointer->colorPointer -= D3DXCOLOR(0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f);
		pointer->colorPointer += D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		if (pointer->colorPointer.r >= 1.0f)
		{
			pointer->colorPointer.r = 1.0f;
		}
		if (pointer->colorPointer.g <= 0.0f)
		{
			pointer->colorPointer.g = 0.0f;
			pointer->nCollarFlag++;
		}
		if (pointer->colorPointer.b >= 1.0f)
		{
			pointer->colorPointer.b = 1.0f;
		}
		break;
	case 2:
		pointer->colorPointer -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f);
		pointer->colorPointer += D3DXCOLOR(TEXTURE_POINTER_CHANGE_COLLAR, TEXTURE_POINTER_CHANGE_COLLAR, 0.0f, 0.0f);
		if (pointer->colorPointer.r >= 1.0f)
		{
			pointer->colorPointer.r = 1.0f;
		}
		if (pointer->colorPointer.g >= 1.0f)
		{
			pointer->colorPointer.g = 1.0f;
		}
		if (pointer->colorPointer.b <= 0.0f)
		{
			pointer->colorPointer.b = 0.0f;
			pointer->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// 有効設定
//=============================================================================
int SetPointer(D3DXCOLOR colorfloat)
{
	POINTER *pointer = &pointerWk[0];
	for (int i = 0; i < POINTER_MAX; i++, pointer++)
	{	// 未使用を探す
		if (!pointer->bUse)
		{	// 有効化処理
			pointer->bUse = true;
			SetDiffusePointer(i, colorfloat);	// カラー適用処理
			return i;
		}
	}
}

//=============================================================================
// 更新設定
//=============================================================================
void SetUpdatePointer(int nPointer, int nTex, D3DXVECTOR3 pos, float fAngle)
{
	POINTER *pointer = &pointerWk[nPointer];

	// 各情報の更新
	pointer->pos = pos;
	pointer->rot.z = fAngle;
	pointer->nTex = nTex;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleasePointer(int nPointer)
{
	POINTER *pointer = &pointerWk[nPointer];
	pointer->bUse = false;
}

//=============================================================================
// POINTER取得関数
//=============================================================================
POINTER *GetPointer(int no)
{
	return(&pointerWk[no]);
}

