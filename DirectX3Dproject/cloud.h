//=============================================================================
//
// Cloud処理 [cloud.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_CLOUD	"data/TEXTURE/p_cloud.png"				// 読み込むテクスチャファイル名

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_CLOUD	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_CLOUD	(1)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_CLOUD		(TEXTURE_PATTERN_DIVIDE_X_CLOUD*TEXTURE_PATTERN_DIVIDE_Y_CLOUD)

// αテスト値
#define	CLOUD_ALPHA		(1)

// サイズ
#define	CLOUD_SIZE_X		(15.0f)
#define	CLOUD_SIZE_Y		(15.0f)

#define CLOUD_SIZE_CHANGE	((CLOUD_SIZE_X + CLOUD_SIZE_Y) / 2 * 0.05f)

#define CLOUD_ALPHA_CHANGE	(CLOUD_SIZE_CHANGE * 0.1f)

// 最大数
#define CLOUD_MAX			(1000)

// RGBA
#define CLOUD_DIF_R		(0.8f)
#define CLOUD_DIF_G		(0.8f)
#define CLOUD_DIF_B		(0.8f)
#define CLOUD_DIF_A		(1.0f)


typedef struct
{
	//VERTEX_3D				vertexWK[NUM_VERTEX];	// 頂点情報格納ワーク
	VERTEX_3D				*vertexWK;	// 頂点情報格納ワーク

	D3DXVECTOR3				posCloud;				// 地面の位置
	D3DXVECTOR3				rotCloud;				// 地面の向き(回転)
	D3DXVECTOR3				sclCloud;				// 地面の大きさ(スケール)

	D3DXVECTOR2				vec2Size;

	D3DXMATRIX				mtxView;				// ビューマトリックス
	D3DXMATRIX				mtxProjection;			// プロジェクションマトリックス

	D3DXCOLOR				colorCloud;

	int						nUseCount;				// 消滅カウント
	int						nTex;

	float					fSizeChange;
	float					fAlphaChange;

	bool					bUse;					// 使用フラグ
}CLOUD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCloud(int nType);
void InitStatusCloud(int nCloud);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);
void SetCloud(D3DXVECTOR3 vecPos);
CLOUD *GetCloud(int no);

#endif
