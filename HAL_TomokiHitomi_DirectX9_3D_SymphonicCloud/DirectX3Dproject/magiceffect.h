//=============================================================================
//
// MAGICEFFECT処理 [magiceffect.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _MAGICEFFECT_H_
#define _MAGICEFFECT_H_

#include "main.h"

// マクロ定義
#define NUM_MAGICEFFECT		(2)	// ポリゴン数

#define TEXTURE_GAME_MAGICEFFECT	("data/TEXTURE/magiceffect.png")	// MAGICEFFECT用画像
#define TEXTURE_MAGICEFFECT_SIZE_X	(40 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_MAGICEFFECT_SIZE_Y	(40 * SCREEN_SCALE / 2) // 同上

#define MAGICEFFECT_POS_X			(205 * SCREEN_SCALE)
#define MAGICEFFECT_POS_Y			(35.5f * SCREEN_SCALE)
#define MAGIC_GAGE_INTERVAL			(54.55f * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_L_X			(132 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_L_Y			(585 * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_R_X			(1151 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_R_Y			(585 * SCREEN_SCALE)

#define MAGICEFFECT_POS_MAGIC_SIZE_X		(60 * SCREEN_SCALE)
#define MAGICEFFECT_POS_MAGIC_SIZE_Y		(60 * SCREEN_SCALE)

#define MAGICEFFECT_NORMAL_POS_X	(1074 * SCREEN_SCALE)
#define TEXTURE_MAGICEFFECT_R			(255) // テクスチャカラー赤
#define TEXTURE_MAGICEFFECT_G			(255) // テクスチャカラー緑
#define TEXTURE_MAGICEFFECT_B			(255) // テクスチャカラー青

#define TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT	(8)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT	(7)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_MAGICEFFECT			(TEXTURE_PATTERN_DIVIDE_X_MAGICEFFECT*TEXTURE_PATTERN_DIVIDE_Y_MAGICEFFECT)	// アニメーションパターン数
#define TIME_ANIMATION_MAGICEFFECT				(8)	// アニメーションの切り替わるカウント
#define TEXTURE_PATTERN_NONACTIVE_MAGICEFFECT	(4)	// 無効時に停止するフレーム


#define MAGICEFFECT_MAX					(7)	// MAGICEFFECTの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// MAGICEFFECTの移動量
	D3DXVECTOR3				rot;					// MAGICEFFECTの回転量
	D3DXVECTOR2				size;					// MAGICEFFECTのテクスチャサイズ(x,y)
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	int						nType;					// アニメーションタイプ
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}MAGICEFFECT;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum
{
	MAGICEFFECT_FIRE,
	MAGICEFFECT_THUNDER,
	MAGICEFFECT_ICE,
	MAGICEFFECT_WIND,
	MAGICEFFECT_WATER,
	MAGICEFFECT_EARTH,
	MAGICEFFECT_NORMAL,
	MAGICEFFECT_MAGIC_L,
	MAGICEFFECT_MAGIC_R,
	MAGICEFFECT_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMagiceffect(int type);
void UninitMagiceffect(void);
void UpdateMagiceffect(void);
void DrawMagiceffect(void);
MAGICEFFECT *GetMagiceffect(int no);
void SetMagiceffect(int nMagicEffect);
void ReleaseMagiceffect(void);

#endif
