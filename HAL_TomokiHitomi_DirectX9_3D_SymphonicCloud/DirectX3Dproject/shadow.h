//=============================================================================
//
// 影処理 [shadow.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(50.0f)							// 弾の幅
#define	SHADOW_SIZE_Z		(50.0f)							// 弾の高さ

#define SHADOW_POS_Y		(0.2f)							// シャドウの高さを固定

#define	MAX_SHADOW			(5000)							// 影最大数

#define SHADOW_ALPHA		(1)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	bool bUse;				// 使用しているかどうか
	bool bDraw;				// 描画の有無
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(int nType);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseShadow(int nIdxShadow);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);
LPDIRECT3DVERTEXBUFFER9 DebugShadow(void);
#endif
