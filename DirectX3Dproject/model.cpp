//=============================================================================
//
// モデル処理 [model.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "camera.h"
#include "main.h"
#include "shadow.h"
#include "calculate.h"
#include "bullet.h"
#include "enemy.h"
#include "fade.h"
#include "parameter.h"
#include "cloud.h"
#include "lockon.h"
#include "game.h"
#include "magic.h"
#include "lockon.h"
#include "sound.h"
#include "player.h"
#include "Light.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MoveModel(int nModel, int CameraMode, int CameraGameMode);
void AttackNormalModel(int nModel, int CameraMode, int CameraGameMode);
void AttackMagicModel(int nModel, int CameraMode, int CameraGameMode);
void MoveAngleModel(int nModel, float fAngle);
void JumpModel(int nModel);
void TiltModel(int nModel);
void FloatModel(int nModel);
void TagModel(int nModel);
void MagicRecoverModel(int nModel, int nMagic);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	*g_pD3DTextureModel;		// テクスチャへのポインタ
D3DMATERIAL9		*g_pD3DMatMeshModel;

LPD3DXMESH			g_pD3DXMeshModel;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatModel;	// マテリアル情報へのポインタ
DWORD				g_nNumMatModel;			// マテリアル情報の数

MODEL				modelWk[MODEL_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(int nType)
{
	MODEL *model = &modelWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < MODEL_MAX; i++, model++)
	{
		// 位置・回転・スケールの設定
		model->posModel = D3DXVECTOR3(MODEL_POS_X, 0.0f, MODEL_POS_Z);
		model->rotModel = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
		model->sclModel = D3DXVECTOR3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
		model->moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		model->nTag = 0;
		model->nTagNum = 0;
		model->nTagReleaseCount = 0;

		model->nAttackNormalCount = 0;

		model->nCharge = 0;
		model->nChargeCount = 0;

		model->nChangeLightSpot = 0;
		model->nChangeLightPoint = 0;

		model->nAttackFireCount = 0;
		model->nAttackThunderCount = 0;
		model->nAttackIceCount = 0;
		model->nAttackWindCount = 0;
		model->nAttackWaterCount = 0;
		model->nAttackEarthCount = 0;

		model->nInvisibleCount = 0;
		model->nAttackSpCount = 0;

		model->bUse = true;
		model->bLockOn = false;
		model->bParameter = false;
		model->bJump = false;
		model->bStatusMPLimiter = false;
		model->bDash = false;
		model->bAttack = false;
		model->bAttackSp = false;

		model->bActiveFire = false;
		model->bActiveThunder = false;
		model->bActiveIce = false;
		model->bActiveWind = false;
		model->bActiveWater = false;
		model->bActiveEarth = false;

		model->fStatusHP = MODEL_STATUS_HP;
		model->fStatusMP = MODEL_STATUS_MP;
		model->fStatusNormal = MODEL_STATUS_NORMAL;
		model->fStatusFire = 0.0f;
		model->fStatusThunder = 0.0f;
		model->fStatusIce = 0.0f;
		model->fStatusWind = 0.0f;
		model->fStatusWater = 0.0f;
		model->fStatusEarth = 0.0f;

		model->fMoveSpeed = MODEL_MOVE_SPEED;
		model->fHAngle = 0.0f;
		model->fHAngleDest = MODEL_H_ANGLE;
		model->fHAngleDiff = 0.0f;
		model->fFloatAngle = 0.0f;
		model->fFloatPos = 0.0f;
		model->fJumpAccel = 0.0f;

		// シャドウ用
		model->nIdxShadow = 0;
		model->fSizeShadow = 0.0f;
		model->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
		model->bShadow = false;

		// ミニマップ
		model->bUseMinimap = false;
		//SetParameter(0, model->posModel, i);

		// マジックサークル
		for (int j = 0; j < MAGICCIRCLE_MAX; j++)
		{
			model->magicCircle[j].bUse = false;
			model->magicCircle[j].nIdx = -1;
		}
	}

	if (nType == 0)
	{
		//SetParameter(0, model->posModel, 0);
		// モデル関係の初期化
		g_pD3DTextureModel = NULL;
		g_pD3DXMeshModel = NULL;
		g_pD3DXBuffMatModel = NULL;
		g_nNumMatModel = 0;

		//// モデルデータ読み込み
		//if (FAILED(D3DXLoadMeshFromX(MODEL_MODEL,			// モデルデータ
		//	D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
		//	pDevice,					// デバイス
		//	NULL,						// 未使用
		//	&g_pD3DXBuffMatModel,		// マテリアルデータ
		//	NULL,						// 未使用
		//	&g_nNumMatModel,			// D3DXMATERIAL構造体の数
		//	&g_pD3DXMeshModel)))		// メッシュデータへのポインタ
		//{
		//	return E_FAIL;
		//}

		//// マテリアル情報を取り出す
		//D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();
		//g_pD3DMatMeshModel = new D3DMATERIAL9[g_nNumMatModel];		// メッシュ情報を確保
		//g_pD3DTextureModel = new LPDIRECT3DTEXTURE9[g_nNumMatModel];// テクスチャを確保
		//for (int i = 0; i < g_nNumMatModel; i++)
		//{
		//	g_pD3DMatMeshModel[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
		//	g_pD3DMatMeshModel[i].Ambient = g_pD3DMatMeshModel[i].Diffuse;// 環境光初期化
		//	g_pD3DTextureModel[i] = NULL;	// テクスチャ初期化

		//	// 使用しているテクスチャがあれば読み込む
		//	if (d3Mat[i].pTextureFilename != NULL &&
		//		lstrlen(d3Mat[i].pTextureFilename) > 0)
		//	{
		//		// テクスチャ読み込み
		//		if (FAILED(D3DXCreateTextureFromFile(
		//			pDevice,
		//			d3Mat[i].pTextureFilename,
		//			&g_pD3DTextureModel[i])))
		//		{
		//			return E_FAIL;
		//		}
		//	}
		//}
	}
	return S_OK;
}

