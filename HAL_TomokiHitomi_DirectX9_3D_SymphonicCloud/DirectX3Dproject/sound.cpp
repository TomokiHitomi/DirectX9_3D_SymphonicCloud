//=============================================================================
//
// 入力処理 [sound.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "sound.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

// おまじない
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void		SetSound(void);
long		GetVol(LPDIRECTSOUNDBUFFER8 pBuffer);
void		SetBgmVol(int no, int type);
void		UpdateSoundBgm(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECTSOUNDBUFFER8	g_pBgm[SOUND_BGM_MAX];
LPDIRECTSOUNDBUFFER8	g_pSe[SOUND_SE_MAX];
LPDIRECTSOUNDBUFFER8	g_pVoice[SOUND_VOICE_MAX];
int						g_nBgmFlag;
bool					g_bBgmBoss;
long g_nVol[SOUND_BGM_MAX];


// サウンドファイルのパス（sound.hの通しナンバーと順番を合わせること）
const TCHAR* c_soundFilename[] = {
	// Atmospheres
	_T("data/BGM/Atmospheres/ATM 004 D# 138BPM.wav"),
	_T("data/BGM/Atmospheres/ATM 013 F# 138BPM.wav"),//
	// Basslines
	_T("data/BGM/Basslines/MUFX1 LOOP 027 138BPM.wav"),
	_T("data/BGM/Basslines/BA 028 G#.wav"),//
	_T("data/BGM/Basslines/BA 043 B.wav"),//
	_T("data/BGM/Basslines/BA 072 A#.wav"),//
	_T("data/BGM/Basslines/BL 010 G 138BPM.wav"),//
	_T("data/BGM/Basslines/BASS 010 F# 138BPM.wav"),//
	_T("data/BGM/Basslines/BA 014 C.wav"),//
	// Drumloops
	_T("data/BGM/Drumloops/DL 003 138BPM.wav"),
	_T("data/BGM/Drumloops/MSD2 Drum Loop 138 BPM 036.wav"),
	// Sequences
	_T("data/BGM/Sequences/SEQ 104 C.wav"),
	_T("data/BGM/Sequences/SEQ 144 D#.wav"),
	_T("data/BGM/Sequences/SEQ 131 B.wav"),//
	_T("data/BGM/Sequences/SQ 013 A# 138BPM.wav"),//
	_T("data/BGM/Sequences/SQ 017 G 138BPM.wav"),//
	_T("data/BGM/Sequences/SEQ 098 B.wav"),//
	_T("data/BGM/Sequences/SQ 001 F# 138BPM.wav"),//
	_T("data/BGM/Sequences/SQ 016 A# 138BPM.wav"),//
	_T("data/BGM/Sequences/SQ 035 C 138BPM.wav"),//
	_T("data/BGM/Sequences/SQ 062 G 138BPM.wav"),//
	_T("data/BGM/Sequences/SQ 065 G# 138BPM.wav"),//
	// TCE
	_T("data/BGM/TCE/TCE3 06 Piano 138 BPM G# Wet.wav"),//
	// SE
	_T("data/SE/close.wav"),
	_T("data/SE/select.wav"),
	_T("data/SE/decision.wav"),
	_T("data/SE/pause.wav"),
	_T("data/SE/nogravity.wav"),
	_T("data/SE/gravity.wav"),
	_T("data/SE/enemybullet00.wav"),
	_T("data/SE/result.wav"),
	_T("data/SE/playerbullet00.wav"),
	_T("data/SE/alert.wav"),
	_T("data/SE/jump.wav"),
	_T("data/SE/hit.wav"),
	_T("data/SE/enemyfalse.wav"),
	_T("data/SE/enemybulletreversal.wav"),
	_T("data/SE/se_maoudamashii_onepoint33.wav"),
	_T("data/SE/TC/Crash.wav"),
	_T("data/SE/TC/MRE Open Hat 009.wav"),
	_T("data/SE/TC/MUFX1 EXPL 045.wav"),
	_T("data/SE/TC/MRE Clap 009.wav"),
	// VOICE
	_T("data/VOICE/fire_01.wav"),
	_T("data/VOICE/thunder_01.wav"),
	_T("data/VOICE/ice_01.wav"),
	_T("data/VOICE/storm_01.wav"),
	_T("data/VOICE/water_01.wav"),
	_T("data/VOICE/daichiyo_01.wav"),
	_T("data/VOICE/powerup_02.wav"),
	_T("data/VOICE/warenichikarawo_01.wav"),
	_T("data/VOICE/stageclear_01.wav"),
	_T("data/VOICE/gameover_02.wav"),
	_T("data/VOICE/ittaa_01.wav")
};

// グローバル変数
IDirectSound8 *g_pDirectSound = NULL;	// サウンドインターフェース

// サウンドの初期化
// hWnd:ウィンドウハンドル
HRESULT InitSound( HWND hWnd )
{
	// DirectSoundオブジェクトの作成
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;
	
	// 協調レベル設定 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;
	g_nBgmFlag = SOUND_BGM_TITLE;
	g_bBgmBoss = false;

	for (int i = 0; i < SOUND_BGM_MAX; i++)
	{
		g_nVol[i] = VOLUME_MIN;
	}

	SetSound();

	return S_OK;
}

void UpdateSound(void)
{
	UpdateSoundBgm();
#ifdef _DEBUG
	PrintDebugProc("【 SOUND 】  BGM FLAG[%d]\n", g_nBgmFlag);
	for (int i = 0; i < SOUND_BGM_MAX; i++)
	{
		PrintDebugProc("BGM00%d[%l]\n", i, GetVol(g_pBgm[i]));
	}
#endif
}

// 後片付け
void UninitSound()
{
	if( g_pDirectSound != NULL )
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}

// サウンドのロード
// no:サウンドナンバー（ヘッダに定義された列挙型定数）
LPDIRECTSOUNDBUFFER8 LoadSound( int no )
{
	// MMIO = マルチメディア入出力、の略。
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;	// 曲データのバッファ
	DSBUFFERDESC buff;						// バッファ設定構造体

	HMMIO hMmio = NULL;						// MMIOハンドル
	MMIOINFO mmioInfo;						// マルチメディアデータ構造体

	MMRESULT mmRes;								// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;	// チャンクデータ構造体
	DWORD size;									// データサイズ格納用
	WAVEFORMATEX pcm;							// 曲データフォーマット構造体

	LPVOID pBlock;				// セカンダリバッファのデータ書き込み先アドレス
	DWORD  dwSize;				// セカンダリバッファのサイズ

	// 1.ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// 検索情報をもとに読み込み
	
	if(size != formatChunk.cksize) 		// 正しく読み込めなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// キーワードをもとに検索
	
	if(mmRes != MMSYSERR_NOERROR) 		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.データ読み込み
	char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保して
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む
	
	if(size != dataChunk.cksize)  		// 正しく読み込めなかったら異常終了
	{
		delete[] pData;
		return NULL;
	}

	// 6.曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// まず初期化
	buff.dwSize = sizeof(DSBUFFERDESC);			// そこから各種設定
	buff.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// 総合バッファを作る
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// サウンドバッファを取り出す
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// 取り出したら総合バッファを捨てる
	pBaseBuffer->Release();

	// 7.作られたセカンダリバッファにデータを転送
	// まずは書き込みできるようバッファをロック
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// データ転送
	delete[] pData;					// 元の読み込み領域を消す

	// バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// セカンダリバッファを返してようやく完了...
	return pBuffer;
}

void UpdateSoundBgm(void)
{
	switch (g_nBgmFlag)
	{
	case SOUND_BGM_TITLE:
		SetBgmVol(BGM_ATM_00, true);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, true);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_NORMAL:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, true);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_FIRE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, true);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, true);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_05, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_THUNDER:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, true);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, true);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_07, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_ICE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, true);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_10, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, true);
		break;
	case SOUND_BGM_GAME_WIND:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, true);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, true);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_06, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_WATER:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, true);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, true);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_08, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_EARTH:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, true);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, true);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_09, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_PAUSE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, VOLUME_FADE_HALF);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_RESULT:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, true);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, true);
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	}
}

