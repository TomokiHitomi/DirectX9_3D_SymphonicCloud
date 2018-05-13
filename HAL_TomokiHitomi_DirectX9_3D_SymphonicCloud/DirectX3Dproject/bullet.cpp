//=============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "bullet.h"
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "effect.h"
#include "calculate.h"
#include "lockon.h"


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

void InitStatusBulletSys(int nBulletSys);
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVtxBullet(int nBullet);
void SetDiffuseBullet(int nBullet, D3DXCOLOR col);
void SetTexBullet(int nBullet, int nTex);
void SetSpecialBullet(int nSys);
void UpdateSpecialBullet(int nBullet);

void SetSpecialBulletFireSys(int nSys);
void SetSpecialBulletThunderSys(int nSys);
void SetSpecialBulletIceSys(int nSys);
void SetSpecialBulletWindSys(int nSys);
void SetSpecialBulletWaterSys(int nSys);
void SetSpecialBulletEarthSys(int nSys);

void UpdateSpecialBulletFire(int nBullet);
void UpdateSpecialBulletThunder(int nBullet);
void UpdateSpecialBulletIce(int nBullet);
void UpdateSpecialBulletWind(int nBullet);
void UpdateSpecialBulletWater(int nBullet);
void UpdateSpecialBulletEarth(int nBullet);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldBullet;			// ���[���h�}�g���b�N�X

BULLET					bulletWk[BULLET_MAX];
BULLET_SYS				bulletSysWk[BULLET_SYS_MAX];

float					g_fBaseAngleBullet;			// �p�x�i�Ɓj
float					g_fRadiusBullet;			// ���a

// �V�F�[�_�[�֘A
ID3DXBuffer *errorBullet = 0;
ID3DXEffect *bulletshader;
UINT numPassBullet;
HRESULT hrBullet;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(int nType)
{
	BULLET *bullet = &bulletWk[0];
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* path = SHADER_BULLET;
	// �t�@�C��( const char* path )����V�F�[�_�[�t�@�C����ǂݍ��ݓǂݍ���
	hrBullet = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &bulletshader, &errorBullet);
	if (FAILED(hrBullet))
	{
		// �G���[
		return S_FALSE;
	}
	// �g�p����e�N�j�b�N���`
	hrBullet = bulletshader->SetTechnique("Tec01");
	if (FAILED(hrBullet)) {

		// �G���[
		return S_FALSE;
	}


	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_BULLET,
			&g_pD3DTextureBullet);
	}

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{	// �o���b�g�̏�����
		InitStatusBullet(i);
	}

	for (int i = 0; i < BULLET_SYS_MAX; i++,bulletSys)
	{	// �o���b�g�V�X�e���̏�����
		bulletSys->nCoolDown = 0;
		InitStatusBulletSys(i);
	}

	// ���_���̍쐬�i���[�J�����W�̐ݒ�j
	MakeVertexBullet(pDevice);

	return S_OK;
}

//=============================================================================
// �ė��p����
//=============================================================================
void InitStatusBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	// �ʒu�E��]�E�X�P�[���̐ݒ�
	bullet->posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->rotBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->sclBullet = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	bullet->vecTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bullet->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


	bullet->vec2Size = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);
	bullet->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bullet->nUseCount = 0;
	bullet->nSysNum = 0;
	bullet->nTex = 0;
	bullet->nEffectCount = 0;
	bullet->nAnimeCount = 0;
	bullet->nAnimePattern = 0;
	bullet->nType = 0;
	bullet->nNum = 0;
	bullet->nEffect = 0;

	bullet->fSpeed = 0.0f;
	bullet->fHAngle = 0.0f;
	bullet->fVAngle = 0.0f;
	bullet->fAngle = 0.0f;
	bullet->fBaseAngle = 0.0f;
	bullet->fRadius = 0.0f;
	bullet->fEffectSizeChange = 0.0f;
	bullet->fEffectAlphaChange = 0.0f;

	bullet->bUse = false;
	bullet->bEnemy = false;
	bullet->bVertex = false;
	bullet->bSys = false;
	bullet->bEffect = true;

	bullet->nIdxShadow = 0;
	bullet->fSizeShadow = 0.0f;
	bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
}

