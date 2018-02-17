//=============================================================================
//
// タイトル処理 [titlemenu.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_TITLEMENU		(2)	// ポリゴン数

#define TEXTURE_GAME_TITLEMENU	("data/TEXTURE/titlemenu.png")	// タイトル00用画像
#define TEXTURE_TITLEMENU00_SIZE_X	(300 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_TITLEMENU00_SIZE_Y	(100 * SCREEN_SCALE/2) // 同上
#define TITLEMENU_POS_X			(SCREEN_CENTER_X)	// 初期座標X
#define TITLEMENU_POS_Y			(320 * SCREEN_SCALE)				// 初期座標Y
#define TITLEMENU_SPACING		(70 * SCREEN_SCALE)

#define TEXTURE_PATTERN_DIVIDE_X_TITLEMENU	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU	(5)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_TITLEMENU			(TEXTURE_PATTERN_DIVIDE_X_TITLEMENU*TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU)	// アニメーションパターン数
#define TIME_ANIMATION_TITLEMENU				(1)	// アニメーションの切り替わるカウント

#define TITLEMENU_MAX					(5)	// タイトルの最大数
#define TITLEMENU_SELECT_PRESS			(40)	// メニュー自動送り開始タイム
#define TITLEMENU_SELECT_SPEED			(7)		// メニュ－自動送り速度

#define TITLEMENU_SYS_MAX				(1)	// タイトルシステムの最大数

enum
{
	TITLE_MENU_START,
	TITLE_MENU_TUTORIAL,
	TITLE_MENU_CREDIT,
	TITLE_MENU_OPTION,
	TITLE_MENU_EXIT
};

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
	float					fRadius;					// 半径
	bool					bUse;					// true:使用  false:未使用
}TITLEMENU;

typedef struct
{
	bool					bSelect;
}TITLEMENU_SYS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitlemenu(int type);
void UninitTitlemenu(void);
void UpdateTitlemenu(void);
void DrawTitlemenu(void);
void SetTitlemenu(void);
void ReleaseTitlemenu(void);
TITLEMENU *GetTitlemenu(int no);


#endif
