//=============================================================================
//
// JOYCONHELP処理 [joyconhelp.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "joyconhelp.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexJoyconhelp(int no);
void SetTextureJoyconhelp( int no, int cntPattern );	//
void SetVertexJoyconhelp(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
JOYCONHELP					joyconhelpWk[JOYCONHELP_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureJoyconhelp = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitJoyconhelp(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_JOYCONHELP,				// ファイルの名前
			&pD3DTextureJoyconhelp);			// 読み込むメモリのポインタ
	}

	// JOYCONHELPの初期化
	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		joyconhelp->bUse = true;
		joyconhelp->pos = D3DXVECTOR3((float)JOYCONHELP_POS_X, (float)JOYCONHELP_POS_Y, 0.0f);
		joyconhelp->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		joyconhelp->nCountAnim = 0;
		joyconhelp->nPatternAnim = 0;

		joyconhelp->Texture = pD3DTextureJoyconhelp;			// テクスチャへのJOYCONHELP
		joyconhelp->size = D3DXVECTOR2(TEXTURE_JOYCONHELP_SIZE_X, TEXTURE_JOYCONHELP_SIZE_Y);

		// 頂点情報の作成
		MakeVertexJoyconhelp(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitJoyconhelp(void)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	// メモリ解放
	if (pD3DTextureJoyconhelp != NULL)
	{
		pD3DTextureJoyconhelp->Release();
		pD3DTextureJoyconhelp = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateJoyconhelp(void)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	if (GetKeyboardTrigger(DIK_H) || IsButtonTriggered(0, BUTTON_CAP))
	{
		joyconhelp->bUse = joyconhelp->bUse ? false : true;
	}

	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		if (joyconhelp->bUse)
		{
			// アニメーション
			joyconhelp->nCountAnim++;
			if ((joyconhelp->nCountAnim % TIME_ANIMATION_JOYCONHELP) == 0)
			{
				// パターンの切り替え
				joyconhelp->nPatternAnim = (joyconhelp->nPatternAnim + 1) % ANIM_PATTERN_NUM_JOYCONHELP;
			}

			// テクスチャ座標を設定
			SetTextureJoyconhelp(i, joyconhelp->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexJoyconhelp(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawJoyconhelp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < JOYCONHELP_MAX; i++, joyconhelp++)
	{
		if (joyconhelp->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, joyconhelp->Texture);

			// JOYCONHELPの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_JOYCONHELP, joyconhelp->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexJoyconhelp(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	// 頂点座標の設定
	joyconhelp->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	joyconhelp->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_JOYCONHELP_SIZE_X, 100.0f, 0.0f);
	joyconhelp->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_JOYCONHELP_SIZE_Y, 0.0f);
	joyconhelp->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_JOYCONHELP_SIZE_X, 100.0f+TEXTURE_JOYCONHELP_SIZE_Y, 0.0f);
	//SetVertexJoyconhelp();

	// rhwの設定
	joyconhelp->vertexWk[0].rhw =
	joyconhelp->vertexWk[1].rhw =
	joyconhelp->vertexWk[2].rhw =
	joyconhelp->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	joyconhelp->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);
	joyconhelp->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_JOYCONHELP_R, TEXTURE_JOYCONHELP_G, TEXTURE_JOYCONHELP_B, 255);

	// テクスチャ座標の設定
	joyconhelp->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	joyconhelp->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP, 0.0f );
	joyconhelp->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP );
	joyconhelp->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureJoyconhelp( int no, int cntPattern )
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP;
	joyconhelp->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	joyconhelp->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	joyconhelp->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	joyconhelp->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexJoyconhelp(int no)
{
	JOYCONHELP *joyconhelp = &joyconhelpWk[no];

	joyconhelp->vertexWk[0].vtx.x = joyconhelp->pos.x - TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[0].vtx.y = joyconhelp->pos.y - TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[0].vtx.z = 0.0f;

	joyconhelp->vertexWk[1].vtx.x = joyconhelp->pos.x + TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[1].vtx.y = joyconhelp->pos.y - TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[1].vtx.z = 0.0f;

	joyconhelp->vertexWk[2].vtx.x = joyconhelp->pos.x - TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[2].vtx.y = joyconhelp->pos.y + TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[2].vtx.z = 0.0f;

	joyconhelp->vertexWk[3].vtx.x = joyconhelp->pos.x + TEXTURE_JOYCONHELP_SIZE_X;
	joyconhelp->vertexWk[3].vtx.y = joyconhelp->pos.y + TEXTURE_JOYCONHELP_SIZE_Y;
	joyconhelp->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// JOYCONHELP取得関数
//=============================================================================
JOYCONHELP *GetJoyconhelp(int no)
{
	return(&joyconhelpWk[no]);
}