//=============================================================================
// �ė��p����SYS
//=============================================================================
void InitStatusBulletSys(int nBulletSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nBulletSys];
	bulletSys->posBulletSys = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->posBulletSysTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->vecBulletSys = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->posTag = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bulletSys->vec2Size = D3DXVECTOR2(0.0f, 0.0f);
	bulletSys->xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	bulletSys->fHAngle = 0.0f;
	bulletSys->fVAngle = 0.0f;
	bulletSys->fAngle = 0.0f;
	bulletSys->fSpeed = 0.0f;
	bulletSys->fRotSpeed = 0.0f;
	bulletSys->fRotLength = 0.0f;
	bulletSys->fWave = 0.0f;
	bulletSys->fLength = 0.0f;

	bulletSys->nType = 0;
	bulletSys->nChain = 0;
	bulletSys->nFireTime = 0;
	bulletSys->nSplit = 0;
	bulletSys->nTex = 0;
	bulletSys->nBulletCount = 0;
	bulletSys->nUseCount = 0;
	bulletSys->nLockon = 0;

	bulletSys->bUse = false;
	bulletSys->bSet = false;
	bulletSys->bProcess = false;
	bulletSys->bTrack = false;
	bulletSys->bMove = true;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{

	// �V�F�[�_�[�����
	bulletshader->Release();

	if(g_pD3DTextureBullet != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet = NULL;
	}

	if(g_pD3DVtxBuffBullet != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBullet->Release();
		g_pD3DVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	BULLET *bullet = &bulletWk[0];
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	MODEL *model = GetModel(0);
	int *nTotalCount = GetTotalCount();

	// �g�p���̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (bulletSys->bUse)
		{
			SetSpecialBullet(i);
		}
	}

#ifdef _DEBUG
	PrintDebugProc("�y BULLET �z\n");
	int nBulletCountDebug = 0;
#endif

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{	// �g�p���̃I�u�W�F�N�g�̂ݏ���
		if (bullet->bUse)
		{
#ifdef _DEBUG
			nBulletCountDebug++;
#endif
			if (bullet->nType == BULLET_NORMAL)
			{	// �ʏ�o���b�g����
				bullet->posBullet += bullet->vecTag * bullet->fSpeed;
				bullet->rotBullet.z += BULLET_ROT_SPEED;			// �r���{�[�h��]

			}
			else
			{	// ����o���b�g����
				UpdateSpecialBullet(i);
				bullet->rotBullet.z -= BULLET_ROT_SPEED;			// �r���{�[�h��]
			}

			// ���_���W�̕ύX����
			SetVtxBullet(i);

			// �V���h�E�Ǘ�
			SetPositionShadow(bullet->nIdxShadow, D3DXVECTOR3(bullet->posBullet.x, bullet->posBullet.y, bullet->posBullet.z));
			SetVertexShadow(bullet->nIdxShadow, bullet->fSizeShadow, bullet->fSizeShadow);
			SetColorShadow(bullet->nIdxShadow, bullet->colShadow);

			if (bullet->bEffect)
			{
				// �G�t�F�N�g�ݒu
				SetEffect(bullet->nTex,
					bullet->vec2Size * bullet->sclBullet.x,
					bullet->xColor,
					bullet->posBullet,
					bullet->fEffectSizeChange,
					bullet->fEffectAlphaChange);

			}

			// �g�p�J�E���g�œ��߁�������
			if (bullet->nUseCount > BULLET_FALSE_COUNT)
			{

				bullet->xColor.a -= BULLET_ALPHA_CHANGE;
				if (bullet->xColor.a < 0.0f)
				{
					bullet->xColor.a = 0.0f;
				}

				bullet->colShadow.a -= BULLET_ALPHA_CHANGE;
				if (bullet->colShadow.a < 0.0f)
				{
					bullet->colShadow.a = 0.0f;
				}

				if (bullet->xColor.a > 0.0f)
				{
					SetDiffuseBullet(i, bullet->xColor);	// ���ˌ��ύX�̕ύX����
				}
				else
				{
					if (bullet->nSysNum != BULLET_NORMAL)
					{
						bulletSys = &bulletSysWk[bullet->nSysNum];
						bulletSys->nBulletCount--;
					}
					// �V���h�E�̖�����
					ReleaseShadow(bullet->nIdxShadow);
					InitStatusBullet(i);
				}
			}
			bullet->nUseCount++;
		}
	}
#ifdef _DEBUG
	PrintDebugProc("bulletCount[%d]\n",nBulletCountDebug);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView,mtxScl, mtxRot, mtxTranslate;	// �X�P�[��, ��], ���s�ړ�

	BULLET *bullet = &bulletWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	D3DXMATRIX mtxProjection = GetMtxProjection();

	//// ���Z�����ɐݒ�
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, BULLET_ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (bullet->bUse)
		{
			/******************** ���[���h�ϊ� ********************/
			// ���[���h�}�g���N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBullet);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// �r���{�[�h��
			g_mtxWorldBullet._11 = mtxView._11;
			g_mtxWorldBullet._12 = mtxView._21;
			g_mtxWorldBullet._13 = mtxView._31;
			g_mtxWorldBullet._21 = mtxView._12;
			g_mtxWorldBullet._22 = mtxView._22;
			g_mtxWorldBullet._23 = mtxView._32;
			g_mtxWorldBullet._31 = mtxView._13;
			g_mtxWorldBullet._32 = mtxView._23;
			g_mtxWorldBullet._33 = mtxView._33;

			// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
			D3DXMatrixScaling(&mtxScl, bullet->sclBullet.x, bullet->sclBullet.y, bullet->sclBullet.z);
			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxScl);

			//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bullet->rotBullet.y, bullet->rotBullet.x, bullet->rotBullet.z);
			//D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &GetInvRotateMat(bullet->posBullet));
			////D3DXMatrixMultiply(&bullet->mtxWorld, &bullet->mtxWorld, &mtxRot);

			// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
			D3DXMatrixTranslation(&mtxTranslate, bullet->posBullet.x, bullet->posBullet.y, bullet->posBullet.z);
			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTranslate);

			//// ���[���h�}�g���N�X�̐ݒ�
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);


			/******************** �r���[�|�[�g�ϊ� ********************/
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �K�v�ȃO���[�o������^����
			bulletshader->SetMatrix("proj", &mtxProjection);
			bulletshader->SetMatrix("view", &mtxView);
			bulletshader->SetMatrix("world", &g_mtxWorldBullet);

			//// �e�N�X�`���̐ݒ�
			//pDevice->SetTexture(0, g_pD3DTextureBullet);

			// �V�F�[�_�[�̊J�n�AnumPass�ɂ͎w�肵�Ă���e�N�j�b�N�ɒ�`���Ă���pass�̐����ς���
			bulletshader->Begin(&numPassBullet, 0);

			// �p�X���w�肵�ĊJ�n
			bulletshader->BeginPass(0);

			// �e�N�X�`�����Z�b�g
			bulletshader->SetTexture("tex", g_pD3DTextureBullet);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);

			// �V�F�[�_�I��
			bulletshader->EndPass();
			bulletshader->End();
		}
	}

	//// �ʏ�u�����h�ɖ߂�
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// �p�x�Ɣ��a�����߂�
	g_fBaseAngleBullet = atan2f(BULLET_SIZE_Y, BULLET_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);
	g_fRadiusBullet = D3DXVec2Length(&temp);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffBullet,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
		 return E_FAIL;
	}

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < BULLET_MAX; i++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULLET_SIZE_X, -BULLET_SIZE_Y, 0.0f);

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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_BULLET;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_BULLET;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBullet->Unlock();
	}
	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
