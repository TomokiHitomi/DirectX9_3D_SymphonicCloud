//=============================================================================
//
// リザルト処理 [result.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_RESULT		(2)	// ポリゴン数

#define TEXTURE_GAME_RESULT00	("data/TEXTURE/result.png")	// リザルト00用画像
#define TEXTURE_RESULT00_SIZE_X	(350 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_RESULT00_SIZE_Y	(250 * SCREEN_SCALE/2) // 同上
#define RESULT_POS_X			(SCREEN_CENTER_X)	// 初期座標X
#define RESULT_POS_Y			(100 * SCREEN_SCALE)	// 初期座標Y

#define TEXTURE_PATTERN_DIVIDE_X_RESULT	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_RESULT	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_RESULT			(TEXTURE_PATTERN_DIVIDE_X_RESULT*TEXTURE_PATTERN_DIVIDE_Y_RESULT)	// アニメーションパターン数
#define TIME_ANIMATION_RESULT				(4)	// アニメーションの切り替わるカウント

#define RESULT_MAX					(1)		// リザルトの最大数
#define RESULT_MOVE_UI				(250 * SCREEN_SCALE)	// リザルト時の他UI移動


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// リザルトの移動量
	D3DXVECTOR3				rot;					// リザルトの回転量
	D3DXVECTOR3				vecPosMove;				// リザルト時のUI移動量
	D3DXVECTOR2				size;					// リザルトのテクスチャサイズ(x,y)
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bPosMoveFlag;			// 他UIの移動フラグ
	bool					bUse;					// true:使用  false:未使用
}RESULT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(int nType);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT *GetResult(int no);


#endif
