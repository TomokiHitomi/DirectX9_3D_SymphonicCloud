//=============================================================================
//
// GAGEFREAM処理 [gagefream.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _GAGEFREAM_H_
#define _GAGEFREAM_H_

#include "main.h"

// マクロ定義
#define NUM_GAGEFREAM		(2)	// ポリゴン数

#define TEXTURE_GAME_GAGEFREAM00	("data/TEXTURE/gagefream00.png")	// GAGEFREAM用画像
#define TEXTURE_GAME_GAGEFREAM01	("data/TEXTURE/gagefream01.png")	// GAGEFREAM用画像
#define TEXTURE_GAME_GAGEFREAM02	("data/TEXTURE/gagefream02.png")	// GAGEFREAM用画像

// マジックフレーム
#define TEXTURE_GAGEFREAM_SIZE_X	(260 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEFREAM_SIZE_Y	(390 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEFREAM_POS_X	(121 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGEFREAM_POS_Y	(172 * SCREEN_SCALE)	// 同上

// ボスフレーム
#define TEXTURE_GAGEFREAM_BOSS_SIZE_X	(640 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEFREAM_BOSS_SIZE_Y	(100 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEFREAM_BOSS_POS_X	(SCREEN_CENTER_X)	// テクスチャポジション
#define TEXTURE_GAGEFREAM_BOSS_POS_Y	(36 * SCREEN_SCALE)	// 同上

// プレイヤーフレーム
#define TEXTURE_GAGEFREAM_MODEL_SIZE_X	(640 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEFREAM_MODEL_SIZE_Y	(100 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEFREAM_MODEL_POS_X	(SCREEN_CENTER_X)	// テクスチャポジション
#define TEXTURE_GAGEFREAM_MODEL_POS_Y	(662.5 * SCREEN_SCALE)	// 同上

// 通常バレット追加
#define TEXTURE_GAGEFREAM_BULLET_SIZE_X	(260 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEFREAM_BULLET_SIZE_Y	(65 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEFREAM_BULLET_POS_X	(1159 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGEFREAM_BULLET_POS_Y	(308 * SCREEN_SCALE)	// 同上


#define TEXTURE_GAGEFREAM_R			(255) // テクスチャカラー赤
#define TEXTURE_GAGEFREAM_G			(255) // テクスチャカラー緑
#define TEXTURE_GAGEFREAM_B			(255) // テクスチャカラー青

#define TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GAGEFREAM			(TEXTURE_PATTERN_DIVIDE_X_GAGEFREAM*TEXTURE_PATTERN_DIVIDE_Y_GAGEFREAM)	// アニメーションパターン数
#define TIME_ANIMATION_GAGEFREAM			(4)	// アニメーションの切り替わるカウント

#define GAGEFREAM_MAX					(4)	// GAGEFREAMの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// GAGEFREAMの移動量
	D3DXVECTOR3				rot;					// GAGEFREAMの回転量
	D3DXVECTOR2				size;					// GAGEFREAMのテクスチャサイズ(x,y)
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
	bool					bPatternFlag;			// テクスチャ座標分割フラグ
}GAGEFREAM;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum
{
	GAGEFREAM_MAGIC,
	GAGEFREAM_DEFAULT,
	GAGEFREAM_BULLET,
	GAGEFREAM_TEXTURE_MAX
};
enum
{
	GAGEFREAM_TYPE_MAGIC,
	GAGEFREAM_TYPE_BOSS,
	GAGEFREAM_TYPE_MODEL,
	GAGEFREAM_TYPE_BULLET,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGagefream(int type);
void UninitGagefream(void);
void UpdateGagefream(void);
void DrawGagefream(void);
GAGEFREAM *GetGagefream(int no);
void SetGagefream(int nGage);
void ReleaseGagefream(void);

#endif
