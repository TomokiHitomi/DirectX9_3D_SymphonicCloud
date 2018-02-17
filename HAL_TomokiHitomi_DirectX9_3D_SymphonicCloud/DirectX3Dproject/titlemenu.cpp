//=============================================================================
//
// �^�C�g������ [titlemenu.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "main.h"
#include "titlemenu.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "calculate.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitlemenu(int no);
void SetTextureTitlemenu( int no, int cntPattern );
void SetVertexTitlemenu(int no);
void SetDiffuseTitlemenu(int no, D3DXCOLOR color);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TITLEMENU					titlemenuWk[TITLEMENU_MAX];
TITLEMENU_SYS				titlemenusysWk[TITLEMENU_SYS_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureTitlemenu = NULL;

int g_nSelectTitlemenu;
int g_nSelectTitlemenuCount;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitlemenu(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[0];
	TITLEMENU_SYS *titlemenusys = &titlemenusysWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TITLEMENU,				// �t�@�C���̖��O
			&pD3DTextureTitlemenu);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		titlemenu->bUse = true;
		titlemenu->pos = D3DXVECTOR3((float)TITLEMENU_POS_X, i*(float)TITLEMENU_SPACING + (float)TITLEMENU_POS_Y, 0.0f);
		titlemenu->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		titlemenu->nCountAnim = 0;
		titlemenu->nPatternAnim = 0;

		titlemenu->Texture = pD3DTextureTitlemenu;			// �e�N�X�`���ւ̃G�l�~�[
		titlemenu->size = D3DXVECTOR2(TEXTURE_TITLEMENU00_SIZE_X, TEXTURE_TITLEMENU00_SIZE_Y);

		// ���_���̍쐬
		MakeVertexTitlemenu(i);
	}

	// �^�C�g���̏�����
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		titlemenusys->bSelect = false;
	}

	g_nSelectTitlemenu = 0;
	g_nSelectTitlemenuCount = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// ���������
	if (pD3DTextureTitlemenu != NULL)
	{
		pD3DTextureTitlemenu->Release();
		pD3DTextureTitlemenu = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];
	TITLEMENU_SYS *titlemenusys = &titlemenusysWk[0];

	// �^�C�g�����j���[���L���A������O�͏���
	if (titlemenu->bUse && !titlemenusys->bSelect)
	{
		// ��ړ�
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, BUTTON_UP) || IsButtonTriggered(0, LSTICK_UP))
		{
			g_nSelectTitlemenu--;
			if (g_nSelectTitlemenu < 0)
			{
				g_nSelectTitlemenu += TITLEMENU_MAX;
			}
			SetSe(1, E_DS8_FLAG_NONE, true);
		}
		// ���ړ�
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, BUTTON_DOWN) || IsButtonTriggered(0, LSTICK_DOWN))
		{
			g_nSelectTitlemenu++;
			SetSe(1, E_DS8_FLAG_NONE, true);
		}

		// ��ړ�������
		if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{
			g_nSelectTitlemenuCount++;
			if (g_nSelectTitlemenuCount > TITLEMENU_SELECT_PRESS && g_nSelectTitlemenuCount % TITLEMENU_SELECT_SPEED == 0)
			{
				g_nSelectTitlemenu--;
				if (g_nSelectTitlemenu < 0)
				{
					g_nSelectTitlemenu += TITLEMENU_MAX;
				}
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		// ���ړ�������
		else if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{
			g_nSelectTitlemenuCount++;
			if (g_nSelectTitlemenuCount > TITLEMENU_SELECT_PRESS && g_nSelectTitlemenuCount  % TITLEMENU_SELECT_SPEED == 0)
			{
				g_nSelectTitlemenu++;
				SetSe(1, E_DS8_FLAG_NONE, true);
			}
		}
		else
		{
			g_nSelectTitlemenuCount = 0;
		}


		for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
		{
			if (i == g_nSelectTitlemenu % TITLEMENU_MAX)
			{	// �I�𒆂̃��j���[�̓J���[�`�F���W
				SetDiffuseTitlemenu(i, SetColorPallet(COLOR_PALLET_LIGHTBLUE));
			}
			else
			{
				SetDiffuseTitlemenu(i, SetColorPallet(COLOR_PALLET_WHITE));
			}
			titlemenu->nPatternAnim = i;
			// �e�N�X�`�����W��ݒ�
			SetTextureTitlemenu(i, titlemenu->nPatternAnim);
			// �ړ���̍��W�Œ��_��ݒ�
			SetVertexTitlemenu(i);
		}

		// �I�����Ă��郁�j���[�ɑJ��
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_NUMPADENTER) || IsButtonTriggered(0, R_BUTTON_A))
		{
			switch (g_nSelectTitlemenu % TITLEMENU_MAX)
			{
			case TITLE_MENU_START:
				SetFade(FADE_OUT, STAGE_GAME);
				SetSe(2, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_TUTORIAL:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_CREDIT:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_OPTION:
				SetSe(14, E_DS8_FLAG_NONE, true);
				break;
			case TITLE_MENU_EXIT:
				SetSe(2, E_DS8_FLAG_NONE, true);
				SetEndFlag();
				break;
			}
			titlemenusys->bSelect = true;		// �����͏������~�߂�
		}

		// ESC��EXIT��I��
		if (GetKeyboardTrigger(DIK_ESCAPE) || IsButtonTriggered(0, R_BUTTON_B))
		{
			g_nSelectTitlemenu = TITLE_MENU_EXIT;
			SetSe(3, E_DS8_FLAG_NONE, true);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitlemenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (titlemenu->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, titlemenu->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLEMENU, titlemenu->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitlemenu(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// ���_���W�̐ݒ�
	titlemenu->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	titlemenu->vertexWk[1].vtx = D3DXVECTOR3(100.0f+TEXTURE_TITLEMENU00_SIZE_X, 100.0f, 0.0f);
	titlemenu->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f+TEXTURE_TITLEMENU00_SIZE_Y, 0.0f);
	titlemenu->vertexWk[3].vtx = D3DXVECTOR3(100.0f+TEXTURE_TITLEMENU00_SIZE_X, 100.0f+TEXTURE_TITLEMENU00_SIZE_Y, 0.0f);
	//SetVertexTitlemenu();

	// rhw�̐ݒ�
	titlemenu->vertexWk[0].rhw =
	titlemenu->vertexWk[1].rhw =
	titlemenu->vertexWk[2].rhw =
	titlemenu->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	titlemenu->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	titlemenu->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	titlemenu->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	titlemenu->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLEMENU, 0.0f );
	titlemenu->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU );
	titlemenu->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLEMENU, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU );

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTitlemenu( int no, int cntPattern )
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TITLEMENU;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TITLEMENU;
	titlemenu->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	titlemenu->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	titlemenu->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	titlemenu->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexTitlemenu(int no)
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	titlemenu->vertexWk[0].vtx.x = titlemenu->pos.x - TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[0].vtx.y = titlemenu->pos.y - TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[0].vtx.z = 0.0f;

	titlemenu->vertexWk[1].vtx.x = titlemenu->pos.x + TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[1].vtx.y = titlemenu->pos.y - TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[1].vtx.z = 0.0f;

	titlemenu->vertexWk[2].vtx.x = titlemenu->pos.x - TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[2].vtx.y = titlemenu->pos.y + TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[2].vtx.z = 0.0f;

	titlemenu->vertexWk[3].vtx.x = titlemenu->pos.x + TEXTURE_TITLEMENU00_SIZE_X;
	titlemenu->vertexWk[3].vtx.y = titlemenu->pos.y + TEXTURE_TITLEMENU00_SIZE_Y;
	titlemenu->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTitlemenu(int no, D3DXCOLOR color)
{
	TITLEMENU *titlemenu = &titlemenuWk[no];

	// ���ˌ��̐ݒ�
	titlemenu->vertexWk[0].diffuse =
		titlemenu->vertexWk[1].diffuse =
		titlemenu->vertexWk[2].diffuse =
		titlemenu->vertexWk[3].diffuse = color;
}

//=============================================================================
// �A�N�e�B�u����
//=============================================================================
void SetTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// ���g�p����
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (!titlemenu->bUse)
		{
			// �L���ɂ���
			titlemenu->bUse = true;
			// �A�j���[�V�����J�E���^�̏�����
			titlemenu->nCountAnim = 0;
			// �A�j���[�V�����p�^�[���̏�����
			titlemenu->nPatternAnim = 0;
		}
	}
}

//=============================================================================
// �m���A�N�e�B�u����
//=============================================================================
void ReleaseTitlemenu(void)
{
	TITLEMENU *titlemenu = &titlemenuWk[0];

	// �g�p����
	for (int i = 0; i < TITLEMENU_MAX; i++, titlemenu++)
	{
		if (titlemenu->bUse)
		{
			// �����ɂ���
			titlemenu->bUse = false;
		}
	}
}

//=============================================================================
// �^�C�g���擾�֐�
//=============================================================================
TITLEMENU *GetTitlemenu(int no)
{
	return(&titlemenuWk[no]);
}

