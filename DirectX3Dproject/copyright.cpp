//=============================================================================
//
// COPYRIGHT処理 [copyright.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "copyright.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCopyright(int no);
void SetTextureCopyright( int no, int cntPattern );	//
void SetVertexCopyright(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
COPYRIGHT					copyrightWk[COPYRIGHT_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureCopyright = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCopyright(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_COPYRIGHT,				// ファイルの名前
			&pD3DTextureCopyright);			// 読み込むメモリのポインタ
	}

	// COPYRIGHTの初期化
	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		copyright->bUse = true;
		copyright->pos = D3DXVECTOR3((float)COPYRIGHT_POS_X, (float)COPYRIGHT_POS_Y, 0.0f);
		copyright->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		copyright->nCountAnim = 0;
		copyright->nPatternAnim = 0;

		copyright->Texture = pD3DTextureCopyright;			// テクスチャへのCOPYRIGHT
		copyright->size = D3DXVECTOR2(TEXTURE_COPYRIGHT_SIZE_X, TEXTURE_COPYRIGHT_SIZE_Y);

		// 頂点情報の作成
		MakeVertexCopyright(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCopyright(void)
{
	COPYRIGHT *copyright = &copyrightWk[0];

	// メモリ解放
	if (pD3DTextureCopyright != NULL)
	{
		pD3DTextureCopyright->Release();
		pD3DTextureCopyright = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCopyright(void)
{
	COPYRIGHT *copyright = &copyrightWk[0];
	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		if (copyright->bUse == true)
		{
			// アニメーション
			copyright->nCountAnim++;
			if ((copyright->nCountAnim % TIME_ANIMATION_COPYRIGHT) == 0)
			{
				// パターンの切り替え
				copyright->nPatternAnim = (copyright->nPatternAnim + 1) % ANIM_PATTERN_NUM_COPYRIGHT;
			}

			// テクスチャ座標を設定
			SetTextureCopyright(i, copyright->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexCopyright(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCopyright(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < COPYRIGHT_MAX; i++, copyright++)
	{
		if (copyright->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, copyright->Texture);

			// COPYRIGHTの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_COPYRIGHT, copyright->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCopyright(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COPYRIGHT *copyright = &copyrightWk[no];

	// 頂点座標の設定
	copyright->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	copyright->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_COPYRIGHT_SIZE_X, 100.0f, 0.0f);
	copyright->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	copyright->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_COPYRIGHT_SIZE_X, 100.0f+TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	//SetVertexCopyright();

	// rhwの設定
	copyright->vertexWk[0].rhw =
	copyright->vertexWk[1].rhw =
	copyright->vertexWk[2].rhw =
	copyright->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	copyright->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);
	copyright->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_COPYRIGHT_R, TEXTURE_COPYRIGHT_G, TEXTURE_COPYRIGHT_B, 255);

	// テクスチャ座標の設定
	copyright->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	copyright->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT, 0.0f );
	copyright->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT );
	copyright->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureCopyright( int no, int cntPattern )
{
	COPYRIGHT *copyright = &copyrightWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT;
	copyright->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	copyright->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	copyright->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	copyright->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexCopyright(int no)
{
	COPYRIGHT *copyright = &copyrightWk[no];

	copyright->vertexWk[0].vtx.x = copyright->pos.x - TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[0].vtx.y = copyright->pos.y - TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[0].vtx.z = 0.0f;

	copyright->vertexWk[1].vtx.x = copyright->pos.x + TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[1].vtx.y = copyright->pos.y - TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[1].vtx.z = 0.0f;

	copyright->vertexWk[2].vtx.x = copyright->pos.x - TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[2].vtx.y = copyright->pos.y + TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[2].vtx.z = 0.0f;

	copyright->vertexWk[3].vtx.x = copyright->pos.x + TEXTURE_COPYRIGHT_SIZE_X;
	copyright->vertexWk[3].vtx.y = copyright->pos.y + TEXTURE_COPYRIGHT_SIZE_Y;
	copyright->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// COPYRIGHT取得関数
//=============================================================================
COPYRIGHT *GetCopyright(int no)
{
	return(&copyrightWk[no]);
}

