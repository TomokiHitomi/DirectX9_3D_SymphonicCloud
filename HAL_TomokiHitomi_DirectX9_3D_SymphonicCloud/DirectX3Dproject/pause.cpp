//=============================================================================
//
// タイトル処理 [pause.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pausemenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(int no);
void SetTexturePause( int no, int cntPattern );	//
void SetVertexPause(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
PAUSE					pauseWk[PAUSE_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTexturePause = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PAUSE00,				// ファイルの名前
			&pD3DTexturePause);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		pause->bUse = false;
		pause->bStop = false;
		pause->pos = D3DXVECTOR3((float)PAUSE_POS_X, (float)PAUSE_POS_Y, 0.0f);
		pause->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pause->nCountAnim = 0;
		pause->nPatternAnim = 0;

		pause->Texture = pD3DTexturePause;			// テクスチャへのエネミー
		pause->size = D3DXVECTOR2(TEXTURE_PAUSE00_SIZE_X, TEXTURE_PAUSE00_SIZE_Y);

		// 頂点情報の作成
		MakeVertexPause(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	PAUSE *pause = &pauseWk[0];

	// メモリ解放
	if (pD3DTexturePause != NULL)
	{
		pD3DTexturePause->Release();
		pD3DTexturePause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	PAUSE *pause = &pauseWk[0];
	PAUSEMENU *pausemenu = GetPausemenu(0);

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		if (pause->bUse)
		{
			SetSoundBgm(SOUND_BGM_GAME_PAUSE);
			// アニメーション
			pause->nCountAnim++;
			if ((pause->nCountAnim % TIME_ANIMATION_PAUSE) == 0)
			{
				// パターンの切り替え
				pause->nPatternAnim = (pause->nPatternAnim + 1) % ANIM_PATTERN_NUM_PAUSE;
			}

			if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_HOME))
			{
				pause->bUse = false;
				pause->bStop = false;
				SetPausemenu(false);
				SetSe(2, E_DS8_FLAG_NONE, true);
				SetFade(FADE_IN, STAGE_MAX);
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_HOME))
			{
				pause->bUse = true;
				pause->bStop = true;
				SetPausemenu(true);
				SetSe(3, E_DS8_FLAG_NONE, true);
				SetFade(FADE_OUT_HALF, STAGE_MAX);
				// テクスチャ座標を設定
				SetTexturePause(i, pause->nPatternAnim);
				// 移動後の座標で頂点を設定
				SetVertexPause(i);
			}
		}
		// テクスチャ座標を設定
		SetTexturePause(i, pause->nPatternAnim);
		// 移動後の座標で頂点を設定
		SetVertexPause(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		if (pause->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pause->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSE, pause->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPause(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[no];

	// 頂点座標の設定
	pause->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pause->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSE00_SIZE_X, 100.0f, 0.0f);
	pause->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_PAUSE00_SIZE_Y, 0.0f);
	pause->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSE00_SIZE_X, 100.0f+TEXTURE_PAUSE00_SIZE_Y, 0.0f);
	//SetVertexPause();

	// rhwの設定
	pause->vertexWk[0].rhw =
	pause->vertexWk[1].rhw =
	pause->vertexWk[2].rhw =
	pause->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	pause->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pause->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pause->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSE, 0.0f );
	pause->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSE );
	pause->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSE, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSE );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePause( int no, int cntPattern )
{
	PAUSE *pause = &pauseWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PAUSE;
	pause->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	pause->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	pause->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	pause->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexPause(int no)
{
	PAUSE *pause = &pauseWk[no];

	pause->vertexWk[0].vtx.x = pause->pos.x - TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[0].vtx.y = pause->pos.y - TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[0].vtx.z = 0.0f;

	pause->vertexWk[1].vtx.x = pause->pos.x + TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[1].vtx.y = pause->pos.y - TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[1].vtx.z = 0.0f;

	pause->vertexWk[2].vtx.x = pause->pos.x - TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[2].vtx.y = pause->pos.y + TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[2].vtx.z = 0.0f;

	pause->vertexWk[3].vtx.x = pause->pos.x + TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[3].vtx.y = pause->pos.y + TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ポーズフラグ取得関数（0）を渡す
//=============================================================================
bool GetPauseFlag(void)
{
	PAUSE *pause = &pauseWk[0];
	return (pause->bUse);
}

//=============================================================================
// 取得関数
//=============================================================================
PAUSE *GetPause(int no)
{
	return(&pauseWk[no]);
}