void SetSoundBgm(int nBgm)
{
	g_nBgmFlag = nBgm;
}

void SetSoundBgmBoss(bool bBgm)
{
	g_bBgmBoss = bBgm;
}

// 音を鳴らす
// pBuffer:音を鳴らしたいサウンドデータのセカンダリバッファ
// flag   :1(E_DS8_FLAG_LOOP)ならループ再生
void PlaySound(  LPDIRECTSOUNDBUFFER8 pBuffer, int flag/*=0*/ )
{	// 続きから鳴らすので、最初から鳴らしたい場合はSetCurrentPosition(0)をすること
	pBuffer->Play(0, 0, flag);
}

void PlayContinuitySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag)
{
	pBuffer->SetCurrentPosition(0);
	PlaySound(pBuffer, flag);
}

// 音を止める
void StopSound( LPDIRECTSOUNDBUFFER8 pBuffer )
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// 鳴っていたら
	{
		pBuffer->Stop();	// 意味的にはPabUseになる。
	}
}

// 再生中かどうか調べる
// pBuffer:音を鳴らしたいサウンドデータのセカンダリバッファ
bool IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer )
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}

HRESULT SetVol( LPDIRECTSOUNDBUFFER8 pBuffer, long nVol)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	pBuffer->SetVolume(nVol);
	return DS_OK;
}

