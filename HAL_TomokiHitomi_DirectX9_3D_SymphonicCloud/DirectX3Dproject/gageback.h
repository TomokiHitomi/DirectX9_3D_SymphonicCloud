//=============================================================================
//
// GAGEBACK処理 [gageback.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _GAGEBACK_H_
#define _GAGEBACK_H_

#include "main.h"

// マクロ定義
#define NUM_GAGEBACK		(2)	// ポリゴン数

#define TEXTURE_GAME_GAGEBACK00	("data/TEXTURE/gageback00.png")	// GAGEBACK用画像
#define TEXTURE_GAME_GAGEBACK01	("data/TEXTURE/gageback01.png")	// GAGEBACK用画像
#define TEXTURE_GAME_GAGEBACK02	("data/TEXTURE/gageback02.png")	// GAGEBACK用画像

// マジックフレーム
#define TEXTURE_GAGEBACK_SIZE_X	(260 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEBACK_SIZE_Y	(390 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEBACK_POS_X	(121 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGEBACK_POS_Y	(172 * SCREEN_SCALE)	// 同上

// ボスフレーム
#define TEXTURE_GAGEBACK_BOSS_SIZE_X	(640 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEBACK_BOSS_SIZE_Y	(100 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEBACK_BOSS_POS_X	(SCREEN_CENTER_X)	// テクスチャポジション
#define TEXTURE_GAGEBACK_BOSS_POS_Y	(36 * SCREEN_SCALE)	// 同上

// プレイヤーフレーム
#define TEXTURE_GAGEBACK_MODEL_SIZE_X	(640 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEBACK_MODEL_SIZE_Y	(100 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEBACK_MODEL_POS_X	(SCREEN_CENTER_X)	// テクスチャポジション
#define TEXTURE_GAGEBACK_MODEL_POS_Y	(662.5 * SCREEN_SCALE)	// 同上

// 通常バレット追加
#define TEXTURE_GAGEBACK_BULLET_SIZE_X	(260 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGEBACK_BULLET_SIZE_Y	(65 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_GAGEBACK_BULLET_POS_X	(1159 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGEBACK_BULLET_POS_Y	(308 * SCREEN_SCALE)	// 同上


#define TEXTURE_GAGEBACK_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_GAGEBACK_G			(1.0f) // テクスチャカラー緑
#define TEXTURE_GAGEBACK_B			(1.0f) // テクスチャカラー青
#define TEXTURE_GAGEBACK_A			(0.4f) // テクスチャα値

#define TEXTURE_PATTERN_DIVIDE_X_GAGEBACK	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GAGEBACK			(TEXTURE_PATTERN_DIVIDE_X_GAGEBACK*TEXTURE_PATTERN_DIVIDE_Y_GAGEBACK)	// アニメーションパターン数
#define TIME_ANIMATION_GAGEBACK			(4)	// アニメーションの切り替わるカウント

#define GAGEBACK_MAX					(4)	// GAGEBACKの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// GAGEBACKの移動量
	D3DXVECTOR3				rot;					// GAGEBACKの回転量
	D3DXVECTOR2				size;					// GAGEBACKのテクスチャサイズ(x,y)
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
	bool					bPatternFlag;			// テクスチャ座標分割フラグ
}GAGEBACK;

/*******************************************************************************
* 構造体定義
*******************************************************************************/
enum
{
	GAGEBACK_MAGIC,
	GAGEBACK_DEFAULT,
	GAGEBACK_BULLET,
	GAGEBACK_TEXTURE_MAX
};

enum
{
	GAGEBACK_TYPE_MAGIC,
	GAGEBACK_TYPE_BOSS,
	GAGEBACK_TYPE_MODEL,
	GAGEBACK_TYPE_BULLET,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGageback(int type);
void UninitGageback(void);
void UpdateGageback(void);
void DrawGageback(void);
GAGEBACK *GetGageback(int no);
void SetGageback(int nGage);
void ReleaseGageback(void);

#endif
