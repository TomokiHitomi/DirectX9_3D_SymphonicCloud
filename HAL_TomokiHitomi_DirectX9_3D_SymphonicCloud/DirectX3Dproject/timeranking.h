//=============================================================================
//
// TIMERANKING処理 [timeranking.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _TIMERANKING_H_
#define _TIMERANKING_H_

// マクロ定義
#define NUM_TIMERANKING				(2)	// ポリゴン数
#define TIMERANKING_NUM_FILE			("data/FILE/timeranking")

#define TEXTURE_GAME_TIMERANKING		("data/TEXTURE/number.png")	// サンプル用画像
#define TEXTURE_TIMERANKING_SIZE_X		(70 * SCREEN_SCALE)	// テクスチャサイズ
#define TEXTURE_TIMERANKING_SIZE_Y		(70 * SCREEN_SCALE)	// 同上

#define TEXTURE_TIMERANKING_SPAN		(45 * SCREEN_SCALE)	// 同上
#define TEXTURE_TIMERANKING_INTERVAL_X	(25 * SCREEN_SCALE)	// 同上
#define TEXTURE_TIMERANKING_INTERVAL_Y	(110 * SCREEN_SCALE)	// 同上

#define TIMERANKING_RESULT_POS_Y		(543 * SCREEN_SCALE)	// 同上

#define TIMERANKING_SECOND_MAX			(9959)		// 最大秒数
#define TIMERANKING_SECOND				(60)		// 60秒
#define TIMERANKING_DIGIT				(4)			// 表示桁数

#define TIMERANKING_POS_X				(1155 * SCREEN_SCALE)		// ポリゴンの初期位置X
#define TIMERANKING_POS_Y				(105 * SCREEN_SCALE)		// 同上
#define TIMERANKING_MAX					(5)			// 最大秒数

#define TIMERANKING_NUM1				(330)
#define TIMERANKING_NUM2				(400)
#define TIMERANKING_NUM3				(430)
#define TIMERANKING_NUM4				(500)
#define TIMERANKING_NUM5				(530)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	VERTEX_2D				vertexWkTimeranking[TIMERANKING_DIGIT][NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				posTimeranking;						// ポリゴンの移動量
	int						nTimeranking;						// タイム
}TIMERANKING;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimeranking(int type);
void UninitTimeranking(void);
void UpdateTimeranking(void);
void DrawTimeranking(void);
void SetTimeranking(int nNum, int nTimeranking);

#endif
