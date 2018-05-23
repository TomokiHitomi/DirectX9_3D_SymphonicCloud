//=============================================================================
//
// モデル処理 [skydomeeffect.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _SKYDOMEEFFECT_H_
#define _SKYDOMEEFFECT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SKYDOMEEFFECT_MODEL		"data/MODEL/Skydome/Dome_EE401.x"	// 読み込むモデル名
#define	SKYDOMEEFFECT_TEXTURE	"data/MODEL/Skydome/EE401.png"	// 読み込むモデル名

#define SKYDOMEEFFECT_MAX			(1)
#define SKYDOMEEFFECT_HEIGHT		(200.0f)

#define SKYDOMEEFFECT_SCALE		(3.0f)
#define SKYDOMEEFFECT_ROT_SPEED	(-0.0003f)

typedef struct
{
	D3DXVECTOR3		posSkydomeeffect;			// 地面の位置
	D3DXVECTOR3		rotSkydomeeffect;			// 地面の向き(回転)
	D3DXVECTOR3		sclSkydomeeffect;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス

}SKYDOMEEFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSkydomeeffect(int nType);
void UninitSkydomeeffect(void);
void UpdateSkydomeeffect(void);
void DrawSkydomeeffect(void);
SKYDOMEEFFECT *GetSkydomeeffect(int no);

#endif
