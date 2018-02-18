//=============================================================================
//
// 判定処理 [checkhit.cpp]
// Author : GP11B243 24 人見友基
//
// 当たり判定を行うcpp
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

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool CheckHitRayToSphere(D3DXVECTOR3 posRay, D3DXVECTOR3 vecRay, D3DXVECTOR3 posTag, float nLength);

//=============================================================================
// 当たり判定関数
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
		PrintDebugProc("【 CHECKHIT 】\n");
#endif
		int breakPoint;
		breakPoint = 0;
		// モデルとエネミーの当たり判定(BC)
		model = GetModel(0);
		for (int i = 0; i < MODEL_MAX; i++, model++)
		{
			// 使用しているモデルがあるかを確認
			if (model->bUse)
			{
				if (model->nInvisibleCount <= 0)
				{
					enemybullet = GetEnemybullet(0);
					for (int j = 0; j < ENEMYBULLET_MAX; j++, enemybullet++)
					{
						if (enemybullet->bUse)
						{
							// BCの確認（ホーミング解除）
							if (CheckHitBC(enemybullet->posEnemybullet, model->posModel + D3DXVECTOR3(0.0f, MODEL_CENTER, 0.0f), ENEMYBULLET_SIZE_X, ENEMYBULLET_HOMING_LENGTH))
							{
								enemybullet->bHoming = false;
							}

							// BCの確認
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
								// シャドウの無効化
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
							// BCの確認
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
								// シャドウの無効化
								ReleaseShadow(bulletqua->nIdxShadow);
								InitStatusBulletQua(j);
							}
						}
					}
				}

				enemy = GetEnemy(0);
				for (int j = 0; j < ENEMY_MAX; j++, enemy++)
				{
					// 使用しているエネミーがあるかを確認
					if (enemy->bUse)
					{
						if (model->nInvisibleCount <= 0)
						{
							// BCの確認
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
							// レイとエネミーの当たり判定
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
								// BCの確認
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
									// シャドウの無効化
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
// BCの判定関数
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	D3DXVECTOR3 temp = pos1 - pos2;
	float fTempLengthSq = D3DXVec3LengthSq(&temp);

	// 当たり判定の確認(BC)
	if (fTempLengthSq <= (size1 + size2)*(size1 + size2))
	{
		// 当たり判定
		return true;
	}
	return false;
}

//=============================================================================
// レイとスフィアの当たり判定
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
	{	// レイの長さが0
		return false;
	}

	float fS = fB * fB - fA * fC;
	if (fS < 0.0f)
	{	// 衝突なし
		return false;
	}

	fS = sqrtf(fS);
	float fA1 = (fB - fS) / fA;
	float fA2 = (fB + fS) / fA;

	if (fA1 < 0.0f || fA2 < 0.0f)
	{	// レイの反対で衝突
		return false;
	}

	return true;
}