//=============================================================================
void SetVtxBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// ���_0:��]����
		pVtx[0].vtx.x = 0.0f - cosf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[0].vtx.y = 0.0f - sinf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[0].vtx.z = 0.0f;

		// ���_1:��]����
		pVtx[1].vtx.x = 0.0f - cosf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[1].vtx.y = 0.0f + sinf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[1].vtx.z = 0.0f;

		// ���_2:��]����
		pVtx[2].vtx.x = 0.0f + cosf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[2].vtx.y = 0.0f - sinf(g_fBaseAngleBullet - bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[2].vtx.z = 0.0f;

		// ���_3:��]����
		pVtx[3].vtx.x = 0.0f + cosf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[3].vtx.y = 0.0f + sinf(g_fBaseAngleBullet + bullet->rotBullet.z) * g_fRadiusBullet;
		pVtx[3].vtx.z = 0.0f;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseBullet(int nBullet, D3DXCOLOR col)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�֐�
//=============================================================================
void SetTexBullet(int nBullet, int nTex)
{
	{// ���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nBullet * 4);

		// �e�N�X�`�����W�̐ݒ�
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_BULLET;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_BULLET;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_BULLET;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_BULLET;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// �o���b�g��Ώۂɐݒu
//=============================================================================
void SetBullet(D3DXVECTOR3 vecPos1, D3DXVECTOR3 vecPos2, D3DXCOLOR xColor, float fScl, int nEffect)
{
	BULLET *bullet = &bulletWk[0];
	int *nTotalCount = GetTotalCount();

	// ���g�p�̒e��T��
	for (int i = 0; i < BULLET_MAX; i++, bullet++)
	{
		if (!bullet->bUse)
		{
			bullet->xColor = xColor;										// �J���[��ݒ�
			bullet->vec2Size = D3DXVECTOR2(BULLET_SIZE_X, BULLET_SIZE_Y);	// �T�C�Y��ݒ�
			bullet->fSpeed = BULLET_SPEED;									// �ړ��X�s�[�h��ݒ�

			// ���ŗp�J�E���g
			bullet->nUseCount = 0;
			bullet->nEffect = nEffect;
			bullet->fEffectSizeChange = BULLET_NORMAL_EFFECT_SIZE_CHANGE;
			bullet->fEffectAlphaChange = BULLET_NORMAL_EFFECT_ALPHA_CHANGE;
			bullet->bUse = true;
			bullet->vecTag = vecPos1 - vecPos2;
			D3DXVec3Normalize(&bullet->vecTag, &bullet->vecTag);
			bullet->posBullet = vecPos2;
			bullet->sclBullet *= fScl;
			// Vtx�ݒ�
			SetVtxBullet(i);// ���_���W�̕ύX����
			SetDiffuseBullet(i, bullet->xColor);				// ���ˌ��ύX�̕ύX����
			SetTexBullet(i, bullet->nTex);							// �e�N�X�`�����W�̐ݒ�

			// �e�̐���
			bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
			bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
			bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);
			return;
		}
	}
}


//=============================================================================
// ����o���b�g�U�蕪������
//=============================================================================
void SetSpecialBullet(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	
	switch (bulletSys->nType)
	{
	case BULLET_SPECIAL_FIRE:
		SetSpecialBulletFireSys(nSys);
		break;
	case BULLET_SPECIAL_THUNDER:
		SetSpecialBulletThunderSys(nSys);
		break;
	case BULLET_SPECIAL_ICE:
		SetSpecialBulletIceSys(nSys);
		break;
	case BULLET_SPECIAL_WIND:
		SetSpecialBulletWindSys(nSys);
		break;
	case BULLET_SPECIAL_WATER:
		SetSpecialBulletWaterSys(nSys);
		break;
	case BULLET_SPECIAL_EARTH:
		SetSpecialBulletEarthSys(nSys);
		break;
	}
}

//=============================================================================
// ����o���b�g�X�V����
//=============================================================================
void UpdateSpecialBullet(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];

	switch (bullet->nType)
	{
	case BULLET_SPECIAL_FIRE:
		UpdateSpecialBulletFire(nBullet);
		break;
	case BULLET_SPECIAL_THUNDER:
		UpdateSpecialBulletThunder(nBullet);
		break;
	case BULLET_SPECIAL_ICE:
		UpdateSpecialBulletIce(nBullet);
		break;
	case BULLET_SPECIAL_WIND:
		UpdateSpecialBulletWind(nBullet);
		break;
	case BULLET_SPECIAL_WATER:
		UpdateSpecialBulletWater(nBullet);
		break;
	case BULLET_SPECIAL_EARTH:
		UpdateSpecialBulletEarth(nBullet);
		break;
	}
}

//=============================================================================
// �t�@�C�A�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletFire(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// �L����
			bulletSys->bTrack = true;								// �ǔ��t���O
			bulletSys->posBulletSys = pos1;							// �Z�b�gPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// �ړ��x�N�g��
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_FIRE;					// �^�C�v
			bulletSys->nTex = BULLET_NORMAL;						// �e�N�X�`���[
			bulletSys->nLockon = nLockon;							// ���b�N�I���t���O
			bulletSys->vec2Size = D3DXVECTOR2(						// �T�C�Y
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_FIRE_SPEED;			// �X�s�[�h
			bulletSys->fRotSpeed = BULLET_SPECIAL_FIRE_ROT_SPEED;	// ��]�X�s�[�h
			bulletSys->fRotLength = 0.0f;							// ��]�W�J����
			bulletSys->nSplit = BULLET_SPECIAL_FIRE_SPLIT + BULLET_SPECIAL_FIRE_SPLIT * nCharge;// �����p�x
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_FIRE_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_ORANGE);	// �J���[
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// ��]����ݒ�
			bulletSys->vecTa = vecTa;								// ��]���ɐ����ȃx�N�g��
			return;
		}
	}
}

