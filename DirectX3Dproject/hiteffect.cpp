//=============================================================================
//
// HITEFFECT���� [hiteffect.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "hiteffect.h"
#include "main.h"

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
HRESULT	MakeVertexHiteffect(LPDIRECT3DDEVICE9 pDevice);
void	SetVtxHiteffect(int nHiteffect, float fSizeX, float fSizeY);
void	SetDiffuseHiteffect(int nHiteffect, D3DXCOLOR col);
void	SetTexHiteffect(int nHiteffect, int nTex);
void	InitStatusHiteffect(int nHiteffect);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureHiteffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffHiteffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldHiteffect;			// ���[���h�}�g���b�N�X

HITEFFECT					hiteffectWk[HITEFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHiteffect(int nType)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_HITEFFECT,
			&g_pD3DTextureHiteffect);
	}

	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		hiteffect->posHiteffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		hiteffect->rotHiteffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		hiteffect->sclHiteffect = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		hiteffect->vec2Size = D3DXVECTOR2(HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y);
		InitStatusHiteffect(i);
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexHiteffect(pDevice);

	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusHiteffect(int nHiteffect)
{
	HITEFFECT *hiteffect = &hiteffectWk[nHiteffect];

	hiteffect->bUse = false;
	hiteffect->nTex = 0;
	hiteffect->nAnimeCount = 0;
	hiteffect->nAnimePattern = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHiteffect(void)
{
	if (g_pD3DTextureHiteffect != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureHiteffect->Release();
		g_pD3DTextureHiteffect = NULL;
	}

	if (g_pD3DVtxBuffHiteffect != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffHiteffect->Release();
		g_pD3DVtxBuffHiteffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHiteffect(void)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];

#ifdef _DEBUG
	PrintDebugProc("�y HITEFFECT �z\n");
#endif
	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{	// �g�p���̃I�u�W�F�N�g�̂ݏ���
		if (hiteffect->bUse)
		{
			hiteffect->nAnimeCount++;
			if (hiteffect->nAnimeCount % TIME_ANIMATION_HITEFFECT == 0)
			{
				// �p�^�[���̐؂�ւ�
				hiteffect->nAnimePattern = (hiteffect->nAnimePattern + 1)
					% TEXTURE_PATTERN_DIVIDE_X_HITEFFECT
					+ (TEXTURE_PATTERN_DIVIDE_X_HITEFFECT* hiteffect->nTex);
				SetTexHiteffect(i, hiteffect->nAnimePattern);
				if (hiteffect->nAnimePattern == hiteffect->nTex * TEXTURE_PATTERN_DIVIDE_X_HITEFFECT)
				{
					InitStatusHiteffect(i);
				}

				//// �A�j���[�V����
				//player->nCountAnim++;
				//if ((player->nCountAnim % TIME_ANIMATION_PLAYER) == 0)
				//{
				//	// �p�^�[���̐؂�ւ�
				//	player->nPatternAnim = (player->nPatternAnim + 1) % (TEXTURE_PATTERN_DIVIDE_X_PLAYER)+(TEXTURE_PATTERN_DIVIDE_X_PLAYER * player->nDirection);
				//}

				//// �A�j���[�V����
				//effect_knockdown->nCountAnim++;
				//if ((effect_knockdown->nCountAnim % TIME_ANIMATION_EFFECT_KNOCKDOWN) == 0)
				//{
				//	// �p�^�[���̐؂�ւ�
				//	effect_knockdown->nPatternAnim++;
				//}

				//// �A�j���[�V�����I����false
				//if (effect_knockdown->nPatternAnim >= TEXTURE_PATTERN_DIVIDE_X_EFFECT_KNOCKDOWN * (effect_knockdown->nTypeAnim + 1))
				//{
				//	effect_knockdown->bUse = false;
				//}
			}
		}
	}
#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHiteffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	HITEFFECT *hiteffect = &hiteffectWk[0];

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, HITEFFECT_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		if (hiteffect->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldHiteffect);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();
			// �r���{�[�h��
			g_mtxWorldHiteffect._11 = mtxView._11;
			g_mtxWorldHiteffect._12 = mtxView._21;
			g_mtxWorldHiteffect._13 = mtxView._31;
			g_mtxWorldHiteffect._21 = mtxView._12;
			g_mtxWorldHiteffect._22 = mtxView._22;
			g_mtxWorldHiteffect._23 = mtxView._32;
			g_mtxWorldHiteffect._31 = mtxView._13;
			g_mtxWorldHiteffect._32 = mtxView._23;
			g_mtxWorldHiteffect._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, hiteffect->sclHiteffect.x, hiteffect->sclHiteffect.y, hiteffect->sclHiteffect.z);
			D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, hiteffect->rotHiteffect.y, hiteffect->rotHiteffect.x, hiteffect->rotHiteffect.z);
			//D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &GetInvRotateMat(hiteffect->posHiteffect));
			////D3DXMatrixMultiply(&hiteffect->mtxWorld, &hiteffect->mtxWorld, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, hiteffect->posHiteffect.x, hiteffect->posHiteffect.y, hiteffect->posHiteffect.z);
			D3DXMatrixMultiply(&g_mtxWorldHiteffect, &g_mtxWorldHiteffect, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldHiteffect);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffHiteffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureHiteffect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexHiteffect(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * HITEFFECT_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffHiteffect,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < HITEFFECT_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X, HITEFFECT_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-HITEFFECT_SIZE_X, -HITEFFECT_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(HITEFFECT_SIZE_X, -HITEFFECT_SIZE_Y, 0.0f);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffHiteffect->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxHiteffect(int nHiteffect, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseHiteffect(int nHiteffect, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexHiteffect(int nHiteffect, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffHiteffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nHiteffect * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_HITEFFECT;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_HITEFFECT;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffHiteffect->Unlock();
	}
}

//=============================================================================
// �o���b�g��Ώۂɐݒu
//=============================================================================
void SetHiteffect(D3DXVECTOR3 pos,int nTex)
{
	HITEFFECT *hiteffect = &hiteffectWk[0];

	// ���g�p��T��
	for (int i = 0; i < HITEFFECT_MAX; i++, hiteffect++)
	{
		if (!hiteffect->bUse)
		{
			hiteffect->bUse = true;
			hiteffect->posHiteffect = pos;
			hiteffect->nTex = nTex;
			return;
		}
	}
}

//=============================================================================
// �擾�֐�
//=============================================================================
HITEFFECT *GetHiteffect(int no)
{
	return(&hiteffectWk[no]);
}