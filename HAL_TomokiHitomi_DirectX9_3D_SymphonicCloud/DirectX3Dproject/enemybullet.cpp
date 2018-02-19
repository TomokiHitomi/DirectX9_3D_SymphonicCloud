//=============================================================================
//
// �o���b�g���� [enemybullet.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "camera.h"
#include "enemybullet.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
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

void InitStatusEnemybulletSys(int nEnemybulletSys);
HRESULT MakeVertexEnemybullet(LPDIRECT3DDEVICE9 pDevice);
void SetVtxEnemybullet(int nEnemybullet, float fSizeX, float fSizeY);
void SetDiffuseEnemybullet(int nEnemybullet, D3DXCOLOR col);
void SetTexEnemybullet(int nEnemybullet, int nTex);
void SetSysEnemybulletFire(int nSys);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemybullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEnemybullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldEnemybullet;			// ���[���h�}�g���b�N�X

ENEMYBULLET					enemybulletWk[ENEMYBULLET_MAX];
ENEMYBULLET_SYS				enemybulletSysWk[ENEMYBULLET_SYS_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemybullet(int nType)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_ENEMYBULLET,
			&g_pD3DTextureEnemybullet);
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		enemybullet->posEnemybullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->rotEnemybullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->sclEnemybullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		enemybullet->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemybullet->colorEnemybullet = D3DXCOLOR(ENEMYBULLET_DIF_R, ENEMYBULLET_DIF_G, ENEMYBULLET_DIF_B, ENEMYBULLET_DIF_A);
		InitStatusEnemybullet(i);
	}

	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++,enemybulletSys)
	{
		enemybulletSys->nCoolDown = 0;
		InitStatusEnemybulletSys(i);
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexEnemybullet(pDevice);

	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusEnemybullet(int nEnemybullet)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[nEnemybullet];

	enemybullet->vec2Size = D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);
	enemybullet->colorEnemybullet = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	enemybullet->nUseCount = 0;
	enemybullet->nSysNum = 0;
	enemybullet->nTex = 0;
	enemybullet->nEffectCount = 0;
	enemybullet->nAnimeCount = 0;
	enemybullet->nAnimePattern = 0;
	enemybullet->nType = 0;

	enemybullet->fMoveSpeed = 0.0f;
	enemybullet->fHAngle = 0.0f;
	enemybullet->fVAngle = 0.0f;
	enemybullet->fVecPower = 0.0f;

	enemybullet->bUse = false;
	enemybullet->bEnemy = false;
	enemybullet->bVertex = false;
	enemybullet->bHoming = true;

	enemybullet->nIdxShadow = 0;
	enemybullet->fSizeShadow = 0.0f;
	enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// �ė��p����SYS