//=============================================================================
// �t�@�C�A�o���b�g����
//=============================================================================
void SetSpecialBulletFireSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];
	MODEL *model = GetModel(0);
	CAMERA *camera = GetCamera(CAMERA_GAME);

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}

		bulletSys->fRotLength += BULLET_SPECIAL_FIRE_ROT_LENGTH;
		if (bulletSys->fRotLength > BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX;
		}

		D3DXVECTOR3 vecTag = camera->posCameraAt - camera->posCameraEye;
		D3DXVec3Normalize(&vecTag, &vecTag);
		bulletSys->posBulletSysTag = camera->posCameraAt + vecTag * BULLET_TAG_VECTOR;

		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->posBulletSys.x = model->posModel.x;
		bulletSys->posBulletSys.y = model->posModel.y + MODEL_CENTER;
		bulletSys->posBulletSys.z = model->posModel.z;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->bEffect = false;										// �G�t�F�N�g�Ȃ�
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->nNum = i;
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_FIRE_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_FIRE_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_FIRE_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �t�@�C�A�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletFire(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];
	MODEL *model = GetModel(0);

	bulletSys->vecAxis = D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI), 0.0f, sinf(model->fHAngle + D3DX_PI));
	bulletSys->vecTa = D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f));

	if (bulletSys->fRotLength >= BULLET_SPECIAL_FIRE_ROT_LENGTH_MAX)
	{
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_FIRE_RAPID == 0)
		//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
		//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			SetBullet(
				bulletSys->posBulletSysTag,
				bullet->posBullet,
				SetColorPallet(COLOR_PALLET_ORANGE),
				BULLET_SPECIAL_FIRE_CHILD__SCALE,
				BULLET_SPECIAL_FIRE);
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// �T���_�[�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletThunder(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// �L����
			bulletSys->bTrack = true;								// �ǔ��t���O
			bulletSys->posBulletSys = pos1;							// �Z�b�gPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// �ړ��x�N�g��
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_THUNDER;				// �^�C�v
			bulletSys->nTex = BULLET_NORMAL;						// �e�N�X�`���[
			bulletSys->nLockon = nLockon;							// ���b�N�I���t���O
			bulletSys->vec2Size = D3DXVECTOR2(						// �T�C�Y
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_THUNDER_SPEED;		// �X�s�[�h
			bulletSys->fRotSpeed = BULLET_SPECIAL_THUNDER_ROT_SPEED;// ��]�X�s�[�h
			bulletSys->fRotLength = 0.0f;							// ��]�W�J����
			bulletSys->nSplit = BULLET_SPECIAL_THUNDER_SPLIT + BULLET_SPECIAL_THUNDER_SPLIT * nCharge;// �����p�x
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_THUNDER_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_YELLOW);// �J���[
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// ��]����ݒ�
			bulletSys->vecTa = vecTa;								// ��]���ɐ����ȃx�N�g��
			return;
		}
	}
}

