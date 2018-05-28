//=============================================================================
//
// ライト処理 [light.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	LIGHT_MAX				(4)						// ライトの数

//#define LIGHT_DIFFUSE_0			(1.0f)
//#define LIGHT_DIFFUSE_1			(0.2f)
#define LIGHT_DIFFUSE_2			(0.1f)
//#define LIGHT_AMBIENT			(0.6f)

#define LIGHT_DIFFUSE_0			(0.2f)
#define LIGHT_DIFFUSE_1			(0.2f)
#define LIGHT_AMBIENT			(0.1f)


#define LIGHT_SUB_DIFFUSE		(0.3f)
#define LIGHT_SUB_XZ			(0.8)
#define LIGHT_SUB_Y				(1.0f)

#define LIGHT_AMBIENT_POINT		(0.1f)
#define LIGHT_AMBIENT_SPOT		(0.3f)

//*****************************************************************************
// 列挙型定義
//*****************************************************************************
enum
{
	LIGHT_MAIN,
	LIGHT_SUB,
	LIGHT_POINT,
	LIGHT_SPOT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void SetLight(int nLight, bool bFlag);
void SetLightPoint(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos,
	FLOAT fRange, FLOAT fAttenuation);
void SetLightSpot(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 pos, D3DXVECTOR3 vecDir,
	FLOAT fRange, FLOAT fAttenuation, FLOAT fTheta, FLOAT fPhi);
void SetLightDirectional(int nLight, D3DXCOLOR xColor, D3DXVECTOR3 vecDir);

#ifdef _DEBUG
void UpdateDebugLight(void);
#endif

#endif
