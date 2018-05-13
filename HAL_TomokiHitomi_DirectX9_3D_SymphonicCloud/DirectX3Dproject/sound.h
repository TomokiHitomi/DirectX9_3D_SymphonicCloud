//=============================================================================
//
// 入力処理 [sound.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VOLUME_CONTROL_UP	(100)			// 音量上昇幅
#define VOLUME_CONTROL_DOWN	(15)			// 音量下降幅
#define VOLUME_MAX	(-500)					// 最大音量
#define VOLUME_HALF	(-2000)					// 中間音量
#define VOLUME_MIN	(-8000)					// 最小音量

#define VOLUME_SE		(-800)				// SEボリューム
#define VOLUME_VOICE	(-500)				// ボイスボリューム

#define SE_11_CONTINUITY	(5)				// ヒットSEの再生間隔
#define SE_11_VOLUME_SE		(-1200)			// SE11のボリューム設定

#define SOUND_BGM_MAX		(SE_00)
#define SOUND_SE_MAX		(VOICE_00 - SE_00)
#define SOUND_VOICE_MAX		(SOUND_MAX - VOICE_00)

enum
{	// サウンド通しナンバー(sound.cppの順番と合わせる事)
	// Atmospheres
	BGM_ATM_00,		// D#
	BGM_ATM_01,		// F#
	// Basslines
	BGM_BASS_00,	//
	BGM_BASS_01,	// G#
	BGM_BASS_02,	// B
	BGM_BASS_03,	// A#
	BGM_BASS_04,	// G
	BGM_BASS_05,	// F#
	BGM_BASS_06,	// C
	// Drumloops
	BGM_DRUM_00,
	BGM_DRUM_01,
	// Sequences
	BGM_SEQ_00,		// D#
	BGM_SEQ_01,		// D#
	BGM_SEQ_02,		// B
	BGM_SEQ_03,		// A#
	BGM_SEQ_04,		// G
	BGM_SEQ_05,		// B
	BGM_SEQ_06,		// F#
	BGM_SEQ_07,		// A#
	BGM_SEQ_08,		// C
	BGM_SEQ_09,		// G
	BGM_SEQ_10,		// G#
	// TCE
	BGM_TCE_00,		// G#
	// SE
	SE_00,
	SE_01,
	SE_02,
	SE_03,
	SE_04,
	SE_05,
	SE_06,
	SE_07,
	SE_08,
	SE_09,
	SE_10,
	SE_11,
	SE_12,
	SE_13,
	SE_14,
	SE_15,
	SE_16,
	SE_17,
	SE_18,
	// VOICE
	VOICE_00,
	VOICE_01,
	VOICE_02,
	VOICE_03,
	VOICE_04,
	VOICE_05,
	VOICE_06,
	VOICE_07,
	VOICE_08,
	VOICE_09,
	VOICE_10,
	SOUND_MAX
};

enum
{
	SE_CLOSE,
	SE_SELECT,
	SE_DECISION,
	SE_PAUSE,
	SE_NOGRAVITY,
	SE_GRAVITY,
	SE_ENEMYBULLET,
	SE_RESULT,
	SE_PLAYERBULLET,
	SE_ALERT,
	SE_JUMP,
	SE_HIT,
	SE_ENEMYFALSE,
	SE_ENEMYBULLETREVERSAL,
	SE_SE_MAOU_ONEPOINT33,
	SE_CRASH,
	SE_OPENHAT,
	SE_EXPL045,
	SE_CLAP009
};

enum
{
	VOICE_FIRE,
	VOICE_THUNDER,
	VOICE_ICE,
	VOICE_WIND,
	VOICE_WATER,
	VOICE_EARTH,
	VOICE_POWERUP,
	VOICE_WARENICHIKARAWO,
	VOICE_CLEAR,
	VOICE_GAMEOVER,
	VOICE_ITTAA
};

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

enum
{	// 再生用フラグ
	VOLUME_FADE_OUT,
	VOLUME_FADE_IN,
	VOLUME_FADE_HALF
};

enum
{	// 連続再生フラグ
	CONTINUITY_OFF,
	CONTINUITY_ON
};

enum
{	// BGMフラグ
	SOUND_BGM_TITLE,
	SOUND_BGM_GAME_NORMAL,
	SOUND_BGM_GAME_FIRE,
	SOUND_BGM_GAME_THUNDER,
	SOUND_BGM_GAME_ICE,
	SOUND_BGM_GAME_WIND,
	SOUND_BGM_GAME_WATER,
	SOUND_BGM_GAME_EARTH,
	SOUND_BGM_GAME_PAUSE,
	SOUND_BGM_RESULT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// 初期化
void					UninitSound();			// 後片付け
void					UpdateSound(void);
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// サウンドのロード
void					PlaySound( LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );	// 音ごとに再生
void					PlayContinuitySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// 再生中かどうか
HRESULT					SetVol(LPDIRECTSOUNDBUFFER8 pBuffer, long nVol);
void					SetSoundBgm(int nBgm);
void					SetSoundBgmBoss(bool bBgm);
void					SetSe(int nVoice, int nFlag, bool bContinuity);
void					SetVoice(int nVoice, int nFlag, bool bContinuity);
#endif