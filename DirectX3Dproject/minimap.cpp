//=============================================================================
//
// MINIMAP処理 [minimap.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "minimap.h"
#include "model.h"
#include "enemy.h"
#include "main.h"
#include "calculate.h"
#include "pointer.h"
#include "magic.h"
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
HRESULT MakeVertexMinimap(int no);
void SetVertexMinimap(int no);
void SetDiffuseMinimap(int no, D3DXCOLOR color);
void SetTextureMinimap(int no, int cntPattern);
void SetChangeCollarMinimap(int no);
void SearchMinimap(void);
void UpdateMinimapPoint(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
MINIMAP					minimapWk[MINIMAP_MAX];
MINIMAP_SYS				minimapSysWk[MINIMAP_SYS_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureMinimap[MINIMAP_MAX];

D3DXVECTOR3				g_posMinimap;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMinimap(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[0];
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

	g_posMinimap = D3DXVECTOR3(TEXTURE_MINIMAP_POS_X, TEXTURE_MINIMAP_POS_Y, 0.0f);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_MINIMAP00,				// ファイルの名前
			&pD3DTextureMinimap[0]);			// 読み込むメモリのポインタ
												// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_MINIMAP01,				// ファイルの名前
			&pD3DTextureMinimap[1]);			// 読み込むメモリのポインタ
	}

	// MINIMAPの初期化
	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		minimap->bUse = true;
		minimap->pos = D3DXVECTOR3(TEXTURE_MINIMAP_POS_X, TEXTURE_MINIMAP_POS_Y, 0.0f);
		minimap->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (i == MINIMAP_BACK)
		{
			minimap->colorMinimap = D3DXCOLOR(1.0f, 1.0f, 1.0f, MINIMAP_ALPHA);
		}
		else
		{
			minimap->colorMinimap = GetMagicColor();
		}
		minimap->fBaseAngle = 0.0f;
		minimap->fRadius = 0.0f;

		minimap->nCollarFlag = 0;

		minimap->Texture = pD3DTextureMinimap[i];			// テクスチャへのMINIMAP
		minimap->size = D3DXVECTOR2(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y);

		// 頂点情報の作成
		MakeVertexMinimap(i);
	}

	for (int i = 0; i < MINIMAP_SYS_MAX; i++, minimapSys++)
	{
		minimapSys->bUse = false;
		minimapSys->nType = 0;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	for (int i = 0; i < MINIMAP_MAX; i++)
	{
		// メモリ解放
		if (pD3DTextureMinimap[i] != NULL)
		{
			pD3DTextureMinimap[i]->Release();
			pD3DTextureMinimap[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

#ifdef _DEBUG
	PrintDebugProc("【 MINIMAP 】\n");
#endif
	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		if (minimap->bUse)
		{
			if (i == MINIMAP_BACK)
			{	// バックのみ回転・色変更
				minimap->rot.z += 0.003f;				// 回転
				SetDiffuseMinimap(i, GetMagicColor());	// magicのカラーを取得して適用
			}
			SetVertexMinimap(i);						// 移動後の座標で頂点を設定
		}
	}

	SearchMinimap();
	UpdateMinimapPoint();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMinimap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MINIMAP_MAX; i++, minimap++)
	{
		if (minimap->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, minimap->Texture);

			// MINIMAPの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MINIMAP, minimap->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 表示対象検索処理
//=============================================================================
void SearchMinimap(void)
{
	MODEL *model = GetModel(0);
	ENEMY *enemy = GetEnemy(0);
	MINIMAP_SYS *minimapSys;

	// モデル
	for (int i = 0; i < MODEL_MAX; i++, model++)
	{	// 全てのモデルを確認
		if (model->bUse && !model->bUseMinimap)
		{	// ミニマップフラグがなければ
			minimapSys = &minimapSysWk[0];
			for (int j = 0; j < MINIMAP_SYS_MAX; j++, minimapSys++)
			{	// 未使用のマップシステムを探して
				if (!minimapSys->bUse)
				{	// セットアップする
					minimapSys->bUse = true;
					minimapSys->nType = MINIMAP_MODEL;
					minimapSys->nIndexTag = i;
					minimapSys->nIndexPointer = SetPointer(SetColorPallet(COLOR_PALLET_GREEN));
					model->bUseMinimap = true;
					break;
				}
			}
		}
	}

	// エネミー
	for (int i = 0; i < ENEMY_MAX; i++, enemy++)
	{	// 全てのエネミーを確認
		if (enemy->bUse && !enemy->bUseMinimap)
		{	// ミニマップフラグがなければ
			minimapSys = &minimapSysWk[0];
			for (int j = 0; j < MINIMAP_SYS_MAX; j++, minimapSys++)
			{	// 未使用のマップシステムを探して
				if (!minimapSys->bUse)
				{	// セットアップする
					minimapSys->bUse = true;
					minimapSys->nType = MINIMAP_ENEMY;
					minimapSys->nIndexTag = i;
					minimapSys->nIndexPointer = SetPointer(SetColorPallet(COLOR_PALLET_RED));
					enemy->bUseMinimap = true;
					break;
				}
			}
		}
	}
}

//=============================================================================
// ポイント更新処理
//=============================================================================
void UpdateMinimapPoint(void)
{
	MODEL *model;
	ENEMY *enemy;
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);
	MINIMAP_SYS *minimapSys = &minimapSysWk[0];

	D3DXVECTOR2 pos2Temp;		// 比較テンポラリ
	D3DXVECTOR3 pos3Temp;		// 座標
	float		fTempTa;		// 距離テンポラリ
	float		fTempAngle;		// 角度テンポラリ

	for (int i = 0; i < MINIMAP_SYS_MAX; i++, minimapSys++)
	{
		if (minimapSys->bUse)
		{	// 使われているミニマップシステムを探す
			switch (minimapSys->nType)
			{	// 対象がモデルかエネミーかで振り分け
			case MINIMAP_MODEL:
				model = GetModel(minimapSys->nIndexTag);
				if (model->bUse)
				{	// 使われていたら更新
					SetUpdatePointer(minimapSys->nIndexPointer, POINTER_TEXTURE_NORMAIL, g_posMinimap, 0.0f);
				}
				else
				{	// 使われていなかったらポインターを破棄
					ReleasePointer(minimapSys->nIndexPointer);
					minimapSys->bUse = false;			// ミニマップシステムを無破棄
				}
				break;
			case MINIMAP_ENEMY:
				enemy = GetEnemy(minimapSys->nIndexTag);
				if (enemy->bUse)
				{	// 使われていたら更新
					pos2Temp = D3DXVECTOR2(0.0f, 0.0f);						// 比較テンポラリを初期化
					fTempTa = 0.0f;											// 距離テンポラリを初期化
					fTempAngle = 0.0f;										// 角度テンポラリを初期化

					pos2Temp.x = (enemy->posEnemy.x - camera->posCameraAt.x) / MINIMAP_POS_CONVERT;	// X座標を算出
					pos2Temp.y = -((enemy->posEnemy.z - camera->posCameraAt.z) / MINIMAP_POS_CONVERT);	// Z座標をY座標に変換して算出

					fTempTa = D3DXVec2Length(&pos2Temp);							// 距離を算出

#ifdef _DEBUG
					PrintDebugProc("Minimap[%d]  TempTa:%f\n",i, fTempTa);
#endif
					if (pos2Temp.x == 0.0f && pos2Temp.y == 0.0f)		// atan2のエラー防止	
					{	// エラー防止修正
						pos2Temp.x += 0.00001f;
					}
					fTempAngle = atan2(pos2Temp.y, pos2Temp.x);			// 角度算出
					fTempAngle += camera->fHAngle - D3DX_PI * 1.5f;		// 算出した角度にカメラアングルを加算

					if (fTempTa <= MINIMAP_POS_LENGTH_MAX)
					{	// ミニマップ表示距離内であればポインターを更新
						pos3Temp = D3DXVECTOR3(								// 座標テンポラリに回転後の
							fTempTa * cosf(fTempAngle),						// X座標
							fTempTa * sinf(fTempAngle),						// Y座標
							0.0f);											// Z座標（0.0f)を格納
						SetUpdatePointer(minimapSys->nIndexPointer,			// ポインターの更新
							POINTER_TEXTURE_NORMAIL,
							g_posMinimap + pos3Temp,
							(enemy->rotEnemy.y + camera->fHAngle) - D3DX_PI * 0.5f);
					}
					else
					{	// ミニマップ表示距離外であればアウトラインを更新
						pos3Temp = D3DXVECTOR3(								// 座標テンポラリに回転後の
							MINIMAP_POS_LENGTH_MAX * cosf(fTempAngle),						// X座標
							MINIMAP_POS_LENGTH_MAX * sinf(fTempAngle),						// Y座標
							0.0f);											// Z座標（0.0f)を格納
						SetUpdatePointer(minimapSys->nIndexPointer,			// ポインターの更新
							POINTER_TEXTURE_OUTLINE,
							g_posMinimap + pos3Temp,
							(enemy->rotEnemy.y + camera->fHAngle) - D3DX_PI * 0.5f);
					}
				}
				else
				{	// 使われていなかったらポインターを破棄
					ReleasePointer(minimapSys->nIndexPointer);
					minimapSys->bUse = false;			// ミニマップシステムを無破棄
				}
				break;
			}
		}
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMinimap(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MINIMAP *minimap = &minimapWk[no];

	// 角度と半径を求める
	minimap->fBaseAngle = atan2f(TEXTURE_MINIMAP_SIZE_Y, TEXTURE_MINIMAP_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_MINIMAP_SIZE_X, TEXTURE_MINIMAP_SIZE_Y);
	minimap->fRadius = D3DXVec2Length(&temp);

	// 頂点座標の設定
	minimap->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	minimap->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MINIMAP_SIZE_X, 100.0f, 0.0f);
	minimap->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MINIMAP_SIZE_Y, 0.0f);
	minimap->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MINIMAP_SIZE_X, 100.0f + TEXTURE_MINIMAP_SIZE_Y, 0.0f);
	//SetVertexMinimap();

	// rhwの設定
	minimap->vertexWk[0].rhw =
	minimap->vertexWk[1].rhw =
	minimap->vertexWk[2].rhw =
	minimap->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	minimap->vertexWk[0].diffuse =
	minimap->vertexWk[1].diffuse =
	minimap->vertexWk[2].diffuse =
	minimap->vertexWk[3].diffuse = minimap->colorMinimap;

	// テクスチャ座標の設定
	minimap->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	minimap->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP, 0.0f);
	minimap->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP);
	minimap->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexMinimap(int no)
{
	MINIMAP *minimap = &minimapWk[no];
	// 頂点0:回転あり
	minimap->vertexWk[0].vtx.x = minimap->pos.x - cosf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[0].vtx.y = minimap->pos.y - sinf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	minimap->vertexWk[1].vtx.x = minimap->pos.x + cosf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[1].vtx.y = minimap->pos.y - sinf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	minimap->vertexWk[2].vtx.x = minimap->pos.x - cosf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[2].vtx.y = minimap->pos.y + sinf(minimap->fBaseAngle - minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	minimap->vertexWk[3].vtx.x = minimap->pos.x + cosf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[3].vtx.y = minimap->pos.y + sinf(minimap->fBaseAngle + minimap->rot.z) * minimap->fRadius;
	minimap->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseMinimap(int no, D3DXCOLOR color)
{
	MINIMAP *minimap = &minimapWk[no];

	// 反射光の設定
	minimap->vertexWk[0].diffuse =
	minimap->vertexWk[1].diffuse =
	minimap->vertexWk[2].diffuse =
	minimap->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureMinimap(int no, int cntPattern)
{
	MINIMAP *minimap = &minimapWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MINIMAP;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MINIMAP;
	minimap->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	minimap->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	minimap->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	minimap->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarMinimap(int no)
{
	MINIMAP *minimap = &minimapWk[no];

	switch (minimap->nCollarFlag)
	{
	case 0:
		minimap->colorMinimap -= D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		if (minimap->colorMinimap.r <= 0.0f)
		{
			minimap->colorMinimap.r = 0.0f;
			minimap->nCollarFlag++;
		}
		if (minimap->colorMinimap.g >= 1.0f)
		{
			minimap->colorMinimap.g = 1.0f;
		}
		if (minimap->colorMinimap.b >= 1.0f)
		{
			minimap->colorMinimap.b = 1.0f;
		}
		break;
	case 1:
		minimap->colorMinimap -= D3DXCOLOR(0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		if (minimap->colorMinimap.r >= 1.0f)
		{
			minimap->colorMinimap.r = 1.0f;
		}
		if (minimap->colorMinimap.g <= 0.0f)
		{
			minimap->colorMinimap.g = 0.0f;
			minimap->nCollarFlag++;
		}
		if (minimap->colorMinimap.b >= 1.0f)
		{
			minimap->colorMinimap.b = 1.0f;
		}
		break;
	case 2:
		minimap->colorMinimap -= D3DXCOLOR(0.0f, 0.0f, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f);
		minimap->colorMinimap += D3DXCOLOR(TEXTURE_MINIMAP_CHANGE_COLLAR, TEXTURE_MINIMAP_CHANGE_COLLAR, 0.0f, 0.0f);
		if (minimap->colorMinimap.r >= 1.0f)
		{
			minimap->colorMinimap.r = 1.0f;
		}
		if (minimap->colorMinimap.g >= 1.0f)
		{
			minimap->colorMinimap.g = 1.0f;
		}
		if (minimap->colorMinimap.b <= 0.0f)
		{
			minimap->colorMinimap.b = 0.0f;
			minimap->nCollarFlag = 0;
		}
		break;
	}
}

//=============================================================================
// 有効設定
//=============================================================================
void SetMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	minimap->bUse = true;
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseMinimap(void)
{
	MINIMAP *minimap = &minimapWk[0];
	minimap->bUse = false;
}

//=============================================================================
// MINIMAP取得関数
//=============================================================================
MINIMAP *GetMinimap(int no)
{
	return(&minimapWk[no]);
}

