//=============================================================================
//
// モデル処理 [skydome.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _SKYDOME_H_
#define _SKYDOME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SKYDOME_MODEL		"data/MODEL/Skydome/skydome.x"	// 読み込むモデル名
//#define	SKYDOME_TEXTURE		"data/MODEL/Skydome/incskies_014_8k.png"	// 読み込むモデル名

#define	SKYDOME_TEXTURE		"data/MODEL/Skydome/QQ101.png"	// 読み込むモデル名

#define SKYDOME_MAX			(1)
//#define SKYDOME_HEIGHT		(200.0f)
#define SKYDOME_HEIGHT		(-200.0f)

#define SKYDOME_SCALE		(30.0f)
#define SKYDOME_ROT_SPEED	(0.00001f)

typedef struct
{
	D3DXVECTOR3		posSkydome;			// 地面の位置
	D3DXVECTOR3		rotSkydome;			// 地面の向き(回転)
	D3DXVECTOR3		sclSkydome;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス

}SKYDOME;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSkydome(int nType);
void UninitSkydome(void);
void UpdateSkydome(void);
void DrawSkydome(void);
SKYDOME *GetSkydome(int no);

#endif
