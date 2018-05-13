//=============================================================================
//
// GAGESELECT処理 [gageselect.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _GAGESELECT_H_
#define _GAGESELECT_H_

#include "main.h"

// マクロ定義
#define NUM_GAGESELECT		(2)	// ポリゴン数

#define TEXTURE_GAME_GAGESELECT	("data/TEXTURE/gageselect.png")	// GAGESELECT用画像
#define TEXTURE_GAGESELECT_SIZE_X	(260 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_GAGESELECT_SIZE_Y	(65 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_GAGESELECT_R			(255) // テクスチャカラー赤
#define TEXTURE_GAGESELECT_G			(255) // テクスチャカラー緑
#define TEXTURE_GAGESELECT_B			(255) // テクスチャカラー青

// マジックバレットPOS
#define TEXTURE_GAGESELECT_MAGIC_POS_X	(121 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGESELECT_MAGIC_POS_Y	(35.5f * SCREEN_SCALE)	// 同上
#define TEXTURE_GAGESELECT_MAGIC_INTERVAL	(54.55f * SCREEN_SCALE)

// 通常バレットPOS
#define TEXTURE_GAGESELECT_BULLET_POS_X	(1159 * SCREEN_SCALE)	// テクスチャポジション
#define TEXTURE_GAGESELECT_BULLET_POS_Y	(308 * SCREEN_SCALE)	// 同上

#define TEXTURE_GAGESELECT_CHANGE_COLLAR	(0.05f)
#define TEXTURE_GAGESELECT_MOVESPEED		(0.1f)

#define TEXTURE_PATTERN_DIVIDE_X_GAGESELECT	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GAGESELECT			(TEXTURE_PATTERN_DIVIDE_X_GAGESELECT*TEXTURE_PATTERN_DIVIDE_Y_GAGESELECT)	// アニメーションパターン数
#define TIME_ANIMATION_GAGESELECT			(4)	// アニメーションの切り替わるカウント

#define GAGESELECT_MAX					(2)	// GAGESELECTの最大数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// GAGESELECTの移動量
	D3DXVECTOR3				rot;					// GAGESELECTの回転量
	D3DXVECTOR2				size;					// GAGESELECTのテクスチャサイズ(x,y)
	D3DXCOLOR				xColor;

	int						nCollarFlag;
	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用
}GAGESELECT;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum 
{
	GAGESELECT_MAGIC,
	GAGESELECT_BULLET
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGageselect(int type);
void UninitGageselect(void);
void UpdateGageselect(void);
void DrawGageselect(void);
GAGESELECT *GetGageselect(int no);
void SetGageselect(void);
void ReleaseGageselect(void);

#endif
