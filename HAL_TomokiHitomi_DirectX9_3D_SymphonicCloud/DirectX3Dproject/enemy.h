//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : GP11B243 24 人見友基 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SYSTEM_FILE			("data/FILE/enemy_system.csv")
#define ENEMY_SYSTEM_DATA_MAX		(8)

#define ENEMY_WAVE_MAX				(7)		// 最大ウェーブ数

//#define	ENEMY_CUBE		"data/MODEL/cube.x"							// 読み込むモデル名

#define ENEMY_MAX		(100)

#define ENEMY_SCALE		(0.2f)
#define ENEMY_SIZE		(25.0f)

#define ENEMY_HEIGHT	(30.0f)
#define ENEMY_CENTER	(25.0f)

// エネミーのバレット発射位置
#define ENEMY_BULLET_MUZZELE_HEIGHT	(35.0f)				// 高さ
#define ENEMY_BULLET_MUZZELE_LENGTH	(1.0f)				// 距離
#define ENEMY_BULLET_MUZZELE_ANGLE	(D3DX_PI * 1.5f)

#define ENEMY_ATTACK_NORMAL_INTERVAL	(20)
#define ENEMY_ATTACK_SPECIAL_INTERVAL	(100)

#define ENEMY_MOVE_SPEED	(2)
#define ENEMY_MOVE_SLANT	(ENEMY_MOVE_SPEED * 0.71)

#define ENEMY_ACTIVE_COUNT_MAX	(5)


// エネミーのステータス
#define ENEMY_STATUS_HP	(50.0f)
#define ENEMY_STATUS_BOSS_HP	(600.0f)

typedef struct
{
	D3DXVECTOR3		posEnemy;			// 地面の位置
	D3DXVECTOR3		rotEnemy;			// 地面の向き(回転)
	D3DXVECTOR3		sclEnemy;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス

	int				nAttackNorCount;
	int				nAttackSpCount;
	int				nAttackPattern;

	float			fStatusHP;
	float			fAngle;
	float			fMoveSpeed;

	bool			bUse;
	bool			bShadow;
	bool			bParameter;

	int				nWave;		// 出現ウェーブ
	int				nMoveType;
	int				nAttackType;

	// モデル
	int				nModel;
	int				nModelIndex;
	bool			bModelSet;

	// シャドウ用
	int				nIdxShadow;
	float			fSizeShadow;
	D3DXCOLOR		colShadow;

	// ロックオン用
	bool			bLockon;
	int				nIdxLockon;

	// ミニマップ用
	bool			bUseMinimap;
}ENEMY;

typedef struct
{
	int					nActiveEnemy[ENEMY_ACTIVE_COUNT_MAX];
	int					nActiveCount;
}ENEMY_SYS;

enum
{
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_BOSS
};

enum
{
	ENEMY_MOVE_TRACK
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int nType);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemyFile(void);
ENEMY *GetEnemy(int no);
ENEMY_SYS *GetEnemySys(void);
void SetWaveEnemy(int no);
void ReleaseEnemy(int nEnemy);

#endif
