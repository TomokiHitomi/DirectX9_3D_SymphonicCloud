//=============================================================================
//
// タイトル処理 [pausemenu.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_PAUSEMENU		(2)	// ポリゴン数

#define TEXTURE_GAME_PAUSEMENU00	_T("data/TEXTURE/pausemenu.png")	// タイトル00用画像
#define TEXTURE_PAUSEMENU00_SIZE_X	(300 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_PAUSEMENU00_SIZE_Y	(100 * SCREEN_SCALE/2) // 同上

#define PAUSEMENU_POS_X			(SCREEN_CENTER_X)	// 初期座標X
#define PAUSEMENU_POS_Y			(285 * SCREEN_SCALE)				// 初期座標Y
#define PAUSEMENU_SPACING		(80 * SCREEN_SCALE)

#define TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU	(3)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_PAUSEMENU			(TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU*TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU)	// アニメーションパターン数
#define TIME_ANIMATION_PAUSEMENU				(1)	// アニメーションの切り替わるカウント

#define PAUSEMENU_MAX					(3)	// タイトルの最大数
#define PAUSEMENU_SELECT_PRESS			(40)	// ポーズメニュー自動送り開始タイム
#define PAUSEMENU_SELECT_SPEED			(7)		// ポーズメニュ－自動送り速度

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
}PAUSEMENU;

typedef struct
{
	bool					bSelect;
}PAUSEMENU_SYS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPausemenu(int type);
void UninitPausemenu(void);
void UpdatePausemenu(void);
void DrawPausemenu(void);
void SetPausemenu(bool bUse);
PAUSEMENU *GetPausemenu(int no);


#endif
