//=============================================================================
//
// RETICLE処理 [reticle.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_RETICLE		(2)	// ポリゴン数

#define TEXTURE_GAME_RETICLE00		("data/TEXTURE/reticle00.png")	// RETICLE用画像
#define TEXTURE_GAME_RETICLE01		("data/TEXTURE/reticle01.png")
#define TEXTURE_GAME_RETICLE_CENTER	("data/TEXTURE/reticlecenter.png")


#define TEXTURE_RETICLE_SIZE_X	(400 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_RETICLE_SIZE_Y	(400 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_RETICLE_CENTER_SIZE_X	(32 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_RETICLE_CENTER_SIZE_Y	(32 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_RETICLE_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_RETICLE_G			(1.0f) // テクスチャカラー緑
#define TEXTURE_RETICLE_B			(1.0f) // テクスチャカラー青
#define TEXTURE_RETICLE_A			(1.0f) // テクスチャカラー青

#define TEXTURE_RETICLE_CHANGE_COLLAR		(0.005f) // テクスチャカラー変更速度

#define TEXTURE_PATTERN_DIVIDE_X_RETICLE	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_RETICLE	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_RETICLE			(TEXTURE_PATTERN_DIVIDE_X_RETICLE*TEXTURE_PATTERN_DIVIDE_Y_RETICLE)	// アニメーションパターン数
#define TIME_ANIMATION_RETICLE			(4)	// アニメーションの切り替わるカウント

#define RETICLE_MAX					(3)	// RETICLEの最大数
#define RETICLE_ROTATION_SPEED			(0.002f)	// 回転速度

#define RETICLE_RESULT_SCALE		(0.7f)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// RETICLEの移動量
	D3DXVECTOR3				rot;					// RETICLEの回転量
	D3DXVECTOR2				size;					// RETICLEのテクスチャサイズ(x,y)
	float					fScale;					// スケール
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	D3DXCOLOR				colorMagic;				// 色管理
	int						nCollarFlag;			// 色変更フラグ

	bool					bUse;					// true:使用  false:未使用


}RETICLE;

enum
{
	RETICLE_1,
	RETICLE_2,
	RETICLE_CENTER
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitReticle(int type);
void UninitReticle(void);
void UpdateReticle(void);
void DrawReticle(void);
RETICLE *GetReticle(int no);
void SetReticle(void);
void ReleaseReticle(void);

#endif
