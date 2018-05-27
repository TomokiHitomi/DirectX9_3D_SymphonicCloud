//=============================================================================
//
// メッシュ地面の処理 [meshcloud.h]
// Author :  GP12A295 25 人見友基
//
//=============================================================================
#ifndef _MESHCLOUD_H_
#define _MESHCLOUD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/cloud.png"		// 読み込むテクスチャファイル名

#define MESHCLOUD_MAX				(1)

#define MESHCLOUD_X					(120)
#define MESHCLOUD_Z					(120)

#define MESHCLOUD_POS_X				(0.0f)
#define MESHCLOUD_POS_Z				(0.0f)

#define MESHCLOUD_RESET_X			(1000.0f)
#define MESHCLOUD_RESET_Z			(1000.0f)

#define MESHCLOUD_SIZE_X			(50)
#define MESHCLOUD_SIZE_Z			(50)

#define MESHCLOUD_WAVE				(5)
#define MESHCLOUD_WAVE_SPEED		(0.02f)

//#define MESHCLOUD_DIFFUSE_R			(1.0f)
//#define MESHCLOUD_DIFFUSE_G			(1.0f)
//#define MESHCLOUD_DIFFUSE_B			(1.0f)
//#define MESHCLOUD_DIFFUSE_A			(0.8f)

#define MESHCLOUD_DIFFUSE_R			(1.0f)
#define MESHCLOUD_DIFFUSE_G			(1.0f)
#define MESHCLOUD_DIFFUSE_B			(1.0f)
#define MESHCLOUD_DIFFUSE_A			(0.95f)


typedef struct
{
	VERTEX_3D						*vertexWK;	// 頂点情報格納ワーク

	D3DXVECTOR3						vecPos;				// ポリゴン表示位置の中心座標
	D3DXVECTOR3						vecRot;				// ポリゴンの回転角

	D3DXMATRIX						mtxWorldCloud;		// ワールドマトリックス

	float							fVAnglel;
}MESHCLOUD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshcloud(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int nType);
HRESULT InitMeshcloudPos(void);
void UninitMeshcloud(void);
void UpdateMeshcloud(void);
void DrawMeshcloud(void);

#endif
