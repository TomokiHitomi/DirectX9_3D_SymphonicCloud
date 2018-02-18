//=============================================================================
//
// RANKING処理 [ranking.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "magic.h"
#include "rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexRanking(int no);

void SetVertexRanking(int no);
void SetDiffuseRanking(int no, D3DXCOLOR color);
void SetTextureRanking(int no, int cntPattern);
void SetChangeCollarRanking(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
RANKING					rankingWk[RANKING_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureRanking[RANKING_TEXTURE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRanking(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[0];

	if (type == 0)
	{	
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RANKINGBACK,				// ファイルの名前
			&pD3DTextureRanking[RANKING_BACK]);			// 読み込むメモリのポインタ
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RANKINGFREAM,				// ファイルの名前
			&pD3DTextureRanking[RANKING_FREAM]);			// 読み込むメモリのポインタ
	}

	// RANKINGの初期化
	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{

		switch (i)
		{
		case RANKING_BACK:
			ranking->Texture = pD3DTextureRanking[RANKING_BACK];	// テクスチャへのRANKING
			ranking->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, TEXTURE_RANKING_A);
			break;
		case RANKING_FREAM:
			ranking->Texture = pD3DTextureRanking[RANKING_FREAM];	// テクスチャへのRANKING
			ranking->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
		ranking->bUse = true;
		ranking->bPatternFlag = false;
		ranking->nPatternAnim = 0;
		ranking->nCollarFlag = 0;
		ranking->pos = D3DXVECTOR3(TEXTURE_RANKING_POS_X, TEXTURE_RANKING_POS_Y, 0.0f);
		ranking->size = D3DXVECTOR2(TEXTURE_RANKING_SIZE_X, TEXTURE_RANKING_SIZE_Y);
		ranking->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点情報の作成
		MakeVertexRanking(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	for (int i = 0; i < RANKING_TEXTURE_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureRanking[i] != NULL)
		{
			pD3DTextureRanking[i]->Release();
			pD3DTextureRanking[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	RANKING *ranking = &rankingWk[0];
	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{
		if (ranking->bUse)
		{
			if (i == RANKING_FREAM)
			{
				//SetChangeCollarRanking(i);
				SetDiffuseRanking(i, GetRankColor());
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RANKING_MAX; i++, ranking++)
	{
		if (ranking->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, ranking->Texture);

			// RANKINGの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RANKING, ranking->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRanking(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RANKING *ranking = &rankingWk[no];

	// 頂点座標の設定
	SetVertexRanking(no);

	// rhwの設定
	ranking->vertexWk[0].rhw =
	ranking->vertexWk[1].rhw =
	ranking->vertexWk[2].rhw =
	ranking->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	ranking->vertexWk[0].diffuse =
	ranking->vertexWk[1].diffuse =
	ranking->vertexWk[2].diffuse =
	ranking->vertexWk[3].diffuse = ranking->xColor;


	if (ranking->bPatternFlag)
	{
		SetTextureRanking(no, no - 1);
	}
	else
	{
		// テクスチャ座標の設定
		ranking->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		ranking->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		ranking->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		ranking->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexRanking(int no)
{
	RANKING *ranking = &rankingWk[no];

	ranking->vertexWk[0].vtx.x = ranking->pos.x - ranking->size.x;
	ranking->vertexWk[0].vtx.y = ranking->pos.y - ranking->size.y;
	ranking->vertexWk[0].vtx.z = 0.0f;

	ranking->vertexWk[1].vtx.x = ranking->pos.x + ranking->size.x;
	ranking->vertexWk[1].vtx.y = ranking->pos.y - ranking->size.y;
	ranking->vertexWk[1].vtx.z = 0.0f;

	ranking->vertexWk[2].vtx.x = ranking->pos.x - ranking->size.x;
	ranking->vertexWk[2].vtx.y = ranking->pos.y + ranking->size.y;
	ranking->vertexWk[2].vtx.z = 0.0f;

	ranking->vertexWk[3].vtx.x = ranking->pos.x + ranking->size.x;
	ranking->vertexWk[3].vtx.y = ranking->pos.y + ranking->size.y;
	ranking->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseRanking(int no, D3DXCOLOR color)
{
	RANKING *ranking = &rankingWk[no];

	// 反射光の設定
	ranking->vertexWk[0].diffuse =
		ranking->vertexWk[1].diffuse =
		ranking->vertexWk[2].diffuse =
		ranking->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureRanking(int no, int cntPattern)
{
	RANKING *ranking = &rankingWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RANKING;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RANKING;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RANKING;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RANKING;
	ranking->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ranking->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ranking->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ranking->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarRanking(int no)
{
	RANKING *ranking = &rankingWk[no];

	switch (ranking->nCollarFlag)
	{
	case 0:
		ranking->xColor -= D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		ranking->xColor += D3DXCOLOR(0.0f, TEXTURE_RANKING_CHANGE_COLLAR, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		if (ranking->xColor.r <= 0.0f)
		{
			ranking->xColor.r = 0.0f;
			ranking->nCollarFlag++;
		}
		if (ranking->xColor.g >= 1.0f)
		{
			ranking->xColor.g = 1.0f;
		}
		if (ranking->xColor.b >= 1.0f)
		{
			ranking->xColor.b = 1.0f;
		}
		break;
	case 1:
		ranking->xColor -= D3DXCOLOR(0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f);
		ranking->xColor += D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		if (ranking->xColor.r >= 1.0f)
		{
			ranking->xColor.r = 1.0f;
		}
		if (ranking->xColor.g <= 0.0f)
		{
			ranking->xColor.g = 0.0f;
			ranking->nCollarFlag++;
		}
		if (ranking->xColor.b >= 1.0f)
		{
			ranking->xColor.b = 1.0f;
		}
		break;
	case 2:
		ranking->xColor -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f);
		ranking->xColor += D3DXCOLOR(TEXTURE_RANKING_CHANGE_COLLAR, TEXTURE_RANKING_CHANGE_COLLAR, 0.0f, 0.0f);
		if (ranking->xColor.r >= 1.0f)
		{
			ranking->xColor.r = 1.0f;
		}
		if (ranking->xColor.g >= 1.0f)
		{
			ranking->xColor.g = 1.0f;
		}
		if (ranking->xColor.b <= 0.0f)
		{
			ranking->xColor.b = 0.0f;
			ranking->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// カラー取得関数
//=============================================================================
D3DXCOLOR GetRankingColor(void)
{
	RANKING *ranking = &rankingWk[RANKING_FREAM];
	return(ranking->xColor);
}


//=============================================================================
// 有効設定
//=============================================================================
void SetRanking(int nGage)
{
	RANKING *ranking = &rankingWk[nGage];
	ranking->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseRanking(void)
{
	RANKING *ranking = &rankingWk[0];
	ranking->bUse = false;
}

//=============================================================================
// RANKING取得関数
//=============================================================================
RANKING *GetRanking(int no)
{
	return(&rankingWk[no]);
}

