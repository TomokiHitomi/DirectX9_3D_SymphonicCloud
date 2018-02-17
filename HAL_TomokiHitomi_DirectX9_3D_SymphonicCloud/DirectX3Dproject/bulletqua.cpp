
//=============================================================================
//
// �o���b�g���� [bulletqua.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "camera.h"
#include "bulletqua.h"
#include "input.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "calculate.h"
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

void InitStatusBulletQuaSys(int nBulletQuaSys);
HRESULT MakeVertexBulletQua(LPDIRECT3DDEVICE9 pDevice);
void SetVtxBulletQua(int nBulletQua, float fSizeX, float fSizeY);
void SetDiffuseBulletQua(int nBulletQua, D3DXCOLOR col);
void SetTexBulletQua(int nBulletQua, int nTex);
void SetSysBulletQuaFire(int nSys);
void UpdateSysBulletQua(int nSys);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBulletQua = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBulletQua = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldBulletQua;			// ���[���h�}�g���b�N�X

BULLETQUA					bulletquaWk[BULLETQUA_MAX];
BULLETQUA_SYS				bulletquaSysWk[BULLETQUA_SYS_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBulletQua(int nType)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BULLETQUA,
			&g_pD3DTextureBulletQua);
	}

	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		// �ʒu�E��]�E�X�P�[���̐ݒ�
		bulletqua->posBulletQua = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->rotBulletQua = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->sclBulletQua = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		bulletqua->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bulletqua->colorBulletQua = D3DXCOLOR(BULLETQUA_DIF_R, BULLETQUA_DIF_G, BULLETQUA_DIF_B, BULLETQUA_DIF_A);
		InitStatusBulletQua(i);
	}

	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		bulletquaSys->nCoolDown = 0;
		InitStatusBulletQuaSys(i);
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexBulletQua(pDevice);

	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusBulletQua(int nBulletQua)
{
	BULLETQUA *bulletqua = &bulletquaWk[nBulletQua];

	bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y);
	bulletqua->colorBulletQua = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletqua->nUseCount = 0;
	bulletqua->nSysNum = 0;
	bulletqua->nTex = 0;
	bulletqua->nEffectCount = 0;
	bulletqua->nAnimeCount = 0;

	bulletqua->fMoveSpeed = 0.0f;
	bulletqua->fHAngle = 0.0f;
	bulletqua->fVAngle = 0.0f;

	bulletqua->bUse = false;
	bulletqua->bEnemy = false;
	bulletqua->bVertex = false;
	bulletqua->bDraw = true;

	// �e������
	bulletqua->nIdxShadow = 0;
	bulletqua->fSizeShadow = 0.0f;
	bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// �ė��p����SYS
//=============================================================================
void InitStatusBulletQuaSys(int nBulletQuaSys)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nBulletQuaSys];
	bulletquaSys->vecTagPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletquaSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	bulletquaSys->colorBulletQua = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletquaSys->fHAngle = 0.0f;
	bulletquaSys->fVAngle = 0.0f;
	bulletquaSys->fMoveSpeed = 0.0f;
	bulletquaSys->fWave = 0.0f;
	bulletquaSys->fLength = 1.0f;

	bulletquaSys->nType = 0;
	bulletquaSys->nFireTime = 0;
	bulletquaSys->nSplitCount = 0;
	bulletquaSys->nTex = 0;
	bulletquaSys->nBulletCount = 0;

	bulletquaSys->bUse = false;
	bulletquaSys->bProcess = false;

	// �N�H�[�^�j�I���p�ϐ�
	// ��]���a��ݒ�
	bulletquaSys->vecTa = D3DXVECTOR3(0.0f, bulletquaSys->fLength, 0.0f);
	// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	bulletquaSys->vecAxis = D3DXVECTOR3(cosf(bulletquaSys->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(bulletquaSys->fHAngle + D3DX_PI * 0.5f));
}

