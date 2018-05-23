//=============================================================================
//
// タイトル処理 [result.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexResult(int no);
void SetTextureResult( int no, int cntPattern );	//
void SetVertexResult(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
RESULT					resultWk[RESULT_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureResult = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[0];

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_RESULT00,				// ファイルの名前
			&pD3DTextureResult);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化
	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		result->bUse = true;
		result->bPosMoveFlag = true;
		result->pos = D3DXVECTOR3((float)RESULT_POS_X, (float)RESULT_POS_Y, 0.0f);
		result->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		result->vecPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		result->nCountAnim = 0;
		result->nPatternAnim = 0;

		result->Texture = pD3DTextureResult;			// テクスチャへのエネミー
		result->size = D3DXVECTOR2(TEXTURE_RESULT00_SIZE_X, TEXTURE_RESULT00_SIZE_Y);

		// 頂点情報の作成
		MakeVertexResult(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	RESULT *result = &resultWk[0];

	// メモリ解放
	if (pD3DTextureResult != NULL)
	{
		pD3DTextureResult->Release();
		pD3DTextureResult = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	RESULT *result = &resultWk[0];

	// ステージ遷移テスト
	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, R_BUTTON_A))
	{
		SetFade(FADE_OUT, STAGE_TITLE);
	}

	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		if (result->bUse)
		{
			SetSoundBgm(SOUND_BGM_RESULT);
			if (result->bPosMoveFlag)
			{
				result->vecPosMove = D3DXVECTOR3(0.0f, (float)RESULT_MOVE_UI, 0.0f);
				result->bPosMoveFlag = false;
			}

			// アニメーション
			result->nCountAnim++;
			if ((result->nCountAnim % TIME_ANIMATION_RESULT) == 0)
			{
				// パターンの切り替え
				result->nPatternAnim = (result->nPatternAnim + 1) % ANIM_PATTERN_NUM_RESULT;
			}

			// テクスチャ座標を設定
			SetTextureResult(i, result->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVertexResult(i);

		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < RESULT_MAX; i++, result++)
	{
		if (result->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, result->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_RESULT, result->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexResult(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	RESULT *result = &resultWk[no];

	// 頂点座標の設定
	result->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	result->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_RESULT00_SIZE_X, 100.0f, 0.0f);
	result->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_RESULT00_SIZE_Y, 0.0f);
	result->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_RESULT00_SIZE_X, 100.0f+TEXTURE_RESULT00_SIZE_Y, 0.0f);
	//SetVertexResult();

	// rhwの設定
	result->vertexWk[0].rhw =
	result->vertexWk[1].rhw =
	result->vertexWk[2].rhw =
	result->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	result->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	result->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	result->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	result->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_RESULT, 0.0f );
	result->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_RESULT );
	result->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_RESULT, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_RESULT );

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureResult( int no, int cntPattern )
{
	RESULT *result = &resultWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_RESULT;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_RESULT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_RESULT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_RESULT;
	result->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	result->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	result->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	result->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexResult(int no)
{
	RESULT *result = &resultWk[no];

	result->vertexWk[0].vtx.x = result->pos.x - TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[0].vtx.y = result->pos.y - TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[0].vtx.z = 0.0f;

	result->vertexWk[1].vtx.x = result->pos.x + TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[1].vtx.y = result->pos.y - TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[1].vtx.z = 0.0f;

	result->vertexWk[2].vtx.x = result->pos.x - TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[2].vtx.y = result->pos.y + TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[2].vtx.z = 0.0f;

	result->vertexWk[3].vtx.x = result->pos.x + TEXTURE_RESULT00_SIZE_X;
	result->vertexWk[3].vtx.y = result->pos.y + TEXTURE_RESULT00_SIZE_Y;
	result->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// タイトル取得関数
//=============================================================================
RESULT *GetResult(int no)
{
	return(&resultWk[no]);
}

