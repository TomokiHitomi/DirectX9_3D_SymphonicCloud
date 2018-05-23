//=============================================================================
//
// エネミーモデル（ボス）処理 [enemy_boss.h]
// Author : GP12A295 25 人見友基 
//
//=============================================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	ENEMY_BOSS_MODEL	"data/MODEL/iv_grimreaper/grimreaper.x"			// 読み込むモデル名
#define ENEMY_BOSS_MAX			(1)
#define ENEMY_BOSS_SCALE		(0.4f)
#define ENEMY_BOSS_SIZE			(50.0f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// 地面の位置
	D3DXVECTOR3		rotEnemy;			// 地面の向き(回転)
	D3DXVECTOR3		sclEnemy;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス

	bool			bUse;
}ENEMY_BOSS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy_boss(int nType);
void UninitEnemy_boss(void);
void UpdateEnemy_boss(void);
void DrawEnemy_boss(void);
int SetEnemy_boss(void);
void SetEnemy_bossPos(int nIndex, D3DXVECTOR3 pos, float rot);
void ReleaseEnemy_boss(int nIndex);
void SetWaveEnemy(int no);
ENEMY_BOSS *GetEnemy_boss(int no);

#endif
