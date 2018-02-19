//=============================================================================
//
// バレット処理 [enemybullet.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ENEMYBULLET	"data/TEXTURE/enemybullet.png"						// 読み込むテクスチャファイル名

#define ENEMYBULLET_MAX				(2000)		// 最大数
#define	ENEMYBULLET_ALPHA			(20)		// αテスト値
#define	ENEMYBULLET_ALPHA_CHANGE	(0.01f)		// α値変更速度
#define	ENEMYBULLET_SIZE_X			(5.0f)		// サイズX
#define	ENEMYBULLET_SIZE_Y			(5.0f)		// サイズY

#define ENEMYBULLET_SPEED_NORMAL	(10.0f)		// バレットスピード
#define ENEMYBULLET_FALSE_COUNT		(300)		// バレット消滅カウント
#define ENEMYBULLET_DIF_R			(0.0f)		// バレットRGBA
#define ENEMYBULLET_DIF_G			(0.8f)
#define ENEMYBULLET_DIF_B			(0.8f)
#define ENEMYBULLET_DIF_A			(1.0f)

#define ENEMYBULLET_SIZE_CHANGE		(0.06f)		// エフェクトサイズチェンジ
#define ENEMYBULLET_ALPHA_CHANGE	(0.08f)		// エフェクトαチェンジ


// ホーミング
#define ENEMYBULLET_VEC_POWER		(0.5f)		// 誘導力
#define ENEMYBULLET_HOMING_LENGTH	(50.0f)
#define ENEMYBULLET_HOMING_UPDATE	(3)

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET	(5)
#define TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET	(6)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_ENEMYBULLET		(TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET*TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET)
#define TEXTURE_PATTERN_ENEMYBULLET			(TEXTURE_PATTERN_NU_ENEMYBULLET / 5)
// アニメーション
#define TIME_ANIMATION_ENEMYBULLET	(3)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			posEnemybullet;				// 位置
	D3DXVECTOR3			rotEnemybullet;				// 向き(回転)
	D3DXVECTOR3			sclEnemybullet;				// 大きさ(スケール)
	D3DXVECTOR3			vecTag;
	D3DXVECTOR2			vec2Size;				// サイズ
	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス
	D3DXCOLOR			colorEnemybullet;

	int					nUseCount;				// 消滅カウント
	int					nEffectCount;
	int					nSysNum;
	int					nTex;
	int					nAnimeCount;
	int					nAnimePattern;
	int					nType;

	float				fMoveSpeed;				// 移動速度
	float				fHAngle;
	float				fVAngle;
	float				fVecPower;
	
	bool				bUse;					// 使用フラグ
	bool				bEnemy;
	bool				bVertex;
	bool				bHoming;

	// シャドウ用
	int					nIdxShadow;
	float				fSizeShadow;
	float				fSize;
	D3DXCOLOR			colShadow;
}ENEMYBULLET;

enum
{
	ENEMYBULLET_NORMAL,
	ENEMYBULLET_HOMING,
	ENEMYBULLET_NORMAL_HOMING
};

enum
{
	ENEMYBULLET_TEX_1,
	ENEMYBULLET_TEX_2
};


// システムの最大数
#define	ENEMYBULLET_SYS_MAX			(500)
#define	ENEMYBULLET_SYS_TIME		(50)
#define	ENEMYBULLET_SYS_SPLIT		(40)
#define	ENEMYBULLET_SYS_WAVE		(1.5f)
#define ENEMYBULLET_SYS_SPEED		(3.0f)
#define ENEMYBULLET_SYS_COOLDOWN	(500)

// ３Ｄポリゴン頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


typedef struct
{
	D3DXVECTOR3			vecTagPos;
	D3DXVECTOR2			vec2Size;		// サイズ

	D3DXCOLOR			colorEnemybullet;

	float				fHAngle;
	float				fVAngle;
	float				fSplitAngle;
	float				fMoveSpeed;
	float				fWave;
	float				fAddAngle;
	float				fLength;

	int					nType;
	int					nFireTime;
	int					nSplitCount;
	int					nCoolDown;
	int					nTex;
	int					nEnemybulletCount;

	bool				bUse;
	bool			bProcess;

}ENEMYBULLET_SYS;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemybullet(int nType);
void InitStatusEnemybullet(int nEnemybullet);
void UninitEnemybullet(void);
void UpdateEnemybullet(void);
void DrawEnemybullet(void);
void SetEnemybullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor);
void SetSysEnemybullet(int nType, int nTex, int nSize, int nColor, int nSplitint, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos);
void SetHomingEnemybullet(D3DXVECTOR3 posSet, float vecPower, D3DXCOLOR xColor);
ENEMYBULLET *GetEnemybullet(int no);
void SetEndFlag(void);

#endif