//=============================================================================
void InitStatusEnemybulletSys(int nEnemybulletSys)
{
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[nEnemybulletSys];
	enemybulletSys->vecTagPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemybulletSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	enemybulletSys->colorEnemybullet = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	enemybulletSys->fHAngle = 0.0f;
	enemybulletSys->fVAngle = 0.0f;
	enemybulletSys->fMoveSpeed = 0.0f;
	enemybulletSys->fWave = 0.0f;

	enemybulletSys->nType = 0;
	enemybulletSys->nFireTime = 0;
	enemybulletSys->nSplitCount = 0;
	enemybulletSys->nTex = 0;
	enemybulletSys->nEnemybulletCount = 0;

	enemybulletSys->bUse = false;
	enemybulletSys->bProcess = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemybullet(void)
{
	if(g_pD3DTextureEnemybullet != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEnemybullet->Release();
		g_pD3DTextureEnemybullet = NULL;
	}

	if(g_pD3DVtxBuffEnemybullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEnemybullet->Release();
		g_pD3DVtxBuffEnemybullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemybullet(void)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// �g�p���̃V�X�e����T��
	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++, enemybulletSys++)
	{
		if (enemybulletSys->bProcess)
		{
			SetSysEnemybulletFire(i);
		}
		if (enemybulletSys->bUse)
		{
			if (enemybulletSys->nEnemybulletCount > 0)
			{
			}
			else
			{
				InitStatusEnemybulletSys(i);
			}
		}
	}

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{	// �g�p���̃I�u�W�F�N�g�̂ݏ���
		if (enemybullet->bUse)
		{
			if (enemybullet->nSysNum != 0)
			{
				enemybulletSys = &enemybulletSysWk[enemybullet->nSysNum - 1];
				enemybullet->fVAngle += enemybulletSys->fAddAngle;
				enemybullet->fVAngle = PiCalculate180(enemybullet->fVAngle);
				enemybullet->posEnemybullet.y += sinf(enemybullet->fVAngle) * enemybulletSys->fWave;
			}

			enemybullet->nAnimeCount++;
			if (enemybullet->nAnimeCount % TIME_ANIMATION_ENEMYBULLET == 0)
			{
				// �p�^�[���̐؂�ւ�
				enemybullet->nAnimePattern = (enemybullet->nAnimePattern + 1)
					% TEXTURE_PATTERN_ENEMYBULLET
					+ (TEXTURE_PATTERN_ENEMYBULLET* enemybullet->nTex);
				SetTexEnemybullet(i, enemybullet->nAnimePattern);
			}

			// �o���b�g�^�C�v�ɉ����ď���
			if (enemybullet->nUseCount % ENEMYBULLET_HOMING_UPDATE == 0)
			{
				if (enemybullet->bHoming && enemybullet->nType == ENEMYBULLET_HOMING)
				{	// �w�苗�����܂Ńz�[�~���O
					D3DXVECTOR3 posTemp = model->posModel - enemybullet->posEnemybullet;
					posTemp.y += MODEL_CENTER;
					D3DXVec3Normalize(&posTemp, &posTemp);
					enemybullet->vecTag += posTemp * enemybullet->fVecPower;
					D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
				}
			}

			// �ړ�����
			enemybullet->posEnemybullet += enemybullet->vecTag * enemybullet->fMoveSpeed;

			// �V���h�E�Ǘ�
			SetPositionShadow(enemybullet->nIdxShadow, D3DXVECTOR3(enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z));
			SetVertexShadow(enemybullet->nIdxShadow, enemybullet->fSizeShadow, enemybullet->fSizeShadow);
			SetColorShadow(enemybullet->nIdxShadow, enemybullet->colShadow);

			// �G�t�F�N�g�ݒu
			SetEffect(0,
				enemybullet->vec2Size,
				enemybullet->colorEnemybullet,
				enemybullet->posEnemybullet,
				ENEMYBULLET_SIZE_CHANGE,
				ENEMYBULLET_ALPHA_CHANGE);

			// �g�p�J�E���g�œ��߁�������
			if (enemybullet->nUseCount > ENEMYBULLET_FALSE_COUNT )
			{
				enemybullet->colorEnemybullet.a -= ENEMYBULLET_ALPHA_CHANGE;
				if (enemybullet->colorEnemybullet.a < 0.0f)
				{
					enemybullet->colorEnemybullet.a = 0.0f;
				}

				enemybullet->colShadow.a -= ENEMYBULLET_ALPHA_CHANGE;
				if (enemybullet->colShadow.a < 0.0f)
				{
					enemybullet->colShadow.a = 0.0f;
				}

				if (enemybullet->colorEnemybullet.a > 0.0f)
				{
					SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);	// ���ˌ��ύX�̕ύX����
				}
				else
				{
					enemybulletSys = &enemybulletSysWk[enemybullet->nSysNum];
					enemybulletSys->nEnemybulletCount--;
					// �V���h�E�̖�����
					ReleaseShadow(enemybullet->nIdxShadow);
					InitStatusEnemybullet(i);
				}
			}
			enemybullet->nUseCount++;
			// �f�o�b�O�p
//#ifdef _DEBUG
//			PrintDebugProc("EnemybulletNum[%d]  EnemybulletPos[X:%f Z:%f]  EnemybulletSize[X:%f]  EnemybulletColorR:[%f]\n", i, enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.z, enemybullet->vec2Size.x, enemybullet->colorEnemybullet.r);
//#endif
		}
	}
	//	sprintf(str, "EnemybulletPos[X:%4.2f Y:%4.2f Z:%4.2f]",
	//		enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemybullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, ENEMYBULLET_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (enemybullet->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldEnemybullet);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();
			// �r���{�[�h��
			g_mtxWorldEnemybullet._11 = mtxView._11;
			g_mtxWorldEnemybullet._12 = mtxView._21;
			g_mtxWorldEnemybullet._13 = mtxView._31;
			g_mtxWorldEnemybullet._21 = mtxView._12;
			g_mtxWorldEnemybullet._22 = mtxView._22;
			g_mtxWorldEnemybullet._23 = mtxView._32;
			g_mtxWorldEnemybullet._31 = mtxView._13;
			g_mtxWorldEnemybullet._32 = mtxView._23;
			g_mtxWorldEnemybullet._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, enemybullet->sclEnemybullet.x, enemybullet->sclEnemybullet.y, enemybullet->sclEnemybullet.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, enemybullet->rotEnemybullet.y, enemybullet->rotEnemybullet.x, enemybullet->rotEnemybullet.z);
			//D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &GetInvRotateMat(enemybullet->posEnemybullet));
			////D3DXMatrixMultiply(&enemybullet->mtxWorld, &enemybullet->mtxWorld, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, enemybullet->posEnemybullet.x, enemybullet->posEnemybullet.y, enemybullet->posEnemybullet.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemybullet, &g_mtxWorldEnemybullet, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemybullet);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEnemybullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureEnemybullet);

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
HRESULT MakeVertexEnemybullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * ENEMYBULLET_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffEnemybullet,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
		 return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < ENEMYBULLET_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-ENEMYBULLET_SIZE_X, -ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ENEMYBULLET_SIZE_X, -ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxEnemybullet(int nEnemybullet, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseEnemybullet(int nEnemybullet, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexEnemybullet(int nEnemybullet, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemybullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nEnemybullet * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMYBULLET;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMYBULLET;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemybullet->Unlock();
	}
}

//=============================================================================
// �o���b�g��Ώۂɐݒu
//=============================================================================
void SetEnemybullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			//enemybullet->colorEnemybullet = SetColorPallet(COLOR_PALLET_RED);
			enemybullet->colorEnemybullet = xColor;						// �J���[��ݒ� 
			enemybullet->vec2Size =
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);	// �T�C�Y��ݒ�
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;			// �ړ��X�s�[�h��ݒ�
			enemybullet->nUseCount = 0;									// ���ŗp�J�E���g
			enemybullet->bUse = true;
			enemybullet->vecTag = vecPos1 - vecPos2;
			enemybullet->nTex = ENEMYBULLET_TEX_1;

			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
			enemybullet->posEnemybullet = vecPos2;

			// Vtx�ݒ�
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);// ���_���W�̕ύX����
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// ���ˌ��ύX�̕ύX����
			SetTexEnemybullet(i, enemybullet->nTex);							// �e�N�X�`�����W�̐ݒ�

			// �e�̐���
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// �ʏ�G�l�~�[�o���b�g��Ώۂɐݒu
//=============================================================================
void SetEnemybullet00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->colorEnemybullet = SetColorPallet(3);						// �J���[
			enemybullet->vec2Size = 
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);				// �T�C�Y
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;						// �ړ��X�s�[�h
			enemybullet->nTex = 0;													// �e�N�X�`���[
			enemybullet->nUseCount = 0;												// ���ŗp�J�E���g


			enemybullet->bUse = true;
			enemybullet->bEnemy = true;

			enemybullet->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);
			enemybullet->posEnemybullet = vecPos2;

			// Vtx�ݒ�
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);	// ���_���W�̕ύX����
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// ���ˌ��ύX�̕ύX����
			SetTexEnemybullet(i, enemybullet->nTex);								// �e�N�X�`�����W�̐ݒ�

			// �e�̐���
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// �z�[�~���O�o���b�g��Ώۂɐݒ�
//=============================================================================
void SetHomingEnemybullet(D3DXVECTOR3 posSet, float vecPower, D3DXCOLOR xColor)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->vec2Size =
				D3DXVECTOR2(ENEMYBULLET_SIZE_X, ENEMYBULLET_SIZE_Y);		// �T�C�Y��ݒ�
			enemybullet->fMoveSpeed = ENEMYBULLET_SPEED_NORMAL;				// �ړ��X�s�[�h��ݒ�
			enemybullet->nUseCount = 0;										// ���ŗp�J�E���g
			enemybullet->bUse = true;										// �g�p�t���O
			enemybullet->nType = ENEMYBULLET_HOMING;
			enemybullet->colorEnemybullet = xColor;							// �J���[��ݒ�
			enemybullet->posEnemybullet = posSet;
			enemybullet->fVecPower = ENEMYBULLET_VEC_POWER;
			enemybullet->nTex = ENEMYBULLET_TEX_2;

			// �����_���Ńx�N�g���𐶐�
			for (int i = 0; i < 3; i++)
			{
				if (i == 1)
				{
					enemybullet->vecTag[i] = rand() % 100;	// �����_��
					enemybullet->vecTag[i] *= 0.01f;		// �����_��
				}
				else
				{
					enemybullet->vecTag[i] = rand() % 200;	// �����_��
					enemybullet->vecTag[i] *= 0.01f;		// �����_��
					enemybullet->vecTag[i] -= 1.0f;			// -1.0�`1.0�֕ύX
				}

			}

			enemybullet->vecTag = enemybullet->vecTag;
			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);

			// Vtx�ݒ�
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);	// ���_���W�̕ύX����
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// ���ˌ��ύX�̕ύX����
			SetTexEnemybullet(i, enemybullet->nTex);								// �e�N�X�`�����W�̐ݒ�

			// �e�̐���
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}
//=============================================================================
// �V�X�e���o���b�g��Ώۂɐݒ�
//=============================================================================
void SetSysEnemybullet(int nType, int nTex, int nSize, int nColor, int nSplit, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos)
{
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < ENEMYBULLET_SYS_MAX; i++, enemybulletSys++)
	{
		if (!enemybulletSys->bUse)
		{
			enemybulletSys->bUse = true;						// �L����
			enemybulletSys->bProcess = true;
			enemybulletSys->vecTagPos = vecPos;				// �Ώ�POS���L��
			enemybulletSys->colorEnemybullet = SetColorPallet(nColor);
			enemybulletSys->vec2Size = D3DXVECTOR2(nSize, nSize);				// �T�C�Y

			enemybulletSys->fMoveSpeed = fSpeed;	// �ړ��X�s�[�h
			enemybulletSys->fVAngle = 0.0f;
			enemybulletSys->fWave = fWave;
			enemybulletSys->nType = nType;
			enemybulletSys->nFireTime = 0;
			enemybulletSys->nCoolDown = 0;
			enemybulletSys->nTex = nTex;
			enemybulletSys->nSplitCount = nSplit;
			if (nType == 4)
			{
				enemybulletSys->fSplitAngle = D3DX_PI * 2 / nSplit;
				enemybulletSys->fAddAngle = fAngle;
				enemybulletSys->fHAngle = 0.0f;
			}
			else
			{
				switch (nType % 2)
				{
				case 0:
					enemybulletSys->fSplitAngle = D3DX_PI * 2 / nSplit;
					enemybulletSys->fAddAngle = fAngle;
					enemybulletSys->fHAngle = enemybulletSys->fSplitAngle / 2;
					break;
				case 1:
					enemybulletSys->fSplitAngle = (D3DX_PI * 2) - (D3DX_PI * 2 / nSplit);
					enemybulletSys->fAddAngle = -fAngle;
					enemybulletSys->fHAngle = enemybulletSys->fSplitAngle / 2;
					break;
				}
			}
			return;
		}
	}
}
//=============================================================================
// �V�X�e���o���b�g����
//=============================================================================
void SetSysEnemybulletFire(int nSys)
{
	ENEMYBULLET *enemybullet = &enemybulletWk[0];
	ENEMYBULLET_SYS *enemybulletSys = &enemybulletSysWk[nSys];

	// ���g�p�̒e��T��
	for (int i = 0; i < ENEMYBULLET_MAX; i++, enemybullet++)
	{
		if (!enemybullet->bUse)
		{
			enemybullet->colorEnemybullet = enemybulletSys->colorEnemybullet;			// �J���[
			enemybullet->vec2Size = enemybulletSys->vec2Size;					// �T�C�Y
			enemybullet->fMoveSpeed = enemybulletSys->fMoveSpeed;				// �ړ��X�s�[�h
			enemybullet->nTex = enemybulletSys->nTex;							// �e�N�X�`���[
			enemybullet->nUseCount = 0;						// ���ŗp�J�E���g
			enemybullet->bUse = true;									// �L����
			enemybullet->bEnemy = true;									// �G�l�~�[�t���O��L����
			enemybullet->nSysNum = nSys + 1;								// �V�X�e���t���O��L����

			// �A���O������x�N�g���v�Z
			enemybullet->vecTag = (enemybulletSys->vecTagPos + D3DXVECTOR3(cosf(enemybulletSys->fHAngle),
				sinf(enemybulletSys->fVAngle), sinf(enemybulletSys->fHAngle)) - enemybulletSys->vecTagPos);

			D3DXVec3Normalize(&enemybullet->vecTag, &enemybullet->vecTag);	// ���K��
			enemybullet->posEnemybullet = enemybulletSys->vecTagPos;				// ���ˈʒu�ݒ�
			enemybulletSys->fHAngle += enemybulletSys->fSplitAngle;			// ���e�̊p�x��ύX
			enemybullet->fVAngle = enemybulletSys->fHAngle;					// �E�F�[�u�p

			// Vtx�ݒ�
			SetVtxEnemybullet(i, enemybullet->vec2Size.x, enemybullet->vec2Size.y);// ���_���W�̕ύX����
			SetDiffuseEnemybullet(i, enemybullet->colorEnemybullet);				// ���ˌ��ύX�̕ύX����
			SetTexEnemybullet(i, enemybullet->nTex);							// �e�N�X�`�����W�̐ݒ�

			// �e�̐���
			enemybullet->nIdxShadow = CreateShadow(enemybullet->posEnemybullet, 25.0f, 25.0f);
			enemybullet->fSizeShadow = ((enemybullet->vec2Size.x + enemybullet->vec2Size.y) / 2) * 2;
			enemybullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

			enemybulletSys->nEnemybulletCount++;

			// 360�x���˂�����������V�X�e�����I��
			enemybulletSys->nSplitCount--;
			if (enemybulletSys->nType < 2)
			{
				if (enemybulletSys->nSplitCount <= 0)
				{
					enemybulletSys->bProcess = false;
					return;
				}
			}
			else if (enemybulletSys->nType >= 2)
			{
				if (enemybulletSys->nSplitCount <= 0)
				{
					enemybulletSys->bProcess = false;
				}
				return;
			}
		}
	}
}

//=============================================================================
// �擾�֐�
//=============================================================================
ENEMYBULLET *GetEnemybullet(int no)
{
	return(&enemybulletWk[no]);
}