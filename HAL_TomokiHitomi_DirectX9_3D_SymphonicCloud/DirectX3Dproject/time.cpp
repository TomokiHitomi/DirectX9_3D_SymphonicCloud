//=============================================================================
//
// TIME処理 [time.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "time.h"
#include "result.h"
#include "game.h"
#include "magic.h"
#include "stage.h"
#include "ranking.h"
#include "rank.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTime(void);
void SetDiffuseTime(D3DXCOLOR color);
void SetTextureTime(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTime = NULL;					// テクスチャへのポリゴン
VERTEX_2D				g_vertexWkTime[TIME_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク
D3DXVECTOR3				g_posTime;									// ポリゴンの移動量

int						g_nTime;									// タイム

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTime(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIME,				// ファイルの名前
			&g_pD3DTextureTime);				// 読み込むメモリのポインタ
	}

	if (GetStage() == STAGE_RESULT)
	{
		g_posTime = D3DXVECTOR3((float)TIME_POS_X, (float)TIME_RESULT_POS_Y, 0.0f);

		if (GetClearFlag())
		{
			g_nTime = GetResultTime();
		}
		else
		{
			g_nTime = TIME_MAX;
		}
	}
	else
	{
		g_posTime = D3DXVECTOR3((float)TIME_POS_X, (float)TIME_POS_Y, 0.0f);
		g_nTime = 0;
	}

	// 頂点情報の作成
	MakeVertexTime();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	if (g_pD3DTextureTime != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTime->Release();
		g_pD3DTextureTime = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	RESULT *result = GetResult(0);
	GAME_SYS *GetGameSys();

	if (GetStage() == STAGE_GAME)
	{
		int nCount = GetGameSysCount();
		SetTime(nCount / TIME_SECOND);
		SetDiffuseTime(GetMagicColor());
	}
	else if (GetStage() == STAGE_RESULT && GetClearFlag())
	{
		SetDiffuseTime(GetRankColor());
	}
	//if (g_bTimeMoveFlag && !result->bPosMoveFlag)
	//{
	//	g_posTime += result->vecPosMove;
	//	MakeVertexTime();
	//	g_bTimeMoveFlag = false;
	//}


	SetTextureTime();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (GetStage() == STAGE_RESULT && !GetClearFlag())
	{

	}
	else
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTime);

		// タイム
		for (int i = 0; i < TIME_DIGIT; i++)
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIME, g_vertexWkTime[i], sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTime(void)
{
	float fWidth = TEXTURE_TIME_SPAN;	// 数字の横幅
	int nTimeInterval = 0;

	// 桁数分処理する
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		if (i % 2 == 0 && i != 0)
		{
			nTimeInterval++;
		}
		// 頂点座標の設定
		g_vertexWkTime[i][0].vtx.x = -fWidth * i + g_posTime.x - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][0].vtx.y = g_posTime.y;
		g_vertexWkTime[i][0].vtx.z = 0.0f;
		g_vertexWkTime[i][1].vtx.x = -fWidth * i + g_posTime.x + TEXTURE_TIME_SIZE_X - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][1].vtx.y = g_posTime.y;
		g_vertexWkTime[i][1].vtx.z = 0.0f;
		g_vertexWkTime[i][2].vtx.x = -fWidth * i + g_posTime.x - (TEXTURE_TIME_INTERVAL* nTimeInterval);
		g_vertexWkTime[i][2].vtx.y = g_posTime.y + TEXTURE_TIME_SIZE_Y;
		g_vertexWkTime[i][2].vtx.z = 0.0f;
		g_vertexWkTime[i][3].vtx.x = -fWidth * i + g_posTime.x + TEXTURE_TIME_SIZE_X - (TEXTURE_TIME_INTERVAL * nTimeInterval);
		g_vertexWkTime[i][3].vtx.y = g_posTime.y + TEXTURE_TIME_SIZE_Y;
		g_vertexWkTime[i][3].vtx.z = 0.0f;

		// rhwの設定
		g_vertexWkTime[i][0].rhw =
		g_vertexWkTime[i][1].rhw =
		g_vertexWkTime[i][2].rhw =
		g_vertexWkTime[i][3].rhw = 1.0f;

		// 反射光の設定
		g_vertexWkTime[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_vertexWkTime[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		g_vertexWkTime[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_vertexWkTime[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_vertexWkTime[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_vertexWkTime[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTime(D3DXCOLOR color)
{
	// 桁数分処理する
	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// 反射光の設定
		g_vertexWkTime[i][0].diffuse =
			g_vertexWkTime[i][1].diffuse =
			g_vertexWkTime[i][2].diffuse =
			g_vertexWkTime[i][3].diffuse = color;
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureTime(void)
{
	int number = g_nTime;

	for (int i = 0; i < TIME_DIGIT; i++)
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		g_vertexWkTime[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		g_vertexWkTime[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		g_vertexWkTime[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		g_vertexWkTime[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}

}


//=============================================================================
// タイムの加算処理
//=============================================================================
void SetTime(int nTime)
{
	g_nTime = nTime / TIME_SECOND * 100;
	g_nTime += nTime % TIME_SECOND;

	if (g_nTime > TIME_MAX)
	{
		g_nTime = TIME_MAX;
	}
	else if (g_nTime < 0)
	{
		g_nTime = 0;
	}

	if (GetClearFlag())
	{
		SetResultTime(g_nTime);
	}
}