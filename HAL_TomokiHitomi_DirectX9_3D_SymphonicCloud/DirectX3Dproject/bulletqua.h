//=============================================================================
//
// バレット処理 [bulletqua.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _BULLETQUA_H_
#define _BULLETQUA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BULLETQUA	"data/TEXTURE/bulletanime.png"						// 読み込むテクスチャファイル名


// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_BULLETQUA	(5)
#define TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA	(15)
// テクスチャ分割パターン
#define TEXTURE_PATTERN_NU_BULLETQUA		(TEXTURE_PATTERN_DIVIDE_X_BULLETQUA*TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA)

// アニメーション
#define TIME_ANIMATION_TEX	(3)

// αテスト値
#define	BULLETQUA_ALPHA		(100)

// α値変更速度
#define	BULLETQUA_ALPHA_CHANGE	(0.01f)

// サイズ
#define	BULLETQUA_SIZE_X		(15.0f)
#define	BULLETQUA_SIZE_Y		(15.0f)


// 最大数
#define BULLETQUA_MAX			(2000)

// 移動スピード
#define BULLETQUA_SPEED		(10.0f)

#define BULLETQUA_FALSE_COUNT		(500)		// 消滅カウント

#define BULLETQUA_SIZE_CHANGE		(0.06f)		// エフェクトサイズチェンジ
#define BULLETQUA_ALPHA_CHANGE		(0.08f)		// エフェクトαチェンジ

// バレットテスト用
#define BULLETQUA_TAG_VECTOR		(5000)

// バレットRGBA
#define BULLETQUA_DIF_R		(0.8f)
#define BULLETQUA_DIF_G		(0.0f)
#define BULLETQUA_DIF_B		(0.8f)
#define BULLETQUA_DIF_A		(1.0f)


// エネミー用
#define BULLETQUA_ENEMY_SPEED		(5.0f)

#define BULLETQUA_ENEMY_DIF_R		(0.0f)
#define BULLETQUA_ENEMY_DIF_G		(0.8f)
#define BULLETQUA_ENEMY_DIF_B		(0.8f)
#define BULLETQUA_ENEMY_DIF_A		(1.0f)

#define	BULLETQUA_ENEMY_SIZE_X		(100.0f)
#define	BULLETQUA_ENEMY_SIZE_Y		(100.0f)

typedef struct
{
	//VERTEX_3D			vertexWK[NUM_VERTEX];	// 頂点情報格納ワーク
	VERTEX_3D			*vertexWK;	// 頂点情報格納ワーク

	D3DXVECTOR3			posBulletQua;				// 位置
	D3DXVECTOR3			rotBulletQua;				// 向き(回転)
	D3DXVECTOR3			sclBulletQua;				// 大きさ(スケール)
	D3DXVECTOR3			vecTag;

	D3DXVECTOR2			vec2Size;				// サイズ

	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス

	D3DXCOLOR			colorBulletQua;

	int					nUseCount;				// 消滅カウント
	int					nEffectCount;
	int					nSysNum;
	int					nTex;
	int					nAnimeCount;

	float				fMoveSpeed;				// 移動速度
	float				fHAngle;
	float				fVAngle;
	
	bool				bUse;					// 使用フラグ
	bool				bEnemy;
	bool				bVertex;
	bool				bDraw;

	// シャドウ用
	int					nIdxShadow;
	float				fSizeShadow;
	float				fSize;
	bool				bShadow;
	D3DXCOLOR			colShadow;
}BULLETQUA;


// システムの最大数
#define	BULLETQUA_SYS_MAX			(100)

#define	BULLETQUA_SYS_TIME			(50)
#define	BULLETQUA_SYS_SPLIT		(40)

#define	BULLETQUA_SYS_WAVE			(1.5f)

#define BULLETQUA_SYS_SPEED		(3.0f)
#define BULLETQUA_SYS_COOLDOWN	(500)

typedef struct
{
	// クォータニオン用変数
	D3DXVECTOR3			vecTa;			// 回転半径を設定
	D3DXVECTOR3			vecAxis;		// 現在の角度に垂直な回転軸ベクトルを設定

	D3DXVECTOR3			vecTagPos;
	D3DXVECTOR2			vec2Size;		// サイズ

	D3DXCOLOR			colorBulletQua;

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
	int					nBulletCount;

	bool				bUse;
	bool				bProcess;

}BULLETQUA_SYS;

enum
{
	BULLET_QUA_ROT_L,
	BULLET_QUA_ROT_R,
	BULLET_QUA_ROT_LL,
	BULLET_QUA_ROT_RR,
	BULLET_QUA_ROT_LV,
	BULLET_QUA_ROT_RV
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBulletQua(int nType);
void InitStatusBulletQua(int nBulletQua);
void UninitBulletQua(void);
void UpdateBulletQua(void);
void DrawBulletQua(void);
void SetBulletQua(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2);
void SetEnemyBulletQua00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2);
void SetSysBulletQua(int nType, int nTex, int nSize, int nColor, int nSplitint, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos);
BULLETQUA *GetBulletQua(int no);

#endif
