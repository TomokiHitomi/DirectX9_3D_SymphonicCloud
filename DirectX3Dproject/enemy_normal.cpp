//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : GP12A295 25 人見友基 
//
//=============================================================================
#include "enemy.h"
#include "enemy_normal.h"
#include "main.h"
#include "calculate.h"
#include "magic.h"
#include "calculate.h"
#include "bullet.h"

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
LPDIRECT3DTEXTURE9	*g_pD3DTextureEnemy_normal;		// テクスチャへのポインタ
D3DMATERIAL9		*g_pD3DMatMeshEnemy_normal;

LPD3DXMESH			g_pD3DXMeshEnemy_normal;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatEnemy_normal;	// マテリアル情報へのポインタ
DWORD				g_nNumMatEnemy_normal;			// マテリアル情報の数
D3DXMATRIX			g_mtxWorldEnemy_normal;			// ワールドマトリックス

ENEMY_NORMAL		enemy_normalWk[ENEMY_NORMAL_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy_normal(int nType)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		enemy_normal->bUse = false;
		enemy_normal->posEnemy = D3DXVECTOR3(0.0, 0.0f, 0.0f);
		enemy_normal->rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy_normal->sclEnemy = D3DXVECTOR3(ENEMY_NORMAL_SCALE, ENEMY_NORMAL_SCALE, ENEMY_NORMAL_SCALE);
	}

	if (nType == 0)
	{
		// モデル関係の初期化
		g_pD3DTextureEnemy_normal = NULL;
		g_pD3DXMeshEnemy_normal = NULL;
		g_pD3DXBuffMatEnemy_normal = NULL;
		g_nNumMatEnemy_normal = 0;

		// モデルデータ読み込み
		if (FAILED(D3DXLoadMeshFromX(ENEMY_NORMAL_MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,			// 使用するメモリのオプション
			pDevice,					// デバイス
			NULL,						// 未使用
			&g_pD3DXBuffMatEnemy_normal,		// マテリアルデータ
			NULL,						// 未使用
			&g_nNumMatEnemy_normal,			// D3DXMATERIAL構造体の数
			&g_pD3DXMeshEnemy_normal)))		// メッシュデータへのポインタ
		{
			return E_FAIL;
		}

		// マテリアル情報を取り出す
		D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_normal->GetBufferPointer();
		g_pD3DMatMeshEnemy_normal = new D3DMATERIAL9[g_nNumMatEnemy_normal];		// メッシュ情報を確保
		g_pD3DTextureEnemy_normal = new LPDIRECT3DTEXTURE9[g_nNumMatEnemy_normal];// テクスチャを確保
		for (int i = 0; i < g_nNumMatEnemy_normal; i++)
		{
			g_pD3DMatMeshEnemy_normal[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
			g_pD3DMatMeshEnemy_normal[i].Ambient = g_pD3DMatMeshEnemy_normal[i].Diffuse;// 環境光初期化
			g_pD3DTextureEnemy_normal[i] = NULL;	// テクスチャ初期化

			// 使用しているテクスチャがあれば読み込む
			if (d3Mat[i].pTextureFilename != NULL &&
				lstrlen(d3Mat[i].pTextureFilename) > 0)
			{
				// テクスチャ読み込み
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					d3Mat[i].pTextureFilename,
					&g_pD3DTextureEnemy_normal[i])))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy_normal(void)
{
	for (int i = 0; i < g_nNumMatEnemy_normal; i++)
	{	// テクスチャの開放
		if (g_pD3DTextureEnemy_normal[i] != NULL)
		{
			g_pD3DTextureEnemy_normal[i]->Release();
			g_pD3DTextureEnemy_normal[i] = NULL;
		}
	}

	if(g_pD3DXMeshEnemy_normal != NULL)
	{// メッシュの開放
		g_pD3DXMeshEnemy_normal->Release();
		g_pD3DXMeshEnemy_normal = NULL;
	}

	if(g_pD3DXBuffMatEnemy_normal != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatEnemy_normal->Release();
		g_pD3DXBuffMatEnemy_normal = NULL;
	}

	delete[]g_pD3DMatMeshEnemy_normal;
	delete[]g_pD3DTextureEnemy_normal;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy_normal(void)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (enemy_normal->bUse)
		{
			// モデル回転
			enemy_normal->rotEnemy.x += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.y += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.z += ENEMY_NORMAL_ROT;
			enemy_normal->rotEnemy.x = PiCalculate180(enemy_normal->rotEnemy.y);
			enemy_normal->rotEnemy.y = PiCalculate180(enemy_normal->rotEnemy.y);
			enemy_normal->rotEnemy.z = PiCalculate180(enemy_normal->rotEnemy.y);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy_normal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (enemy_normal->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEnemy_normal);

			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, enemy_normal->sclEnemy.x, enemy_normal->sclEnemy.y, enemy_normal->sclEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxScl);

			// 【R】回1転を反映(YawPitchRollはy,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy_normal->rotEnemy.y, enemy_normal->rotEnemy.x, enemy_normal->rotEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, enemy_normal->posEnemy.x, enemy_normal->posEnemy.y, enemy_normal->posEnemy.z);
			D3DXMatrixMultiply(&g_mtxWorldEnemy_normal, &g_mtxWorldEnemy_normal, &mtxTranslate);

			// ワールドマトリクスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEnemy_normal);

			/******************** ビューポート変換 ********************/
			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);
			//// マテリアル情報に対するポインタの取得
			//pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy_normal->GetBufferPointer();

			for (int j = 0; j < g_nNumMatEnemy_normal; j++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&g_pD3DMatMeshEnemy_normal[j]);
				// テクスチャの設定（NULL:テクスチャ無し）
				pDevice->SetTexture(0, g_pD3DTextureEnemy_normal[j]);
				// 描画
				g_pD3DXMeshEnemy_normal->DrawSubset(j);
			}
			// マテリアルを元に戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 出現処理
