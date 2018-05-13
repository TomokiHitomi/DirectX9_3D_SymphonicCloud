//=============================================================================
//
// MAGIC���� [magic.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "magic.h"
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "calculate.h"
#include "gage.h"
#include "magiceffect.h"
#include "model.h"
#include "gageselect.h"
#include "sound.h"

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
HRESULT MakeVertexMagic(int no);
void SetVertexMagic(int no);
void SetDiffuseMagic(int no, D3DXCOLOR color);
void SetTextureMagic(int no, int cntPattern);
void SetChangeCollarMagic(int no);
void SetMagicBgm(void);
void ChangeMagic(int nMagic);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
MAGIC					magicWk[MAGIC_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureMagic = NULL;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMagic(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[0];

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_MAGIC,				// �t�@�C���̖��O
			&pD3DTextureMagic);			// �ǂݍ��ރ������̃|�C���^
	}

	// MAGIC�̏�����
	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		magic->bUse = true;
		if (i == MAGIC_MAIN)	// ���C���}�W�b�N����
		{
			magic->pos = D3DXVECTOR3(TEXTURE_MAGIC_POS_X, TEXTURE_MAGIC_POS_Y, 0.0f);
			magic->rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
		}
		if (i == MAGIC_DUMMY)	// �_�~�[�}�W�b�N01����
		{
			magic->pos = D3DXVECTOR3(TEXTURE_MAGIC_DUMMY_POS_X, TEXTURE_MAGIC_DUMMY_POS_Y, 0.0f);
			magic->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		magic->colorMagic = SetColorPallet(COLOR_PALLET_GRAY);
		magic->fBaseAngle = 0.0f;
		magic->fRadius = 0.0f;
		magic->fTypeAngle = (D3DX_PI * 2) / (BULLET_TYPE_MAX - 1);

		magic->bActiveFire = false;
		magic->bActiveThunder = false;
		magic->bActiveIce = false;
		magic->bActiveWind = false;
		magic->bActiveWater = false;
		magic->bActiveEarth = false;

		magic->nColorFlag = 0;
		magic->nType = 0;
		magic->nCoolDown = 0;
		magic->bActive = false;
		magic->bActiveColor = false;
		magic->bSelectCheck = false;


		magic->Texture = pD3DTextureMagic;			// �e�N�X�`���ւ�MAGIC
		magic->size = D3DXVECTOR2(TEXTURE_MAGIC_SIZE_X, TEXTURE_MAGIC_SIZE_Y);

		// ���_���̍쐬
		MakeVertexMagic(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMagic(void)
{
	MAGIC *magic = &magicWk[0];

	// ���������
	if (pD3DTextureMagic != NULL)
	{
		pD3DTextureMagic->Release();
		pD3DTextureMagic = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMagic(void)
{
	MAGIC *magic = &magicWk[0];

#ifdef _DEBUG
	PrintDebugProc("�y MAGIC �z\n");
#endif
	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		if (magic->bUse)
		{
			if (i == MAGIC_MAIN)	// ���C���}�W�b�N����
			{
				SetMagicBgm();		// BGM����
			}
			if (magic->bActive)
			{
				if (i == MAGIC_MAIN)	// ���C���}�W�b�N����
				{
					ChangeMagic(i);		// �X�L���ύX
					// ��]��ƌ��݂̊p�x���v�Z
				

					magic->fGoalAngle = (magic->nType * magic->fTypeAngle - D3DX_PI * 0.5f) - magic->rot.z;
					magic->fGoalAngle = PiCalculate180(magic->fGoalAngle);
					// �p�x����K�p
					magic->rot.z += magic->fGoalAngle * MAGIC_ROT_INERTIA;
					magic->rot.z = PiCalculate180(magic->rot.z);
				}
				if (i == MAGIC_DUMMY)	// �_�~�[�}�W�b�N����
				{
					magic->colorMagic = magicWk[MAGIC_MAIN].colorMagic;
					magic->rot.z = magicWk[MAGIC_MAIN].rot.z + D3DX_PI * 0.5f;
				}


				SetChangeCollarMagic(i);					// �J���[�ύX����
				SetDiffuseMagic(i, magic->colorMagic);		// �J���[�K�p����
			}
			SetVertexMagic(i);							// �ړ���̍��W�Œ��_��ݒ�
#ifdef _DEBUG
			PrintDebugProc("MagicType[%d]  RotZ:%f  Goal:%f\n", magic->nType, magic->rot.z, magic->fGoalAngle);
#endif
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		if (magic->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, magic->Texture);

			// MAGIC�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MAGIC, magic->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexMagic(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[no];

	// �p�x�Ɣ��a�����߂�
	magic->fBaseAngle = atan2f(TEXTURE_MAGIC_SIZE_Y, TEXTURE_MAGIC_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_MAGIC_SIZE_X, TEXTURE_MAGIC_SIZE_Y);
	magic->fRadius = D3DXVec2Length(&temp);

	// ���_���W�̐ݒ�
	magic->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	magic->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGIC_SIZE_X, 100.0f, 0.0f);
	magic->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MAGIC_SIZE_Y, 0.0f);
	magic->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGIC_SIZE_X, 100.0f + TEXTURE_MAGIC_SIZE_Y, 0.0f);
	//SetVertexMagic();

	// rhw�̐ݒ�
	magic->vertexWk[0].rhw =
	magic->vertexWk[1].rhw =
	magic->vertexWk[2].rhw =
	magic->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	magic->vertexWk[0].diffuse =
	magic->vertexWk[1].diffuse =
	magic->vertexWk[2].diffuse =
	magic->vertexWk[3].diffuse = magic->colorMagic;

	// �e�N�X�`�����W�̐ݒ�
	magic->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	magic->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC, 0.0f);
	magic->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC);
	magic->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexMagic(int no)
{
	MAGIC *magic = &magicWk[no];
	// ���_0:��]����
	magic->vertexWk[0].vtx.x = magic->pos.x - cosf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[0].vtx.y = magic->pos.y - sinf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	magic->vertexWk[1].vtx.x = magic->pos.x + cosf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[1].vtx.y = magic->pos.y - sinf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	magic->vertexWk[2].vtx.x = magic->pos.x - cosf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[2].vtx.y = magic->pos.y + sinf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	magic->vertexWk[3].vtx.x = magic->pos.x + cosf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[3].vtx.y = magic->pos.y + sinf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseMagic(int no, D3DXCOLOR color)
{
	MAGIC *magic = &magicWk[no];

	// ���ˌ��̐ݒ�
	magic->vertexWk[0].diffuse =
	magic->vertexWk[1].diffuse =
	magic->vertexWk[2].diffuse =
	magic->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureMagic(int no, int cntPattern)
{
	MAGIC *magic = &magicWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC;
	magic->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	magic->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	magic->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	magic->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarMagic(int no)
{
	MAGIC *magic = &magicWk[no];

	// �I�𒆂̃}�W�b�N��MP�����邩�m�F
	if (CheckMagicModel(0, magic->nType))
	{	// MP������ΊY���F�ɃJ���[�`�F���W
		switch (magic->nType + 1)
		{
		case BULLET_SPECIAL_FIRE:
			magic->nColorFlag = COLOR_PALLET_ORANGE;
			break;
		case BULLET_SPECIAL_THUNDER:
			magic->nColorFlag = COLOR_PALLET_YELLOW;
			break;
		case BULLET_SPECIAL_ICE:
			magic->nColorFlag = COLOR_PALLET_CYAN;
			break;
		case BULLET_SPECIAL_WIND:
			magic->nColorFlag = COLOR_PALLET_LIGHTGREEN;
			break;
		case BULLET_SPECIAL_WATER:
			magic->nColorFlag = COLOR_PALLET_BLUE;
			break;
		case BULLET_SPECIAL_EARTH:
			magic->nColorFlag = COLOR_PALLET_BROWN;
			break;
		}
	}
	else
	{	// MP���Ȃ���΃O���[�A�E�g
		magic->nColorFlag = COLOR_PALLET_GRAY;
	}
	
	// �ԐF����
	if (magic->colorMagic.r > SetColorPallet(magic->nColorFlag).r)
	{	// �Z�b�g�J���[��荂���ꍇ�͌��Z
		magic->colorMagic.r -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.r < 0.0f)
		{
			magic->colorMagic.r = 0.0f;
		}
	}
	else if (magic->colorMagic.r < SetColorPallet(magic->nColorFlag).r)
	{	// �Z�b�g�J���[���Ⴂ�ꍇ�͌��Z
		magic->colorMagic.r += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.r > 1.0f)
		{
			magic->colorMagic.r = 1.0f;
		}
	}
	// �ΐF����
	if (magic->colorMagic.g > SetColorPallet(magic->nColorFlag).g)
	{	// �Z�b�g�J���[��荂���ꍇ�͌��Z
		magic->colorMagic.g -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.g < 0.0f)
		{
			magic->colorMagic.g = 0.0f;
		}
	}
	else if (magic->colorMagic.g < SetColorPallet(magic->nColorFlag).g)
	{	// �Z�b�g�J���[���Ⴂ�ꍇ�͌��Z
		magic->colorMagic.g += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.g > 1.0f)
		{
			magic->colorMagic.g = 1.0f;
		}
	}
	// �F����
	if (magic->colorMagic.b > SetColorPallet(magic->nColorFlag).b)
	{	// �Z�b�g�J���[��荂���ꍇ�͌��Z
		magic->colorMagic.b -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.b < 0.0f)
		{
			magic->colorMagic.b = 0.0f;
		}
	}
	else if (magic->colorMagic.b < SetColorPallet(magic->nColorFlag).b)
	{	// �Z�b�g�J���[���Ⴂ�ꍇ�͌��Z
		magic->colorMagic.b += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.b > 1.0f)
		{
			magic->colorMagic.b = 1.0f;
		}
	}
}