////=============================================================================
//// マジック初期化処理
////=============================================================================
//void InitModelMagic(int nModel, int nMagic)
//{
//	MODEL *model = &modelWk[nModel];
//	if (nMagic == BULLET_NORMAL)
//	{
//		model->modelMagic[nMagic].bUse = true;
//		model->modelMagic[nMagic].fStatus = MODEL_STATUS_NORMAL;
//	}
//	else
//	{
//		model->modelMagic[nMagic].bUse = false;
//		model->modelMagic[nMagic].fStatus = 0;
//	}
//
//	model->modelMagic[nMagic].nAttackCount = 0;
//
//	switch (nMagic)
//	{
//	case BULLET_NORMAL:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_NORMAL_SUB;
//		break;
//	case BULLET_SPECIAL_FIRE:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_FIRE_SUB;
//		break;
//	case BULLET_SPECIAL_THUNDER:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_THUNDER_SUB;
//		break;
//	case BULLET_SPECIAL_ICE:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_ICE_SUB;
//		break;
//	case BULLET_SPECIAL_WIND:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_WIND_SUB;
//		break;
//	case BULLET_SPECIAL_WATER:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_WATER_SUB;
//		break;
//	case BULLET_SPECIAL_EARTH:
//		model->modelMagic[nMagic].fStatusSub = MODEL_STATUS_EARTH_SUB;
//		break;
//
//	}
//}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	if(g_pD3DTextureModel != NULL)
	{// テクスチャの開放
		for (int i = 0; i < g_nNumMatModel; i++)
		{
			g_pD3DTextureModel[i]->Release();
			g_pD3DTextureModel[i] = NULL;
		}
	}

	if(g_pD3DXMeshModel != NULL)
	{// メッシュの開放
		g_pD3DXMeshModel->Release();
		g_pD3DXMeshModel = NULL;
	}

	if(g_pD3DXBuffMatModel != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatModel->Release();
		g_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	MODEL *model = &modelWk[0];
	int *pCameraMode = GetCameraMode();
	int *pCameraGameMode = GetCameraGameMode();
	E_STAGE eStage = GetStage();
#ifdef _DEBUG
	PrintDebugProc("【 MODEL 】\n");
	PrintDebugProc("HP:%f  rotZ:%f\n", model->fStatusHP, model->rotModel.z);
#endif

	// スキル全開放
	if (GetKeyboardPress(DIK_P) || (IsButtonPressed(0, BUTTON_R) && IsButtonPressed(0, R_BUTTON_ZR) && IsButtonPressed(0, BUTTON_HOME)))
	{
		SetMagicType(BULLET_SPECIAL_FIRE);
		SetMagicType(BULLET_SPECIAL_THUNDER);
		SetMagicType(BULLET_SPECIAL_ICE);
		SetMagicType(BULLET_SPECIAL_WIND);
		SetMagicType(BULLET_SPECIAL_WATER);
		SetMagicType(BULLET_SPECIAL_EARTH);
	}

	for (int i = 0; i < MODEL_MAX; i++, model++)
	{
		if (model->bUse)
		{
			//if (!model->bParameter)
			//{	// HPバーセット（仮）
			//	SetParameter(0, model->posModel, 0);
			//	model->bParameter = true;
			//}

			if (model->nInvisibleCount > 0)
			{
				model->nInvisibleCount--;
			}

			if (!model->bShadow)
			{
				model->nIdxShadow = CreateShadow(model->posModel, 25.0f, 25.0f);
				model->fSizeShadow = 30.0f;
				model->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.7f);
				model->bShadow = true;
			}

			//// ステージ遷移テスト
			//if (GetKeyboardTrigger(DIK_RETURN))
			//{
			//	SetFade(FADE_OUT, STAGE_RESULT);
			//}

			switch (eStage)
			{
			case STAGE_TITLE:
				FloatModel(i);									// 浮遊処理
				break;
			case STAGE_TUTORIAL:
				break;
			case STAGE_CREDIT:
				break;
			case STAGE_GAME:
				TagModel(i);									// ターゲット変更処理
				JumpModel(i);									// ジャンプ処理
				TiltModel(i);									// 傾動処理
				MoveModel(i, *pCameraMode, *pCameraGameMode);	// 移動処理
				MagicRecoverModel(i,0);
				AttackNormalModel(i, *pCameraMode, *pCameraGameMode);	// 攻撃処理
				AttackMagicModel(i, *pCameraMode, *pCameraGameMode);	// 攻撃処理

				// シャドウ管理
				SetPositionShadow(model->nIdxShadow, D3DXVECTOR3(model->posModel.x, 0.2f, model->posModel.z));
				SetVertexShadow(model->nIdxShadow, model->fSizeShadow, model->fSizeShadow);
				SetColorShadow(model->nIdxShadow, model->colShadow);

				break;
			case STAGE_RESULT:
				FloatModel(i);									// 浮遊処理
				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	MODEL *model = &modelWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	//// αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, MODEL_ALPHA);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&model->mtxWorld);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, model->sclModel.x, model->sclModel.y, model->sclModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rotModel.y, model->rotModel.x, model->rotModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, model->posModel.x, model->posModel.y, model->posModel.z);
	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

	/******************** ビューポート変換 ********************/
	// 現在のマテリアルを保存
	pDevice->GetMaterial(&matDef);
	//// マテリアル情報に対するポインタの取得
	//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for (int i = 0; i < g_nNumMatModel; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&g_pD3DMatMeshModel[i]);
		// テクスチャの設定（NULL:テクスチャ無し）
		pDevice->SetTexture(0, g_pD3DTextureModel[i]);
		// 描画
		g_pD3DXMeshModel->DrawSubset(i);
	}
	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	//// αテストを無効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 傾き処理関数
//=============================================================================
void TiltModel(int nModel)
{
	MODEL *model = &modelWk[nModel];

	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
	{	// 前方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (model->rotModel.x > -MODEL_TILT_MARGIN_BOOST_FRONT)
			{	// ブースト最大角度以下であればブーストスピードで傾動
				model->rotModel.x -= MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (model->rotModel.x > -MODEL_TILT_MARGIN_FRONT)
			{	// 通常最大角度以下であれば通常スピードで傾動
				model->rotModel.x -= MODEL_TILT_ROT;
			}
			else if (model->rotModel.x < -MODEL_TILT_MARGIN_FRONT)
			{	// 通常最大角度以上であればオートスピードで傾動
				model->rotModel.x += MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
	{	// 後方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (model->rotModel.x < MODEL_TILT_MARGIN_BOOST_LEAR)
			{	// ブースト最大角度以下であればブーストスピードで傾動
				model->rotModel.x += MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (model->rotModel.x < MODEL_TILT_MARGIN_LEAR)
			{	// 通常最大角度以下であれば通常スピードで傾動
				model->rotModel.x += MODEL_TILT_ROT;
			}
			else if (model->rotModel.x > MODEL_TILT_MARGIN_LEAR)
			{	// 通常最大角度以上であればオートスピードで傾動
				model->rotModel.x -= MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else
	{	// 前後移動がない
		if (model->rotModel.x < 0.0f)
		{	// 原点より後方にいる場合は原点までオートスピードで傾動
			model->rotModel.x += MODEL_TILT_ROT_AUTO;
		}
		if (model->rotModel.x > 0.0f)
		{	// 原点より前方にいる場合は原点までオートスピードで傾動
			model->rotModel.x -= MODEL_TILT_ROT_AUTO;
		}
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
	{	// 左方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (model->rotModel.z > -(MODEL_TILT_MARGIN_BOOST_SIDE))
			{	// ブースト最大角度以下であればブーストスピードで傾動
				model->rotModel.z -= MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (model->rotModel.z > -(MODEL_TILT_MARGIN_SIDE))
			{	// 通常最大角度以下であれば通常スピードで傾動
				model->rotModel.z -= MODEL_TILT_ROT;
				if (model->rotModel.z < -(MODEL_TILT_MARGIN_SIDE))
				{
					model->rotModel.z = -(MODEL_TILT_MARGIN_SIDE);
				}
			}
			else if (model->rotModel.z < -(MODEL_TILT_MARGIN_SIDE))
			{	// 通常最大角度以上であればオートスピードで傾動
				model->rotModel.z += MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
	{	// 右方移動時
		if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
		{	// シフトキーが押されている場合
			if (model->rotModel.z < (MODEL_TILT_MARGIN_BOOST_SIDE))
			{	// ブースト最大角度以下であればブーストスピードで傾動
				model->rotModel.z += MODEL_TILT_ROT_BOOST;
			}
		}
		else
		{	// シフトキーが押されていない場合
			if (model->rotModel.z < (MODEL_TILT_MARGIN_SIDE))
			{	// 通常最大角度以下であれば通常スピードで傾動
				model->rotModel.z += MODEL_TILT_ROT;
				if (model->rotModel.z > MODEL_TILT_MARGIN_SIDE)
				{
					model->rotModel.z = MODEL_TILT_MARGIN_SIDE;
				}
			}
			else if (model->rotModel.z >(MODEL_TILT_MARGIN_SIDE))
			{	// 通常最大角度以上であればオートスピードで傾動
				model->rotModel.z -= MODEL_TILT_ROT_AUTO;
			}
		}
	}
	else
	{	// 左右移動がない
		if (model->rotModel.z < 0.0f)
		{	// 原点より後方にいる場合は原点までオートスピードで傾動
			model->rotModel.z += MODEL_TILT_ROT_AUTO;
		}
		if (model->rotModel.z > 0.0f)
		{	// 原点より前方にいる場合は原点までオートスピードで傾動
			model->rotModel.z -= MODEL_TILT_ROT_AUTO;
		}
	}
}

//=============================================================================
// 移動処理関数
//=============================================================================
void MoveModel(int nModel ,int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];

	if (CameraMode == CAMERA_GAME)
	{
		CAMERA *camera = GetCamera(CAMERA_GAME);

		if (GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, LSTICK_BUTTON) || IsButtonPressed(0, BUTTON_R))						// シフトキー押下中
		{
			if (model->fStatusMP < MODEL_STATUS_MP_SUB)			// MPが減算値を下回ったら
			{
				SetSe(9, E_DS8_FLAG_NONE, false);
				model->bStatusMPLimiter = true;					// MPリミッター有効
				model->bDash = false;
			}
			else if(model->fStatusMP > MODEL_STATUS_MP_LIMITER)	// MPがリミッター値を越えたら
			{
				model->bStatusMPLimiter = false;				// MPリミッター無効
				model->bDash = true;
			}
			model->fStatusMP -= MODEL_STATUS_MP_SUB;			// MPを減算
			if (model->fStatusMP < 0.0f)
			{													// 最小値を下回ったら
				model->fStatusMP = 0.0f;						// 最小値を代入
			}
		}
		else
		{
			model->fStatusMP += MODEL_STATUS_MP_RECOVER;		// MP自然回復
			if (model->fStatusMP > MODEL_STATUS_MP)				// MP最大値を越えたら
			{
				model->fStatusMP = MODEL_STATUS_MP;
			}				// MP最大値を維持
			model->bDash = false;
		}

		// カメラの角度を取得
		int nAngelTemp = camera->fHAngle * 100;
		model->fHAngle = (nAngelTemp % 628) * 0.01f;

		// 移動処理
		if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_LEFT))
		{
			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
			{// 左前移動
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.25f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.25f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.25f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// 左後移動
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.75f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.75f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.75f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
			{// 左右同時押しは処理なし
			}
			else
			{// 左移動
				MoveAngleModel(nModel, model->fHAngle + D3DX_PI * 0.50f);
				//model->moveModel.x = cosf(model->fHAngle + D3DX_PI * 0.50f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle + D3DX_PI * 0.50f) * model->fMoveSpeed;
			}
			//SetPlayerAnime(0, ANIME05);
		}
		else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT) || IsButtonPressed(0, LSTICK_RIGHT))
		{
			if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, LSTICK_UP))
			{// 右前移動
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.25f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.25f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.25f) * model->fMoveSpeed;
			}
			else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// 右後移動
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.75f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.75f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.75f) * model->fMoveSpeed;
			}
			else
			{// 右移動
				MoveAngleModel(nModel, model->fHAngle - D3DX_PI * 0.50f);
				//model->moveModel.x = cosf(model->fHAngle - D3DX_PI * 0.50f) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle - D3DX_PI * 0.50f) * model->fMoveSpeed;
			}
			//SetPlayerAnime(0, ANIME06);
		}
		else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, LSTICK_UP))
		{// 前移動
			if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
			{// 前後同時押しは処理なし
			}
			else
			{
				MoveAngleModel(nModel, model->fHAngle);
				//model->moveModel.x = cosf(model->fHAngle) * model->fMoveSpeed;
				//model->moveModel.z = sinf(model->fHAngle) * model->fMoveSpeed;
				//SetPlayerAnime(0, ANIME04);
			}
		}
		else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN) || IsButtonPressed(0, LSTICK_DOWN))
		{// 後移動
			MoveAngleModel(nModel, model->fHAngle + D3DX_PI);
			//model->moveModel.x = cosf(model->fHAngle + D3DX_PI) * model->fMoveSpeed;
			//model->moveModel.z = sinf(model->fHAngle + D3DX_PI) * model->fMoveSpeed;
			//SetPlayerAnime(0, ANIME07);
		}
		else
		{
			//SetPlayerAnime(0, ANIME03);
		}

		// カメラとモデルの角度を同期
		model->fHAngleDest = D3DX_PI - model->fHAngle - D3DX_PI * 0.50f;

		// 回転先と現在の角度差を計算
		model->fHAngleDiff = model->fHAngleDest - model->rotModel.y;
		model->fHAngleDiff = PiCalculate180(model->fHAngleDiff);

		// 角度差を適用
		model->rotModel.y += model->fHAngleDiff * MODEL_ROT_AUTO_SPEED;
		model->rotModel.y = PiCalculate180(model->rotModel.y);

		model->posModel -= model->moveModel;

		// 移動量に慣性をかける
		model->moveModel.x += (0.0f - model->moveModel.x) * MODEL_MOVE_INERTIA;
		model->moveModel.y += (0.0f - model->moveModel.y) * MODEL_MOVE_INERTIA;
		model->moveModel.z += (0.0f - model->moveModel.z) * MODEL_MOVE_INERTIA;
	}