//=============================================================================
// �T���_�[�o���b�g����
//=============================================================================
void SetSpecialBulletThunderSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}

		// ��]���a�����Z
		bulletSys->fRotLength += BULLET_SPECIAL_THUNDER_ROT_LENGTH;
		// �ő唼�a���z������
		if (bulletSys->fRotLength > BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{
				if (bulletSys->bMove)
				{
					D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
					vecTemp.y += BULLET_SPECIAL_THUNDER_HEIGHT;

					D3DXVec3Normalize(&vecTemp, &vecTemp);
					bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
					bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
					if ((bulletSys->posBulletSys.x - lockon->posLockon.x < BULLET_SPECIAL_THUNDER_STOP
						&& bulletSys->posBulletSys.x - lockon->posLockon.x > -BULLET_SPECIAL_THUNDER_STOP)
						&& (bulletSys->posBulletSys.z - lockon->posLockon.z < BULLET_SPECIAL_THUNDER_STOP
							&& bulletSys->posBulletSys.z - lockon->posLockon.z > -BULLET_SPECIAL_THUNDER_STOP))
					{
						bulletSys->bMove = false;
					}
				}
				else
				{
					bulletSys->posBulletSys = lockon->posLockon;
					bulletSys->posBulletSys.y += BULLET_SPECIAL_THUNDER_HEIGHT;
				}
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		if(bulletSys->nLockon < 0 && bulletSys->bMove)
		{
			MODEL *model = GetModel(0);
			D3DXVECTOR3 vecTemp = model->posModel - bulletSys->posBulletSys;
			vecTemp.y += BULLET_SPECIAL_THUNDER_HEIGHT;

			D3DXVec3Normalize(&vecTemp, &vecTemp);
			bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
			bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
			if (bulletSys->posBulletSys.y >= model->posModel.y + BULLET_SPECIAL_THUNDER_HEIGHT)
			{
				bulletSys->bMove = false;
			}
		}
		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->fAngle -= bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->nNum = i;												// �i���o�[
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_THUNDER_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_THUNDER_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_THUNDER_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �T���_�[�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletThunder(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	// �N�H�[�^�j�I������ύX
	if (bulletSys->vecAxis.y < BULLET_SPECIAL_THUNDER_AXIS_Y_MAX)
	{
		bulletSys->vecAxis.y += BULLET_SPECIAL_THUNDER_AXIS_Y;
		D3DXVec3Normalize(&bulletSys->vecAxis, &bulletSys->vecAxis);
	}

	// �ő唼�a���z������
	if (bulletSys->fRotLength >= BULLET_SPECIAL_THUNDER_ROT_LENGTH_MAX)
	{
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_THUNDER_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_THUNDER_RAPID == 0)
			//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
			//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse && bulletSys->bTrack)
			{
				SetBullet(
					lockon->posLockon,
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_YELLOW),
					BULLET_SPECIAL_THUNDER_CHILD__SCALE,
					BULLET_SPECIAL_THUNDER);
			}
			else
			{
				SetBullet(
					bullet->posBullet - D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_YELLOW),
					BULLET_SPECIAL_THUNDER_CHILD__SCALE,
					BULLET_SPECIAL_THUNDER);
			}
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// �A�C�X�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletIce(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// �L����
			bulletSys->bTrack = true;								// �ǔ��t���O
			bulletSys->posBulletSys = pos1;							// �Z�b�gPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// �ړ��x�N�g��
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_ICE;					// �^�C�v
			bulletSys->nTex = BULLET_NORMAL;						// �e�N�X�`���[
			bulletSys->nLockon = nLockon;							// ���b�N�I���t���O
			bulletSys->vec2Size = D3DXVECTOR2(						// �T�C�Y
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_ICE_SPEED;			// �X�s�[�h
			bulletSys->fRotSpeed = BULLET_SPECIAL_ICE_ROT_SPEED;	// ��]�X�s�[�h
			bulletSys->fRotLength = 0.0f;							// ��]�W�J����
			bulletSys->nSplit = BULLET_SPECIAL_ICE_SPLIT + BULLET_SPECIAL_ICE_SPLIT * nCharge;// �����p�x
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_ICE_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_CYAN);	// �J���[
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// ��]����ݒ�
			bulletSys->vecTa = vecTa;								// ��]���ɐ����ȃx�N�g��
			return;
		}
	}
}

//=============================================================================
// �A�C�X�o���b�g����
//=============================================================================
void SetSpecialBulletIceSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{

				D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;

				D3DXVec3Normalize(&vecTemp, &vecTemp);
				bulletSys->vecBulletSys =
					(bulletSys->vecBulletSys + vecTemp) / 2;
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
		//bulletSys->fAngle += bulletSys->fRotSpeed;
		//bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_ICE_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_ICE_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_ICE_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �A�C�X�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletIce(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
	{
		LOCKON *lockon = GetLockon(bulletSys->nLockon);
		if (lockon->bUse)
		{
			D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
			float fTemp = D3DXVec3Length(&vecTemp);

			if (fTemp < BULLET_SPECIAL_ICE_LENGTH)
			{
				if (fTemp < BULLET_SPECIAL_ICE_LENGTH / 4)
				{
					bulletSys->fRotLength = fTemp * BULLET_SPECIAL_ICE_ROT_LENGTH * 2;
				}
				else
				{
					bulletSys->fRotLength = (BULLET_SPECIAL_ICE_LENGTH - fTemp)
						* BULLET_SPECIAL_ICE_ROT_LENGTH / 2;
				}
			}
		}
	}




	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// �E�B���h�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletWind(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];
	for (int i = 0; i < BULLET_SPECIAL_WIND_MAX; i++)
	{
		// ���g�p�̃V�X�e����T��
		for (int j = 0; j < BULLET_SYS_MAX; j++, bulletSys++)
		{
			if (!bulletSys->bUse)
			{
				bulletSys->bUse = true;										// �L����
				bulletSys->bTrack = true;									// �ǔ��t���O
				bulletSys->posBulletSys = pos1;								// �Z�b�gPOS
				bulletSys->posBulletSysTag = pos2 * (i * BULLET_SPECIAL_WIND_INTERVAL);
				bulletSys->vecBulletSys = pos2;								// �ړ��x�N�g��
				bulletSys->nType = BULLET_SPECIAL_WIND;						// �^�C�v
				bulletSys->nChain = i;										// �A���ݒu�Ǘ�
				bulletSys->nTex = BULLET_NORMAL;							// �e�N�X�`���[
				bulletSys->nLockon = nLockon;								// ���b�N�I���t���O
				bulletSys->vec2Size = D3DXVECTOR2(							// �T�C�Y
					BULLET_SIZE_X, BULLET_SIZE_Y);
				bulletSys->fSpeed = BULLET_SPECIAL_WIND_SPEED;				// �X�s�[�h
				if (i % 2 == 0)
				{
					bulletSys->fRotSpeed = BULLET_SPECIAL_WIND_ROT_SPEED;		// ��]�X�s�[�h
				}
				else
				{
					bulletSys->fRotSpeed = -BULLET_SPECIAL_WIND_ROT_SPEED;		// ��]�X�s�[�h
				}
				bulletSys->fRotLength = 0.0f;								// ��]�W�J����
				bulletSys->nSplit = BULLET_SPECIAL_WIND_SPLIT + BULLET_SPECIAL_WIND_SPLIT * nCharge;	// �����p�x
				if (nCharge == MAGICCIRCLE_MAX)
				{
					bulletSys->nSplit += BULLET_SPECIAL_WIND_SPLIT_BONUS;
				}
				bulletSys->xColor = SetColorPallet(COLOR_PALLET_LIGHTGREEN);// �J���[
				bulletSys->vecAxis = bulletSys->vecBulletSys;				// ��]����ݒ�
				bulletSys->vecTa = vecTa;									// ��]���ɐ����ȃx�N�g��
				break;
			}
		}
	}
	return;
}

