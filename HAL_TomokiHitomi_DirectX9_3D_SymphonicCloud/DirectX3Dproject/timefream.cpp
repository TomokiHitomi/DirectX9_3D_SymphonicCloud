//=============================================================================
//
// TIMEFREAM処理 [timefream.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "timefream.h"
#include "main.h"
#include "magic.h"
#include "ranking.h"
#include "stage.h"
#include "rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimefream(int no);

void SetVertexTimefream(int no);
void SetDiffuseTimefream(int no, D3DXCOLOR color);
void SetTextureTimefream(int no, int cntPattern);
void SetChangeCollarTimefream(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TIMEFREAM					timefreamWk[TIMEFREAM_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureTimefream[TIMEFREAM_TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimefream(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIMEFREAMBACK,				// ファイルの名前
			&pD3DTextureTimefream[TIMEFREAM_BACK]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIMEFREAMFREAM,				// ファイルの名前
			&pD3DTextureTimefream[TIMEFREAM_FREAM]);			// 読み込むメモリのポインタ
	}

	// TIMEFREAMの初期化
	for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
	{

		switch (i)
		{
		case TIMEFREAM_BACK:
			timefream->Texture = pD3DTextureTimefream[TIMEFREAM_BACK];	// テクスチャへのTIMEFREAM
			timefream->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, TEXTURE_TIMEFREAM_A);
			break;
		case TIMEFREAM_FREAM:
			timefream->Texture = pD3DTextureTimefream[TIMEFREAM_FREAM];	// テクスチャへのTIMEFREAM
			timefream->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		timefream->bUse = true;
		timefream->bPatternFlag = false;
		timefream->nPatternAnim = 0;
		timefream->nCollarFlag = 0;
		timefream->pos = D3DXVECTOR3(TEXTURE_TIMEFREAM_POS_X, TEXTURE_TIMEFREAM_POS_Y, 0.0f);
		timefream->size = D3DXVECTOR2(TEXTURE_TIMEFREAM_SIZE_X, TEXTURE_TIMEFREAM_SIZE_Y);
		timefream->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点情報の作成
		MakeVertexTimefream(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimefream(void)
{
	for (int i = 0; i < TIMEFREAM_TEXTURE_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureTimefream[i] != NULL)
		{
			pD3DTextureTimefream[i]->Release();
			pD3DTextureTimefream[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimefream(void)
{
	TIMEFREAM *timefream = &timefreamWk[0];
	for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
	{
		if (timefream->bUse)
		{
			if (i == TIMEFREAM_FREAM)
			{
				if (GetStage() == STAGE_GAME)
				{
					SetDiffuseTimefream(i, GetMagicColor());
				}
				else
				{
					SetDiffuseTimefream(i, GetRankColor());
				}			
			}
			else
			{
				SetDiffuseTimefream(i, timefream->xColor);
			}
			if (GetStage() == STAGE_RESULT)
			{
				timefream->pos.y = TEXTURE_TIMEFREAM_RESULT_POS_Y;
				SetVertexTimefream(i);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimefream(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[0];
	if (GetStage() == STAGE_RESULT && !GetClearFlag())
	{

	}
	else
	{
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < TIMEFREAM_MAX; i++, timefream++)
		{
			if (timefream->bUse)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, timefream->Texture);

				// TIMEFREAMの描画
				pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIMEFREAM, timefream->vertexWk, sizeof(VERTEX_2D));
			}
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimefream(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMEFREAM *timefream = &timefreamWk[no];

	// 頂点座標の設定
	SetVertexTimefream(no);

	// rhwの設定
	timefream->vertexWk[0].rhw =
	timefream->vertexWk[1].rhw =
	timefream->vertexWk[2].rhw =
	timefream->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	timefream->vertexWk[0].diffuse =
	timefream->vertexWk[1].diffuse =
	timefream->vertexWk[2].diffuse =
	timefream->vertexWk[3].diffuse = timefream->xColor;


	if (timefream->bPatternFlag)
	{
		SetTextureTimefream(no, no - 1);
	}
	else
	{
		// テクスチャ座標の設定
		timefream->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		timefream->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		timefream->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		timefream->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexTimefream(int no)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	timefream->vertexWk[0].vtx.x = timefream->pos.x - timefream->size.x;
	timefream->vertexWk[0].vtx.y = timefream->pos.y - timefream->size.y;
	timefream->vertexWk[0].vtx.z = 0.0f;

	timefream->vertexWk[1].vtx.x = timefream->pos.x + timefream->size.x;
	timefream->vertexWk[1].vtx.y = timefream->pos.y - timefream->size.y;
	timefream->vertexWk[1].vtx.z = 0.0f;

	timefream->vertexWk[2].vtx.x = timefream->pos.x - timefream->size.x;
	timefream->vertexWk[2].vtx.y = timefream->pos.y + timefream->size.y;
	timefream->vertexWk[2].vtx.z = 0.0f;

	timefream->vertexWk[3].vtx.x = timefream->pos.x + timefream->size.x;
	timefream->vertexWk[3].vtx.y = timefream->pos.y + timefream->size.y;
	timefream->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTimefream(int no, D3DXCOLOR color)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	// 反射光の設定
	timefream->vertexWk[0].diffuse =
		timefream->vertexWk[1].diffuse =
		timefream->vertexWk[2].diffuse =
		timefream->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTimefream(int no, int cntPattern)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM;
	timefream->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timefream->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timefream->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timefream->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarTimefream(int no)
{
	TIMEFREAM *timefream = &timefreamWk[no];

	switch (timefream->nCollarFlag)
	{
	case 0:
		timefream->xColor -= D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		timefream->xColor += D3DXCOLOR(0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		if (timefream->xColor.r <= 0.0f)
		{
			timefream->xColor.r = 0.0f;
			timefream->nCollarFlag++;
		}
		if (timefream->xColor.g >= 1.0f)
		{
			timefream->xColor.g = 1.0f;
		}
		if (timefream->xColor.b >= 1.0f)
		{
			timefream->xColor.b = 1.0f;
		}
		break;
	case 1:
		timefream->xColor -= D3DXCOLOR(0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f);
		timefream->xColor += D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		if (timefream->xColor.r >= 1.0f)
		{
			timefream->xColor.r = 1.0f;
		}
		if (timefream->xColor.g <= 0.0f)
		{
			timefream->xColor.g = 0.0f;
			timefream->nCollarFlag++;
		}
		if (timefream->xColor.b >= 1.0f)
		{
			timefream->xColor.b = 1.0f;
		}
		break;
	case 2:
		timefream->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f);
		timefream->xColor += D3DXCOLOR(TEXTURE_TIMEFREAM_CHANGE_COLLAR, TEXTURE_TIMEFREAM_CHANGE_COLLAR, 0.0f, 0.0f);
		if (timefream->xColor.r >= 1.0f)
		{
			timefream->xColor.r = 1.0f;
		}
		if (timefream->xColor.g >= 1.0f)
		{
			timefream->xColor.g = 1.0f;
		}
		if (timefream->xColor.b <= 0.0f)
		{
			timefream->xColor.b = 0.0f;
			timefream->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// 有効設定
//=============================================================================
void SetTimefream(int nGage)
{
	TIMEFREAM *timefream = &timefreamWk[nGage];
	timefream->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseTimefream(void)
{
	TIMEFREAM *timefream = &timefreamWk[0];
	timefream->bUse = false;
}

//=============================================================================
// TIMEFREAM取得関数
//=============================================================================
TIMEFREAM *GetTimefream(int no)
{
	return(&timefreamWk[no]);
}

