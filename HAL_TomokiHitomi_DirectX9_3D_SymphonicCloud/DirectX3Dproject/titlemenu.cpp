//=============================================================================
//
// タイトル処理 [titlemenu.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "titlemenu.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "calculate.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitlemenu(int no);
void SetTextureTitlemenu( int no, int cntPattern );
void SetVertexTitlemenu(int no);
void SetDiffuseTitlemenu(int no, D3DXCOLOR color);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLEMENU					titlemenuWk[TITLEMENU_MAX];
TITLEMENU_SYS				titlemenusysWk[TITLEMENU_SYS_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureTitlemenu = NULL;

int g_nSelectTitlemenu;
int g_nSelectTitlemenuCount;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitlemenu(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[0];
	TITLEMENU_SYS *titlemenusys = &titlemenusysWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TITLEMENU,				// ファイルの名前
			&pD3DTextureTitlemenu);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		titlemenu->bUse = true;
		titlemenu->pos = D3DXVECTOR3((float)TITLEMENU_POS_X, i*(float)TITLEMENU_SPACING + (float)TITLEMENU_POS_Y, 0.0f);
		titlemenu->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		titlemenu->nCountAnim = 0;
		titlemenu->nPatternAnim = 0;

		titlemenu->Texture = pD3DTextureTitlemenu;			// テクスチャへのエネミー
		titlemenu->size = D3DXVECTOR2(TEXTURE_TITLEMENU00_SIZE_X, TEXTURE_TITLEMENU00_SIZE_Y);

		// 頂点情報の作成
		MakeVertexTitlemenu(i);
	}

	// タイトルの初期化
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		titlemenusys->bSelect = false;
	}

	g_nSelectTitlemenu = 0;
	g_nSelectTitlemenuCount = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// メモリ解放
	if (pD3DTextureTitlemenu != NULL)
	{
		pD3DTextureTitlemenu->Release();
		pD3DTextureTitlemenu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];
	TITLEMENU_SYS *titlemenusys = &titlemenusysWk[0];

	// タイトルメニューが有効、かつ決定前は処理
	if (titlemenu->bUse && !titlemenusys->bSelect)
	{
		// 上移動
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, LSTICK_UP))
		{
			g_nSelectTitlemenu--;
			if (g_nSelectTitlemenu < 0)
			{
				g_nSelectTitlemenu += TITLEMENU_MAX;
			}
			SetSe(1, E_DS8_FLAG_NONE, true);
		}
		// 下移動
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, LSTICK_DOWN))
		{
			g_nSelectTitlemenu++;
			SetSe(1, E_DS8_FLAG_NONE, true);
		}

		// 上移動長押し
		if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{
			g_nSelectTitlemenuCount++;
			if (g_nSelectTitlemenuCount > TITLEMENU_SELECT_PRESS && g_nSelectTitlemenuCount % TITLEMENU_SELECT_SPEED == 0)
			{
				g_nSelectTitlemenu--;
				if (g_nSelectTitlemenu < 0)
				{
					g_nSelectTitlemenu += TITLEMENU_MAX;
				}
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		// 下移動長押し
		else if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{
			g_nSelectTitlemenuCount++;
			if (g_nSelectTitlemenuCount > TITLEMENU_SELECT_PRESS && g_nSelectTitlemenuCount  % TITLEMENU_SELECT_SPEED == 0)
			{
				g_nSelectTitlemenu++;
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		else
		{
			g_nSelectTitlemenuCount = 0;
		}


		for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
		{
			if (i == g_nSelectTitlemenu % TITLEMENU_MAX)
			{	// 選択中のメニューはカラーチェンジ
				SetDiffuseTitlemenu(i, SetColorPallet(COLOR_PALLET_LIGHTBLUE));
			}
			else
			{
				SetDiffuseTitlemenu(i, SetColorPallet(COLOR_PALLET_WHITE));
			}
			titlemenu->nPatternAnim = i;
			// テクスチャ座標を設定
			SetTextureTitlemenu(i, titlemenu->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexTitlemenu(i);
		}

		// 選択しているメニューに遷移
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || IsButtonTriggered(0, R_BUTTON_A))
		{
			switch (g_nSelectTitlemenu % TITLEMENU_MAX)
			{
			case TITLE_MENU_START:
				SetFade(FADE_OUT, STAGE_GAME);
				SetSe(2, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_TUTORIAL:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_CREDIT:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_OPTION:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_EXIT:
				SetSe(2, E_DS8_FLAG_NONE, true);
				SetEndFlag();
				break;
			}
			titlemenusys->bSelect = true;		// 決定後は処理を止める
		}

		// ESCでEXITを選択
		if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, R_BUTTON_B))
		{
			g_nSelectTitlemenu = TITLE_MENU_EXIT;
			SetSe(3, E_DS8_FLAG_NONE, true);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitlemenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (titlemenu->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, titlemenu->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLEMENU, titlemenu->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitlemenu(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// 頂点座標の設定
	titlemenu->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	titlemenu->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_TITLEMENU00_SIZE_X, 100.0f, 0.0f);
	titlemenu->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_TITLEMENU00_SIZE_Y, 0.0f);
	titlemenu->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_TITLEMENU00_SIZE_X, 100.0f+TEXTURE_TITLEMENU00_SIZE_Y, 0.0f);
	//SetVertexTitlemenu();

	// rhwの設定
	titlemenu->vertexWk[0].rhw =
	titlemenu->vertexWk[1].rhw =
	titlemenu->vertexWk[2].rhw =
	titlemenu->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	titlemenu->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	titlemenu->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	titlemenu->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLEMENU, 0.0f );
	titlemenu->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU );
	titlemenu->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLEMENU, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTitlemenu( int no, int cntPattern )
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU;
	titlemenu->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	titlemenu->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	titlemenu->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	titlemenu->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexTitlemenu(int no)
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	titlemenu->vertexWk[0].vtx.x = titlemenu->pos.x - TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[0].vtx.y = titlemenu->pos.y - TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[0].vtx.z = 0.0f;

	titlemenu->vertexWk[1].vtx.x = titlemenu->pos.x + TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[1].vtx.y = titlemenu->pos.y - TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[1].vtx.z = 0.0f;

	titlemenu->vertexWk[2].vtx.x = titlemenu->pos.x - TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[2].vtx.y = titlemenu->pos.y + TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[2].vtx.z = 0.0f;

	titlemenu->vertexWk[3].vtx.x = titlemenu->pos.x + TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[3].vtx.y = titlemenu->pos.y + TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTitlemenu(int no, D3DXCOLOR color)
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// 反射光の設定
	titlemenu->vertexWk[0].diffuse =
		titlemenu->vertexWk[1].diffuse =
		titlemenu->vertexWk[2].diffuse =
		titlemenu->vertexWk[3].diffuse = color;
}

//=============================================================================
// アクティブ処理
//=============================================================================
void SetTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// 未使用検索
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (!titlemenu->bUse)
		{
			// 有効にする
			titlemenu->bUse = true;
			// アニメーションカウンタの初期化
			titlemenu->nCountAnim = 0;
			// アニメーションパターンの初期化
			titlemenu->nPatternAnim = 0;
		}
	}
}

//=============================================================================
// ノンアクティブ処理
//=============================================================================
void ReleaseTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// 使用検索
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (titlemenu->bUse)
		{
			// 無効にする
			titlemenu->bUse = false;
		}
	}
}

//=============================================================================
// タイトル取得関数
//=============================================================================
TITLEMENU *GetTitlemenu(int no)
{
	return(&titlemenuWk[no]);
}

