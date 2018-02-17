//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : GP11B243 24 人見友基 
//
//=============================================================================
#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAGICCIRCLE_MODEL		"data/MODEL/magiccircle.x"							// 読み込むモデル名

#define MAGICCIRCLE_MAX					(5)
#define MAGICCIRCLE_SCALE				(18.0f)
#define MAGICCIRCLE_SCALE_Y				(23.0f)
#define MAGICCIRCLE_SIZE				(25.0f)
#define MAGICCIRCLE_ROT_Y				(0.03f)
#define MAGICCIRCLE_ROT					(0.003f)

#define MAGICCIRCLE_HEIGHT				(14.0f)
#define MAGICCIRCLE_HEIGHT_INTERVAL		(7.0f)

#define MAGICCIRCLE_SCALE_CHANGE		(0.7f)

#define MAGICCIRCLE_ALPHA					(1)

typedef struct
{
	D3DXVECTOR3		pos;			// 地面の位置
	D3DXVECTOR3		rot;			// 地面の向き(回転)
	D3DXVECTOR3		scl;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス

	bool			bUse;
	bool			bSe;
}MAGICCIRCLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMagiccircle(int nType);
void UninitMagiccircle(void);
void UpdateMagiccircle(void);
void DrawMagiccircle(void);
int SetMagiccircle(void);
void SetMagiccirclePos(int nIndex, D3DXVECTOR3 pos, float fRotY);
void ReleaseMagiccircle(int nIndex);
MAGICCIRCLE *GetMagiccircle(int no);

#endif
