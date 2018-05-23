//=============================================================================
//
// RANKING処理 [ranking.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

// マクロ定義
#define NUM_RANKING		(2)	// ポリゴン数

#define TEXTURE_GAME_RANKINGBACK	("data/TEXTURE/rankingback.png")	// RANKING用画像
#define TEXTURE_GAME_RANKINGFREAM	("data/TEXTURE/rankingfream.png")	// RANKING用画像

#define TEXTURE_RANKING_SIZE_X	(425 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_RANKING_SIZE_Y	(587 * SCREEN_SCALE / 2) // 同上
#define TEXTURE_RANKING_POS_X	(1035 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_RANKING_POS_Y	(SCREEN_CENTER_Y)	// 同上

#define TEXTURE_RANKING_R			(255) // テクスチャカラー赤
#define TEXTURE_RANKING_G			(255) // テクスチャカラー緑
#define TEXTURE_RANKING_B			(255) // テクスチャカラー青
#define TEXTURE_RANKING_A			(0.4f) // テクスチャα値

#define TEXTURE_RANKING_CHANGE_COLLAR		(0.01f)

#define TEXTURE_PATTERN_DIVIDE_X_RANKING	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_RANKING	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_RANKING			(TEXTURE_PATTERN_DIVIDE_X_RANKING*TEXTURE_PATTERN_DIVIDE_Y_RANKING)	// アニメーションパターン数
#define TIME_ANIMATION_RANKING			(4)	// アニメーションの切り替わるカウント

#define RANKING_MAX					(2)	// RANKINGの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXCOLOR				xColor;					// カラー
	D3DXVECTOR3				pos;					// RANKINGの移動量
	D3DXVECTOR3				rot;					// RANKINGの回転量
	D3DXVECTOR2				size;					// RANKINGのテクスチャサイズ(x,y)
	int						nPatternAnim;			// アニメーションパターンナンバー
	int						nCollarFlag;			// カラー移行フラグ
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
	bool					bPatternFlag;			// テクスチャ座標分割フラグ
}RANKING;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum
{
	RANKING_BACK,
	RANKING_FREAM,
	RANKING_TEXTURE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRanking(int type);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
D3DXCOLOR GetRankingColor(void);
RANKING *GetRanking(int no);
void SetRanking(int nGage);
void ReleaseRanking(void);

#endif