//=============================================================================
// �E�B���h�o���b�g����
//=============================================================================
void SetSpecialBulletWindSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];
	MODEL *model = GetModel(0);

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}
		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->posBulletSysTag.y += bulletSys->vecBulletSys.y * bulletSys->fSpeed;
		bulletSys->posBulletSys.x = model->posModel.x;
		bulletSys->posBulletSys.y = model->posModel.y + bulletSys->posBulletSysTag.y;
		bulletSys->posBulletSys.z = model->posModel.z;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_WIND_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_WIND_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_WIND_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �E�B���h�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletWind(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];
	bulletSys->fRotLength += BULLET_SPECIAL_WIND_ROT_LENGTH + (BULLET_SPECIAL_WIND_ROT_LENGTH_ADD * bulletSys->nChain);

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// �E�H�[�^�[�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletWater(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa, int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// �L����
			bulletSys->bTrack = true;								// �ǔ��t���O
			bulletSys->posBulletSys = pos1;							// �Z�b�gPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// �ړ��x�N�g��
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_WATER;				// �^�C�v
			bulletSys->nTex = BULLET_NORMAL;						// �e�N�X�`���[
			bulletSys->nLockon = nLockon;							// ���b�N�I���t���O
			bulletSys->vec2Size = D3DXVECTOR2(						// �T�C�Y
				BULLET_SIZE_X, BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_WATER_SPEED;			// �X�s�[�h
			bulletSys->fRotSpeed = BULLET_SPECIAL_WATER_ROT_SPEED;	// ��]�X�s�[�h
			bulletSys->fRotLength = 0.0f;							// ��]�W�J����
			bulletSys->nSplit = BULLET_SPECIAL_WATER_SPLIT + BULLET_SPECIAL_WATER_SPLIT * nCharge;// �����p�x
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_WATER_SPLIT_BONUS;
			}		
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_BLUE);	// �J���[
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// ��]����ݒ�
			bulletSys->vecTa = vecTa;								// ��]���ɐ����ȃx�N�g��
			return;
		}
	}
}

