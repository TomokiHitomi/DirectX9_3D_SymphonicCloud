//=============================================================================
//
// モデル処理 [skydomeeffect.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "skydomeeffect.h"
#include "main.h"
#include "input.h"
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

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureSkydomeeffect;		// テクスチャへのポインタ

LPD3DXMESH			g_pD3DXMeshSkydomeeffect;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatSkydomeeffect;	// マテリアル情報へのポインタ
DWORD				g_nNumMatSkydomeeffect;			// マテリアル情報の数

SKYDOMEEFFECT				skydomeeffectWk[SKYDOMEEFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkydomeeffect(int nType)
{
	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// 位置・回転・スケールの設定
	skydomeeffect->posSkydomeeffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydomeeffect->rotSkydomeeffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydomeeffect->sclSkydomeeffect = D3DXVECTOR3(SKYDOMEEFFECT_SCALE, SKYDOMEEFFECT_SCALE, SKYDOMEEFFECT_SCALE);

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureSkydomeeffect = NULL;
		g_pD3DXMeshSkydomeeffect = NULL;
		g_pD3DXBuffMatSkydomeeffect = NULL;
		g_nNumMatSkydomeeffect = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(SKYDOMEEFFECT_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatSkydomeeffect,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatSkydomeeffect,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshSkydomeeffect)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイス
			SKYDOMEEFFECT_TEXTURE,	// ファイル名
			&g_pD3DTextureSkydomeeffect);		// 読み込むメモリ（複数なら配列に）
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkydomeeffect(void)
{
	if(g_pD3DTextureSkydomeeffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureSkydomeeffect->Release();
		g_pD3DTextureSkydomeeffect = NULL;
	}

	if(g_pD3DXMeshSkydomeeffect != NULL)
	{// メッシュの開放
		g_pD3DXMeshSkydomeeffect->Release();
		g_pD3DXMeshSkydomeeffect = NULL;
	}

	if(g_pD3DXBuffMatSkydomeeffect != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatSkydomeeffect->Release();
		g_pD3DXBuffMatSkydomeeffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSkydomeeffect(void)
{
	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];
	int pCameraMode = *GetCameraMode();
	CAMERA *camera = GetCamera(pCameraMode);
	if (pCameraMode == CAMERA_GAME)
	{
		skydomeeffect->posSkydomeeffect.x = camera->posCameraAt.x;
		skydomeeffect->posSkydomeeffect.y = SKYDOMEEFFECT_HEIGHT;
		skydomeeffect->posSkydomeeffect.z = camera->posCameraAt.z;
	}
	skydomeeffect->rotSkydomeeffect.y += SKYDOMEEFFECT_ROT_SPEED;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkydomeeffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	SKYDOMEEFFECT *skydomeeffect = &skydomeeffectWk[0];

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&skydomeeffect->mtxWorld);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, skydomeeffect->sclSkydomeeffect.x, skydomeeffect->sclSkydomeeffect.y, skydomeeffect->sclSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, skydomeeffect->rotSkydomeeffect.y, skydomeeffect->rotSkydomeeffect.x, skydomeeffect->rotSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, skydomeeffect->posSkydomeeffect.x, skydomeeffect->posSkydomeeffect.y, skydomeeffect->posSkydomeeffect.z);
	D3DXMatrixMultiply(&skydomeeffect->mtxWorld, &skydomeeffect->mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &skydomeeffect->mtxWorld);


	/******************** ビューポート変換 ********************/
	// 現在のマテリアルを保存
	pDevice->GetMaterial(&matDef);
	// マテリアル情報に対するポインタの取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatSkydomeeffect->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatSkydomeeffect; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// テクスチャの設定（NULL:テクスチャ無し）
		pDevice->SetTexture(0, g_pD3DTextureSkydomeeffect);
		// 描画
		g_pD3DXMeshSkydomeeffect->DrawSubset(i);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 取得関数
//=============================================================================
SKYDOMEEFFECT *GetSkydomeeffect(int no)
{
	return(&skydomeeffectWk[no]);
}