long GetVol(LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;
	long lVol;
	pBuffer->GetStatus(&status);
	pBuffer->GetVolume(&lVol);
	return lVol;
}

//=============================================================================
// サウンド設定関数
//=============================================================================
void SetSound(void)
{
	// BGMがずれないようにロードと分ける
	for (int i = BGM_ATM_00; i < SOUND_BGM_MAX; i++)
	{
		// 各BGMロード
		g_pBgm[i] = LoadSound(i);
	}
	for (int i = BGM_ATM_00; i < SOUND_BGM_MAX; i++)
	{
		// 各BGMボリュームセット
		PlaySound(g_pBgm[i], E_DS8_FLAG_LOOP);
		SetVol(g_pBgm[i], VOLUME_MIN);
	}

	for (int i = 0; i < SOUND_SE_MAX; i++)
	{	
		// 各SEロード
		g_pSe[i] = LoadSound(i + SE_00);
		//// 各SEボリュームセット
		//if (i != SE_00 || i != SE_11 || i != SE_13)
		//{
		//	SetVol(g_pSe[i], VOLUME_SE);
		//}
	}

	//SetVol(g_pSe[11], SE_11_VOLUME_SE);

	for (int i = 0; i < SOUND_VOICE_MAX; i++)
	{
		// 各SEロード
		g_pVoice[i] = LoadSound(i + VOICE_00);
	}
}

//=============================================================================
// SE再生関数
//=============================================================================
void SetSe(int nVoice, int nFlag, bool bContinuity)
{
	if (bContinuity)
	{
		PlayContinuitySound(g_pSe[nVoice], nFlag);
	}
	else
	{
		PlaySound(g_pSe[nVoice], nFlag);
	}
}

//=============================================================================
// Voice再生関数
//=============================================================================
void SetVoice(int nVoice, int nFlag, bool bContinuity)
{
	if (bContinuity)
	{
		PlayContinuitySound(g_pVoice[nVoice], nFlag);
	}
	else
	{
		PlaySound(g_pVoice[nVoice], nFlag);
	}
}

//=============================================================================
// BGMボリュームコントロール
//=============================================================================
void SetBgmVol(int no, int type)
{
	// type 0=フェードアウト 1=フェードイン
	if (type == VOLUME_FADE_OUT)
	{
		if (g_nVol[no] > VOLUME_MIN)
		{
			g_nVol[no] -= VOLUME_CONTROL_DOWN;
			if (g_nVol[no] < VOLUME_MIN)
			{
				g_nVol[no] = VOLUME_MIN;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
	else if (type == VOLUME_FADE_IN)
	{
		if (g_nVol[no] < VOLUME_MAX)
		{
			g_nVol[no] += VOLUME_CONTROL_UP;
			if (g_nVol[no] > VOLUME_MAX)
			{
				g_nVol[no] = VOLUME_MAX;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
	else if (type == VOLUME_FADE_HALF)
	{
		if (g_nVol[no] > VOLUME_HALF)
		{
			g_nVol[no] -= VOLUME_CONTROL_DOWN;
			if (g_nVol[no] < VOLUME_HALF)
			{
				g_nVol[no] = VOLUME_HALF;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
}