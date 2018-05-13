//=============================================================================
//
// ウォール処理 [wall.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_WALL	"data/TEXTURE/wall000.jpg"						// 読み込むテクスチャファイル名

#define	WALL_SIZE_X		(100.0f)			// 地面のサイズ(X方向)
#define	WALL_SIZE_Z		(100.0f)			// 地面のサイズ(Z方向)

#define WALL_X				(15)
#define WALL_Y				(3)
#define WALL_MAX			(WALL_X * WALL_Y)	// フィールドの最大数

typedef struct
{
	D3DXVECTOR3				posWall;			// 地面の位置
	D3DXVECTOR3				rotWall;			// 地面の向き(回転)
	D3DXVECTOR3				sclWall;			// 地面の大きさ(スケール)

	D3DXMATRIX				mtxView;			// ビューマトリックス
	D3DXMATRIX				mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX				mtxWorld;			// ワールドマトリックス
}WALL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWall(int nType);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
WALL *GetWall(int no);

#endif
