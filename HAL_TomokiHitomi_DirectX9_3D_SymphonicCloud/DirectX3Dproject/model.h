//=============================================================================
//
// モデル処理 [model.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "bullet.h"
#include "magiccircle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_MODEL		"data/MODEL/Alicia/Alicia_solid.x"							// 読み込むモデル名
//#define	MODEL_MODEL		"data/MODEL/test.x"							// 読み込むモデル名

//#define	MODEL_CAR		"data/MODEL/Tune/Tune.x"							// 読み込むモデル名

#define MODEL_MAX						(1)

//#define MODEL_SCALE						(0.45f)
#define MODEL_SCALE						(0.20f)
#define MODEL_SIZE						(1.0f)

#define MODEL_POS_X						(0.0f)
#define MODEL_POS_Z						(0.0f)

#define MODEL_FLOAT_SPEED				(0.03f)		// 浮遊速度
#define MODEL_FLOAT_LENGTH				(0.1f)		// 浮遊速度

#define MODEL_ALPHA						(200)
#define MODEL_CENTER					(30)

#define MODEL_HEIGHT_EYE				(45.0f)
#define MODEL_HEIGHT_WEAPON				(42.0f)
#define MODEL_H_ANGLE					(D3DX_PI * 1.5f)

#define MODEL_MOVE_SPEED				(5)
#define MODEL_MOVE_BOOST				(10)
#define MODEL_MOVE_INERTIA				(0.3f)

#define MODEL_ROT_SPEED					(0.1f)
#define MODEL_ROT_AUTO_SPEED			(0.2f)

#define MODEL_INVISIBLE_COUNT			(20)

#define MODEL_TAG_COUNT					(30)		// ターゲット解除カウント

/* ステータス */
#define MODEL_JUMP_POWER				(8.0f)		// ジャンプパワー
#define MODEL_JUMP_POWER_DOWN			(0.07f)		// ジャンプパワー

#define MODEL_STATUS_HP					(40.0f)	// HP
#define MODEL_STATUS_MP					(1000.0f)	// MP
#define MODEL_STATUS_MP_SUB				(10.0f)		// MPの減算数
#define MODEL_STATUS_MP_LIMITER			(100.0f)	// MPの連続使用制限
#define MODEL_STATUS_MP_RECOVER			(5.0f)		// MPの回復速度

// マジックステータス
#define MODEL_STATUS_NORMAL				(100.0f)	// ノーマル
#define MODEL_STATUS_FIRE				(100.0f)	// ファイア
#define MODEL_STATUS_THUNDER			(100.0f)	// サンダー
#define MODEL_STATUS_ICE				(100.0f)	// アイス
#define MODEL_STATUS_WIND				(100.0f)	// ウィンド
#define MODEL_STATUS_WARER				(100.0f)	// ウォーター
#define MODEL_STATUS_EARTH				(100.0f)	// アース

// マジックの減算数
#define MODEL_STATUS_NORMAL_SUB			(0.5f)		// ノーマル
#define MODEL_STATUS_FIRE_SUB			(20.0f)		// ファイア
#define MODEL_STATUS_THUNDER_SUB		(20.0f)		// サンダー
#define MODEL_STATUS_ICE_SUB			(20.0f)		// アイス
#define MODEL_STATUS_WIND_SUB			(20.0f)		// ウィンド
#define MODEL_STATUS_WATER_SUB			(20.0f)		// ウォーター
#define MODEL_STATUS_EARTH_SUB			(20.0f)		// アース
#define MODEL_STATUS_MAGIC_RECOVER		(0.03f)		// マジックの回復速度

#define MODEL_STATUS_CHARGE_MAX			(5)			// 最大チャージ
#define MODEL_STATUS_CHARGE_SPEED		(12)		// チャージスピード

