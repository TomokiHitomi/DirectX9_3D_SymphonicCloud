//=============================================================================
//
// TIMEFREAM処理 [timefream.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _TIMEFREAM_H_
#define _TIMEFREAM_H_

#include "main.h"

// マクロ定義
#define NUM_TIMEFREAM		(2)	// ポリゴン数

#define TEXTURE_GAME_TIMEFREAMBACK	("data/TEXTURE/timeback.png")	// TIMEFREAM用画像
#define TEXTURE_GAME_TIMEFREAMFREAM	("data/TEXTURE/timefream.png")	// TIMEFREAM用画像

#define TEXTURE_TIMEFREAM_SIZE_X	(253 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_TIMEFREAM_SIZE_Y	(100 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_TIMEFREAM_POS_X	(SCREEN_CENTER_X)	// テクスチャポジション
#define TEXTURE_TIMEFREAM_POS_Y	(112 * SCREEN_SCALE)	// 同上

#define TEXTURE_TIMEFREAM_RESULT_POS_Y	(580 * SCREEN_SCALE)	// 同上

#define TEXTURE_TIMEFREAM_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_TIMEFREAM_G			(1.0f) // テクスチャカラー緑
#define TEXTURE_TIMEFREAM_B			(1.0f) // テクスチャカラー青
#define TEXTURE_TIMEFREAM_A			(0.4f) // テクスチャα値

#define TEXTURE_TIMEFREAM_CHANGE_COLLAR		(0.005f)

#define TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_TIMEFREAM			(TEXTURE_PATTERN_DIVIDE_X_TIMEFREAM*TEXTURE_PATTERN_DIVIDE_Y_TIMEFREAM)	// アニメーションパターン数
#define TIME_ANIMATION_TIMEFREAM			(4)	// アニメーションの切り替わるカウント

#define TIMEFREAM_MAX					(2)	// TIMEFREAMの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXCOLOR				xColor;					// カラー
	D3DXVECTOR3				pos;					// TIMEFREAMの移動量
	D3DXVECTOR3				rot;					// TIMEFREAMの回転量
	D3DXVECTOR2				size;					// TIMEFREAMのテクスチャサイズ(x,y)
	int						nPatternAnim;			// アニメーションパターンナンバー
	int						nCollarFlag;			// カラー移行フラグ
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
	bool					bPatternFlag;			// テクスチャ座標分割フラグ
}TIMEFREAM;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum
{
	TIMEFREAM_BACK,
	TIMEFREAM_FREAM,
	TIMEFREAM_TEXTURE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimefream(int type);
void UninitTimefream(void);
void UpdateTimefream(void);
void DrawTimefream(void);
TIMEFREAM *GetTimefream(int no);
void SetTimefream(int nGage);
void ReleaseTimefream(void);

#endif
