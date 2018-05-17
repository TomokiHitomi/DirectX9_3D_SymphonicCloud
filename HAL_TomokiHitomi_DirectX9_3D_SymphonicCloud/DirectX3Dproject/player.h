//=============================================================================
//
// モデル処理 [player.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "bullet.h"
#include "magiccircle.h"
#include "SkinMeshX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	PLAYER_MODEL		"data/MODEL/Alicia/AliciaSolidMMD.x"							// 読み込むモデル名
#define	PLAYER_MODEL		"hackadollMMD.x"							// 読み込むモデル名

//#define	PLAYER_CAR		"data/PLAYER/Tune/Tune.x"							// 読み込むモデル名

#define PLAYER_MAX						(1)

//#define PLAYER_SCALE						(0.45f)
#define PLAYER_SCALE						(0.27f)
#define PLAYER_SIZE						(1.0f)

#define PLAYER_POS_X						(0.0f)
#define PLAYER_POS_Z						(0.0f)

#define PLAYER_FLOAT_SPEED				(0.03f)		// 浮遊速度
#define PLAYER_FLOAT_LENGTH				(0.1f)		// 浮遊速度

#define PLAYER_ALPHA						(200)
#define PLAYER_CENTER					(30)

#define PLAYER_HEIGHT_EYE				(45.0f)
#define PLAYER_HEIGHT_WEAPON				(42.0f)
#define PLAYER_H_ANGLE					(D3DX_PI * 1.5f)

#define PLAYER_MOVE_SPEED				(5)
#define PLAYER_MOVE_BOOST				(10)
#define PLAYER_MOVE_INERTIA				(0.03f)

#define PLAYER_ROT_SPEED					(0.1f)
#define PLAYER_ROT_AUTO_SPEED			(0.2f)

#define PLAYER_INVISIBLE_COUNT			(20)

#define PLAYER_TAG_COUNT					(30)		// ターゲット解除カウント

/* ステータス */
#define PLAYER_JUMP_POWER				(8.0f)		// ジャンプパワー
#define PLAYER_JUMP_POWER_DOWN			(0.07f)		// ジャンプパワー

#define PLAYER_STATUS_HP					(40.0f)	// HP
#define PLAYER_STATUS_MP					(1000.0f)	// MP
#define PLAYER_STATUS_MP_SUB				(10.0f)		// MPの減算数
#define PLAYER_STATUS_MP_LIMITER			(100.0f)	// MPの連続使用制限
#define PLAYER_STATUS_MP_RECOVER			(5.0f)		// MPの回復速度

// マジックステータス
#define PLAYER_STATUS_NORMAL				(100.0f)	// ノーマル
#define PLAYER_STATUS_FIRE				(100.0f)	// ファイア
#define PLAYER_STATUS_THUNDER			(100.0f)	// サンダー
#define PLAYER_STATUS_ICE				(100.0f)	// アイス
#define PLAYER_STATUS_WIND				(100.0f)	// ウィンド
#define PLAYER_STATUS_WARER				(100.0f)	// ウォーター
#define PLAYER_STATUS_EARTH				(100.0f)	// アース

// マジックの減算数
#define PLAYER_STATUS_NORMAL_SUB			(0.5f)		// ノーマル
#define PLAYER_STATUS_FIRE_SUB			(20.0f)		// ファイア
#define PLAYER_STATUS_THUNDER_SUB		(20.0f)		// サンダー
#define PLAYER_STATUS_ICE_SUB			(20.0f)		// アイス
#define PLAYER_STATUS_WIND_SUB			(20.0f)		// ウィンド
#define PLAYER_STATUS_WATER_SUB			(20.0f)		// ウォーター
#define PLAYER_STATUS_EARTH_SUB			(20.0f)		// アース
#define PLAYER_STATUS_MAGIC_RECOVER		(0.03f)		// マジックの回復速度

#define PLAYER_STATUS_CHARGE_MAX			(5)			// 最大チャージ
#define PLAYER_STATUS_CHARGE_SPEED		(12)		// チャージスピード

