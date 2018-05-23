//=============================================================================
//
// HITEFFECT処理 [hiteffect.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _HITEFFECT_H_
#define _HITEFFECT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 読み込むテクスチャファイル名
#define	TEXTURE_HITEFFECT	"data/TEXTURE/hiteffect.png"
#define HITEFFECT_MAX			(100)		// 最大数
#define	HITEFFECT_SIZE_X		(45.0f)		// サイズX
#define	HITEFFECT_SIZE_Y		(45.0f)		// サイズY
#define	HITEFFECT_ALPHA			(30)		// αテスト値

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_HITEFFECT	(8)
#define TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT	(7)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_HITEFFECT			(TEXTURE_PATTERN_DIVIDE_X_HITEFFECT*TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT)
// アニメーション
#define TIME_ANIMATION_HITEFFECT			(4)

typedef struct
{
	D3DXVECTOR3			posHiteffect;			// 位置
	D3DXVECTOR3			rotHiteffect;			// 向き(回転)
	D3DXVECTOR3			sclHiteffect;			// 大きさ(スケール)
	D3DXVECTOR2			vec2Size;				// サイズ
	int					nTex;					// テクスチャパターン
	int					nAnimeCount;			// アニメーションカウント
	int					nAnimePattern;			// アニメーションパターン
	bool				bUse;					// 使用フラグ
}HITEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHiteffect(int nType);
void UninitHiteffect(void);
void UpdateHiteffect(void);
void DrawHiteffect(void);
void SetHiteffect(D3DXVECTOR3 pos, int nTex);
HITEFFECT *GetHiteffect(int no);

#endif
