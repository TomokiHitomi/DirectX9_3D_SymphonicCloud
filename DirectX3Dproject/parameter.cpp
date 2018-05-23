//=============================================================================
//
// パラメータ処理 [parameter.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "enemy.h"
#include "parameter.h"
#include "sound.h"
#include "camera.h"

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
HRESULT MakeVertexParameter(LPDIRECT3DDEVICE9 pDevice);
void SetVtxParameter(int nParameter, float fSizeX, float fSizeY);
void SetDiffuseParameter(int nParameter, D3DXCOLOR col);
void SetTexParameter(int nParameter, int nTex);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PARAMETER				parameterWk[PARAMETER_MAX];

LPDIRECT3DTEXTURE9		g_pD3DTextureParameter = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParameter = NULL;	// 頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldParameter;			// ワールドマトリックス


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParameter(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARAMETER *parameter = &parameterWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_PARAMETER00,				// ファイルの名前
			&g_pD3DTextureParameter);			// 読み込むメモリのポインタ
	}

	// パラメータの初期化
	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{

		parameter->Texture = g_pD3DTextureParameter;			// テクスチャへのエネミー
		parameter->size = D3DXVECTOR2(TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y);
		parameter->vecChangeSize = D3DXVECTOR3(TEXTURE_CHANGE_PARAMETER_X, TEXTURE_CHANGE_PARAMETER_Y, 0.0f);

		InitStatusParameter(i);
	}

	// 頂点情報の作成
	MakeVertexParameter(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParameter(void)
{
	PARAMETER *parameter = &parameterWk[0];

	// メモリ解放
	if (g_pD3DTextureParameter != NULL)
	{
		g_pD3DTextureParameter->Release();
		g_pD3DTextureParameter = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParameter(void)
{
	PARAMETER *parameter = &parameterWk[0];
	MODEL *model;
	ENEMY *enemy;

	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (parameter->bUse)
		{
			parameter->nPatternAnim = parameter->nType;
			switch (parameter->nType)
			{
			case 0:
				model = GetModel(parameter->nTarget);
				parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)MODEL_STATUS_HP * model->fStatusHP;
				parameter->posParameter.x = model->posModel.x;
				parameter->posParameter.y = model->posModel.y + PARAMETER_HEIGHT;
				parameter->posParameter.z = model->posModel.z;
				if (!model->bUse)
				{
					InitStatusParameter(i);
				}
				break;
			case 1:
				model = GetModel(parameter->nTarget);
				parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)MODEL_STATUS_HP * model->fStatusHP;
				parameter->posParameter.x = model->posModel.x;
				parameter->posParameter.y = model->posModel.y + PARAMETER_HEIGHT;
				parameter->posParameter.z = model->posModel.z;
				if (!model->bUse)
				{
					InitStatusParameter(i);
				}
				break;
			case 2:
				enemy = GetEnemy(parameter->nTarget);
				switch (enemy->nModel)
				{
				case ENEMY_TYPE_NORMAL:
					parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)ENEMY_STATUS_HP * enemy->fStatusHP;
					parameter->posParameter.x = enemy->posEnemy.x;
					parameter->posParameter.y = enemy->posEnemy.y + PARAMETER_HEIGHT;
					parameter->posParameter.z = enemy->posEnemy.z;
					break;
				case ENEMY_TYPE_BOSS:
					parameter->fPointGauge = (float)TEXTURE_PARAMETER00_SIZE_X / (float)ENEMY_STATUS_BOSS_HP * enemy->fStatusHP;
					parameter->posParameter.x = enemy->posEnemy.x;
					parameter->posParameter.y = enemy->posEnemy.y + PARAMETER_HEIGHT;
					parameter->posParameter.z = enemy->posEnemy.z;
					break;
				}
				if (!enemy->bUse)
				{
					InitStatusParameter(i);
				}
			}
#ifdef _DEBUG
			PrintDebugProc("para[%d]  pos[X:%f  Y:%f  Z:%f] Gauge[%f]\n", i, parameter->posParameter.x, parameter->posParameter.y, parameter->posParameter.z, parameter->fPointGauge);
#endif

			// テクスチャ座標を設定
			SetTexParameter(i, parameter->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVtxParameter(i, parameter->posParameter.x, parameter->posParameter.y);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParameter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	PARAMETER *parameter = &parameterWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (parameter->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParameter);

			// ビューマトリックスを取得
			mtxView = GetMtxView();
			// ビルボード化
			g_mtxWorldParameter._11 = mtxView._11;
			g_mtxWorldParameter._12 = mtxView._21;
			g_mtxWorldParameter._13 = mtxView._31;
			g_mtxWorldParameter._21 = mtxView._12;
			g_mtxWorldParameter._22 = mtxView._22;
			g_mtxWorldParameter._23 = mtxView._32;
			g_mtxWorldParameter._31 = mtxView._13;
			g_mtxWorldParameter._32 = mtxView._23;
			g_mtxWorldParameter._33 = mtxView._33;

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, parameter->sclParameter.x, parameter->sclParameter.y, parameter->sclParameter.z);
			D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bullet->rotParameter.y, bullet->rotParameter.x, bullet->rotParameter.z);
			//D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &GetInvRotateMat(bullet->posParameter));
			////D3DXMatrixMultiply(&bullet->mtxWorld, &bullet->mtxWorld, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, parameter->posParameter.x, parameter->posParameter.y, parameter->posParameter.z);
			D3DXMatrixMultiply(&g_mtxWorldParameter, &g_mtxWorldParameter, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParameter);


			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParameter, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureParameter);

			// ポリゴンの描画
			//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, bullet->vertexWK, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexParameter(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * PARAMETER_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffParameter,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < PARAMETER_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-TEXTURE_PARAMETER00_SIZE_X, -TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TEXTURE_PARAMETER00_SIZE_X, -TEXTURE_PARAMETER00_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TEXTURE_PARAMETER00_SIZE_X, TEXTURE_PARAMETER00_SIZE_Y, 0.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PARAMETER;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffParameter->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxParameter(int nParameter, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		PARAMETER *parameter = &parameterWk[nParameter];

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2),
			-TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[1].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2),
			TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[2].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2) + parameter->fPointGauge * parameter->vecChangeSize.x,
			- TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);
		pVtx[3].vtx = D3DXVECTOR3(
			-(TEXTURE_PARAMETER00_SIZE_X / 2) + parameter->fPointGauge * parameter->vecChangeSize.x,
			TEXTURE_PARAMETER00_SIZE_Y,
			0.0f);


		//pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		//pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		//pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		//pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseParameter(int nParameter, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexParameter(int nParameter, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParameter->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nParameter * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_PARAMETER;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_PARAMETER;

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParameter->Unlock();
	}
}

