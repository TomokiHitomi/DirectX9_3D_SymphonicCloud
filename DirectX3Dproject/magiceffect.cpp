//=============================================================================
//
// MAGICEFFECT処理 [magiceffect.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "magiceffect.h"
#include "magic.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMagiceffect(int no);
void SetTextureMagiceffect(int no, int cntPattern);	//
void SetVertexMagiceffect(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
MAGICEFFECT					magiceffectWk[MAGICEFFECT_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureMagiceffect = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMagiceffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_MAGICEFFECT,				// ファイルの名前
			&pD3DTextureMagiceffect);			// 読み込むメモリのポインタ
	}

	// MAGICEFFECTの初期化
	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (i < MAGICEFFECT_TYPE_MAX)
		{
			if (i == MAGICEFFECT_NORMAL)
			{
				magiceffect->pos = D3DXVECTOR3(MAGICEFFECT_NORMAL_POS_X, MAGICEFFECT_POS_Y + (i - 1) * MAGIC_GAGE_INTERVAL, 0.0f);
				magiceffect->bUse = true;
				magiceffect->size = D3DXVECTOR2(TEXTURE_MAGICEFFECT_SIZE_X, TEXTURE_MAGICEFFECT_SIZE_Y);

			}
			else if (i == MAGICEFFECT_MAGIC_L || i == MAGICEFFECT_MAGIC_R)
			{
				magiceffect->bUse = false;
				if(i == MAGICEFFECT_MAGIC_L)magiceffect->pos = 
					D3DXVECTOR3(MAGICEFFECT_POS_MAGIC_L_X, MAGICEFFECT_POS_MAGIC_L_Y, 0.0f);
				if (i == MAGICEFFECT_MAGIC_R)magiceffect->pos =
					D3DXVECTOR3(MAGICEFFECT_POS_MAGIC_R_X, MAGICEFFECT_POS_MAGIC_R_Y, 0.0f);
				magiceffect->size = D3DXVECTOR2(MAGICEFFECT_POS_MAGIC_SIZE_X, MAGICEFFECT_POS_MAGIC_SIZE_Y);
			}
			else
			{
				magiceffect->pos = D3DXVECTOR3(MAGICEFFECT_POS_X, MAGICEFFECT_POS_Y + i * MAGIC_GAGE_INTERVAL, 0.0f);
				magiceffect->bUse = false;
				magiceffect->size = D3DXVECTOR2(TEXTURE_MAGICEFFECT_SIZE_X, TEXTURE_MAGICEFFECT_SIZE_Y);
			}
			magiceffect->nType = i;
		}
		else
		{
			magiceffect->bUse = false;
			magiceffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			magiceffect->nType = 0;
		}

		magiceffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		magiceffect->nCountAnim = 0;
		magiceffect->nPatternAnim = 0;

		magiceffect->Texture = pD3DTextureMagiceffect;			// テクスチャへのMAGICEFFECT

		// 頂点情報の作成
		MakeVertexMagiceffect(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	// メモリ解放
	if (pD3DTextureMagiceffect != NULL)
	{
		pD3DTextureMagiceffect->Release();
		pD3DTextureMagiceffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];
	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (magiceffect->bUse)
		{
			magiceffect->nCountAnim++;
			// アニメーション
			if (i == GetTypeMagic() || i >= MAGICEFFECT_NORMAL)
			{	
				if ((magiceffect->nCountAnim % TIME_ANIMATION_MAGICEFFECT) == 0)
				{
					if (i == MAGICEFFECT_MAGIC_L || i == MAGICEFFECT_MAGIC_R)
					{
						int nTypeMagic = GetTypeMagic();
						if (magiceffect->nType != nTypeMagic)
						{
							magiceffect->nCountAnim = 0;
							magiceffect->nType = nTypeMagic;
						}
					}
					// パターンの切り替え
					//magiceffect->nPatternAnim = (magiceffect->nPatternAnim + 1) % ANIM_PATTERN_NUM_MAGICEFFECT;
					magiceffect->nPatternAnim = (magiceffect->nPatternAnim + 1)
						% TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT
						+ TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT * magiceffect->nType;
				}
			}
			else
			{
				magiceffect->nPatternAnim = TEXTURE_PATTERN_NONACTIVE_MAGICEFFECT
					+ TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT*magiceffect->nType;
			}

			// テクスチャ座標を設定
			SetTextureMagiceffect(i, magiceffect->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexMagiceffect(i);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagiceffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAGICEFFECT_MAX; i++, magiceffect++)
	{
		if (magiceffect->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, magiceffect->Texture);

			// MAGICEFFECTの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MAGICEFFECT, magiceffect->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMagiceffect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	// 頂点座標の設定
	magiceffect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	magiceffect->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGICEFFECT_SIZE_X, 100.0f, 0.0f);
	magiceffect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MAGICEFFECT_SIZE_Y, 0.0f);
	magiceffect->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGICEFFECT_SIZE_X, 100.0f + TEXTURE_MAGICEFFECT_SIZE_Y, 0.0f);
	//SetVertexMagiceffect();

	// rhwの設定
	magiceffect->vertexWk[0].rhw =
	magiceffect->vertexWk[1].rhw =
	magiceffect->vertexWk[2].rhw =
	magiceffect->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	magiceffect->vertexWk[0].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[1].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[2].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);
	magiceffect->vertexWk[3].diffuse = D3DCOLOR_RGBA(TEXTURE_MAGICEFFECT_R, TEXTURE_MAGICEFFECT_G, TEXTURE_MAGICEFFECT_B, 255);

	// テクスチャ座標の設定
	magiceffect->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	magiceffect->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT, 0.0f);
	magiceffect->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT);
	magiceffect->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureMagiceffect(int no, int cntPattern)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT;
	magiceffect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	magiceffect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	magiceffect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	magiceffect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexMagiceffect(int no)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[no];

	magiceffect->vertexWk[0].vtx.x = magiceffect->pos.x - magiceffect->size.x;
	magiceffect->vertexWk[0].vtx.y = magiceffect->pos.y - magiceffect->size.y;
	magiceffect->vertexWk[0].vtx.z = 0.0f;

	magiceffect->vertexWk[1].vtx.x = magiceffect->pos.x + magiceffect->size.x;
	magiceffect->vertexWk[1].vtx.y = magiceffect->pos.y - magiceffect->size.y;
	magiceffect->vertexWk[1].vtx.z = 0.0f;

	magiceffect->vertexWk[2].vtx.x = magiceffect->pos.x - magiceffect->size.x;
	magiceffect->vertexWk[2].vtx.y = magiceffect->pos.y + magiceffect->size.y;
	magiceffect->vertexWk[2].vtx.z = 0.0f;

	magiceffect->vertexWk[3].vtx.x = magiceffect->pos.x + magiceffect->size.x;
	magiceffect->vertexWk[3].vtx.y = magiceffect->pos.y + magiceffect->size.y;
	magiceffect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 有効設定
//=============================================================================
void SetMagiceffect(int nMagicEffect)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[nMagicEffect];
	magiceffect->bUse = true;

	magiceffectWk[MAGICEFFECT_MAGIC_L].bUse = true;
	magiceffectWk[MAGICEFFECT_MAGIC_R].bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseMagiceffect(void)
{
	MAGICEFFECT *magiceffect = &magiceffectWk[0];
	magiceffect->bUse = false;
}

//=============================================================================
// MAGICEFFECT取得関数
//=============================================================================
MAGICEFFECT *GetMagiceffect(int no)
{
	return(&magiceffectWk[no]);
}

