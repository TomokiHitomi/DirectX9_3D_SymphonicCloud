//=============================================================================
//
// Game���� [game.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "game.h"
#include "sound.h"
#include "pause.h"
#include "enemy.h"
#include "magic.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_normal.h"
#include "calculate.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAME_SYS					gameSysWk;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	SetSoundBgmBoss(false);
	for (int i = 0; i < GAME_MAGIC_MAX; i++)
	{
		gameSys->nShuffle[i] = i;
	}
	SetShuffle(&gameSys->nShuffle[0], GAME_MAGIC_MAX);
	gameSys->nTotalCount = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	gameSys->nTotalCount++;

	if (gameSys->nTotalCount == 100)
	{
		SetVoice(VOICE_WARENICHIKARAWO, E_DS8_FLAG_NONE, CONTINUITY_ON);
	}
	if (gameSys->nTotalCount == 200)
	{
		//SetMagicType(BULLET_SPECIAL_WATER);
		//SetMagicType(BULLET_SPECIAL_ICE);
		//SetMagicType(BULLET_SPECIAL_EARTH);
		//SetMagicType(BULLET_SPECIAL_THUNDER);
		//SetMagicType(BULLET_SPECIAL_WIND);
		//SetMagicType(BULLET_SPECIAL_FIRE);
		SetEnemy_normalColor(GetGameShuffle(1 - 1) + 1);
		SetWaveEnemy(1);
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{

}

//=============================================================================
// �擾�֐�
//=============================================================================
GAME_SYS *GetGameSys(void)
{
	return(&gameSysWk);
}

//=============================================================================
// �J�E���g�擾�֐�
//=============================================================================
int GetGameSysCount(void)
{
	GAME_SYS *gameSys = &gameSysWk;
	return(gameSys->nTotalCount);
}

//=============================================================================
// �V���b�t���l�擾�֐�
//=============================================================================
int GetGameShuffle(int nWave)
{
	GAME_SYS *gameSys = &gameSysWk;
	return(gameSys->nShuffle[nWave]);
}