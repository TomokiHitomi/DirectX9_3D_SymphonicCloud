//=============================================================================
//
// モデル処理 [skydome.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "skydome.h"
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
LPDIRECT3DTEXTURE9	g_pD3DTextureSkydome;		// テクスチャへのポインタ

LPD3DXMESH			g_pD3DXMeshSkydome;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatSkydome;	// マテリアル情報へのポインタ
DWORD				g_nNumMatSkydome;			// マテリアル情報の数

SKYDOME				skydomeWk[SKYDOME_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkydome(int nType)
{
	SKYDOME *skydome = &skydomeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// 位置・回転・スケールの設定
	skydome->posSkydome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydome->rotSkydome = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skydome->sclSkydome = D3DXVECTOR3(SKYDOME_SCALE, SKYDOME_SCALE, SKYDOME_SCALE);

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureSkydome = NULL;
		g_pD3DXMeshSkydome = NULL;
		g_pD3DXBuffMatSkydome = NULL;
		g_nNumMatSkydome = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(SKYDOME_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatSkydome,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatSkydome,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshSkydome)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,				// デバイス
			SKYDOME_TEXTURE,	// ファイル名
			&g_pD3DTextureSkydome);		// 読み込むメモリ（複数なら配列に）
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkydome(void)
{
	if(g_pD3DTextureSkydome != NULL)
	{// テクスチャの開放
		g_pD3DTextureSkydome->Release();
		g_pD3DTextureSkydome = NULL;
	}

	if(g_pD3DXMeshSkydome != NULL)
	{// メッシュの開放
		g_pD3DXMeshSkydome->Release();
		g_pD3DXMeshSkydome = NULL;
	}

	if(g_pD3DXBuffMatSkydome != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatSkydome->Release();
		g_pD3DXBuffMatSkydome = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSkydome(void)
{
	SKYDOME *skydome = &skydomeWk[0];
	int pCameraMode = *GetCameraMode();
	CAMERA *camera = GetCamera(pCameraMode);

	if (pCameraMode == CAMERA_GAME)
	{
		skydome->posSkydome.x = camera->posCameraAt.x;
		skydome->posSkydome.y = camera->posCameraAt.y + SKYDOME_HEIGHT;
		skydome->posSkydome.z = camera->posCameraAt.z;
	}
	//skydome->rotSkydome.y += SKYDOME_ROT_SPEED;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkydome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	SKYDOME *skydome = &skydomeWk[0];

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&skydome->mtxWorld);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, skydome->sclSkydome.x, skydome->sclSkydome.y, skydome->sclSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, skydome->rotSkydome.y, skydome->rotSkydome.x, skydome->rotSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, skydome->posSkydome.x, skydome->posSkydome.y, skydome->posSkydome.z);
	D3DXMatrixMultiply(&skydome->mtxWorld, &skydome->mtxWorld, &mtxTranslate);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &skydome->mtxWorld);


	/******************** ビューポート変換 ********************/
	// 現在のマテリアルを保存
	pDevice->GetMaterial(&matDef);
	// マテリアル情報に対するポインタの取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatSkydome->GetBufferPointer();

	for (int i = 0; i < (int)g_nNumMatSkydome; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// テクスチャの設定（NULL:テクスチャ無し）
		pDevice->SetTexture(0, g_pD3DTextureSkydome);
		// 描画
		g_pD3DXMeshSkydome->DrawSubset(i);
	}


	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 取得関数
//=============================================================================
SKYDOME *GetSkydome(int no)
{
	return(&skydomeWk[no]);
}