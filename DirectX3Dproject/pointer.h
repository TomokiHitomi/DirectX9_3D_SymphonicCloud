//=============================================================================
//
// POINTER処理 [pointer.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _POINTER_H_
#define _POINTER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POINTER		(2)	// ポリゴン数

#define TEXTURE_GAME_POINTER		("data/TEXTURE/pointer.png")	// POINTER用画像
#define TEXTURE_GAME_OUTLINE		("data/TEXTURE/outline.png")	// OUTLINE用画像
#define TEXTURE_GAME_POINTER_MAX	(2)

#define TEXTURE_POINTER_SIZE_X	(13 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_POINTER_SIZE_Y	(13 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_POINTER_POS_X	(SCREEN_WIDTH) // テクスチャサイズ
#define TEXTURE_POINTER_POS_Y	(SCREEN_HEIGHT) // 同上

#define TEXTURE_POINTER_R			(1.0f) // テクスチャカラー赤
#define TEXTURE_POINTER_G			(1.0f) // テクスチャカラー緑
#define TEXTURE_POINTER_B			(1.0f) // テクスチャカラー青
#define TEXTURE_POINTER_A			(1.0f) // テクスチャカラー青

#define TEXTURE_POINTER_CHANGE_COLLAR		(0.005f) // テクスチャカラー変更速度

#define TEXTURE_PATTERN_DIVIDE_X_POINTER	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_POINTER	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_POINTER			(TEXTURE_PATTERN_DIVIDE_X_POINTER*TEXTURE_PATTERN_DIVIDE_Y_POINTER)	// アニメーションパターン数
#define TIME_ANIMATION_POINTER			(1)	// アニメーションの切り替わるカウント

#define POINTER_MAX					(30)	// POINTERの最大数

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// POINTERの移動量
	D3DXVECTOR3				rot;					// POINTERの回転量
	D3DXVECTOR2				size;					// POINTERのテクスチャサイズ(x,y)
	D3DXCOLOR				colorPointer;			// 色管理

	int						nCollarFlag;			// 色変更フラグ
	int						nTex;					// 使用テクスチャ

	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}POINTER;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/

enum
{
	POINTER_TEXTURE_NORMAIL,
	POINTER_TEXTURE_OUTLINE
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPointer(int type);
void UninitPointer(void);
void UpdatePointer(void);
void DrawPointer(void);
POINTER *GetPointer(int no);
int SetPointer(D3DXCOLOR colorfloat);
void SetUpdatePointer(int nPointer, int nTex, D3DXVECTOR3 pos, float fAngle);
void ReleasePointer(int nPointer);
#endif