// デバッグ用
#ifdef _DEBUG
	PrintDebugProc("【MODEL】\n");
	PrintDebugProc("Pos[X:%f Z:%f]  Rot[Y:%f]\n", model->posModel.x, model->posModel.z, model->rotModel.y);
	PrintDebugProc("Tag[%d]  TagNum[%d]  TagReleaseCount[%d]  ", model->nTag, model->nTagNum, model->nTagReleaseCount);
	if (model->bLockOn)
	{
		PrintDebugProc("LockOn[true]\n");
	}
	else
	{
		PrintDebugProc("LockOn[false]\n");
	}
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 移動処理関数
//=============================================================================
void MoveAngleModel(int nModel, float fAngle)
{
	MODEL *model = GetModel(nModel);
	model->moveModel.x = cosf(fAngle) * model->fMoveSpeed;
	model->moveModel.z = sinf(fAngle) * model->fMoveSpeed;

	if (model->bAttack || model->bAttackSp)
	{
	}
	else
	{
		model->fHAngle = fAngle;
	}
}

//=============================================================================
// ターゲット変更処理関数
//=============================================================================
void TagModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	ENEMY *enemy;
	ENEMY_SYS *enemySys = GetEnemySys();

	int pCameraGameMode = *GetCameraGameMode();


	if (enemySys->nActiveCount == 0)
	{
		model->bLockOn = false;
		if (pCameraGameMode != CAMERA_TPS)
		{
			SetCameraGameMode(CAMERA_NORMAL);
		}
		else if (pCameraGameMode != CAMERA_NORMAL)
		{
			SetCameraGameMode(CAMERA_TPS);
		}
	}
	

	if (GetKeyboardTrigger(DIK_TAB) && enemySys->nActiveCount != 0)
	{
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			// ターゲット送り（戻り）
			if (model->bLockOn)
			{
				model->nTagNum--;
			}
			// 0を下回った場合はアクティエネミー最大数から-1にもどる
			if (model->nTagNum < 0)
			{
				model->nTagNum = enemySys->nActiveCount - 1;
				if (model->nTagNum > ENEMY_ACTIVE_COUNT_MAX)
				{
					model->nTagNum = ENEMY_ACTIVE_COUNT_MAX - 1;
				}
			}
		}
		else
		{
			// ターゲット送り
			if (model->bLockOn)
			{
				model->nTagNum++;
			}
			// アクティブエネミー、最大カウントを越えた場合は0番目にもどる
			if (model->nTagNum > enemySys->nActiveCount || model->nTagNum > ENEMY_ACTIVE_COUNT_MAX - 1)
			{
				model->nTagNum = 0;
			}
		}
		// 1以上はエネミーにセット
		SetCameraTag(enemySys->nActiveEnemy[model->nTagNum]);
		// モデルがターゲットするエネミーをセット
		model->nTag = enemySys->nActiveEnemy[model->nTagNum];
		SetCameraGameMode(CAMERA_TARGET);
		model->bLockOn = true;
	}

	if (model->bLockOn)
	{
		// モデルがターゲットしているエネミーを呼び出す
		enemy = GetEnemy(model->nTag);
		if (!enemy->bUse)
		{
			// ターゲット送り
			model->nTagNum++;
			// アクティブエネミー、最大カウントを越えた場合は0番目にもどる
			if (model->nTagNum > enemySys->nActiveCount -1 || model->nTagNum > ENEMY_ACTIVE_COUNT_MAX - 1)
			{
				model->nTagNum = 0;
			}
			// 1以上はエネミーにセット
			SetCameraTag(enemySys->nActiveEnemy[model->nTagNum]);
			// モデルがターゲットするエネミーをセット
			model->nTag = enemySys->nActiveEnemy[model->nTagNum];
		}
		// TAB長押しでターゲット解除
		if (GetKeyboardPress(DIK_TAB) || enemySys->nActiveCount == 0)
		{
			model->nTagReleaseCount++;
			if (model->nTagReleaseCount > MODEL_TAG_COUNT)
			{
				model->bLockOn = false;
				// 0はプレイヤーにセット
				SetCameraTag(0);
				model->nTagReleaseCount = 0;
				// ターゲット解除時カメラモード変更
				SetCameraGameMode(CAMERA_NORMAL);
			}
		}
		else
		{
			model->nTagReleaseCount = 0;
		}
	}
}

