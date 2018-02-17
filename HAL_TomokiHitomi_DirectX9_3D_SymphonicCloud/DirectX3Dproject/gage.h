//=============================================================================
//
// パラメータ処理 [gage.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _GAGE_H_
#define _GAGE_H_

#include "main.h"

// マクロ定義
#define NUM_GAGE		(2)	// ポリゴン数

#define TEXTURE_GAME_GAGE00	_T("data/TEXTURE/gage.png")	// hp用画像
#define TEXTURE_GAGE00_SIZE_X	(50 * SCREEN_SCALE) // テクスチャサイズ
#define TEXTURE_GAGE00_SIZE_Y	(5 * SCREEN_SCALE/2) // 同上

#define TEXTURE_CHANGE_GAGE_X		(1.0f)
#define TEXTURE_CHANGE_GAGE_Y		(1.0f)

#define TEXTURE_CHANGE_BOSS_GAGE_X	(15.0f)
#define TEXTURE_CHANGE_BOSS_GAGE_Y	(1.5f)

#define HP_GAGE_SIZE_X			(500 * SCREEN_SCALE)
#define HP_GAGE_SIZE_Y			(15 * SCREEN_SCALE)
#define HP_GAGE_POS_X			(SCREEN_CENTER_X - (HP_GAGE_SIZE_X / 2))
#define HP_GAGE_POS_Y			(645 * SCREEN_SCALE)

#define MP_GAGE_SIZE_X			(500 * SCREEN_SCALE)
#define MP_GAGE_SIZE_Y			(15 * SCREEN_SCALE)
#define MP_GAGE_POS_X			(SCREEN_CENTER_X - (MP_GAGE_SIZE_X / 2))
#define MP_GAGE_POS_Y			(680 * SCREEN_SCALE)

#define BULLET_GAGE_SIZE_X		(157 * SCREEN_SCALE)
#define BULLET_GAGE_SIZE_Y		(14.5f * SCREEN_SCALE)
#define BULLET_GAGE_POS_X		(1187 * SCREEN_SCALE + (BULLET_GAGE_SIZE_X / 2))
#define BULLET_GAGE_POS_Y		(308 * SCREEN_SCALE)

#define MAGIC_GAGE_SIZE_X		(157 * SCREEN_SCALE)
#define MAGIC_GAGE_SIZE_Y		(14.5f * SCREEN_SCALE)
#define MAGIC_GAGE_POS_X		(94 * SCREEN_SCALE - (MAGIC_GAGE_SIZE_X / 2))
#define MAGIC_GAGE_POS_Y		(35.5f * SCREEN_SCALE)
#define MAGIC_GAGE_INTERVAL		(54.55f * SCREEN_SCALE)

#define BOSS_GAGE_SIZE_X		(500 * SCREEN_SCALE)
#define BOSS_GAGE_SIZE_Y		(15 * SCREEN_SCALE)
#define BOSS_GAGE_POS_X			(SCREEN_CENTER_X - (BOSS_GAGE_SIZE_X / 2))
#define BOSS_GAGE_POS_Y			(36 * SCREEN_SCALE)

#define GAGE_ALPHA				(0.5f)		// ゲージのα値（高い方が透明）

#define GAGE_STANDBY_SPEED		(0.008f)
#define GAGE_CHANGE_SPEED		(0.01f)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// パラメータの移動量
	D3DXVECTOR3				rot;					// パラメータの回転量
	D3DXVECTOR2				size;					// パラメータのテクスチャサイズ(x,y)
	D3DXVECTOR3				vecTargetPos;			// ターゲット座標
	D3DXVECTOR2				vecSize;			// テクスチャサイズ変更(x,y)

	int						nCountAnim;				// アニメーションカウント
	int						nPatternAnim;			// アニメーションパターンナンバー
	float					fPointGauge;			// 現在のポイント
	int						nTarget;				// ターゲット
	int						nType;					// パラメータタイプ
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径

	bool					bUse;					// true:使用  false:未使用
	bool					bStandby;				// ゲージ充填フラグ
}GAGE;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/

enum
{
	GAGE_PLAYER_HP,					// プレイヤーのHP
	GAGE_PLAYER_MP,					// プレイヤーのMP
	GAGE_BULLET_NORMAL,				// ノーマルバレット
	GAGE_BULLET_SPECIAL_FIRE,		// ファイア
	GAGE_BULLET_SPECIAL_THUNDER,	// サンダー
	GAGE_BULLET_SPECIAL_ICE,		// アイス
	GAGE_BULLET_SPECIAL_WIND,		// サンダー
	GAGE_BULLET_SPECIAL_WATER,		// ウォーター
	GAGE_BULLET_SPECIAL_EARTH,		// アース
	GAGE_BOSS_HP,					// ボスのHP
	GAGE_MAX						// ゲージの最大数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGage(int type);
void UninitGage(void);
void UpdateGage(void);
void DrawGage(void);
void SetGage(int nGage, int nBoss);
void InitStatusGage(int nGage);
GAGE *GetGage(int no);


#endif
