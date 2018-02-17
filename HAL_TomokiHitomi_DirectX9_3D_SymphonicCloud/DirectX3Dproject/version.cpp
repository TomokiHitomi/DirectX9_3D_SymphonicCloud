//=============================================================================
//
// VERSION処理 [version.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "version.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexVersion(int no);
void SetTextureVersion( int no, int cntPattern );	//
void SetVertexVersion(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
VERSION					versionWk[VERSION_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureVersion = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitVersion(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_VERSION,				// ファイルの名前
			&pD3DTextureVersion);			// 読み込むメモリのポインタ
	}

	// VERSIONの初期化
	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		version->bUse = true;
		version->pos = D3DXVECTOR3((float)VERSION_POS_X, (float)VERSION_POS_Y, 0.0f);
		version->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		version->nCountAnim = 0;
		version->nPatternAnim = 0;

		version->Texture = pD3DTextureVersion;			// テクスチャへのVERSION
		version->size = D3DXVECTOR2(TEXTURE_VERSION_SIZE_X, TEXTURE_VERSION_SIZE_Y);

		// 頂点情報の作成
		MakeVertexVersion(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitVersion(void)
{
	VERSION *version = &versionWk[0];

	// メモリ解放
	if (pD3DTextureVersion != NULL)
	{
		pD3DTextureVersion->Release();
		pD3DTextureVersion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateVersion(void)
{
	VERSION *version = &versionWk[0];
	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		if (version->bUse == true)
		{
			// アニメーション
			version->nCountAnim++;
			if ((version->nCountAnim % TIME_ANIMATION_VERSION) == 0)
			{
				// パターンの切り替え
				version->nPatternAnim = (version->nPatternAnim + 1) % ANIM_PATTERN_NUM_VERSION;
			}

			// テクスチャ座標を設定
			SetTextureVersion(i, version->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexVersion(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawVersion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < VERSION_MAX; i++, version++)
	{
		if (version->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, version->Texture);

			// VERSIONの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_VERSION, version->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexVersion(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERSION *version = &versionWk[no];

	// 頂点座標の設定
	version->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	version->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_VERSION_SIZE_X, 100.0f, 0.0f);
	version->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_VERSION_SIZE_Y, 0.0f);
	version->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_VERSION_SIZE_X, 100.0f+TEXTURE_VERSION_SIZE_Y, 0.0f);
	//SetVertexVersion();

	// rhwの設定
	version->vertexWk[0].rhw =
	version->vertexWk[1].rhw =
	version->vertexWk[2].rhw =
	version->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	version->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);
	version->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_VERSION_R, TEXTURE_VERSION_G, TEXTURE_VERSION_B, 255);

	// テクスチャ座標の設定
	version->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	version->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_VERSION, 0.0f );
	version->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_VERSION );
	version->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_VERSION, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_VERSION );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureVersion( int no, int cntPattern )
{
	VERSION *version = &versionWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_VERSION;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_VERSION;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_VERSION;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_VERSION;
	version->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	version->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	version->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	version->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexVersion(int no)
{
	VERSION *version = &versionWk[no];

	version->vertexWk[0].vtx.x = version->pos.x - TEXTURE_VERSION_SIZE_X;
	version->vertexWk[0].vtx.y = version->pos.y - TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[0].vtx.z = 0.0f;

	version->vertexWk[1].vtx.x = version->pos.x + TEXTURE_VERSION_SIZE_X;
	version->vertexWk[1].vtx.y = version->pos.y - TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[1].vtx.z = 0.0f;

	version->vertexWk[2].vtx.x = version->pos.x - TEXTURE_VERSION_SIZE_X;
	version->vertexWk[2].vtx.y = version->pos.y + TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[2].vtx.z = 0.0f;

	version->vertexWk[3].vtx.x = version->pos.x + TEXTURE_VERSION_SIZE_X;
	version->vertexWk[3].vtx.y = version->pos.y + TEXTURE_VERSION_SIZE_Y;
	version->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// VERSION取得関数
//=============================================================================
VERSION *GetVersion(int no)
{
	return(&versionWk[no]);
}

