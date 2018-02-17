//=============================================================================
//
// Lockon処理 [lockon.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_LOCKON	"data/TEXTURE/lockon.png"						// 読み込むテクスチャファイル名

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_LOCKON	(1)
#define TEXTURE_PATTERN_DIVIDE_Y_LOCKON	(1)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_LOCKON		(TEXTURE_PATTERN_DIVIDE_X_LOCKON*TEXTURE_PATTERN_DIVIDE_Y_LOCKON)

// αテスト値
#define	LOCKON_ALPHA		(1)

// サイズ
#define	LOCKON_SIZE_X		(70.0f)
#define	LOCKON_SIZE_Y		(70.0f)

#define	LOCKON_LENGTH		(500.0f)

#define LOCKON_SIZE_CHANGE	((LOCKON_SIZE_X + LOCKON_SIZE_Y) / 2 * 0.05f)

#define LOCKON_ALPHA_CHANGE	(LOCKON_SIZE_CHANGE * 0.1f)

// 最大数
#define LOCKON_MAX				(100)

// 再使用までのカウント
#define LOCKON_COOLDOWN			(3)

// RGBA
#define LOCKON_DIF_R			(1.0f)
#define LOCKON_DIF_G			(1.0f)
#define LOCKON_DIF_B			(1.0f)
#define LOCKON_DIF_A			(1.0f)

#define LOCKON_RADIUS			(0.6f)
#define LOCKON_RELEASE_COUNT	(70)

#define LOCKON_SCALE_CHANGE		(0.05f)
#define LOCKON_SCALE_MIN		(0.6f)
#define LOCKON_SCALE_MAX		(1.0f)

typedef struct
{
	//VERTEX_3D				vertexWK[NUM_VERTEX];	// 頂点情報格納ワーク
	VERTEX_3D				*vertexWK;	// 頂点情報格納ワーク

	D3DXVECTOR3				posLockon;				// 地面の位置
	D3DXVECTOR3				rotLockon;				// 地面の向き(回転)
	D3DXVECTOR3				sclLockon;				// 地面の大きさ(スケール)

	D3DXVECTOR2				vec2Size;

	D3DXMATRIX				mtxView;				// ビューマトリックス
	D3DXMATRIX				mtxProjection;			// プロジェクションマトリックス

	D3DXCOLOR				colorLockon;

	int						nUseCount;				// 消滅カウント
	int						nTex;
	int						nIndex;
	int						nCoolDown;
	int						nReleaseCount;

	float					fSizeChange;
	float					fAlphaChange;

	bool					bUse;					// 使用フラグ
	bool					bRelease;				// 破棄フラグ
}LOCKON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitLockon(int nType);
void InitStatusLockon(int nLockon);
void UninitLockon(void);
void UpdateLockon(void);
void DrawLockon(void);
int SetLockon(D3DXVECTOR3 vecPos);
void SetPosLockon(int nIndex, D3DXVECTOR3 vecPos);
void ReleaseLockon(int nIndex);
bool AddReleaseCountLockon(int nIndex);
void ResetReleaseCountLockon(int nIndex);
LOCKON *GetLockon(int no);

#endif
