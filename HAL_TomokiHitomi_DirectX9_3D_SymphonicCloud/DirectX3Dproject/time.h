//=============================================================================
//
// TIME処理 [time.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

// マクロ定義
#define NUM_TIME				(2)	// ポリゴン数

#define TEXTURE_GAME_TIME		("data/TEXTURE/number.png")	// サンプル用画像
#define TEXTURE_TIME_SIZE_X		(70 * SCREEN_SCALE)	// テクスチャサイズ
#define TEXTURE_TIME_SIZE_Y		(70 * SCREEN_SCALE)	// 同上

#define TEXTURE_TIME_SPAN		(45 * SCREEN_SCALE)	// 同上
#define TEXTURE_TIME_INTERVAL	(25 * SCREEN_SCALE)	// 同上

#define TIME_RESULT_POS_Y		(543 * SCREEN_SCALE)	// 同上

#define TIME_MAX				(9959)		// 最大秒数
#define TIME_SECOND				(60)		// 60秒
#define TIME_DIGIT				(4)			// 表示桁数

#define TIME_POS_X				(686 * SCREEN_SCALE)		// ポリゴンの初期位置X
#define TIME_POS_Y				(75 * SCREEN_SCALE)		// 同上


/*******************************************************************************
* 構造体定義
*******************************************************************************/


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTime(int type);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int add);

#endif
