//=============================================================================
//
// MAGIC処理 [magic.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _MAGIC_H_
#define _MAGIC_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_MAGIC		(2)	// ポリゴン数

#define TEXTURE_GAME_MAGIC	("data/TEXTURE/magic.png")	// MAGIC用画像
#define TEXTURE_MAGIC_SIZE_X	(500 * SCREEN_SCALE / 2) // テクスチャサイズ
#define TEXTURE_MAGIC_SIZE_Y	(500 * SCREEN_SCALE / 2) // 同上

#define TEXTURE_MAGIC_POS_X	(0.0f) // テクスチャサイズ
#define TEXTURE_MAGIC_POS_Y	(SCREEN_HEIGHT) // 同上

#define TEXTURE_MAGIC_DUMMY_POS_X	(SCREEN_WIDTH) // テクスチャサイズ
#define TEXTURE_MAGIC_DUMMY_POS_Y	(SCREEN_HEIGHT) // 同上

#define TEXTURE_MAGIC_R			(0.5f) // テクスチャカラー赤
#define TEXTURE_MAGIC_G			(0.5f) // テクスチャカラー緑
#define TEXTURE_MAGIC_B			(0.5f) // テクスチャカラー青
#define TEXTURE_MAGIC_A			(1.0f) // テクスチャカラー青

#define TEXTURE_MAGIC_CHANGE_COLLAR		(0.01f) // テクスチャカラー変更速度

#define TEXTURE_PATTERN_DIVIDE_X_MAGIC	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_MAGIC	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_MAGIC			(TEXTURE_PATTERN_DIVIDE_X_MAGIC*TEXTURE_PATTERN_DIVIDE_Y_MAGIC)	// アニメーションパターン数
#define TIME_ANIMATION_MAGIC			(1)	// アニメーションの切り替わるカウント

#define MAGIC_MAX					(2)		// MAGICの最大数
#define MAGIC_TYPE_MAX				(6)		// スキルの最大数
#define MAGIC_ROT_SPEED				(0.01f)	// 回転スピード
#define MAGIC_ROT_INERTIA			(0.1f)	// 回転慣性
#define MAGIC_ROT_COOLDOWN			(10)	// 回転クールダウンカウント

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// MAGICの移動量
	D3DXVECTOR3				rot;					// MAGICの回転量
	D3DXVECTOR2				size;					// MAGICのテクスチャサイズ(x,y)
	D3DXCOLOR				colorMagic;				// 色管理
	int						nColorFlag;				// 色変更フラグ
	int						nCoolDown;				// 回転クールダウン
	int						nType;					// タイプ

	float					fGoalAngle;
	float					fTypeAngle;				// 分割角度
	float					fBaseAngle;				// 角度（θ）
	float					fRadius;				// 半径
	bool					bUse;					// true:使用  false:未使用

	bool					bActive;				// マジックアクティブフラグ
	bool					bActiveColor;			// カラーアクティブ
	bool					bActiveFire;
	bool					bActiveThunder;
	bool					bActiveIce;
	bool					bActiveWind;
	bool					bActiveWater;
	bool					bActiveEarth;
	bool					bSelectCheck;
}MAGIC;

/*******************************************************************************
* 列挙型定義
*******************************************************************************/
enum 
{
	MAGIC_MAIN,
	MAGIC_DUMMY,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMagic(int type);
void UninitMagic(void);
void UpdateMagic(void);
void DrawMagic(void);
void SetMagicType(int nMagicType);
void SetMagic(void);
void ReleaseMagic(void);
MAGIC *GetMagic(int no);
D3DXCOLOR GetMagicColor(void);
int GetTypeMagic(void);

#endif