//=============================================================================
// �X�L���ύX�֐�
//=============================================================================
void ChangeMagic(int nMagic)
{
	MAGIC *magic = &magicWk[nMagic];

	// �L�[�{�[�h
	if (GetKeyboardTrigger(DIK_1) && magic->bActiveFire)
	{
		magic->nType = BULLET_SPECIAL_FIRE - 1;
	}
	else if (GetKeyboardTrigger(DIK_2) && magic->bActiveThunder)
	{
		magic->nType = BULLET_SPECIAL_THUNDER - 1;
	}
	else if (GetKeyboardTrigger(DIK_3) && magic->bActiveIce)
	{
		magic->nType = BULLET_SPECIAL_ICE - 1;
	}
	else if (GetKeyboardTrigger(DIK_4) && magic->bActiveWind)
	{
		magic->nType = BULLET_SPECIAL_WIND - 1;
	}
	else if (GetKeyboardTrigger(DIK_5) && magic->bActiveWater)
	{
		magic->nType = BULLET_SPECIAL_WATER - 1;
	}
	else if (GetKeyboardTrigger(DIK_6) && magic->bActiveEarth)
	{
		magic->nType = BULLET_SPECIAL_EARTH - 1;
	}

	if (GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(0, R_BUTTON_X))
	{
		magic->bSelectCheck = true;
		magic->nType--;							// �^�C�v�����Z
		if (magic->nType < 0)					// �^�C�v�ŏ������z���Ă�����
		{
			magic->nType = MAGIC_TYPE_MAX - 1;	// �ő吔��n��
		}
	}
	else if (GetKeyboardTrigger(DIK_E) || IsButtonTriggered(0, R_BUTTON_A))
	{
		magic->bSelectCheck = false;
		magic->nType++;							// �^�C�v�����Z
		if (magic->nType >= MAGIC_TYPE_MAX)		// �^�C�v�ő吔���z���Ă�����
		{
			magic->nType = 0;					// �ŏ�����n��
		}
	}

	// �}�E�X�z�C�[������
	if (magic->nCoolDown <= 0)							// ���]�h�~
	{
		float fTempRot = (float)GetMobUseZ();			// �}�E�X�z�C�[���̉�]�擾
		if ((float)GetMobUseZ())
		{
			magic->nCoolDown = MAGIC_ROT_COOLDOWN;
			if (fTempRot > 0.0f)						// �߂�l���v���X
			{
				magic->bSelectCheck = true;
				magic->nType--;							// �^�C�v�����Z
				if (magic->nType < 0)					// �^�C�v�ŏ������z���Ă�����
				{
					magic->nType = MAGIC_TYPE_MAX - 1;	// �ő吔��n��
				}
			}
			else if (fTempRot < 0.0f)					// �߂�l���}�C�i�X
			{
				magic->bSelectCheck = false;
				magic->nType++;							// �^�C�v�����Z
				if (magic->nType >= MAGIC_TYPE_MAX)		// �^�C�v�ő吔���z���Ă�����
				{
					magic->nType = 0;					// �ŏ�����n��
				}
			}
		}
	}
	magic->nCoolDown--;

	if(!magic->bSelectCheck)
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			if (!magic->bActiveFire) magic->nType++;
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			if (!magic->bActiveThunder) magic->nType++;
			break;
		case BULLET_SPECIAL_ICE - 1:
			if (!magic->bActiveIce) magic->nType++;
			break;
		case BULLET_SPECIAL_WIND - 1:
			if (!magic->bActiveWind) magic->nType++;
			break;
		case BULLET_SPECIAL_WATER - 1:
			if (!magic->bActiveWater) magic->nType++;
			break;
		case BULLET_SPECIAL_EARTH - 1:
			if (!magic->bActiveEarth) magic->nType++;
			if (magic->nType >= MAGIC_TYPE_MAX) magic->nType = 0;
			break;
		}
	}
	else
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			if (!magic->bActiveFire) magic->nType--;
			if (magic->nType < 0) magic->nType = BULLET_SPECIAL_EARTH - 1;
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			if (!magic->bActiveThunder) magic->nType--;
			break;
		case BULLET_SPECIAL_ICE - 1:
			if (!magic->bActiveIce) magic->nType--;
			break;
		case BULLET_SPECIAL_WIND - 1:
			if (!magic->bActiveWind) magic->nType--;
			break;
		case BULLET_SPECIAL_WATER - 1:
			if (!magic->bActiveWater) magic->nType--;
			break;
		case BULLET_SPECIAL_EARTH - 1:
			if (!magic->bActiveEarth) magic->nType--;
			break;
		}
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetMagicBgm(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];

	if (!magic->bActive)
	{
		SetSoundBgm(SOUND_BGM_GAME_NORMAL);
	}
	else
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			SetSoundBgm(SOUND_BGM_GAME_FIRE);
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			SetSoundBgm(SOUND_BGM_GAME_THUNDER);
			break;
		case BULLET_SPECIAL_ICE - 1:
			SetSoundBgm(SOUND_BGM_GAME_ICE);
			break;
		case BULLET_SPECIAL_WIND - 1:
			SetSoundBgm(SOUND_BGM_GAME_WIND);
			break;
		case BULLET_SPECIAL_WATER - 1:
			SetSoundBgm(SOUND_BGM_GAME_WATER);
			break;
		case BULLET_SPECIAL_EARTH - 1:
			SetSoundBgm(SOUND_BGM_GAME_EARTH);
			break;
		}
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetMagic(void)
{
	MAGIC *magic = &magicWk[0];
	magic->bUse = true;
}

