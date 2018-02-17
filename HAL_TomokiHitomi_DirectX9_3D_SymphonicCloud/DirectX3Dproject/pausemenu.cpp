//=============================================================================
//
// タイトル処理 [pausemenu.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "pausemenu.h"
#include "input.h"
#include "sound.h"
#include "pause.h"
#include "fade.h"
#include "calculate.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPausemenu(int no);
void SetTexturePausemenu( int no, int cntPattern );	//
void SetVertexPausemenu(int no);
void SetDiffusePausemenu(int no, D3DXCOLOR color);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
PAUSEMENU					pausemenuWk[PAUSEMENU_MAX];
PAUSEMENU_SYS				pausemenusysWk;

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTexturePausemenu = NULL;

int g_nSelectPauseMenu;
int g_nSelectPauseMenuCount;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPausemenu(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[0];
	PAUSEMENU_SYS *pausemenusys = &pausemenusysWk;

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PAUSEMENU00,				// ファイルの名前
			&pD3DTexturePausemenu);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化
	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		pausemenu->bUse = false;
		pausemenu->pos = D3DXVECTOR3((float)PAUSEMENU_POS_X, i*(float)PAUSEMENU_SPACING + (float)PAUSEMENU_POS_Y, 0.0f);
		pausemenu->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pausemenu->nCountAnim = 0;
		pausemenu->nPatternAnim = 0;

		pausemenu->Texture = pD3DTexturePausemenu;			// テクスチャへのエネミー
		pausemenu->size = D3DXVECTOR2(TEXTURE_PAUSEMENU00_SIZE_X, TEXTURE_PAUSEMENU00_SIZE_Y);

		// 頂点情報の作成
		MakeVertexPausemenu(i);
	}

	pausemenusys->bSelect = false;
	g_nSelectPauseMenu = 0;
	g_nSelectPauseMenuCount = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPausemenu(void)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	// メモリ解放
	if (pD3DTexturePausemenu != NULL)
	{
		pD3DTexturePausemenu->Release();
		pD3DTexturePausemenu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePausemenu(void)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];
	PAUSEMENU_SYS *pausemenusys = &pausemenusysWk;
	PAUSE *pause = GetPause(0);

	// ポーズが有効、かつ決定前は処理
	if (pause->bUse && !pausemenusys->bSelect)
	{
		// 上移動
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, LSTICK_UP))
		{
			g_nSelectPauseMenu--;
			if (g_nSelectPauseMenu < 0)
			{
				g_nSelectPauseMenu += PAUSEMENU_MAX;
			}
			SetSe(1, E_DS8_FLAG_NONE, true);
		}
		// 下移動
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, LSTICK_DOWN))
		{
			g_nSelectPauseMenu++;
			SetSe(1, E_DS8_FLAG_NONE, true);
		}

		// 上移動長押し
		if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{
			g_nSelectPauseMenuCount++;
			if (g_nSelectPauseMenuCount > PAUSEMENU_SELECT_PRESS && g_nSelectPauseMenuCount % PAUSEMENU_SELECT_SPEED == 0)
			{
				g_nSelectPauseMenu--;
				if (g_nSelectPauseMenu < 0)
				{
					g_nSelectPauseMenu += PAUSEMENU_MAX;
				}
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		// 下移動長押し
		else if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{
			g_nSelectPauseMenuCount++;
			if (g_nSelectPauseMenuCount > PAUSEMENU_SELECT_PRESS && g_nSelectPauseMenuCount  % PAUSEMENU_SELECT_SPEED == 0)
			{
				g_nSelectPauseMenu++;
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		else
		{
			g_nSelectPauseMenuCount = 0;
		}


		for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
		{
			if (pausemenu->bUse)
			{
				if (i == g_nSelectPauseMenu % PAUSEMENU_MAX)
				{
					SetDiffusePausemenu(i, SetColorPallet(COLOR_PALLET_LIGHTBLUE));
				}
				else
				{
					SetDiffusePausemenu(i, SetColorPallet(COLOR_PALLET_WHITE));
				}
				pausemenu->nPatternAnim = i;
				// テクスチャ座標を設定
				SetTexturePausemenu(i, pausemenu->nPatternAnim);
				// 移動後の座標で頂点を設定
				SetVertexPausemenu(i);

			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || IsButtonTriggered(0, R_BUTTON_A))
		{
			switch (g_nSelectPauseMenu % PAUSEMENU_MAX)
			{
			case 0:
				pause->bUse = false;
				pause->bStop = false;
				SetFade(FADE_IN, STAGE_MAX);
				SetPausemenu(false);
				break;
			case 1:
				SetFade(FADE_OUT, STAGE_GAME);
				pausemenusys->bSelect = true;	// 決定後は処理を停止
				break;
			case 2:
				SetFade(FADE_OUT, STAGE_TITLE);
				pausemenusys->bSelect = true;	// 決定後は処理を停止
				break;
			}
			SetSe(2, E_DS8_FLAG_NONE, true);
		}

		if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, R_BUTTON_B))
		{
			g_nSelectPauseMenu = 0;
		}
	}
}

//=============================================================================
// 描画処理1
//=============================================================================
void DrawPausemenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		if (pausemenu->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pausemenu->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSEMENU, pausemenu->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPausemenu(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// 頂点座標の設定
	pausemenu->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pausemenu->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSEMENU00_SIZE_X, 100.0f, 0.0f);
	pausemenu->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_PAUSEMENU00_SIZE_Y, 0.0f);
	pausemenu->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSEMENU00_SIZE_X, 100.0f+TEXTURE_PAUSEMENU00_SIZE_Y, 0.0f);
	//SetVertexPausemenu();

	// rhwの設定
	pausemenu->vertexWk[0].rhw =
	pausemenu->vertexWk[1].rhw =
	pausemenu->vertexWk[2].rhw =
	pausemenu->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	pausemenu->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pausemenu->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pausemenu->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU, 0.0f );
	pausemenu->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU );
	pausemenu->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePausemenu( int no, int cntPattern )
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU;
	pausemenu->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	pausemenu->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	pausemenu->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	pausemenu->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexPausemenu(int no)
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	pausemenu->vertexWk[0].vtx.x = pausemenu->pos.x - TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[0].vtx.y = pausemenu->pos.y - TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[0].vtx.z = 0.0f;

	pausemenu->vertexWk[1].vtx.x = pausemenu->pos.x + TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[1].vtx.y = pausemenu->pos.y - TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[1].vtx.z = 0.0f;

	pausemenu->vertexWk[2].vtx.x = pausemenu->pos.x - TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[2].vtx.y = pausemenu->pos.y + TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[2].vtx.z = 0.0f;

	pausemenu->vertexWk[3].vtx.x = pausemenu->pos.x + TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[3].vtx.y = pausemenu->pos.y + TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffusePausemenu(int no, D3DXCOLOR color)
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// 反射光の設定
	pausemenu->vertexWk[0].diffuse =
		pausemenu->vertexWk[1].diffuse =
		pausemenu->vertexWk[2].diffuse =
		pausemenu->vertexWk[3].diffuse = color;
}

//=============================================================================
// ポーズメニュー呼出処理
//=============================================================================
void SetPausemenu(bool bUse)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		// ポーズメニューを有効にする
		pausemenu->bUse = bUse;
	}
	return;
}

//=============================================================================
// タイトル取得関数
//=============================================================================
PAUSEMENU *GetPausemenu(int no)
{
	return(&pausemenuWk[no]);
}