//=============================================================================
// �I������
//=============================================================================
void UninitBulletQua(void)
{
	if(g_pD3DTextureBulletQua != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBulletQua->Release();
		g_pD3DTextureBulletQua = NULL;
	}

	if(g_pD3DVtxBuffBulletQua != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBulletQua->Release();
		g_pD3DVtxBuffBulletQua = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBulletQua(void)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// �g�p���̃V�X�e����T��
	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		if (bulletquaSys->bProcess)
		{
			SetSysBulletQuaFire(i);
		}
		if (bulletquaSys->bUse)
		{
			if (bulletquaSys->nBulletCount > 0)
			{
				UpdateSysBulletQua(i);
			}
			else
			{
				InitStatusBulletQuaSys(i);
			}

#ifdef _DEBUG
			//PrintDebugProc("Sys[%d]  Tex[%d]  Type[%d]  Length[%f]  Angle[V:%f H:%f Add:%f]\n", i, bulletquaSys->nTex, bulletquaSys->nType, bulletquaSys->fLength, bulletquaSys->fVAngle, bulletquaSys->fHAngle, bulletquaSys->fAddAngle);
			//			PrintDebugProc("BulletQuaNum[%d]  BulletQuaPos[X:%f Z:%f]  BulletQuaSize[X:%f]  BulletQuaColorR:[%f]\n", i, bulletqua->posBulletQua.x, bulletqua->posBulletQua.z, bulletqua->vec2Size.x, bulletqua->colorBulletQua.r);
#endif

		}
		//if (bulletquaSys->nCoolDown > 0)
		//{
		//	bulletquaSys->nCoolDown--;
		//	if (bulletquaSys->nCoolDown <= 0)
		//	{
		//		InitStatusBulletQuaSys(i);
		//	}
		//}
	}

	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{	// �g�p���̃I�u�W�F�N�g�̂ݏ���
		if (bulletqua->bUse)
		{
			bulletqua->nAnimeCount++;
			if (bulletqua->nAnimeCount % TIME_ANIMATION_TEX == 0)
			{
				// �p�^�[���̐؂�ւ�
				bulletqua->nTex = (bulletqua->nTex + 1) % TEXTURE_PATTERN_NU_BULLETQUA;
				SetTexBulletQua(i, bulletqua->nTex);
			}
			if (!bulletqua->bVertex)
			{
				SetVtxBulletQua(i, bulletqua->vec2Size.x, bulletqua->vec2Size.y);		// ���_���W�̕ύX����
				SetDiffuseBulletQua(i, bulletqua->colorBulletQua);	// ���ˌ��ύX�̕ύX����
				SetTexBulletQua(i, bulletqua->nTex);		// �e�N�X�`�����W�̐ݒ�
				bulletqua->bVertex = true;
			}


			bulletquaSys = &bulletquaSysWk[bulletqua->nSysNum];

			bulletquaSys->fVAngle += bulletquaSys->fSplitAngle;
			PiCalculate180(bulletquaSys->fVAngle);

			if(bulletquaSys->nType <= 3)
			{ 
				bulletquaSys->vecTa = D3DXVECTOR3(0.0f, 0.0f, bulletquaSys->fLength);
				bulletquaSys->vecAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				bulletquaSys->vecTa = D3DXVECTOR3(0.0f, bulletquaSys->fLength, 0.0f);
				bulletquaSys->vecAxis = D3DXVECTOR3(cosf(bulletquaSys->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(bulletquaSys->fHAngle + D3DX_PI * 0.5f));
			}

			// �N�H�[�^�j�I������
			QuaternionCalculate(&bulletquaSys->vecTa, &bulletquaSys->vecAxis, bulletquaSys->fVAngle,&bulletqua->posBulletQua);
			// ��]���ɐݒu
			bulletqua->posBulletQua += bulletquaSys->vecTagPos;

			// �V���h�E�Ǘ�
			SetPositionShadow(bulletqua->nIdxShadow, D3DXVECTOR3(bulletqua->posBulletQua.x, 0.2f, bulletqua->posBulletQua.z));
			SetVertexShadow(bulletqua->nIdxShadow, bulletqua->fSizeShadow, bulletqua->fSizeShadow);
			SetColorShadow(bulletqua->nIdxShadow, bulletqua->colShadow);

			// �G�t�F�N�g�ݒu
			//SetEffect(bulletqua->nTex, bulletqua->vec2Size, bulletqua->colorBulletQua, bulletqua->posBulletQua, bulletqua->vec2Size.x / 60, bulletqua->vec2Size.x / 80);

			// �g�p�J�E���g�œ��߁�������
			if (bulletqua->nUseCount > BULLETQUA_FALSE_COUNT)
			{
				bulletqua->colorBulletQua.a -= BULLETQUA_ALPHA_CHANGE;
				bulletqua->colShadow.a -= BULLETQUA_ALPHA_CHANGE;
				if (bulletqua->colorBulletQua.a >= 0.0f)
				{
					SetDiffuseBulletQua(i, bulletqua->colorBulletQua);	// ���ˌ��ύX�̕ύX����
				}
				else
				{
					bulletquaSys->nBulletCount--;
					// �V���h�E�̖�����
					ReleaseShadow(bulletqua->nIdxShadow);
					InitStatusBulletQua(i);
				}
			}
		// �g�p�J�E���g����
		bulletqua->nUseCount++;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBulletQua(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxView;	// �X�P�[��, ��], ���s�ړ�

	BULLETQUA *bulletqua = &bulletquaWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, BULLETQUA_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (bulletqua->bUse && bulletqua->bDraw)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBulletQua);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// �r���{�[�h��
			g_mtxWorldBulletQua._11 = mtxView._11;
			g_mtxWorldBulletQua._12 = mtxView._21;
			g_mtxWorldBulletQua._13 = mtxView._31;
			g_mtxWorldBulletQua._21 = mtxView._12;
			g_mtxWorldBulletQua._22 = mtxView._22;
			g_mtxWorldBulletQua._23 = mtxView._32;
			g_mtxWorldBulletQua._31 = mtxView._13;
			g_mtxWorldBulletQua._32 = mtxView._23;
			g_mtxWorldBulletQua._33 = mtxView._33;


			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, bulletqua->sclBulletQua.x, bulletqua->sclBulletQua.y, bulletqua->sclBulletQua.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletQua, &g_mtxWorldBulletQua, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bulletqua->rotBulletQua.y, bulletqua->rotBulletQua.x, bulletqua->rotBulletQua.z);
			//D3DXMatrixMultiply(&bulletqua->mtxWorld, &bulletqua->mtxWorld, &GetInvRotateMat(bulletqua->posBulletQua));
			////D3DXMatrixMultiply(&bulletqua->mtxWorld, &bulletqua->mtxWorld, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, bulletqua->posBulletQua.x, bulletqua->posBulletQua.y, bulletqua->posBulletQua.z);
			D3DXMatrixMultiply(&g_mtxWorldBulletQua, &g_mtxWorldBulletQua, &mtxTranslate);

			// ���[���h�}�g���N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBulletQua);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBulletQua, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureBulletQua);

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
HRESULT MakeVertexBulletQua(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLETQUA_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBulletQua,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLETQUA_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLETQUA_SIZE_X, -BULLETQUA_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLETQUA_SIZE_X, -BULLETQUA_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletQua->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxBulletQua(int nBulletQua, float fSizeX, float fSizeY)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseBulletQua(int nBulletQua, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexBulletQua(int nBulletQua, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBulletQua->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBulletQua * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLETQUA;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLETQUA;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBulletQua->Unlock();
	}
}

//=============================================================================
// �o���b�g��Ώۂɐݒu
//=============================================================================
void SetBulletQua(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];

	// ���g�p�̒e��T��
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			bulletqua->colorBulletQua = SetColorPallet(5);							// �J���[��ݒ�
			bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_SIZE_X, BULLETQUA_SIZE_Y);	// �T�C�Y��ݒ�
			bulletqua->fMoveSpeed = BULLETQUA_SPEED;								// �ړ��X�s�[�h��ݒ�
			bulletqua->nUseCount = 0;

			bulletqua->bUse = true;
			bulletqua->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bulletqua->vecTag, &bulletqua->vecTag);
			bulletqua->posBulletQua = vecPos2;

			// �e�̐���
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// �ʏ�G�l�~�[�o���b�g��Ώۂɐݒu
//=============================================================================
void SetEnemyBulletQua00(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			// �J���[
			bulletqua->colorBulletQua = SetColorPallet(3);
			// �T�C�Y
			bulletqua->vec2Size = D3DXVECTOR2(BULLETQUA_ENEMY_SIZE_X, BULLETQUA_ENEMY_SIZE_Y);
			bulletqua->fMoveSpeed = BULLETQUA_ENEMY_SPEED;			// �ړ��X�s�[�h
			bulletqua->nTex = 0;									// �e�N�X�`���[
			bulletqua->nUseCount = 0;
			bulletqua->bUse = true;
			bulletqua->bEnemy = true;

			bulletqua->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bulletqua->vecTag, &bulletqua->vecTag);
			bulletqua->posBulletQua = vecPos2;

			// �e�̐���
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}

//=============================================================================
// �V�X�e���o���b�g��Ώۂɐݒ�
//=============================================================================
void SetSysBulletQua(int nType, int nTex, int nSize, int nColor, int nSplit, float fSpeed, float fWave, float fAngle, D3DXVECTOR3 vecPos)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLETQUA_SYS_MAX; i++, bulletquaSys++)
	{
		if (!bulletquaSys->bUse)
		{
			bulletquaSys->bUse = true;								// �L����
			bulletquaSys->bProcess = true;							// �v���Z�X�L����
			bulletquaSys->vecTagPos = vecPos;						// �Ώ�POS���L��
			bulletquaSys->colorBulletQua = SetColorPallet(nColor);
			bulletquaSys->vec2Size = D3DXVECTOR2(nSize, nSize);		// �T�C�Y

			bulletquaSys->fMoveSpeed = fSpeed;						// �ړ��X�s�[�h
			bulletquaSys->fVAngle = 0.0f;
			bulletquaSys->fWave = fWave;
			bulletquaSys->nType = nType;
			bulletquaSys->nFireTime = 0;
			bulletquaSys->nCoolDown = 0;
			bulletquaSys->nTex = nTex;
			bulletquaSys->nSplitCount = nSplit;

			bulletquaSys->fSplitAngle = D3DX_PI * 2 / nSplit;
			bulletquaSys->fAddAngle = fAngle;
			bulletquaSys->fHAngle = 0.0f;

			return;
		}
	}
}

