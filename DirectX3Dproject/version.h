//=============================================================================
//
// VERSION処理 [version.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _VERSION_H_
#define _VERSION_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERSION		(2)	// ポリゴン数

#define TEXTURE_GAME_VERSION	("data/TEXTURE/version.png")	// VERSION用画像
#define TEXTURE_VERSION_SIZE_X	(180 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_VERSION_SIZE_Y	(30 * SCREEN_SCALE/2) // 同上

#define VERSION_POS_X				(1100 * SCREEN_SCALE)			// 初期座標X
#define VERSION_POS_Y				(693 * SCREEN_SCALE)			// 初期座標Y

#define TEXTURE_VERSION_R			(255) // テクスチャカラー赤
#define TEXTURE_VERSION_G			(255) // テクスチャカラー緑
#define TEXTURE_VERSION_B			(255) // テクスチャカラー青

#define TEXTURE_PATTERN_DIVIDE_X_VERSION	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_VERSION	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_VERSION			(TEXTURE_PATTERN_DIVIDE_X_VERSION*TEXTURE_PATTERN_DIVIDE_Y_VERSION)	// アニメーションパターン数
#define TIME_ANIMATION_VERSION				(4)	// アニメーションの切り替わるカウント

#define VERSION_MAX					(1)	// VERSIONの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// VERSIONの移動量
	D3DXVECTOR3				rot;					// VERSIONの回転量
	D3DXVECTOR2				size;					// VERSIONのテクスチャサイズ(x,y)
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}VERSION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitVersion(int type);
void UninitVersion(void);
void UpdateVersion(void);
void DrawVersion(void);
VERSION *GetVersion(int no);


#endif