//=============================================================================
// MP確認関数
//=============================================================================
bool CheckMagicModel(int nModel, int nMagic)
{
	MODEL *model = &modelWk[nModel];
	switch (nMagic + 1)
	{
	case BULLET_SPECIAL_FIRE:
		if (model->fStatusFire > MODEL_STATUS_FIRE_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_THUNDER:
		if (model->fStatusThunder > MODEL_STATUS_THUNDER_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_ICE:
		if (model->fStatusIce > MODEL_STATUS_ICE_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_WIND:
		if (model->fStatusWind > MODEL_STATUS_WIND_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_WATER:
		if (model->fStatusWater > MODEL_STATUS_WATER_SUB)
		{
			return true;
		}
		break;
	case BULLET_SPECIAL_EARTH:
		if (model->fStatusEarth > MODEL_STATUS_EARTH_SUB)
		{
			return true;
		}
		break;
	}
	return false;
}

//=============================================================================
// 通常攻撃関数
//=============================================================================
void AttackNormalModel(int nModel, int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];
	CAMERA *camera = GetCamera(CameraMode);
	D3DXVECTOR3 vecTemp = model->posModel + D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI)*MODEL_LENGTH_WEAPON, MODEL_HEIGHT_WEAPON, sinf(model->fHAngle + D3DX_PI)*MODEL_LENGTH_WEAPON);

	if (IsMobUseLeftPressed() || GetKeyboardPress(DIK_B) || IsButtonPressed(0, R_BUTTON_ZR))
	{	// ノーマルバレット発射
		if (model->nAttackNormalCount <= 0 && model->fStatusNormal > MODEL_STATUS_NORMAL_SUB)
		{	// ノーマルバレットのクールダウン確認
			model->nChangeLightPoint = 0;
			SetLight(LIGHT_POINT, TRUE);
			SetLightPoint(LIGHT_POINT, SetColorPallet(COLOR_PALLET_MAGENTA),
				vecTemp,
				30.0f, 0.03f);
			model->bAttack = true;
			// 視点→注視点のベクトルに交わるベクトルをセット
			D3DXVECTOR3 vecTag = camera->posCameraAt - camera->posCameraEye;
			D3DXVec3Normalize(&vecTag, &vecTag);
			SetBullet(
				camera->posCameraAt + vecTag * BULLET_TAG_VECTOR,
				vecTemp,
				SetColorPallet(COLOR_PALLET_MAGENTA),
				1.0f,
				BULLET_NORMAL);
			model->nAttackNormalCount = MODEL_ATTACK_NORMAL;
			model->fStatusNormal -= MODEL_STATUS_NORMAL_SUB;
		}
	}
	else
	{
		model->bAttack = false;
	}

	if (model->nChangeLightPoint < MODEL_LIGHT_POINT)
	{
		model->nChangeLightPoint++;
		//SetLightPoint(LIGHT_POINT, SetColorPallet(COLOR_PALLET_MAGENTA) / (float)model->nChangeLightPoint,
		//	vecTemp,
		//	40.0f, 0.03f);
	}
	else
	{
		SetLight(LIGHT_POINT, FALSE);
	}
}

//=============================================================================
// マジック関数
//=============================================================================
void AttackMagicModel(int nModel, int CameraMode, int CameraGameMode)
{
	MODEL *model = &modelWk[nModel];
	CAMERA *camera = GetCamera(CameraMode);

	// マジックサークル更新
	for (int j = 0; j < MAGICCIRCLE_MAX; j++)
	{
		if (model->magicCircle[j].bUse)
		{
			SetMagiccirclePos(model->magicCircle[j].nIdx, model->posModel, model->fHAngleDiff * MODEL_ROT_AUTO_SPEED);
		}
	}

	if (!CheckMagicModel(0, GetTypeMagic()))
	{
		// MPがない場合はノンチャージ
		model->nAttackSpCount++;
		if (model->nAttackSpCount > PLAYER_RECOIL_TIME)
		{
			model->bAttackSp = false;
			model->nAttackSpCount = 0;
		}
	}
	else if (GetKeyboardPress(DIK_V) || IsMobUseRightPressed() || IsButtonPressed(0, R_BUTTON_R))
	{	// マジックサークルチャージ
		if (model->nChargeCount % MODEL_STATUS_CHARGE_SPEED == 0 || model->nChargeCount == 0)
		{
			model->bAttackSp = false;
			//model->bAttackSp = true;
			for (int j = 0; j < MAGICCIRCLE_MAX; j++)
			{
				if (!model->magicCircle[j].bUse)
				{
					model->magicCircle[j].nIdx = SetMagiccircle();
					model->magicCircle[j].bUse = true;
					break;
				}
			}
		}
		model->nChargeCount++;
		model->nChangeLightSpot = 0;
		SetLight(LIGHT_SPOT, TRUE);
		SetLightSpot(LIGHT_SPOT, GetMagicColor() * ((float)model->nCharge / (float)MAGICCIRCLE_MAX),
			model->posModel + D3DXVECTOR3(0.0f, 100.0f,0.0f),
			D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 0.02f, 0.5f, 0.5f);
	}
	else if (GetKeyboardRelease(DIK_V) || IsMobUseRightReleased() || IsButtonReleased(0, R_BUTTON_R))
	{
		//if (model->nChangeLightSpot < MODEL_LIGHT_SPOT)
		//{
		//	model->nChangeLightSpot++;
		//	SetLightSpot(LIGHT_SPOT, GetMagicColor() / (float)model->nChangeLightSpot,
		//		model->posModel + D3DXVECTOR3(0.0f, 100.0f, 0.0f),
		//		D3DXVECTOR3(0.0f, -1.0f, 0.0f), 300.0f, 0.02f, 0.5f, 0.5f);
		//}
		//else
		//{
			SetLight(LIGHT_SPOT, FALSE);
		//}
		model->bAttackSp = true;
		model->nAttackSpCount = 0;
		//SetPlayerAnime(0, ANIME08);

		// マジックバレット発射
		switch (GetTypeMagic() + 1)
		{
		case BULLET_SPECIAL_FIRE:
			if (model->nAttackFireCount < 0 && model->fStatusFire > MODEL_STATUS_FIRE_SUB)
			{

				D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
				D3DXVec3Normalize(&vecTemp, &vecTemp);

				SetSpecialBulletFire(
					model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
					camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
					D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
					-1,
					model->nCharge);

				model->nAttackFireCount = MODEL_ATTACK_FIRE;
				model->fStatusFire -= MODEL_STATUS_FIRE_SUB;
				SetVoice(VOICE_FIRE, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_THUNDER:
			if (model->nAttackThunderCount < 0 && model->fStatusThunder > MODEL_STATUS_THUNDER_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletThunder(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletThunder(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackThunderCount = MODEL_ATTACK_THUNDER;
				model->fStatusThunder -= MODEL_STATUS_THUNDER_SUB;
				SetVoice(VOICE_THUNDER, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_ICE:
			if (model->nAttackIceCount < 0 && model->fStatusIce > MODEL_STATUS_ICE_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletIce(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletIce(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackIceCount = MODEL_ATTACK_ICE;
				model->fStatusIce -= MODEL_STATUS_ICE_SUB;
				SetVoice(VOICE_ICE, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_WIND:
			if (model->nAttackWindCount < 0 && model->fStatusWind > MODEL_STATUS_WIND_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				//int nLockonCount = 0;
				//for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				//{
					//if (lockon->bUse && !lockon->bRelease)
					//{
						SetSpecialBulletWind(
							model->posModel,
							D3DXVECTOR3(0.0f, 1.0f, 0.0f),
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							-1,
							model->nCharge);
						//nLockonCount++;
					//}
				//}
				//if (nLockonCount == 0)
				//{
				//	D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
				//	D3DXVec3Normalize(&vecTemp, &vecTemp);

				//	SetSpecialBulletWind(
				//		model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
				//		camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
				//		D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
				//		-1,
				//		model->nCharge);
				//}
				model->nAttackWindCount = MODEL_ATTACK_WIND;
				model->fStatusWind -= MODEL_STATUS_WIND_SUB;
				SetVoice(VOICE_WIND, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_WATER:
			// ウォーターバレットのクールダウン確認
			if (model->nAttackWaterCount < 0 && model->fStatusWater > MODEL_STATUS_WATER_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletWater(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletWater(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(camera->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackWaterCount = MODEL_ATTACK_WATER;
				model->fStatusWater -= MODEL_STATUS_WATER_SUB;
				SetVoice(VOICE_WATER, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		case BULLET_SPECIAL_EARTH:
			if (model->nAttackEarthCount < 0 && model->fStatusEarth > MODEL_STATUS_EARTH_SUB)
			{
				LOCKON *lockon = GetLockon(0);
				int nLockonCount = 0;
				for (int i = 0; i < LOCKON_MAX; i++, lockon++)
				{
					if (lockon->bUse && !lockon->bRelease)
					{
						SetSpecialBulletEarth(
							model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
							lockon->posLockon,
							D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
							i,
							model->nCharge);
						nLockonCount++;
					}
				}
				if (nLockonCount == 0)
				{
					D3DXVECTOR3 vecTemp = camera->posCameraAt - camera->posCameraEye;
					D3DXVec3Normalize(&vecTemp, &vecTemp);

					SetSpecialBulletEarth(
						model->posModel + D3DXVECTOR3(0.0f, MODEL_HEIGHT_WEAPON, 0.0f),
						camera->posCameraAt + vecTemp * BULLET_TAG_VECTOR,
						D3DXVECTOR3(cosf(model->fHAngle + D3DX_PI * 0.5f), 0.0f, sinf(model->fHAngle + D3DX_PI * 0.5f)),
						-1,
						model->nCharge);
				}
				model->nAttackEarthCount = MODEL_ATTACK_EARTH;
				model->fStatusEarth -= MODEL_STATUS_EARTH_SUB;
				SetVoice(VOICE_EARTH, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			break;
		}
		// マジックサークル初期化
		for (int j = 0; j < MAGICCIRCLE_MAX; j++)
		{
			model->magicCircle[j].bUse = false;
			ReleaseMagiccircle(j);
		}
		model->nChargeCount = 0;
		model->nCharge = 0;
		// マジックSE
		SetSe(SE_EXPL045, E_DS8_FLAG_NONE, CONTINUITY_ON);
	}
	else
	{
		model->nAttackSpCount++;
		if (model->nAttackSpCount > PLAYER_RECOIL_TIME)
		{
			model->bAttackSp = false;
			model->nAttackSpCount = 0;
		}
	}
}

//=============================================================================
// ジャンプ処理関数
//=============================================================================
void JumpModel(int nModel)
{
	MODEL *model = &modelWk[nModel];

	// 左シフトキー押下中は無重力+ブースト
	if ((GetKeyboardPress(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_R)) && !model->bStatusMPLimiter)
	{	// 左シフトを押下中かつMPリミッター無効時
		if (model->fStatusMP > MODEL_STATUS_MP_SUB)
		{
			if ((GetKeyboardTrigger(DIK_LSHIFT) || IsButtonTriggered(0, BUTTON_R)))
			{
				SetSe(SE_NOGRAVITY, E_DS8_FLAG_NONE, CONTINUITY_ON);
			}
			model->fMoveSpeed = MODEL_MOVE_BOOST;				// ブースト移動速度
			model->fJumpAccel = 0.0f;							// ジャンプアクセルを初期化
		}
	}
	else
	{														// 左シフトを押していない
		model->posModel.y -= GAME_GRAVITI;					// 重力をかける
		model->fMoveSpeed = MODEL_MOVE_SPEED;				// 通常移動速度を適用
		model->fJumpAccel -= MODEL_JUMP_POWER_DOWN;		// ジャンプアクセルに減算
		if (model->fJumpAccel < 0.0f)
		{
			model->fJumpAccel = 0.0f;
		}
		if (!model->bJump)
		{													// ジャンプフラグがfalseなら
			if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, R_BUTTON_B))
			{												// スペースキーが押されたら
				model->fJumpAccel = MODEL_JUMP_POWER;		// ジャンプパワーを適用
				model->bJump = true;						// ジャンプフラグを設定
				SetSe(10, E_DS8_FLAG_NONE, true);
			}
			else
			{
				//FloatModel(nModel);							// 浮遊処理
			}
		}
		else
		{
		}
	}

	// ジャンプをY軸に反映
	model->posModel.y += model->fJumpAccel;

	if (model->posModel.y < 0.0f)
	{												// モデルPOSが0.0fを下回ったら
		model->posModel.y = 0.0f;					// 0.0fを適用
		model->bJump = false;						// ジャンプフラグを解除
	}
}

//=============================================================================
// 浮遊処理関数
//=============================================================================
void FloatModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	model->fFloatAngle += MODEL_FLOAT_SPEED;	// 角度値を加算
	model->fFloatPos = MODEL_FLOAT_LENGTH		// sinカーブを計算
		* sinf(PiCalculate360(model->fFloatAngle));
	model->posModel.y += model->fFloatPos;		// モデルに加算
}

//=============================================================================
// マジック処理関数
//=============================================================================
void MagicRecoverModel(int nModel,int nMagic)
{
	MODEL *model = &modelWk[nModel];
	//if (model->modelMagic[nMagic].bUse)
	//{
	//	model->modelMagic[nMagic].fStatus += 0.0f;
	//}

	model->fStatusNormal += MODEL_STATUS_MAGIC_RECOVER;
	if (model->fStatusNormal > MODEL_STATUS_NORMAL) { model->fStatusNormal = MODEL_STATUS_NORMAL; };
	model->nAttackNormalCount--;

	if (model->bActiveFire)
	{
		model->fStatusFire += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusFire > MODEL_STATUS_FIRE) { model->fStatusFire = MODEL_STATUS_FIRE; };
		model->nAttackFireCount--;
	}

	if (model->bActiveThunder)
	{
		model->fStatusThunder += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusThunder > MODEL_STATUS_THUNDER) { model->fStatusThunder = MODEL_STATUS_THUNDER; };
		model->nAttackThunderCount--;
	}

	if (model->bActiveIce)
	{
		model->fStatusIce += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusIce > MODEL_STATUS_ICE) { model->fStatusIce = MODEL_STATUS_ICE; };
		model->nAttackIceCount--;
	}
	if (model->bActiveWind)
	{
		model->fStatusWind += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusWind > MODEL_STATUS_WIND) { model->fStatusWind = MODEL_STATUS_WIND; };
		model->nAttackWindCount--;
	}

	if (model->bActiveWater)
	{
		model->fStatusWater += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusWater > MODEL_STATUS_WARER) { model->fStatusWater = MODEL_STATUS_WARER; };
		model->nAttackWaterCount--;
	}
	if (model->bActiveEarth)
	{
		model->fStatusEarth += MODEL_STATUS_MAGIC_RECOVER;
		if (model->fStatusEarth > MODEL_STATUS_EARTH) { model->fStatusEarth = MODEL_STATUS_EARTH; };
		model->nAttackEarthCount--;
	}
}

//=============================================================================
// マジック処理関数
//=============================================================================
void SetMagicModel(int nModel, int nMagic)
{
	MODEL *model = &modelWk[nModel];
	switch (nMagic)
	{
	case BULLET_SPECIAL_FIRE:
		model->bActiveFire = true;
		model->fStatusFire = MODEL_STATUS_FIRE;
		break;
	case BULLET_SPECIAL_THUNDER:
		model->bActiveThunder = true;
		model->fStatusThunder = MODEL_STATUS_THUNDER;
		break;
	case BULLET_SPECIAL_ICE:
		model->bActiveIce = true;
		model->fStatusIce = MODEL_STATUS_ICE;
		break;
	case BULLET_SPECIAL_WIND:
		model->bActiveWind = true;
		model->fStatusWind = MODEL_STATUS_WIND;
		break;
	case BULLET_SPECIAL_WATER:
		model->bActiveWater = true;
		model->fStatusWater = MODEL_STATUS_WARER;
		break;
	case BULLET_SPECIAL_EARTH:
		model->bActiveEarth = true;
		model->fStatusEarth = MODEL_STATUS_EARTH;
		break;
	}
}

//=============================================================================
// マジックチャージ処理関数
//=============================================================================
void SetMagicChargeModel(int nModel)
{
	MODEL *model = &modelWk[nModel];
	model->nCharge++;
}

//=============================================================================
// 取得関数
//=============================================================================
MODEL *GetModel(int no)
{
	return(&modelWk[no]);
}