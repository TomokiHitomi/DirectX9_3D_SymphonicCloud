//=============================================================================
//
// JOYCONHELP処理 [joyconhelp.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _JOYCONHELP_H_
#define _JOYCONHELP_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_JOYCONHELP		(2)	// ポリゴン数

#define TEXTURE_GAME_JOYCONHELP	("data/TEXTURE/joycon.png")	// JOYCONHELP用画像
#define TEXTURE_JOYCONHELP_SIZE_X	(1280 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_JOYCONHELP_SIZE_Y	(432 * SCREEN_SCALE/2) // 同上

#define JOYCONHELP_POS_X				(SCREEN_CENTER_X)			// 初期座標X
#define JOYCONHELP_POS_Y				(505 * SCREEN_SCALE)		// 初期座標Y

#define TEXTURE_JOYCONHELP_R			(255) // テクスチャカラー赤
#define TEXTURE_JOYCONHELP_G			(255) // テクスチャカラー緑
#define TEXTURE_JOYCONHELP_B			(255) // テクスチャカラー青

#define TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_JOYCONHELP			(TEXTURE_PATTERN_DIVIDE_X_JOYCONHELP*TEXTURE_PATTERN_DIVIDE_Y_JOYCONHELP)	// アニメーションパターン数
#define TIME_ANIMATION_JOYCONHELP				(4)	// アニメーションの切り替わるカウント

#define JOYCONHELP_MAX					(1)	// JOYCONHELPの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// JOYCONHELPの移動量
	D3DXVECTOR3				rot;					// JOYCONHELPの回転量
	D3DXVECTOR2				size;					// JOYCONHELPのテクスチャサイズ(x,y)
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}JOYCONHELP;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitJoyconhelp(int type);
void UninitJoyconhelp(void);
void UpdateJoyconhelp(void);
void DrawJoyconhelp(void);
JOYCONHELP *GetJoyconhelp(int no);


#endif
