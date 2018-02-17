//=============================================================================
//
// �^�C�g������ [pausemenu.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "pausemenu.h"
#include "input.h"
#include "sound.h"
#include "pause.h"
#include "fade.h"
#include "calculate.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPausemenu(int no);
void SetTexturePausemenu( int no, int cntPattern );	//
void SetVertexPausemenu(int no);
void SetDiffusePausemenu(int no, D3DXCOLOR color);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PAUSEMENU					pausemenuWk[PAUSEMENU_MAX];
PAUSEMENU_SYS				pausemenusysWk;

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTexturePausemenu = NULL;

int g_nSelectPauseMenu;
int g_nSelectPauseMenuCount;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPausemenu(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[0];
	PAUSEMENU_SYS *pausemenusys = &pausemenusysWk;

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_PAUSEMENU00,				// �t�@�C���̖��O
			&pD3DTexturePausemenu);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����
	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		pausemenu->bUse = false;
		pausemenu->pos = D3DXVECTOR3((float)PAUSEMENU_POS_X, i*(float)PAUSEMENU_SPACING + (float)PAUSEMENU_POS_Y, 0.0f);
		pausemenu->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pausemenu->nCountAnim = 0;
		pausemenu->nPatternAnim = 0;

		pausemenu->Texture = pD3DTexturePausemenu;			// �e�N�X�`���ւ̃G�l�~�[
		pausemenu->size = D3DXVECTOR2(TEXTURE_PAUSEMENU00_SIZE_X, TEXTURE_PAUSEMENU00_SIZE_Y);

		// ���_���̍쐬
		MakeVertexPausemenu(i);
	}

	pausemenusys->bSelect = false;
	g_nSelectPauseMenu = 0;
	g_nSelectPauseMenuCount = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPausemenu(void)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	// ���������
	if (pD3DTexturePausemenu != NULL)
	{
		pD3DTexturePausemenu->Release();
		pD3DTexturePausemenu = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePausemenu(void)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];
	PAUSEMENU_SYS *pausemenusys = &pausemenusysWk;
	PAUSE *pause = GetPause(0);

	// �|�[�Y���L���A������O�͏���
	if (pause->bUse && !pausemenusys->bSelect)
	{
		// ��ړ�
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, LSTICK_UP))
		{
			g_nSelectPauseMenu--;
			if (g_nSelectPauseMenu < 0)
			{
				g_nSelectPauseMenu += PAUSEMENU_MAX;
			}
			SetSe(1, E_DS8_FLAG_NONE, true);
		}
		// ���ړ�
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, LSTICK_DOWN))
		{
			g_nSelectPauseMenu++;
			SetSe(1, E_DS8_FLAG_NONE, true);
		}

		// ��ړ�������
		if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{
			g_nSelectPauseMenuCount++;
			if (g_nSelectPauseMenuCount > PAUSEMENU_SELECT_PRESS && g_nSelectPauseMenuCount % PAUSEMENU_SELECT_SPEED == 0)
			{
				g_nSelectPauseMenu--;
				if (g_nSelectPauseMenu < 0)
				{
					g_nSelectPauseMenu += PAUSEMENU_MAX;
				}
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		// ���ړ�������
		else if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{
			g_nSelectPauseMenuCount++;
			if (g_nSelectPauseMenuCount > PAUSEMENU_SELECT_PRESS && g_nSelectPauseMenuCount  % PAUSEMENU_SELECT_SPEED == 0)
			{
				g_nSelectPauseMenu++;
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		else
		{
			g_nSelectPauseMenuCount = 0;
		}


		for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
		{
			if (pausemenu->bUse)
			{
				if (i == g_nSelectPauseMenu % PAUSEMENU_MAX)
				{
					SetDiffusePausemenu(i, SetColorPallet(COLOR_PALLET_LIGHTBLUE));
				}
				else
				{
					SetDiffusePausemenu(i, SetColorPallet(COLOR_PALLET_WHITE));
				}
				pausemenu->nPatternAnim = i;
				// �e�N�X�`�����W��ݒ�
				SetTexturePausemenu(i, pausemenu->nPatternAnim);
				// �ړ���̍��W�Œ��_��ݒ�
				SetVertexPausemenu(i);

			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || IsButtonTriggered(0, R_BUTTON_A))
		{
			switch (g_nSelectPauseMenu % PAUSEMENU_MAX)
			{
			case 0:
				pause->bUse = false;
				pause->bStop = false;
				SetFade(FADE_IN, STAGE_MAX);
				SetPausemenu(false);
				break;
			case 1:
				SetFade(FADE_OUT, STAGE_GAME);
				pausemenusys->bSelect = true;	// �����͏������~
				break;
			case 2:
				SetFade(FADE_OUT, STAGE_TITLE);
				pausemenusys->bSelect = true;	// �����͏������~
				break;
			}
			SetSe(2, E_DS8_FLAG_NONE, true);
		}

		if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, R_BUTTON_B))
		{
			g_nSelectPauseMenu = 0;
		}
	}
}

//=============================================================================
// �`�揈��1
//=============================================================================
void DrawPausemenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		if (pausemenu->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pausemenu->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_PAUSEMENU, pausemenu->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPausemenu(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// ���_���W�̐ݒ�
	pausemenu->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	pausemenu->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSEMENU00_SIZE_X, 100.0f, 0.0f);
	pausemenu->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_PAUSEMENU00_SIZE_Y, 0.0f);
	pausemenu->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_PAUSEMENU00_SIZE_X, 100.0f+TEXTURE_PAUSEMENU00_SIZE_Y, 0.0f);
	//SetVertexPausemenu();

	// rhw�̐ݒ�
	pausemenu->vertexWk[0].rhw =
	pausemenu->vertexWk[1].rhw =
	pausemenu->vertexWk[2].rhw =
	pausemenu->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	pausemenu->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pausemenu->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pausemenu->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pausemenu->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU, 0.0f );
	pausemenu->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU );
	pausemenu->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePausemenu( int no, int cntPattern )
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PAUSEMENU;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PAUSEMENU;
	pausemenu->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	pausemenu->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	pausemenu->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	pausemenu->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexPausemenu(int no)
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	pausemenu->vertexWk[0].vtx.x = pausemenu->pos.x - TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[0].vtx.y = pausemenu->pos.y - TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[0].vtx.z = 0.0f;

	pausemenu->vertexWk[1].vtx.x = pausemenu->pos.x + TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[1].vtx.y = pausemenu->pos.y - TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[1].vtx.z = 0.0f;

	pausemenu->vertexWk[2].vtx.x = pausemenu->pos.x - TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[2].vtx.y = pausemenu->pos.y + TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[2].vtx.z = 0.0f;

	pausemenu->vertexWk[3].vtx.x = pausemenu->pos.x + TEXTURE_PAUSEMENU00_SIZE_X;
	pausemenu->vertexWk[3].vtx.y = pausemenu->pos.y + TEXTURE_PAUSEMENU00_SIZE_Y;
	pausemenu->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffusePausemenu(int no, D3DXCOLOR color)
{
	PAUSEMENU *pausemenu = &pausemenuWk[no];

	// ���ˌ��̐ݒ�
	pausemenu->vertexWk[0].diffuse =
		pausemenu->vertexWk[1].diffuse =
		pausemenu->vertexWk[2].diffuse =
		pausemenu->vertexWk[3].diffuse = color;
}

//=============================================================================
// �|�[�Y���j���[�ďo����
//=============================================================================
void SetPausemenu(bool bUse)
{
	PAUSEMENU *pausemenu = &pausemenuWk[0];

	for (int i = 0; i < PAUSEMENU_MAX; i++, pausemenu++)
	{
		// �|�[�Y���j���[��L���ɂ���
		pausemenu->bUse = bUse;
	}
	return;
}

//=============================================================================
// �^�C�g���擾�֐�
//=============================================================================
PAUSEMENU *GetPausemenu(int no)
{
	return(&pausemenuWk[no]);
}