//=============================================================================
// �V�X�e���o���b�g����������
//=============================================================================
void SetSysBulletQuaFire(int nSys)
{
	BULLETQUA *bulletqua = &bulletquaWk[0];
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nSys];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < BULLETQUA_MAX; i++, bulletqua++)
	{
		if (!bulletqua->bUse)
		{
			bulletqua->colorBulletQua = bulletquaSys->colorBulletQua;		// �J���[
			bulletqua->vec2Size = bulletquaSys->vec2Size;					// �T�C�Y
			bulletqua->fMoveSpeed = bulletquaSys->fMoveSpeed;				// �ړ��X�s�[�h
			bulletqua->nTex = bulletquaSys->nTex;							// �e�N�X�`���[
			bulletqua->nUseCount = 0;
			bulletqua->bUse = true;											// �L����
			bulletqua->bEnemy = true;										// �G�l�~�[�t���O��L����
			bulletqua->nSysNum = nSys;									// �V�X�e���t���O��L����

			SetVtxBulletQua(i, bulletqua->vec2Size.x, bulletqua->vec2Size.y);

			// �e�̐���
			bulletqua->nIdxShadow = CreateShadow(bulletqua->posBulletQua, 25.0f, 25.0f);
			bulletqua->fSizeShadow = bulletqua->vec2Size.x + bulletqua->vec2Size.y;
			bulletqua->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

			bulletquaSys->nBulletCount++;

			// 360�x���˂�����������v���Z�X���I��
			bulletquaSys->nSplitCount--;
			if (bulletquaSys->nSplitCount <= 0)
			{
				bulletquaSys->bProcess = false;
				bulletquaSys->nCoolDown = *nTotalCount;
				return;
			}
		}
	}
}
//=============================================================================
// �V�X�e���o���b�g�X�V����
//=============================================================================
void UpdateSysBulletQua(int nSys)
{
	BULLETQUA_SYS *bulletquaSys = &bulletquaSysWk[nSys];
	ENEMY *enemy = GetEnemy(0);

	switch (bulletquaSys->nType)
	{
	case BULLET_QUA_ROT_L:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_R:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_LL:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_RR:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_LV:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle += (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle += (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	case BULLET_QUA_ROT_RV:
		bulletquaSys->fLength += (float)bulletquaSys->fMoveSpeed;
		bulletquaSys->fHAngle -= (float)bulletquaSys->fAddAngle;
		bulletquaSys->fVAngle -= (float)bulletquaSys->fAddAngle;
		PiCalculate180(bulletquaSys->fHAngle);
		PiCalculate180(bulletquaSys->fVAngle);
		break;
	}
}

//=============================================================================
// �擾�֐�
//=============================================================================
BULLETQUA *GetBulletQua(int no)
{
	return(&bulletquaWk[no]);
}