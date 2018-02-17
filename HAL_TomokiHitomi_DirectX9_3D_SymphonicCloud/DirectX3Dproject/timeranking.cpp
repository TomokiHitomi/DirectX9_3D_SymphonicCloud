//=============================================================================
//
// TIMERANKING処理 [timeranking.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "timeranking.h"
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
HRESULT MakeVertexTimeranking(int nNum);
void SetDiffuseTimeranking(int nNum,D3DXCOLOR color);
void SetTextureTimeranking(int nNum);
void SetTimerankingFile(void);
void CreateTimerankingFile(void);
void UpdateTimerankingFile(FILE *pfp);
void WriteTimerankingFile(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTimeranking = NULL;					// テクスチャへのポリゴン
TIMERANKING				timerankingWk[TIMERANKING_MAX];
int						g_nTimeranking[TIMERANKING_MAX] = {	TIMERANKING_NUM1,
															TIMERANKING_NUM2,
															TIMERANKING_NUM3,
															TIMERANKING_NUM4,
															TIMERANKING_NUM5 };
int						g_nTimerankingFp[TIMERANKING_MAX] = {0, 0, 0, 0, 0};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimeranking(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMERANKING *timeranking = &timerankingWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIMERANKING,				// ファイルの名前
			&g_pD3DTextureTimeranking);				// 読み込むメモリのポインタ
	}

	if (GetStage() == STAGE_RESULT)
	{
		SetTimerankingFile();
		for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
		{
			timeranking->posTimeranking = D3DXVECTOR3(
				(float)TIMERANKING_POS_X,
				(float)TIMERANKING_POS_Y + (float)TEXTURE_TIMERANKING_INTERVAL_Y * i,
				0.0f);
			timeranking->nTimeranking = g_nTimerankingFp[i];
			// 頂点情報の作成
			MakeVertexTimeranking(i);
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimeranking(void)
{
	if (g_pD3DTextureTimeranking != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTimeranking->Release();
		g_pD3DTextureTimeranking = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimeranking(void)
{
	TIMERANKING *timeranking = &timerankingWk[0];
	RESULT *result = GetResult(0);
	GAME_SYS *GetGameSys();

	for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
	{
		if (GetStage() == STAGE_GAME)
		{
			int nCount = GetGameSysCount();
			SetTimeranking(i, nCount / TIMERANKING_SECOND);
			SetDiffuseTimeranking(i, GetMagicColor());
		}
		else if (GetStage() == STAGE_RESULT)
		{
			SetDiffuseTimeranking(i , GetRankColor());
		}
		//if (g_bTimerankingMoveFlag && !result->bPosMoveFlag)
		//{
		//	g_posTimeranking += result->vecPosMove;
		//	MakeVertexTimeranking();
		//	g_bTimerankingMoveFlag = false;
		//}
		SetTextureTimeranking(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimeranking(void)
{
	TIMERANKING *timeranking = &timerankingWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	for (int i = 0; i < TIMERANKING_MAX; i++, timeranking++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureTimeranking);

		// タイム
		for (int j = 0; j < TIMERANKING_DIGIT; j++)
		{
			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TIMERANKING, timeranking->vertexWkTimeranking[j], sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimeranking(int nNum)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	float fWidth = TEXTURE_TIMERANKING_SPAN;	// 数字の横幅
	int nTimerankingInterval = 0;

	// 桁数分処理する
	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		if (i % 2 == 0 && i != 0)
		{
			nTimerankingInterval++;
		}
		// 頂点座標の設定
		timeranking->vertexWkTimeranking[i][0].vtx.x = -fWidth * i + timeranking->posTimeranking.x - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][0].vtx.y = timeranking->posTimeranking.y;
		timeranking->vertexWkTimeranking[i][0].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][1].vtx.x = -fWidth * i + timeranking->posTimeranking.x + TEXTURE_TIMERANKING_SIZE_X - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][1].vtx.y = timeranking->posTimeranking.y;
		timeranking->vertexWkTimeranking[i][1].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][2].vtx.x = -fWidth * i + timeranking->posTimeranking.x - (TEXTURE_TIMERANKING_INTERVAL_X* nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][2].vtx.y = timeranking->posTimeranking.y + TEXTURE_TIMERANKING_SIZE_Y;
		timeranking->vertexWkTimeranking[i][2].vtx.z = 0.0f;
		timeranking->vertexWkTimeranking[i][3].vtx.x = -fWidth * i + timeranking->posTimeranking.x + TEXTURE_TIMERANKING_SIZE_X - (TEXTURE_TIMERANKING_INTERVAL_X * nTimerankingInterval);
		timeranking->vertexWkTimeranking[i][3].vtx.y = timeranking->posTimeranking.y + TEXTURE_TIMERANKING_SIZE_Y;
		timeranking->vertexWkTimeranking[i][3].vtx.z = 0.0f;

		// rhwの設定
		timeranking->vertexWkTimeranking[i][0].rhw =
		timeranking->vertexWkTimeranking[i][1].rhw =
		timeranking->vertexWkTimeranking[i][2].rhw =
		timeranking->vertexWkTimeranking[i][3].rhw = 1.0f;

		// 反射光の設定
		timeranking->vertexWkTimeranking[i][0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		timeranking->vertexWkTimeranking[i][3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		timeranking->vertexWkTimeranking[i][0].tex = D3DXVECTOR2(0.0f, 0.0f);
		timeranking->vertexWkTimeranking[i][1].tex = D3DXVECTOR2(1.0f, 0.0f);
		timeranking->vertexWkTimeranking[i][2].tex = D3DXVECTOR2(0.0f, 1.0f);
		timeranking->vertexWkTimeranking[i][3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	return S_OK;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTimeranking(int nNum,D3DXCOLOR color)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	// 桁数分処理する
	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		// 反射光の設定
		timeranking->vertexWkTimeranking[i][0].diffuse =
		timeranking->vertexWkTimeranking[i][1].diffuse =
		timeranking->vertexWkTimeranking[i][2].diffuse =
		timeranking->vertexWkTimeranking[i][3].diffuse = color;
	}
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetTextureTimeranking(int nNum)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	int number = timeranking->nTimeranking;

	for (int i = 0; i < TIMERANKING_DIGIT; i++)
	{
		// テクスチャ座標の設定
		float x = (float)(number % 10);
		timeranking->vertexWkTimeranking[i][0].tex = D3DXVECTOR2(0.1f * x, 0.0f);
		timeranking->vertexWkTimeranking[i][1].tex = D3DXVECTOR2(0.1f * (x + 1), 0.0f);
		timeranking->vertexWkTimeranking[i][2].tex = D3DXVECTOR2(0.1f * x, 1.0f);
		timeranking->vertexWkTimeranking[i][3].tex = D3DXVECTOR2(0.1f * (x + 1), 1.0f);
		number /= 10;
	}

}


//=============================================================================
// タイムの加算処理
//=============================================================================
void SetTimeranking(int nNum,int nTimeranking)
{
	TIMERANKING *timeranking = &timerankingWk[nNum];
	//CLOSE *close = GetClose(0);
	//RESULT *result = GetResult(0);

	//if (close->bUse)
	//{
	//	g_bTimerankingStop = true;
	//}
	//if (!g_bTimerankingStop)
	//{

	timeranking->nTimeranking = nTimeranking / TIMERANKING_SECOND * 100;
	timeranking->nTimeranking += nTimeranking % TIMERANKING_SECOND;

	if (timeranking->nTimeranking > TIMERANKING_SECOND_MAX)
	{
		timeranking->nTimeranking = TIMERANKING_SECOND_MAX;
	}
	else if (timeranking->nTimeranking < 0)
	{
		timeranking->nTimeranking = 0;
	}
	SetResultTime(timeranking->nTimeranking);
	//}
}

//=============================================================================
// タイムのファイル初期化処理
//=============================================================================
void SetTimerankingFile(void)
{
	FILE *fp = fopen(TIMERANKING_NUM_FILE, "rb");
	
	if (fp == NULL)				// ファイルオープンエラーはファイル生成
	{
		CreateTimerankingFile();
		fp = fopen(TIMERANKING_NUM_FILE, "rb");
		if (fp == NULL)				// ファイルオープンエラーはリターン
		{
			return;
		}
		else
		{
			UpdateTimerankingFile(&*fp);
			fclose(fp);										// ファイル操作終了
			WriteTimerankingFile();
		}
	}
	else							// ファイルオープン
	{
		UpdateTimerankingFile(&*fp);	
		fclose(fp);										// ファイル操作終了
		WriteTimerankingFile();
	}
}

//=============================================================================
// タイムのファイル初期化処理
//=============================================================================
void CreateTimerankingFile(void)
{
	FILE *fpc = fopen(TIMERANKING_NUM_FILE, "wb");	// ファイル作成
	if (fpc == NULL)									// ファイルが作れたかチェック
	{
		return;
	}

	fprintf(fpc, "%d,%d,%d,%d,%d",
		g_nTimeranking[0], g_nTimeranking[1], g_nTimeranking[2], g_nTimeranking[3], g_nTimeranking[4]);

	fclose(fpc);										// ファイル操作終了
}

//=============================================================================
// タイムのファイル更新処理
//=============================================================================
void UpdateTimerankingFile(FILE *fp)
{
	int data[TIMERANKING_MAX + 1];

	fscanf(fp, "%d,%d,%d,%d,%d",					// ファイル読み出し
		&data[0], &data[1], &data[2], &data[3], &data[4]);
	if (GetClearFlag())
	{
		data[5] = GetResultTime();

		// バブルソート
		for (int i = 0; i < TIMERANKING_MAX; i++)
		{
			for (int j = TIMERANKING_MAX; j > i; j--)
			{
				if (data[j] < data[j - 1])
				{
					int nTemp = data[j];
					data[j] = data[j - 1];
					data[j - 1] = nTemp;
				}
			}
		}
	}

	for (int i = 0; i < TIMERANKING_MAX; i++)
	{
		g_nTimerankingFp[i] = data[i];
	}
}

//=============================================================================
// タイムのファイル初期化処理
//=============================================================================
void WriteTimerankingFile(void)
{
	if (GetClearFlag())
	{
		FILE *fpw = fopen(TIMERANKING_NUM_FILE, "wb");	// ファイル書き込み
		if (fpw == NULL)									// ファイルが開けたかチェック
		{
			return;
		}

		// ソートデータをファイルに格納
		fprintf(fpw, "%d,%d,%d,%d,%d",
			g_nTimerankingFp[0], g_nTimerankingFp[1], g_nTimerankingFp[2], g_nTimerankingFp[3], g_nTimerankingFp[4]);

		fclose(fpw);										// ファイル操作終了
	}
}