//=============================================================================
// �E�H�[�^�[�o���b�g����
//=============================================================================
void SetSpecialBulletWaterSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{

				D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;

				D3DXVec3Normalize(&vecTemp, &vecTemp);
				bulletSys->vecBulletSys =
					(bulletSys->vecBulletSys + vecTemp) / 2;
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
		bulletSys->fAngle += bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_WATER_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_WATER_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_WATER_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �E�H�[�^�[�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletWater(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
	{
		LOCKON *lockon = GetLockon(bulletSys->nLockon);
		if (lockon->bUse)
		{
			D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
			float fTemp = D3DXVec3Length(&vecTemp);

			if (fTemp < BULLET_SPECIAL_WATER_LENGTH)
			{
				if (fTemp < BULLET_SPECIAL_WATER_LENGTH / 2)
				{
					bulletSys->fRotLength = fTemp * BULLET_SPECIAL_WATER_ROT_LENGTH;
				}
				else
				{
					bulletSys->fRotLength = (BULLET_SPECIAL_WATER_LENGTH - fTemp)
						* BULLET_SPECIAL_WATER_ROT_LENGTH;
				}
			}
		}
	}




	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}


//=============================================================================
// �A�[�X�o���b�g�Z�b�g
//=============================================================================
void SetSpecialBulletEarth(D3DXVECTOR3 pos1 , D3DXVECTOR3 pos2, D3DXVECTOR3 vecTa,int nLockon, int nCharge)
{
	BULLET_SYS *bulletSys = &bulletSysWk[0];

	// ���g�p�̃V�X�e����T��
	for (int i = 0; i < BULLET_SYS_MAX; i++, bulletSys++)
	{
		if (!bulletSys->bUse)
		{
			bulletSys->bUse = true;									// �L����
			bulletSys->bTrack = true;								// �ǔ��t���O
			bulletSys->posBulletSys = pos1;							// �Z�b�gPOS
			bulletSys->posBulletSysTag = pos2;
			bulletSys->vecBulletSys = pos2 - pos1;					// �ړ��x�N�g��
			D3DXVec3Normalize(&bulletSys->vecBulletSys, &bulletSys->vecBulletSys);
			bulletSys->nType = BULLET_SPECIAL_EARTH;				// �^�C�v
			bulletSys->nTex = BULLET_NORMAL;						// �e�N�X�`���[
			bulletSys->nLockon = nLockon;							// ���b�N�I���t���O
			bulletSys->vec2Size = D3DXVECTOR2(						// �T�C�Y
				BULLET_SIZE_X,BULLET_SIZE_Y);
			bulletSys->fSpeed = BULLET_SPECIAL_EARTH_SPEED;			// �X�s�[�h
			bulletSys->fRotSpeed = BULLET_SPECIAL_EARTH_ROT_SPEED;	// ��]�X�s�[�h
			bulletSys->fRotLength = 0.0f;							// ��]�W�J����
			bulletSys->nSplit = BULLET_SPECIAL_EARTH_SPLIT + BULLET_SPECIAL_EARTH_SPLIT * nCharge;// �����p�x
			if (nCharge == MAGICCIRCLE_MAX)
			{
				bulletSys->nSplit += BULLET_SPECIAL_EARTH_SPLIT_BONUS;
			}
			bulletSys->xColor = SetColorPallet(COLOR_PALLET_BROWN);	// �J���[
			bulletSys->vecAxis = bulletSys->vecBulletSys;			// ��]����ݒ�
			bulletSys->vecTa = vecTa;								// ��]���ɐ����ȃx�N�g��
			return;
		}
	}
}

//=============================================================================
// �A�[�X�o���b�g����
//=============================================================================
void SetSpecialBulletEarthSys(int nSys)
{
	BULLET_SYS *bulletSys = &bulletSysWk[nSys];
	BULLET *bullet = &bulletWk[0];

	if (bulletSys->bSet)
	{	// �Z�b�g�ς݂̏ꍇ�͍X�V����
		if (bulletSys->nUseCount > BULLET_SYS_FALSE_COUNT || bulletSys->nBulletCount <= 0)
		{	// ���ŃJ�E���g��0�����̏ꍇ�̓V�X�e��������
			InitStatusBulletSys(nSys);
			return;
		}
		// ��]���a�����Z
		bulletSys->fRotLength += BULLET_SPECIAL_EARTH_ROT_LENGTH;
		// �ő唼�a���z������
		if (bulletSys->fRotLength > BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX)
		{
			bulletSys->fRotLength = BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX;
		}
		if (bulletSys->nLockon >= 0 && bulletSys->bTrack)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse)
			{
				if (bulletSys->bMove)
				{
					D3DXVECTOR3 vecTemp = lockon->posLockon - bulletSys->posBulletSys;
					vecTemp.y -= BULLET_SPECIAL_EARTH_HEIGHT;

					D3DXVec3Normalize(&vecTemp, &vecTemp);
					bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
					bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
					if ((bulletSys->posBulletSys.x - lockon->posLockon.x < BULLET_SPECIAL_EARTH_STOP
						&& bulletSys->posBulletSys.x - lockon->posLockon.x > -BULLET_SPECIAL_EARTH_STOP)
						&& (bulletSys->posBulletSys.z - lockon->posLockon.z < BULLET_SPECIAL_EARTH_STOP
							&& bulletSys->posBulletSys.z - lockon->posLockon.z > -BULLET_SPECIAL_EARTH_STOP))
					{
						bulletSys->bMove = false;
					}
				}
				else
				{
					bulletSys->posBulletSys = lockon->posLockon;
					bulletSys->posBulletSys.y -= BULLET_SPECIAL_EARTH_HEIGHT;
				}
			}
			else
			{
				bulletSys->bTrack = false;
			}
		}
		if (bulletSys->nLockon < 0 && bulletSys->bMove)
		{
			MODEL *model = GetModel(0);
			D3DXVECTOR3 vecTemp = model->posModel - bulletSys->posBulletSys;
			vecTemp.y -= BULLET_SPECIAL_EARTH_HEIGHT;

			D3DXVec3Normalize(&vecTemp, &vecTemp);
			bulletSys->vecBulletSys = (bulletSys->vecBulletSys + vecTemp) / 2;
			bulletSys->posBulletSys += bulletSys->vecBulletSys * bulletSys->fSpeed;
			if (bulletSys->posBulletSys.y <= model->posModel.y - BULLET_SPECIAL_EARTH_HEIGHT)
			{
				bulletSys->bMove = false;
			}
		}
		bulletSys->nUseCount++;					// ���ŃJ�E���g�X�V
		bulletSys->fAngle -= bulletSys->fRotSpeed;
		bulletSys->fAngle = PiCalculate360(bulletSys->fAngle);
	}
	else
	{	// ���Z�b�g�̏ꍇ�̓Z�b�g�A�b�v
		for (int i = 0; i < BULLET_MAX; i++, bullet++)
		{	// ���g�p�̒e��T��
			if (!bullet->bUse)
			{
				bullet->bUse = true;											// �L����
				bullet->nSysNum = nSys;											// �V�X�e���i���o�[
				bullet->nType = bulletSys->nType;								// �^�C�v
				bullet->nNum = i;												// �i���o�[
				bullet->vec2Size = bulletSys->vec2Size;							// �T�C�Y
				bullet->fEffectSizeChange = BULLET_SPECIAL_EARTH_EFFECT_SIZE_CHANGE;
				bullet->fEffectAlphaChange = BULLET_SPECIAL_EARTH_EFFECT_ALPHA_CHANGE;
				bullet->sclBullet *= BULLET_SPECIAL_EARTH_SCALE;
				bullet->fAngle = D3DX_PI * 2
					/ bulletSys->nSplit * bulletSys->nBulletCount;				// �����p�x
				bullet->xColor = bulletSys->xColor;								// �J���[

																				// Vtx�ݒ�
				SetVtxBullet(i);		// ���_���W�̕ύX����
				SetDiffuseBullet(i, bullet->xColor);							// ���ˌ��ύX�̕ύX����
				SetTexBullet(i, bullet->nTex);									// �e�N�X�`�����W�̐ݒ�

																				// �e�̐���
				bullet->nIdxShadow = CreateShadow(bullet->posBullet, 25.0f, 25.0f);
				bullet->fSizeShadow = ((bullet->vec2Size.x + bullet->vec2Size.y) / 2) * 2;
				bullet->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.4f);

				bulletSys->nBulletCount++;										// �Z�b�g�����V�X�e���ɋL��
				if (bulletSys->nBulletCount == bulletSys->nSplit)
				{	// �����J�E���g���̃Z�b�g���I�������I��
					bulletSys->bSet = true;
					return;
				}
			}
		}
	}
}

