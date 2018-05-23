//=============================================================================
//
// MINIMAP���� [minimap.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "minimap.h"
#include "model.h"
#include "enemy.h"
#include "main.h"
#include "calculate.h"
#include "pointer.h"
#include "magic.h"
#include "camera.h"

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
HRESULT MakeVertexMinimap(int no);
void SetVertexMinimap(int no);
void SetDiffuseMinimap(int no, D3DXCOLOR color);
void SetTextureMinimap(int no, int cntPattern);
void SetChangeCollarMinimap(int no);
void SearchMinimap(void);
void UpdateMinimapPoint(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
MINIMAP					minimapWk[MINIMAP_MAX];
MINIMAP_SYS				minimapSysWk[MINIMAP_SYS_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureMinimap[MINIMAP_MAX];

D3DXVECTOR3				g_posMinimap;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitMinimap(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[0];
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

	g_posMinimap = D3DXVECTOR3(TEXTURE_MINIMAP_POS_X, TEXTURE_MINIMAP_POS_Y, 0.0f);

	if (type == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_MINIMAP00,				// �t�@�C���̖��O
			&pD3DTextureMinimap[0]);			// �ǂݍ��ރ������̃|�C���^
												// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_MINIMAP01,				// �t�@�C���̖��O
			&pD3DTextureMinimap[1]);			// �ǂݍ��ރ������̃|�C���^
	}

	// MINIMAP�̏�����
	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		minimap->bUse = true;
		minimap->pos = D3DXVECTOR3(TEXTURE_MINIMAP_POS_X, TEXTURE_MINIMAP_POS_Y, 0.0f);
		minimap->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (i == MINIMAP_BACK)
		{
			minimap->colorMinimap = D3DXCOLOR(1.0f, 1.0f, 1.0f, MINIMAP_ALPHA);
		}
		else
		{
			minimap->colorMinimap = GetMagicColor();
		}
		minimap->fBaseAngle = 0.0f;
		minimap->fRadius = 0.0f;

		minimap->nCollarFlag = 0;

		minimap->Texture = pD3DTextureMinimap[i];			// �e�N�X�`���ւ�MINIMAP
		minimap->size = D3DXVECTOR2(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y);

		// ���_���̍쐬
		MakeVertexMinimap(i);
	}

	for (int i = 0; i < MINIMAP_SYS_MAX; i++, minimapSys++)
	{
		minimapSys->bUse = false;
		minimapSys->nType = 0;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	for (int i = 0; i < MINIMAP_MAX; i++)
	{
		// ���������
		if (pD3DTextureMinimap[i] != NULL)
		{
			pD3DTextureMinimap[i]->Release();
			pD3DTextureMinimap[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

#ifdef _DEBUG
	PrintDebugProc("�y MINIMAP �z\n");
#endif
	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		if (minimap->bUse)
		{
			if (i == MINIMAP_BACK)
			{	// �o�b�N�̂݉�]�E�F�ύX
				minimap->rot.z += 0.003f;				// ��]
				SetDiffuseMinimap(i, GetMagicColor());	// magic�̃J���[���擾���ēK�p
			}
			SetVertexMinimap(i);						// �ړ���̍��W�Œ��_��ݒ�
		}
	}

	SearchMinimap();
	UpdateMinimapPoint();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMinimap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		if (minimap->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, minimap->Texture);

			// MINIMAP�̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MINIMAP, minimap->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// �\���Ώی�������
//=============================================================================
void SearchMinimap(void)
{
	MODEL *model = GetModel(0);
	ENEMY *enemy = GetEnemy(0);
	MINIMAP_SYS *minimapSys;

	// ���f��
	for (int i = 0; i < MODEL_MAX; i++, model++)
	{	// �S�Ẵ��f�����m�F
		if (model->bUse && !model->bUseMinimap)
		{	// �~�j�}�b�v�t���O���Ȃ����
			minimapSys = &minimapSysWk[0];
			for (int j = 0; j < MINIMAP_SYS_MAX; j++, minimapSys++)
			{	// ���g�p�̃}�b�v�V�X�e����T����
				if (!minimapSys->bUse)
				{	// �Z�b�g�A�b�v����
					minimapSys->bUse = true;
					minimapSys->nType = MINIMAP_MODEL;
					minimapSys->nIndexTag = i;
					minimapSys->nIndexPointer = SetPointer(SetColorPallet(COLOR_PALLET_GREEN));
					model->bUseMinimap = true;
					break;
				}
			}
		}
	}

	// �G�l�~�[
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{	// �S�ẴG�l�~�[���m�F
		if (enemy->bUse && !enemy->bUseMinimap)
		{	// �~�j�}�b�v�t���O���Ȃ����
			minimapSys = &minimapSysWk[0];
			for (int j = 0; j < MINIMAP_SYS_MAX; j++, minimapSys++)
			{	// ���g�p�̃}�b�v�V�X�e����T����
				if (!minimapSys->bUse)
				{	// �Z�b�g�A�b�v����
					minimapSys->bUse = true;
					minimapSys->nType = MINIMAP_ENEMY;
					minimapSys->nIndexTag = i;
					minimapSys->nIndexPointer = SetPointer(SetColorPallet(COLOR_PALLET_RED));
					enemy->bUseMinimap = true;
					break;
				}
			}
		}
	}
}

//=============================================================================
// �|�C���g�X�V����
//=============================================================================
void UpdateMinimapPoint(void)
{
	MODEL *model;
	ENEMY *enemy;
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

	D3DXVECTOR2 pos2Temp;		// ��r�e���|����
	D3DXVECTOR3 pos3Temp;		// ���W
	float		fTempTa;		// �����e���|����
	float		fTempAngle;		// �p�x�e���|����

	for (int i = 0; i < MINIMAP_SYS_MAX; i++, minimapSys++)
	{
		if (minimapSys->bUse)
		{	// �g���Ă���~�j�}�b�v�V�X�e����T��
			switch (minimapSys->nType)
			{	// �Ώۂ����f�����G�l�~�[���ŐU�蕪��
			case MINIMAP_MODEL:
				model = GetModel(minimapSys->nIndexTag);
				if (model->bUse)
				{	// �g���Ă�����X�V
					SetUpdatePointer(minimapSys->nIndexPointer, POINTER_TEXTURE_NORMAIL, g_posMinimap, 0.0f);
				}
				else
				{	// �g���Ă��Ȃ�������|�C���^�[��j��
					ReleasePointer(minimapSys->nIndexPointer);
					minimapSys->bUse = false;			// �~�j�}�b�v�V�X�e���𖳔j��
				}
				break;
			case MINIMAP_ENEMY:
				enemy = GetEnemy(minimapSys->nIndexTag);
				if (enemy->bUse)
				{	// �g���Ă�����X�V
					pos2Temp = D3DXVECTOR2(0.0f, 0.0f);						// ��r�e���|������������
					fTempTa = 0.0f;											// �����e���|������������
					fTempAngle = 0.0f;										// �p�x�e���|������������

					pos2Temp.x = (enemy->posEnemy.x - camera->posCameraAt.x) / MINIMAP_POS_CONVERT;	// X���W���Z�o
					pos2Temp.y = -((enemy->posEnemy.z - camera->posCameraAt.z) / MINIMAP_POS_CONVERT);	// Z���W��Y���W�ɕϊ����ĎZ�o

					fTempTa = D3DXVec2Length(&pos2Temp);							// �������Z�o

#ifdef _DEBUG
					PrintDebugProc("Minimap[%d]  TempTa:%f\n",i, fTempTa);
#endif
					if (pos2Temp.x == 0.0f && pos2Temp.y == 0.0f)		// atan2�̃G���[�h�~	
					{	// �G���[�h�~�C��
						pos2Temp.x += 0.00001f;
					}
					fTempAngle = atan2(pos2Temp.y, pos2Temp.x);			// �p�x�Z�o
					fTempAngle += camera->fHAngle - D3DX_PI * 1.5f;		// �Z�o�����p�x�ɃJ�����A���O�������Z

					if (fTempTa <= MINIMAP_POS_LENGTH_MAX)
					{	// �~�j�}�b�v�\���������ł���΃|�C���^�[���X�V
						pos3Temp = D3DXVECTOR3(								// ���W�e���|�����ɉ�]���
							fTempTa * cosf(fTempAngle),						// X���W
							fTempTa * sinf(fTempAngle),						// Y���W
							0.0f);											// Z���W�i0.0f)���i�[
						SetUpdatePointer(minimapSys->nIndexPointer,			// �|�C���^�[�̍X�V
							POINTER_TEXTURE_NORMAIL,
							g_posMinimap + pos3Temp,
							(enemy->rotEnemy.y + camera->fHAngle) - D3DX_PI * 0.5f);
					}
					else
					{	// �~�j�}�b�v�\�������O�ł���΃A�E�g���C�����X�V
						pos3Temp = D3DXVECTOR3(								// ���W�e���|�����ɉ�]���
							MINIMAP_POS_LENGTH_MAX * cosf(fTempAngle),						// X���W
							MINIMAP_POS_LENGTH_MAX * sinf(fTempAngle),						// Y���W
							0.0f);											// Z���W�i0.0f)���i�[
						SetUpdatePointer(minimapSys->nIndexPointer,			// �|�C���^�[�̍X�V
							POINTER_TEXTURE_OUTLINE,
							g_posMinimap + pos3Temp,
							(enemy->rotEnemy.y + camera->fHAngle) - D3DX_PI * 0.5f);
					}
				}
				else
				{	// �g���Ă��Ȃ�������|�C���^�[��j��
					ReleasePointer(minimapSys->nIndexPointer);
					minimapSys->bUse = false;			// �~�j�}�b�v�V�X�e���𖳔j��
				}
				break;
			}
		}
	}

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexMinimap(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[no];

	// �p�x�Ɣ��a�����߂�
	minimap->fBaseAngle = atan2f(TEXTURE_MINIMAP_SIZE_Y, TEXTURE_MINIMAP_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y);
	minimap->fRadius = D3DXVec2Length(&temp);

	// ���_���W�̐ݒ�
	minimap->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	minimap->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MINIMAP_SIZE_X, 100.0f, 0.0f);
	minimap->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MINIMAP_SIZE_Y, 0.0f);
	minimap->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MINIMAP_SIZE_X, 100.0f + TEXTURE_MINIMAP_SIZE_Y, 0.0f);
	//SetVertexMinimap();

	// rhw�̐ݒ�
	minimap->vertexWk[0].rhw =
	minimap->vertexWk[1].rhw =
	minimap->vertexWk[2].rhw =
	minimap->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	minimap->vertexWk[0].diffuse =
	minimap->vertexWk[1].diffuse =
	minimap->vertexWk[2].diffuse =
	minimap->vertexWk[3].diffuse = minimap->colorMinimap;

	// �e�N�X�`�����W�̐ݒ�
	minimap->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	minimap->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP, 0.0f);
	minimap->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP);
	minimap->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP);

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVertexMinimap(int no)
{
	MINIMAP *minimap = &minimapWk[no];
	// ���_0:��]����
	minimap->vertexWk[0].vtx.x = minimap->pos.x - cosf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[0].vtx.y = minimap->pos.y - sinf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[0].vtx.z = 0.0f;

	// ���_1:��]����
	minimap->vertexWk[1].vtx.x = minimap->pos.x + cosf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[1].vtx.y = minimap->pos.y - sinf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[1].vtx.z = 0.0f;

	// ���_2:��]����
	minimap->vertexWk[2].vtx.x = minimap->pos.x - cosf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[2].vtx.y = minimap->pos.y + sinf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[2].vtx.z = 0.0f;

	// ���_3:��]����
	minimap->vertexWk[3].vtx.x = minimap->pos.x + cosf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[3].vtx.y = minimap->pos.y + sinf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseMinimap(int no, D3DXCOLOR color)
{
	MINIMAP *minimap = &minimapWk[no];

	// ���ˌ��̐ݒ�
	minimap->vertexWk[0].diffuse =
	minimap->vertexWk[1].diffuse =
	minimap->vertexWk[2].diffuse =
	minimap->vertexWk[3].diffuse = color;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureMinimap(int no, int cntPattern)
{
	MINIMAP *minimap = &minimapWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP;
	minimap->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	minimap->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	minimap->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	minimap->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �J���[�ύX�֐�
//=============================================================================
void SetChangeCollarMinimap(int no)
{
	MINIMAP *minimap = &minimapWk[no];

	switch (minimap->nCollarFlag)
	{
	case 0:
		minimap->colorMinimap -= D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		if (minimap->colorMinimap.r <= 0.0f)
		{
			minimap->colorMinimap.r = 0.0f;
			minimap->nCollarFlag++;
		}
		if (minimap->colorMinimap.g >= 1.0f)
		{
			minimap->colorMinimap.g = 1.0f;
		}
		if (minimap->colorMinimap.b >= 1.0f)
		{
			minimap->colorMinimap.b = 1.0f;
		}
		break;
	case 1:
		minimap->colorMinimap -= D3DXCOLOR(0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		if (minimap->colorMinimap.r >= 1.0f)
		{
			minimap->colorMinimap.r = 1.0f;
		}
		if (minimap->colorMinimap.g <= 0.0f)
		{
			minimap->colorMinimap.g = 0.0f;
			minimap->nCollarFlag++;
		}
		if (minimap->colorMinimap.b >= 1.0f)
		{
			minimap->colorMinimap.b = 1.0f;
		}
		break;
	case 2:
		minimap->colorMinimap -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f);
		if (minimap->colorMinimap.r >= 1.0f)
		{
			minimap->colorMinimap.r = 1.0f;
		}
		if (minimap->colorMinimap.g >= 1.0f)
		{
			minimap->colorMinimap.g = 1.0f;
		}
		if (minimap->colorMinimap.b <= 0.0f)
		{
			minimap->colorMinimap.b = 0.0f;
			minimap->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// �L���ݒ�
//=============================================================================
void SetMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	minimap->bUse = true;
}

//=============================================================================
// �����ݒ�
//=============================================================================
void ReleaseMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	minimap->bUse = false;
}

//=============================================================================
// MINIMAP�擾�֐�
//=============================================================================
MINIMAP *GetMinimap(int no)
{
	return(&minimapWk[no]);
}