//=============================================================================
// �^�C�v�ݒ�֐�
//=============================================================================
void SetMagicType(int nMagicType)
{
	MAGIC *magic = &magicWk[0];
	switch (nMagicType)
	{
	case BULLET_SPECIAL_FIRE:
		magic->bActiveFire = true;
		SetMagiceffect(MAGICEFFECT_FIRE);
		SetGage(GAGE_BULLET_SPECIAL_FIRE, 0);
		SetMagicModel(0, BULLET_SPECIAL_FIRE);
		break;
	case BULLET_SPECIAL_THUNDER:
		magic->bActiveThunder = true;
		SetMagiceffect(MAGICEFFECT_THUNDER);
		SetGage(GAGE_BULLET_SPECIAL_THUNDER, 0);
		SetMagicModel(0, BULLET_SPECIAL_THUNDER);
		break;
	case BULLET_SPECIAL_ICE:
		magic->bActiveIce = true;
		SetMagiceffect(MAGICEFFECT_ICE);
		SetGage(GAGE_BULLET_SPECIAL_ICE, 0);
		SetMagicModel(0, BULLET_SPECIAL_ICE);
		break;
	case BULLET_SPECIAL_WIND:
		magic->bActiveWind = true;
		SetMagiceffect(MAGICEFFECT_WIND);
		SetGage(GAGE_BULLET_SPECIAL_WIND, 0);
		SetMagicModel(0, BULLET_SPECIAL_WIND);
		break;
	case BULLET_SPECIAL_WATER:
		magic->bActiveWater = true;
		SetMagiceffect(MAGICEFFECT_WATER);
		SetGage(GAGE_BULLET_SPECIAL_WATER, 0);
		SetMagicModel(0, BULLET_SPECIAL_WATER);
		break;
	case BULLET_SPECIAL_EARTH:
		magic->bActiveEarth = true;
		SetMagiceffect(MAGICEFFECT_EARTH);
		SetGage(GAGE_BULLET_SPECIAL_EARTH, 0);
		SetMagicModel(0, BULLET_SPECIAL_EARTH);
		break;
	}

	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		magic->bActive = true;
	}
	SetGageselect();
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseMagic(void)
{
	MAGIC *magic = &magicWk[0];
	magic->bUse = false;
}

//=============================================================================
// MAGIC�擾�֐�
//=============================================================================
MAGIC *GetMagic(int no)
{
	return(&magicWk[no]);
}

//=============================================================================
// MAGIC�^�C�v�擾�֐�
//=============================================================================
int GetTypeMagic(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];
	return(magic->nType);
}

//=============================================================================
// �J���[�擾�֐�
//=============================================================================
D3DXCOLOR GetMagicColor(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];
	return(magic->colorMagic);
}

