//=============================================================================
//
// DAMAGEEFFECT処理 [damageeffect.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "main.h"
#include "damageeffect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexDamageeffect(int no);
HRESULT ReInitDamageeffect(int nDamageeffect);
void SetTextureDamageeffect( int no, int cntPattern );	//
void SetVertexDamageeffect(int no);
void SetDiffuseDamageeffect(int no, D3DXCOLOR color);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
DAMAGEEFFECT			damageeffectWk[DAMAGEEFFECT_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureDamageeffect = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitDamageeffect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_DAMAGEEFFECT,				// ファイルの名前
			&pD3DTextureDamageeffect);			// 読み込むメモリのポインタ
	}

	// DAMAGEEFFECTの初期化
	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		ReInitDamageeffect(i);
		damageeffect->pos = D3DXVECTOR3((float)DAMAGEEFFECT_POS_X, (float)DAMAGEEFFECT_POS_Y, 0.0f);
		damageeffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		damageeffect->xColor = D3DXCOLOR(TEXTURE_DAMAGEEFFECT_R, TEXTURE_DAMAGEEFFECT_G, TEXTURE_DAMAGEEFFECT_B, TEXTURE_DAMAGEEFFECT_A);
		damageeffect->nCountAnim = 0;
		damageeffect->nPatternAnim = 0;

		damageeffect->Texture = pD3DTextureDamageeffect;			// テクスチャへのDAMAGEEFFECT
		damageeffect->size = D3DXVECTOR2(TEXTURE_DAMAGEEFFECT_SIZE_X, TEXTURE_DAMAGEEFFECT_SIZE_Y);

		// 頂点情報の作成
		MakeVertexDamageeffect(i);
	}

	return S_OK;
}

//=============================================================================
// 再初期化処理
//=============================================================================
HRESULT ReInitDamageeffect(int nDamageeffect)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[nDamageeffect];
	damageeffect->bUse = false;
	damageeffect->bActive = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	// メモリ解放
	if (pD3DTextureDamageeffect != NULL)
	{
		pD3DTextureDamageeffect->Release();
		pD3DTextureDamageeffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];
	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		if (damageeffect->bUse)
		{
			// アニメーション
			damageeffect->nCountAnim++;
			if ((damageeffect->nCountAnim % TIME_ANIMATION_DAMAGEEFFECT) == 0)
			{
				// パターンの切り替え
				damageeffect->nPatternAnim = (damageeffect->nPatternAnim + 1) % ANIM_PATTERN_NUM_DAMAGEEFFECT;
			}

			if (damageeffect->bActive)
			{
				damageeffect->xColor.a += DAMAGEEFFECT_ALPHA_UP;
				if (damageeffect->xColor.a > 1.0f)
				{
					damageeffect->xColor.a = 1.0f;
					damageeffect->bActive = false;
				}
			}
			else
			{
				damageeffect->xColor.a -= DAMAGEEFFECT_ALPHA_DOWN;
				if (damageeffect->xColor.a < 0.0f)
				{
					damageeffect->xColor.a = 0.0f;
					ReInitDamageeffect(i);
				}
			}

			// テクスチャ座標を設定
			SetTextureDamageeffect(i, damageeffect->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexDamageeffect(i);
			// 反射光を設定
			SetDiffuseDamageeffect(i, damageeffect->xColor);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDamageeffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < DAMAGEEFFECT_MAX; i++, damageeffect++)
	{
		if (damageeffect->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, damageeffect->Texture);

			// DAMAGEEFFECTの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_DAMAGEEFFECT, damageeffect->vertexWk, sizeof(VERTEX_2D));
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
HRESULT MakeVertexDamageeffect(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// 頂点座標の設定
	damageeffect->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	damageeffect->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_DAMAGEEFFECT_SIZE_X, 100.0f, 0.0f);
	damageeffect->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_DAMAGEEFFECT_SIZE_Y, 0.0f);
	damageeffect->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_DAMAGEEFFECT_SIZE_X, 100.0f+TEXTURE_DAMAGEEFFECT_SIZE_Y, 0.0f);
	//SetVertexDamageeffect();

	// rhwの設定
	damageeffect->vertexWk[0].rhw =
	damageeffect->vertexWk[1].rhw =
	damageeffect->vertexWk[2].rhw =
	damageeffect->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	damageeffect->vertexWk[0].diffuse =
	damageeffect->vertexWk[1].diffuse =
	damageeffect->vertexWk[2].diffuse =
	damageeffect->vertexWk[3].diffuse = damageeffect->xColor;

	// テクスチャ座標の設定
	damageeffect->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	damageeffect->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT, 0.0f );
	damageeffect->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT );
	damageeffect->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT );

	return S_OK;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseDamageeffect(int no, D3DXCOLOR color)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// 反射光の設定
	damageeffect->vertexWk[0].diffuse =
		damageeffect->vertexWk[1].diffuse =
		damageeffect->vertexWk[2].diffuse =
		damageeffect->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureDamageeffect( int no, int cntPattern )
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT;
	damageeffect->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	damageeffect->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	damageeffect->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	damageeffect->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexDamageeffect(int no)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[no];

	damageeffect->vertexWk[0].vtx.x = damageeffect->pos.x - TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[0].vtx.y = damageeffect->pos.y - TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[0].vtx.z = 0.0f;

	damageeffect->vertexWk[1].vtx.x = damageeffect->pos.x + TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[1].vtx.y = damageeffect->pos.y - TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[1].vtx.z = 0.0f;

	damageeffect->vertexWk[2].vtx.x = damageeffect->pos.x - TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[2].vtx.y = damageeffect->pos.y + TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[2].vtx.z = 0.0f;

	damageeffect->vertexWk[3].vtx.x = damageeffect->pos.x + TEXTURE_DAMAGEEFFECT_SIZE_X;
	damageeffect->vertexWk[3].vtx.y = damageeffect->pos.y + TEXTURE_DAMAGEEFFECT_SIZE_Y;
	damageeffect->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// Damageeffectの設置関数
//=============================================================================
void SetDamageeffect(void)
{
	DAMAGEEFFECT *damageeffect = &damageeffectWk[0];
	damageeffect->bUse = true;
	damageeffect->bActive = true;
}

//=============================================================================
// DAMAGEEFFECT取得関数
//=============================================================================
DAMAGEEFFECT *GetDamageeffect(int no)
{
	return(&damageeffectWk[no]);
}

