//=============================================================================
//
// モデル処理 [cloudfield.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _CLOUDFIELD_H_
#define _CLOUDFIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	CLOUDFIELD_MODEL		"data/MODEL/FIELD/cloudfield.x"	// 読み込むモデル名
#define	CLOUDFIELD_MODEL		"data/MODEL/FIELD/cloudfield.x"	// 読み込むモデル名

#define	CLOUDFIELD_TEXTURE		"data/TEXTURE/cloud.png"	// 読み込むモデル名

#define CLOUDFIELD_MAX			(1)
#define CLOUDFIELD_HEIGHT		(0.0f)

#define CLOUDFIELD_SCALE		(1.0f)
#define CLOUDFIELD_ROT_SPEED	(0.00001f)

typedef struct
{
	D3DXVECTOR3		posCloudfield;			// 地面の位置
	D3DXVECTOR3		rotCloudfield;			// 地面の向き(回転)
	D3DXVECTOR3		sclCloudfield;			// 地面の大きさ(スケール)

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス

}CLOUDFIELD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCloudfield(int nType);
void UninitCloudfield(void);
void UpdateCloudfield(void);
void DrawCloudfield(void);
CLOUDFIELD *GetCloudfield(int no);
LPD3DXBASEMESH GetCloudfieldMesh(void);

#endif
