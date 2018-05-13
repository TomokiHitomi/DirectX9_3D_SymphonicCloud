//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(int no);
void SetVertexTitle(int no);
void SetDiffuseTitle(int no, float fAlpha);
void SetTextureTitle(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLE					titleWk[TITLE_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureTitle[TITLE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[0];

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TITLE_00,				// ファイルの名前
			&pD3DTextureTitle[0]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TITLE_01,				// ファイルの名前
			&pD3DTextureTitle[1]);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化
	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		title->bUse = true;
		title->bAlpha = true;
		title->pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		title->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		title->fAlpha = TITLE_ALPHA_MAX;

		title->Texture = pD3DTextureTitle[i];			// テクスチャへのエネミー
		title->size = D3DXVECTOR2(TEXTURE_TITLE_SIZE_X, TEXTURE_TITLE_SIZE_Y);

		// 頂点情報の作成
		MakeVertexTitle(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	TITLE *title = &titleWk[0];

	for (int i = 0; i < TITLE_MAX; i++)
	{
		// メモリ解放
		if (&pD3DTextureTitle[i] != NULL)
		{
			pD3DTextureTitle[i]->Release();
			pD3DTextureTitle[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	TITLE *title = &titleWk[0];

	SetSoundBgm(SOUND_BGM_TITLE);

	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		if (title->bUse)
		{
			// タイトルの外枠のα値を変更
			if (i == 0)
			{
				if (title->bAlpha)
				{
					title->fAlpha -= TITLE_ALPHA_CHANGE;
					if (title->fAlpha <= TITLE_ALPHA_MIN)
					{
						title->fAlpha = TITLE_ALPHA_MIN;
						title->bAlpha = false;
					}
				}
				else
				{
					title->fAlpha += TITLE_ALPHA_CHANGE;
					if (title->fAlpha >= TITLE_ALPHA_MAX)
					{
						title->fAlpha = TITLE_ALPHA_MAX;
						title->bAlpha = true;
					}
				}
				SetDiffuseTitle(i, title->fAlpha);
#ifdef _DEBUG
				PrintDebugProc("【 TITLE 】\n");
				PrintDebugProc("Alpha:%f\n", title->fAlpha);
#endif
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLE_MAX; i++, title++)
	{
		if (title->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, title->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLE, title->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLE *title = &titleWk[no];

	// 頂点座標の設定
	SetVertexTitle(no);

	// rhwの設定
	title->vertexWk[0].rhw =
	title->vertexWk[1].rhw =
	title->vertexWk[2].rhw =
	title->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	title->vertexWk[0].diffuse =
	title->vertexWk[1].diffuse =
	title->vertexWk[2].diffuse =
	title->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	title->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	title->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLE, 0.0f );
	title->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLE );
	title->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLE, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLE );

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexTitle(int no)
{
	TITLE *title = &titleWk[no];

	title->vertexWk[0].vtx.x = title->pos.x - TEXTURE_TITLE_SIZE_X;
	title->vertexWk[0].vtx.y = title->pos.y - TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[0].vtx.z = 0.0f;

	title->vertexWk[1].vtx.x = title->pos.x + TEXTURE_TITLE_SIZE_X;
	title->vertexWk[1].vtx.y = title->pos.y - TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[1].vtx.z = 0.0f;

	title->vertexWk[2].vtx.x = title->pos.x - TEXTURE_TITLE_SIZE_X;
	title->vertexWk[2].vtx.y = title->pos.y + TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[2].vtx.z = 0.0f;

	title->vertexWk[3].vtx.x = title->pos.x + TEXTURE_TITLE_SIZE_X;
	title->vertexWk[3].vtx.y = title->pos.y + TEXTURE_TITLE_SIZE_Y;
	title->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTitle(int no, float fAlpha)
{
	TITLE *title = &titleWk[no];

	// 反射光の設定
	title->vertexWk[0].diffuse =
	title->vertexWk[1].diffuse =
	title->vertexWk[2].diffuse =
	title->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f,1.0f, fAlpha);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTitle( int no, int cntPattern )
{
	TITLE *title = &titleWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TITLE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TITLE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TITLE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TITLE;
	title->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	title->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	title->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	title->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// タイトル取得関数
//=============================================================================
TITLE *GetTitle(int no)
{
	return(&titleWk[no]);
}