//=============================================================================
// パラメータのセット
//=============================================================================
void SetParameter(int nType, D3DXVECTOR3 vecTargetPos, int nTarget)
{
	PARAMETER *parameter = &parameterWk[0];

	// 未使用のパラメーターを探す
	for (int i = 0; i < PARAMETER_MAX; i++, parameter++)
	{
		if (!parameter->bUse)
		{
			// パラメータを有効にする
			parameter->bUse = true;
			// パラメータタイプを設定
			parameter->nType = nType;
			// パラメータ設置対象を記憶
			parameter->vecTargetPos = vecTargetPos;
			// パラメータターゲットを設定
			parameter->nTarget = nTarget;
			// テクスチャ座標を設定
			SetTexParameter(i, parameter->nPatternAnim);
			// 移動後の座標で頂点を設定
			SetVtxParameter(i, parameter->posParameter.x, parameter->posParameter.y);
			return;
		}
	}
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusParameter(int nParameter)
{
	PARAMETER *parameter = &parameterWk[nParameter];

	// パラメータステータスの初期化
	parameter->bUse = false;

	parameter->posParameter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	parameter->sclParameter = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	parameter->sclParameter = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	parameter->vecTargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	parameter->nCountAnim = 0;
	parameter->nPatternAnim = 0;
	parameter->nTarget = 0;
	parameter->nType = 0;

	parameter->fPointGauge = 0.0f;
	return;}

//=============================================================================
// パラメータ取得関数
//=============================================================================
PARAMETER *GetParameter(int no)
{
	return(&parameterWk[no]);
}

