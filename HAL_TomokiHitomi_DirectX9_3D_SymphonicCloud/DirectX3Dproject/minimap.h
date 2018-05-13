//=============================================================================
//
// MINIMAP処理 [minimap.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_MINIMAP		(2)	// ポリゴン数

#define TEXTURE_GAME_MINIMAP00	("data/TEXTURE/minimap00.png")	// MINIMAP用画像
#define TEXTURE_GAME_MINIMAP01	("data/TEXTURE/minimap01.png")	// MINIMAP用画像

#define TEXTURE_MINIMAP_SIZE_X	(250 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_MINIMAP_SIZE_Y	(250 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_MINIMAP_POS_X	(1150 * SCREEN_SCALE) // テクスチャポジション
#define TEXTURE_MINIMAP_POS_Y	(130 * SCREEN_SCALE) // 同上

#define TEXTURE_MINIMAP_CHANGE_COLLAR		(0.005f) // テクスチャカラー変更速度

#define TEXTURE_PATTERN_DIVIDE_X_MINIMAP	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_MINIMAP	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_MINIMAP			(TEXTURE_PATTERN_DIVIDE_X_MINIMAP*TEXTURE_PATTERN_DIVIDE_Y_MINIMAP)	// アニメーションパターン数
#define TIME_ANIMATION_MINIMAP			(1)	// アニメーションの切り替わるカウント

#define MINIMAP_MAX					(2)		// MINIMAPの最大数
#define MINIMAP_SYS_MAX				(30)	// MINIMAP_SYSの最大数
#define MINIMAP_POS_CONVERT			(10 / SCREEN_SCALE)
#define MINIMAP_POS_LENGTH_MAX		(TEXTURE_MINIMAP_SIZE_X * 0.8f)

#define MINIMAP_ALPHA			(0.1f)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// MINIMAPの移動量
	D3DXVECTOR3				rot;					// MINIMAPの回転量
	D3DXVECTOR2				size;					// MINIMAPのテクスチャサイズ(x,y)
	D3DXCOLOR				colorMinimap;				// 色管理
	int						nCollarFlag;			// 色変更フラグ

	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}MINIMAP;

typedef struct
{
	int						nType;
	int						nIndexTag;
	int						nIndexPointer;
	bool					bUse;					// true:使用  false:未使用
}MINIMAP_SYS;

//*****************************************************************************
// 列挙型定義
//*****************************************************************************
enum
{
	MINIMAP_BACK,
	MINIMAP_MAIN
};

enum
{
	MINIMAP_MODEL,
	MINIMAP_ENEMY
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMinimap(int type);
void UninitMinimap(void);
void UpdateMinimap(void);
void DrawMinimap(void);
MINIMAP *GetMinimap(int no);
void SetMinimap(void);
void ReleaseMinimap(void);

#endif
