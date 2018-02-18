//=============================================================================
//
// ���菈�� [checkhit.cpp]
// Author : GP11B243 24 �l���F��
//
// �����蔻����s��cpp
//
//=============================================================================
#include "main.h"
#include "checkhit.h"

#include "model.h"
#include "enemy.h"
#include "bullet.h"
#include "enemybullet.h"
#include "hiteffect.h"
#include "camera.h"
#include "shadow.h"
#include "fade.h"
#include "lockon.h"
#include "stage.h"
#include "sound.h"
#include "bulletqua.h"
#include "magic.h"
#include "damageeffect.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool CheckHitRayToSphere(D3DXVECTOR3 posRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posTag, float nLength);

//=============================================================================
// �����蔻��֐�
//=============================================================================
void ChackHit(void)
{
	if (!GetEndFlag())
	{
		MODEL *model;
		ENEMY *enemy;
		BULLET *bullet;
		BULLETQUA *bulletqua;
		ENEMYBULLET *enemybullet;
		int *pCameraMode = GetCameraMode();
		CAMERA *camera = GetCamera(*pCameraMode);

#ifdef _DEBUG
		PrintDebugProc("�y CHECKHIT �z\n");
#endif
		int breakPoint;
		breakPoint = 0;
		// ���f���ƃG�l�~�[�̓����蔻��(BC)
		model = GetModel(0);
		for (int i = 0; i < MODEL_MAX; i++, model++)
		{
			// �g�p���Ă��郂�f�������邩���m�F
			if (model->bUse)
			{
				if (model->nInvisibleCount <= 0)
				{
					enemybullet = GetEnemybullet(0);
					for (int j = 0; j < ENEMYBULLET_MAX; j++, enemybullet++)
					{
						if (enemybullet->bUse)
						{
							// BC�̊m�F�i�z�[�~���O�����j
							if (CheckHitBC(enemybullet->posEnemybullet, model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f), ENEMYBULLET_SIZE_X, ENEMYBULLET_HOMING_LENGTH))
							{
								enemybullet->bHoming = false;
							}

							// BC�̊m�F
							if (CheckHitBC(enemybullet->posEnemybullet, model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f), ENEMYBULLET_SIZE_X, MODEL_SIZE))
							{
								SetVoice(VOICE_ITTAA, E_DS8_FLAG_NONE, CONTINUITY_OFF);
								model->fStatusHP -= 1.0f;
								model->nInvisibleCount = MODEL_INVISIBLE_COUNT;
								SetDamageeffect();
								if (model->fStatusHP <= 0.0f)
								{
									model->bUse = false;
									ReleaseShadow(model->nIdxShadow);
									SetClearFlag(false);
									SetVoice(VOICE_GAMEOVER, E_DS8_FLAG_NONE, CONTINUITY_ON);
									SetFade(FADE_OUT, STAGE_RESULT);
								}
								// �V���h�E�̖�����
								ReleaseShadow(enemybullet->nIdxShadow);
								InitStatusEnemybullet(j);
							}
						}
					}

					bulletqua = GetBulletQua(0);
					for (int j = 0; j < ENEMYBULLET_MAX; j++, bulletqua++)
					{
						if (bulletqua->bUse && bulletqua->bDraw)
						{
							// BC�̊m�F
							if (CheckHitBC(bulletqua->posBulletQua, model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_EYE, 0.0f), ENEMYBULLET_SIZE_X, MODEL_SIZE))
							{
								SetVoice(VOICE_ITTAA, E_DS8_FLAG_NONE, CONTINUITY_OFF);
								model->fStatusHP -= 1.0f;
								model->nInvisibleCount = MODEL_INVISIBLE_COUNT;
								SetDamageeffect();
								if (model->fStatusHP <= 0.0f)
								{
									model->bUse = false;
									ReleaseShadow(model->nIdxShadow);
									SetClearFlag(false);
									SetVoice(VOICE_GAMEOVER, E_DS8_FLAG_NONE, CONTINUITY_ON);
									SetFade(FADE_OUT, STAGE_RESULT);
								}
								bulletqua->bDraw = false;
								// �V���h�E�̖�����
								ReleaseShadow(bulletqua->nIdxShadow);
								InitStatusBulletQua(j);
							}
						}
					}
				}

				enemy = GetEnemy(0);
				for (int j = 0; j < ENEMY_MAX; j++, enemy++)
				{
					// �g�p���Ă���G�l�~�[�����邩���m�F
					if (enemy->bUse)
					{
						if (model->nInvisibleCount <= 0)
						{
							// BC�̊m�F
							if (CheckHitBC(enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_HEIGHT, 0.0f),
								model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f),
								ENEMY_SIZE, MODEL_SIZE))
							{
								SetVoice(VOICE_ITTAA, E_DS8_FLAG_NONE, CONTINUITY_OFF);
								model->fStatusHP -= 1.0f;
								model->nInvisibleCount = MODEL_INVISIBLE_COUNT;
								SetDamageeffect();
								if (model->fStatusHP <= 0.0f)
								{
									model->bUse = false;
									ReleaseShadow(model->nIdxShadow);
									SetVoice(VOICE_GAMEOVER, E_DS8_FLAG_NONE, CONTINUITY_ON);
									SetClearFlag(false);
									SetFade(FADE_OUT, STAGE_RESULT);
								}
							}
						}

						if (CheckHitBC(enemy->posEnemy, model->posModel, LOCKON_LENGTH, LOCKON_LENGTH))
						{
							D3DXVECTOR3 vecTemp = enemy->posEnemy - camera->posCameraAt;
							float fLengthTemp = (sinf(VIEW_ANGLE / 2)) * D3DXVec3Length(&vecTemp);
							fLengthTemp = fLengthTemp * LOCKON_RADIUS;
							// ���C�ƃG�l�~�[�̓����蔻��
							if (CheckHitRayToSphere(camera->posCameraAt,
								(camera->posCameraAt - camera->posCameraEye) / 100,
								enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f),
								fLengthTemp))
							{
								breakPoint++;
								if (enemy->bLockon)
								{
									if (enemy->nModel == ENEMY_TYPE_BOSS)
									{
										SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f));
									}
									else
									{
										SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_BULLET_MUZZELE_HEIGHT, 0.0f));
									}
									ResetReleaseCountLockon(enemy->nIdxLockon);
								}
								else
								{
									if (CheckMagicModel(0, GetTypeMagic()))
									{
										if (enemy->nModel == ENEMY_TYPE_BOSS)
										{
											enemy->nIdxLockon = SetLockon(enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f));
										}
										else
										{
											enemy->nIdxLockon = SetLockon(enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_BULLET_MUZZELE_HEIGHT, 0.0f));
										}
										ResetReleaseCountLockon(enemy->nIdxLockon);
										enemy->bLockon = true;
									}
								}
							}
							else
							{
								if (enemy->bLockon)
								{
									if (enemy->nModel == ENEMY_TYPE_BOSS)
									{
										SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f));
									}
									else
									{
										SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_BULLET_MUZZELE_HEIGHT, 0.0f));
									}
									enemy->bLockon = AddReleaseCountLockon(enemy->nIdxLockon);
								}
							}
						}
						else
						{
							if (enemy->bLockon)
							{
								if (enemy->nModel == ENEMY_TYPE_BOSS)
								{
									SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f));
								}
								else
								{
									SetPosLockon(enemy->nIdxLockon, enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_BULLET_MUZZELE_HEIGHT, 0.0f));
								}
								enemy->bLockon = AddReleaseCountLockon(enemy->nIdxLockon);
							}
						}

						bool bSe12Flag = false;
						bullet = GetBullet(0);
						for (int k = 0; k < BULLET_MAX; k++, bullet++)
						{
							if (bullet->bUse && !bullet->bEnemy)
							{
								// BC�̊m�F
								if (CheckHitBC(
									enemy->posEnemy + D3DXVECTOR3(0.0f, ENEMY_CENTER, 0.0f),
									bullet->posBullet, ENEMY_SIZE,
									BULLET_SIZE_X))
								{
									if (bullet->nEffect == 0)
									{
										SetHiteffect(bullet->posBullet, bullet->nType);
										enemy->fStatusHP -= GetBulletDamage(bullet->nType);
									}
									else
									{
										SetHiteffect(bullet->posBullet, bullet->nEffect);
										enemy->fStatusHP -= GetBulletDamage(bullet->nEffect);
									}
									if (!bSe12Flag)
									{
										SetSe(12, E_DS8_FLAG_NONE, true);
										bSe12Flag = true;
									}
									if (enemy->fStatusHP <= 0.0f)
									{
										enemy->fStatusHP = 0.0f;
										if (enemy->bLockon)
										{
											ReleaseLockon(enemy->nIdxLockon);
										}
										ReleaseShadow(enemy->nIdxShadow);
										if (enemy->nModel == ENEMY_TYPE_BOSS)
										{
											SetFade(FADE_OUT, STAGE_RESULT);
											SetVoice(VOICE_CLEAR, E_DS8_FLAG_NONE, CONTINUITY_OFF);
											SetClearFlag(true);
										}
										else
										{
											ReleaseEnemy(j);
										}
									}
									// �V���h�E�̖�����
									ReleaseShadow(bullet->nIdxShadow);

									if (bullet->nType != BULLET_NORMAL)
									{
										BULLET_SYS *bulletSys = GetBulletSys(bullet->nSysNum);
										bulletSys->nBulletCount--;
									}
									InitStatusBullet(k);
								}
							}
						}
						if (breakPoint > 0)
						{
							breakPoint = 0;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// BC�̔���֐�
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	D3DXVECTOR3 temp = pos1 - pos2;
	float fTempLengthSq = D3DXVec3LengthSq(&temp);

	// �����蔻��̊m�F(BC)
	if (fTempLengthSq <= (size1 + size2)*(size1 + size2))
	{
		// �����蔻��
		return true;
	}
	return false;
}

//=============================================================================
// ���C�ƃX�t�B�A�̓����蔻��
//=============================================================================
bool CheckHitRayToSphere(D3DXVECTOR3 posRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posTag, float nLength)
{
	posTag.x -= posRay.x;
	posTag.y -= posRay.y;
	posTag.z -= posRay.z;

	float fA = vecRay.x * vecRay.x + vecRay.y * vecRay.y + vecRay.z * vecRay.z;
	float fB = vecRay.x * posTag.x + vecRay.y * posTag.y + vecRay.z * posTag.z;
	float fC = posTag.x * posTag.x + posTag.y * posTag.y + posTag.z * posTag.z - nLength * nLength;

	if (fA == 0.0f)
	{	// ���C�̒�����0
		return false;
	}

	float fS = fB * fB - fA * fC;
	if (fS < 0.0f)
	{	// �Փ˂Ȃ�
		return false;
	}

	fS = sqrtf(fS);
	float fA1 = (fB - fS) / fA;
	float fA2 = (fB + fS) / fA;

	if (fA1 < 0.0f || fA2 < 0.0f)
	{	// ���C�̔��΂ŏՓ�
		return false;
	}

	return true;
}