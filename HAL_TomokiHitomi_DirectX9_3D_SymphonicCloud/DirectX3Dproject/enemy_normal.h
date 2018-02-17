//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : GP11B243 24 人見友基 
//
//=============================================================================
#ifndef _ENEMY_NORMAL_H_
#define _ENEMY_NORMAL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_NORMAL_MODEL		"data/MODEL/cube.x"							// 読み込むモデル名

#define ENEMY_NORMAL_MAX		(30)
#define ENEMY_NORMAL_SCALE		(0.3f)
#define ENEMY_NORMAL_SIZE		(20.0f)
#define ENEMY_NORMAL_ROT		(0.02f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// 地面の位置
	D3DXVECTOR3		rotEnemy;			// 地面の向き(回転)
	D3DXVECTOR3		sclEnemy;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス

	bool			bUse;
}ENEMY_NORMAL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy_normal(int nType);
void UninitEnemy_normal(void);
void UpdateEnemy_normal(void);
void DrawEnemy_normal(void);
int SetEnemy_normal(void);
void SetEnemy_normalPos(int nIndex, D3DXVECTOR3 pos);
void SetEnemy_normalColor(int nColor);
void ReleaseEnemy_normal(int nIndex);
ENEMY_NORMAL *GetEnemy_normal(int no);

#endif