//=============================================================================
int SetEnemy_normal(void)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[0];

	for (int i = 0; i < ENEMY_NORMAL_MAX; i++, enemy_normal++)
	{
		if (!enemy_normal->bUse)
		{
			enemy_normal->bUse = true;
			return i;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void SetEnemy_normalPos(int nIndex, D3DXVECTOR3 pos)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[nIndex];
	enemy_normal->posEnemy = pos;
	enemy_normal->posEnemy.y += ENEMY_BULLET_MUZZELE_HEIGHT;
}

//=============================================================================
// カラー設定処理
//=============================================================================
void SetEnemy_normalColor(int nColor)
{
	D3DXCOLOR colorTemp = (1.0f, 1.0f, 1.0f, 1.0f);
	switch(nColor)
	{
	case BULLET_SPECIAL_FIRE:
		colorTemp = SetColorPallet(COLOR_PALLET_ORANGE);
		break;
	case BULLET_SPECIAL_THUNDER:
		colorTemp = SetColorPallet(COLOR_PALLET_YELLOW);
		break;
	case BULLET_SPECIAL_ICE:
		colorTemp = SetColorPallet(COLOR_PALLET_CYAN);
		break;
	case BULLET_SPECIAL_WIND:
		colorTemp = SetColorPallet(COLOR_PALLET_LIGHTGREEN);
		break;
	case BULLET_SPECIAL_WATER:
		colorTemp = SetColorPallet(COLOR_PALLET_BLUE);
		break;
	case BULLET_SPECIAL_EARTH:
		colorTemp = SetColorPallet(COLOR_PALLET_BROWN);
		break;
	}
	g_pD3DMatMeshEnemy_normal->Diffuse = (D3DCOLORVALUE)colorTemp;
	g_pD3DMatMeshEnemy_normal->Ambient = (D3DCOLORVALUE)(colorTemp * 0.5f);
	//g_pD3DMatMeshEnemy_normal->Specular = (D3DCOLORVALUE)colorTemp;
	//colorTemp = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	//g_pD3DMatMeshEnemy_normal->Emissive = (D3DCOLORVALUE)colorTemp;
	//g_pD3DMatMeshEnemy_normal->Power = 1.0f;
}

//=============================================================================
// 解放処理
//=============================================================================
void ReleaseEnemy_normal(int nIndex)
{
	ENEMY_NORMAL *enemy_normal = &enemy_normalWk[nIndex];
	enemy_normal->bUse = false;
}

//=============================================================================
// 取得関数
//=============================================================================
ENEMY_NORMAL *GetEnemy_normal(int no)
{
	return(&enemy_normalWk[no]);
}