//=============================================================================
// �A�[�X�o���b�g�X�V����
//=============================================================================
void UpdateSpecialBulletEarth(int nBullet)
{
	BULLET *bullet = &bulletWk[nBullet];
	BULLET_SYS *bulletSys = &bulletSysWk[bullet->nSysNum];

	// �N�H�[�^�j�I������ύX
	if (bulletSys->vecAxis.y < BULLET_SPECIAL_EARTH_AXIS_Y_MAX)
	{
		bulletSys->vecAxis.y += BULLET_SPECIAL_EARTH_AXIS_Y;
		D3DXVec3Normalize(&bulletSys->vecAxis, &bulletSys->vecAxis);
	}

	// �ő唼�a���z������
	if (bulletSys->fRotLength >= BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX)
	{
		bulletSys->fRotLength = BULLET_SPECIAL_EARTH_ROT_LENGTH_MAX;
		if ((int)(bulletSys->nUseCount + bullet->nNum * BULLET_SPECIAL_EARTH_RAPID / bulletSys->nSplit)
			% BULLET_SPECIAL_EARTH_RAPID == 0)
			//int nTemp = bullet->nNum * BULLET_SPECIAL_FIRE_RAPID / bulletSys->nSplit;
			//if (bulletSys->nUseCount % BULLET_SPECIAL_FIRE_RAPID == 0)
		{
			LOCKON *lockon = GetLockon(bulletSys->nLockon);
			if (lockon->bUse && bulletSys->bTrack)
			{
				SetBullet(
					lockon->posLockon,
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_BROWN),
					BULLET_SPECIAL_EARTH_CHILD__SCALE,
					BULLET_SPECIAL_EARTH);
			}
			else
			{
				SetBullet(
					bullet->posBullet + D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					bullet->posBullet,
					SetColorPallet(COLOR_PALLET_BROWN),
					BULLET_SPECIAL_EARTH_CHILD__SCALE,
					BULLET_SPECIAL_EARTH);
			}
		}
	}

	float fAngleTemp = PiCalculate360(bulletSys->fAngle + bullet->fAngle);		// ��]�p�x
	D3DXVECTOR3 vecTaTemp = bulletSys->vecTa * bulletSys->fRotLength;			// ��]���a��ݒ�
																				// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTaTemp, &bulletSys->vecAxis, fAngleTemp, &bullet->posBullet);
	// ��]���ɐݒu
	bullet->posBullet += bulletSys->posBulletSys;
}

//=============================================================================
// �_���[�W�v�Z�֐�
//=============================================================================
float GetBulletDamage(int nType)
{
	float fTemp = 0.0f;

	// �o���b�g�^�C�v�ɉ����ă_���[�W�l���i�[
	switch (nType)
	{
	case BULLET_NORMAL:
		fTemp = BULLET_NORMAL_DAMAGE;
		break;
	case BULLET_SPECIAL_FIRE:
		fTemp = BULLET_SPECIAL_FIRE_DAMAGE;
		break;
	case BULLET_SPECIAL_THUNDER:
		fTemp = BULLET_SPECIAL_THUNDER_DAMAGE;
		break;
	case BULLET_SPECIAL_ICE:
		fTemp = BULLET_SPECIAL_ICE_DAMAGE;
		break;
	case BULLET_SPECIAL_WIND:
		fTemp = BULLET_SPECIAL_WIND_DAMAGE;
		break;
	case BULLET_SPECIAL_WATER:
		fTemp = BULLET_SPECIAL_WATER_DAMAGE;
		break;
	case BULLET_SPECIAL_EARTH:
		fTemp = BULLET_SPECIAL_EARTH_DAMAGE;
		break;
	}

	// �_���[�W�l��ԋp
	return(fTemp);
}

//=============================================================================
// �擾�֐�
//=============================================================================
BULLET *GetBullet(int no)
{
	return(&bulletWk[no]);
}

//=============================================================================
// �V�X�e���擾�֐�
//=============================================================================
BULLET_SYS *GetBulletSys(int no)
{
	return(&bulletSysWk[no]);
}