/* アタック */
#define PLAYER_ATTACK_NORMAL				(10)		// 通常攻撃インターバル
#define PLAYER_ATTACK_FIRE				(25)		// ファイアインターバル
#define PLAYER_ATTACK_THUNDER			(25)		// サンダーインターバル
#define PLAYER_ATTACK_ICE				(25)		// アイスインターバル
#define PLAYER_ATTACK_WIND				(25)		// ウィンドインターバル
#define PLAYER_ATTACK_WATER				(25)		// ウォーターインターバル
#define PLAYER_ATTACK_EARTH				(25)		// アースインターバル


/* 傾き */
#define PLAYER_TILT_ROT					(0.005f)		// 移動時の傾き速度
#define PLAYER_TILT_ROT_BOOST			(0.01f)		// ブーストの傾き速度
#define PLAYER_TILT_ROT_AUTO				(0.005f)		// 自動移動速度
/* 通常移動時 */
#define PLAYER_TILT_MARGIN_FRONT			(0.1f)		// 前方移動限界傾斜
#define PLAYER_TILT_MARGIN_LEAR			(0.1f)		// 後方移動限界傾斜
#define PLAYER_TILT_MARGIN_SIDE			(0.1f)		// 側面移動限界傾斜
/* ブースト時 */
#define PLAYER_TILT_MARGIN_BOOST_FRONT	(0.2f)		// 前方移動限界傾斜
#define PLAYER_TILT_MARGIN_BOOST_LEAR	(0.2f)		// 後方移動限界傾斜
#define PLAYER_TILT_MARGIN_BOOST_SIDE	(0.2f)		// 側面移動限界傾斜


#define SKIN_ANIME_SPEED_PLAYER_NORMAL	(60.0f / 2000.0f)
#define SKIN_ANIME_SPEED_PLAYER_DASH	(60.0f / 1200.0f)
#define SKIN_ANIME_SPEED_PLAYER_ATTACK	(60.0f / 2500.0f)

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
//}PLAYER_MAGIC;

typedef struct
{
	int				nIdx;
	bool			bUse;
}PLAYER_MAGICCIRCLE;

typedef struct
{
	CSkinMesh		m_CSkinMesh;
	//PLAYER_MAGIC		playerMagic[BULLET_TYPE_MAX];
	D3DXVECTOR3		posPlayer;			// 地面の位置
	D3DXVECTOR3		rotPlayer;			// 地面の向き(回転)
	D3DXVECTOR3		sclPlayer;			// 地面の大きさ(スケール)
	D3DXVECTOR3		movePlayer;

	PRS				prs;

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
	PLAYER_MAGICCIRCLE magicCircle[MAGICCIRCLE_MAX];
}PLAYER;

// 追記は逆順（新しいものから格納される）
enum PLAYER_ANIME
{
	PLAYER_ANIME_DANCE_SPIN,
	PLAYER_ANIME_DANCE_YMCA,

	PLAYER_ANIME_DEATH,
	PLAYER_ANIME_HEADSHOT,
	PLAYER_ANIME_IDLE_EX,
	PLAYER_ANIME_RECOIL,
	PLAYER_ANIME_OVERDRAW,

	PLAYER_ANIME_JUMP_END,
	PLAYER_ANIME_JUMP_LOOP,
	PLAYER_ANIME_JUMP_START,
	PLAYER_ANIME_AIM_RIGHT,
	PLAYER_ANIME_AIM_LEFT,

	PLAYER_ANIME_AIM_BACK,
	PLAYER_ANIME_AIM_FORWARD,
	PLAYER_ANIME_AIM_IDLE,
	PLAYER_ANIME_RIGHT,
	PLAYER_ANIME_LEFT,

	PLAYER_ANIME_BACK,
	PLAYER_ANIME_FORWARD,
	PLAYER_ANIME_IDLE,
	PLAYER_ANIME_HAND,
	PLAYER_ANIME_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int nType);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool CheckMagicPlayer(int nPlayer, int nMagic);
void SetMagicPlayer(int nPlayer, int nMagic);
void SetMagicChargePlayer(int nPlayer);
void SetPlayerAnime(int nPlayer, DWORD dAnime);
PLAYER *GetPlayer(int no);
#endif
