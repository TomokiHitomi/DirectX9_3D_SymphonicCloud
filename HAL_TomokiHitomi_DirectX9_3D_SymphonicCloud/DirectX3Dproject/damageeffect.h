//=============================================================================
//
// DAMAGEEFFECT処理 [damageeffect.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _DAMAGEEFFECT_H_
#define _DAMAGEEFFECT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_DAMAGEEFFECT		(2)	// ポリゴン数

#define TEXTURE_GAME_DAMAGEEFFECT	("data/TEXTURE/damageeffect.jpg")	// DAMAGEEFFECT用画像
#define TEXTURE_DAMAGEEFFECT_SIZE_X	(1280 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_DAMAGEEFFECT_SIZE_Y	(720 * SCREEN_SCALE/2) // 同上

#define DAMAGEEFFECT_POS_X				(SCREEN_CENTER_X)			// 初期座標X
#define DAMAGEEFFECT_POS_Y				(SCREEN_CENTER_Y)			// 初期座標Y

#define TEXTURE_DAMAGEEFFECT_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_DAMAGEEFFECT_G			(0.0f) // テクスチャカラー緑
#define TEXTURE_DAMAGEEFFECT_B			(0.0f) // テクスチャカラー青
#define TEXTURE_DAMAGEEFFECT_A			(0.0f) // テクスチャカラーα

#define TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_DAMAGEEFFECT			(TEXTURE_PATTERN_DIVIDE_X_DAMAGEEFFECT*TEXTURE_PATTERN_DIVIDE_Y_DAMAGEEFFECT)	// アニメーションパターン数
#define TIME_ANIMATION_DAMAGEEFFECT				(4)	// アニメーションの切り替わるカウント

#define DAMAGEEFFECT_MAX					(1)	// DAMAGEEFFECTの最大数

#define DAMAGEEFFECT_ALPHA_UP				(0.1f)	// α値の上昇速度
#define DAMAGEEFFECT_ALPHA_DOWN				(0.01f)	// α値の下降速度

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// DAMAGEEFFECTの移動量
	D3DXVECTOR3				rot;					// DAMAGEEFFECTの回転量
	D3DXVECTOR2				size;					// DAMAGEEFFECTのテクスチャサイズ(x,y)
	D3DXCOLOR				xColor;				// 色管理
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
	bool					bActive;				// 有効フラグ
}DAMAGEEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDamageeffect(int type);
void UninitDamageeffect(void);
void UpdateDamageeffect(void);
void DrawDamageeffect(void);
DAMAGEEFFECT *GetDamageeffect(int no);
void SetDamageeffect(void);

#endif