/* アタック */
#define MODEL_ATTACK_NORMAL				(10)		// 通常攻撃インターバル
#define MODEL_ATTACK_FIRE				(25)		// ファイアインターバル
#define MODEL_ATTACK_THUNDER			(25)		// サンダーインターバル
#define MODEL_ATTACK_ICE				(25)		// アイスインターバル
#define MODEL_ATTACK_WIND				(25)		// ウィンドインターバル
#define MODEL_ATTACK_WATER				(25)		// ウォーターインターバル
#define MODEL_ATTACK_EARTH				(25)		// アースインターバル


/* 傾き */
#define MODEL_TILT_ROT					(0.005f)		// 移動時の傾き速度
#define MODEL_TILT_ROT_BOOST			(0.01f)		// ブーストの傾き速度
#define MODEL_TILT_ROT_AUTO				(0.005f)		// 自動移動速度
/* 通常移動時 */
#define MODEL_TILT_MARGIN_FRONT			(0.1f)		// 前方移動限界傾斜
#define MODEL_TILT_MARGIN_LEAR			(0.1f)		// 後方移動限界傾斜
#define MODEL_TILT_MARGIN_SIDE			(0.1f)		// 側面移動限界傾斜
/* ブースト時 */
#define MODEL_TILT_MARGIN_BOOST_FRONT	(0.2f)		// 前方移動限界傾斜
#define MODEL_TILT_MARGIN_BOOST_LEAR	(0.2f)		// 後方移動限界傾斜
#define MODEL_TILT_MARGIN_BOOST_SIDE	(0.2f)		// 側面移動限界傾斜


//*****************************************************************************
// 構造体定義
//*****************************************************************************
//typedef struct
//{
//	bool			bUse;
//	int				nAttackCount;
//	float			fStatus;
//	float			fStatusMax;
//	float			fStatusSub;
//}MODEL_MAGIC;

typedef struct
{
	int				nIdx;
	bool			bUse;
}MODEL_MAGICCIRCLE;

typedef struct
{
	//MODEL_MAGIC		modelMagic[BULLET_TYPE_MAX];
	D3DXVECTOR3		posModel;			// 地面の位置
	D3DXVECTOR3		rotModel;			// 地面の向き(回転)
	D3DXVECTOR3		sclModel;			// 地面の大きさ(スケール)
	D3DXVECTOR3		moveModel;

	D3DXMATRIX		mtxView;			// ビューマトリックス
	D3DXMATRIX		mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス

	int				nTag;
	int				nTagNum;
	int				nTagReleaseCount;

	int				nCharge;
	int				nChargeCount;

	int				nAttackNormalCount;
	int				nAttackFireCount;
	int				nAttackThunderCount;
	int				nAttackIceCount;
	int				nAttackWindCount;
	int				nAttackWaterCount;
	int				nAttackEarthCount;

	int				nInvisibleCount;

	float			fStatusHP;
	float			fStatusMP;
	float			fStatusNormal;
	float			fStatusFire;
	float			fStatusThunder;
	float			fStatusIce;
	float			fStatusWind;
	float			fStatusWater;
	float			fStatusEarth;


	float			fHAngle;
	float			fHAngleDest;
	float			fHAngleDiff;
	float			fMoveSpeed;
	float			fFloatAngle;
	float			fFloatPos;
	float			fJumpAccel;

	bool			bUse;
	bool			bLockOn;
	bool			bParameter;
	bool			bJump;
	bool			bStatusMPLimiter;
	bool			bDash;

	bool			bActiveFire;
	bool			bActiveThunder;
	bool			bActiveIce;
	bool			bActiveWind;
	bool			bActiveWater;
	bool			bActiveEarth;

	// シャドウ用
	int				nIdxShadow;
	float			fSizeShadow;
	D3DXCOLOR		colShadow;
	bool			bShadow;

	// ミニマップ用
	bool			bUseMinimap;

	// マジックサークル用
	MODEL_MAGICCIRCLE magicCircle[MAGICCIRCLE_MAX];
}MODEL;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitModel(int nType);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
bool CheckMagicModel(int nModel, int nMagic);
void SetMagicModel(int nModel, int nMagic);
void SetMagicChargeModel(int nModel);
MODEL *GetModel(int no);
#endif
