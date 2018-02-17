//=============================================================================
//
// OUTLINE処理 [outline.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _OUTLINE_H_
#define _OUTLINE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_OUTLINE		(2)	// ポリゴン数

#define TEXTURE_GAME_OUTLINE	("data/TEXTURE/outline.png")	// OUTLINE用画像
#define TEXTURE_OUTLINE_SIZE_X	(15 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_OUTLINE_SIZE_Y	(15 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_OUTLINE_POS_X	(SCREEN_WIDTH) // テクスチャサイズ
#define TEXTURE_OUTLINE_POS_Y	(SCREEN_HEIGHT) // 同上

#define TEXTURE_OUTLINE_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_OUTLINE_G			(1.0f) // テクスチャカラー緑
#define TEXTURE_OUTLINE_B			(1.0f) // テクスチャカラー青
#define TEXTURE_OUTLINE_A			(1.0f) // テクスチャカラー青

#define TEXTURE_OUTLINE_CHANGE_COLLAR		(0.005f) // テクスチャカラー変更速度

#define TEXTURE_PATTERN_DIVIDE_X_OUTLINE	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_OUTLINE	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_OUTLINE			(TEXTURE_PATTERN_DIVIDE_X_OUTLINE*TEXTURE_PATTERN_DIVIDE_Y_OUTLINE)	// アニメーションパターン数
#define TIME_ANIMATION_OUTLINE			(1)	// アニメーションの切り替わるカウント

#define OUTLINE_MAX					(2)	// OUTLINEの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// OUTLINEの移動量
	D3DXVECTOR3				rot;					// OUTLINEの回転量
	D3DXVECTOR2				size;					// OUTLINEのテクスチャサイズ(x,y)
	D3DXCOLOR				colorOutline;				// 色管理
	int						nCollarFlag;			// 色変更フラグ

	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}OUTLINE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitOutline(int type);
void UninitOutline(void);
void UpdateOutline(void);
void DrawOutline(void);
OUTLINE *GetOutline(int no);
int SetOutline(D3DXCOLOR colorfloat);
void SetUpdateOutline(int nOutline, D3DXVECTOR3 pos, float fAngle);
void ReleaseOutline(int nOutline);
#endif
