//=============================================================================
//
// �^�C�g������ [pause.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "pausemenu.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPause(int no);
void SetTexturePause( int no, int cntPattern );	//
void SetVertexPause(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PAUSE					pauseWk[PAUSE_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTexturePause = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PAUSE00,				// �t�@�C���̖��O
			&pD3DTexturePause);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		pause->bUse = false;
		pause->bStop = false;
		pause->pos = D3DXVECTOR3((float)PAUSE_POS_X, (float)PAUSE_POS_Y, 0.0f);
		pause->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pause->nCountAnim = 0;
		pause->nPatternAnim = 0;

		pause->Texture = pD3DTexturePause;			// �e�N�X�`���ւ̃G�l�~�[
		pause->size = D3DXVECTOR2(TEXTURE_PAUSE00_SIZE_X, TEXTURE_PAUSE00_SIZE_Y);

		// ���_���̍쐬
		MakeVertexPause(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	PAUSE *pause = &pauseWk[0];

	// ���������
	if (pD3DTexturePause != NULL)
	{
		pD3DTexturePause->Release();
		pD3DTexturePause = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	PAUSE *pause = &pauseWk[0];
	PAUSEMENU *pausemenu = GetPausemenu(0);

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		if (pause->bUse)
		{
			SetSoundBgm(SOUND_BGM_GAME_PAUSE);
			// �A�j���[�V����
			pause->nCountAnim++;
			if ((pause->nCountAnim % TIME_ANIMATION_PAUSE) == 0)
			{
				// �p�^�[���̐؂�ւ�
				pause->nPatternAnim = (pause->nPatternAnim + 1) % ANIM_PATTERN_NUM_PAUSE;
			}

			if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_HOME))
			{
				pause->bUse = false;
				pause->bStop = false;
				SetPausemenu(false);
				SetSe(2, E_DS8_FLAG_NONE, true);
				SetFade(FADE_IN, STAGE_MAX);
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_HOME))
			{
				pause->bUse = true;
				pause->bStop = true;
				SetPausemenu(true);
				SetSe(3, E_DS8_FLAG_NONE, true);
				SetFade(FADE_OUT_HALF, STAGE_MAX);
				// �e�N�X�`�����W��ݒ�
				SetTexturePause(i, pause->nPatternAnim);
				// �ړ���̍��W�Œ��_��ݒ�
				SetVertexPause(i);
			}
		}
		// �e�N�X�`�����W��ݒ�
		SetTexturePause(i, pause->nPatternAnim);
		// �ړ���̍��W�Œ��_��ݒ�
		SetVertexPause(i);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PAUSE_MAX; i++, pause++)
	{
		if (pause->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pause->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSE, pause->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPause(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSE *pause = &pauseWk[no];

	// ���_���W�̐ݒ�
	pause->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pause->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSE00_SIZE_X, 100.0f, 0.0f);
	pause->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_PAUSE00_SIZE_Y, 0.0f);
	pause->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSE00_SIZE_X, 100.0f+TEXTURE_PAUSE00_SIZE_Y, 0.0f);
	//SetVertexPause();

	// rhw�̐ݒ�
	pause->vertexWk[0].rhw =
	pause->vertexWk[1].rhw =
	pause->vertexWk[2].rhw =
	pause->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	pause->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pause->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pause->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pause->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSE, 0.0f );
	pause->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSE );
	pause->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSE, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSE );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePause( int no, int cntPattern )
{
	PAUSE *pause = &pauseWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PAUSE;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PAUSE;
	pause->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	pause->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	pause->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	pause->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexPause(int no)
{
	PAUSE *pause = &pauseWk[no];

	pause->vertexWk[0].vtx.x = pause->pos.x - TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[0].vtx.y = pause->pos.y - TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[0].vtx.z = 0.0f;

	pause->vertexWk[1].vtx.x = pause->pos.x + TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[1].vtx.y = pause->pos.y - TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[1].vtx.z = 0.0f;

	pause->vertexWk[2].vtx.x = pause->pos.x - TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[2].vtx.y = pause->pos.y + TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[2].vtx.z = 0.0f;

	pause->vertexWk[3].vtx.x = pause->pos.x + TEXTURE_PAUSE00_SIZE_X;
	pause->vertexWk[3].vtx.y = pause->pos.y + TEXTURE_PAUSE00_SIZE_Y;
	pause->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �|�[�Y�t���O�擾�֐��i0�j��n��
//=============================================================================
bool GetPauseFlag(void)
{
	PAUSE *pause = &pauseWk[0];
	return (pause->bUse);
}

//=============================================================================
// �擾�֐�
//=============================================================================
PAUSE *GetPause(int no)
{
	return(&pauseWk[no]);
}

