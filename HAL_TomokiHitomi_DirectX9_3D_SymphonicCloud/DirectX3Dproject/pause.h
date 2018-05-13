//=============================================================================
//
// タイトル処理 [pause.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_PAUSE		(2)	// ポリゴン数

#define TEXTURE_GAME_PAUSE00	_T("data/TEXTURE/pause.png")	// タイトル00用画像
#define TEXTURE_PAUSE00_SIZE_X	(450 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_PAUSE00_SIZE_Y	(250 * SCREEN_SCALE/2) // 同上
#define PAUSE_POS_X			(SCREEN_CENTER_X)	// 初期座標X
#define PAUSE_POS_Y			(100 * SCREEN_SCALE)				// 初期座標Y

#define TEXTURE_PATTERN_DIVIDE_X_PAUSE	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_PAUSE	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_PAUSE			(TEXTURE_PATTERN_DIVIDE_X_PAUSE*TEXTURE_PATTERN_DIVIDE_Y_PAUSE)	// アニメーションパターン数
#define TIME_ANIMATION_PAUSE				(4)	// アニメーションの切り替わるカウント

#define PAUSE_MAX					(1)	// タイトルの最大数

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// タイトルの移動量
	D3DXVECTOR3				rot;					// タイトルの回転量
	D3DXVECTOR2				size;					// タイトルのテクスチャサイズ(x,y)
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径

	bool					bUse;					// true:使用  false:未使用
	bool					bStop;					// 一時停止
}PAUSE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPause(int type);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
bool GetPauseFlag(void);
PAUSE *GetPause(int no);


#endif
