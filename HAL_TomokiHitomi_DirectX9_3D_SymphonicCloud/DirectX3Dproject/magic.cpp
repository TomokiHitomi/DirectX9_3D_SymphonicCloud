//=============================================================================
//
// MAGIC処理 [magic.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "magic.h"
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "calculate.h"
#include "gage.h"
#include "magiceffect.h"
#include "model.h"
#include "gageselect.h"
#include "sound.h"

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
HRESULT MakeVertexMagic(int no);
void SetVertexMagic(int no);
void SetDiffuseMagic(int no, D3DXCOLOR color);
void SetTextureMagic(int no, int cntPattern);
void SetChangeCollarMagic(int no);
void SetMagicBgm(void);
void ChangeMagic(int nMagic);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
MAGIC					magicWk[MAGIC_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureMagic = NULL;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMagic(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[0];

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_MAGIC,				// ファイルの名前
			&pD3DTextureMagic);			// 読み込むメモリのポインタ
	}

	// MAGICの初期化
	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		magic->bUse = true;
		if (i == MAGIC_MAIN)	// メインマジック処理
		{
			magic->pos = D3DXVECTOR3(TEXTURE_MAGIC_POS_X, TEXTURE_MAGIC_POS_Y, 0.0f);
			magic->rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
		}
		if (i == MAGIC_DUMMY)	// ダミーマジック01処理
		{
			magic->pos = D3DXVECTOR3(TEXTURE_MAGIC_DUMMY_POS_X, TEXTURE_MAGIC_DUMMY_POS_Y, 0.0f);
			magic->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		magic->colorMagic = SetColorPallet(COLOR_PALLET_GRAY);
		magic->fBaseAngle = 0.0f;
		magic->fRadius = 0.0f;
		magic->fTypeAngle = (D3DX_PI * 2) / (BULLET_TYPE_MAX - 1);

		magic->bActiveFire = false;
		magic->bActiveThunder = false;
		magic->bActiveIce = false;
		magic->bActiveWind = false;
		magic->bActiveWater = false;
		magic->bActiveEarth = false;

		magic->nColorFlag = 0;
		magic->nType = 0;
		magic->nCoolDown = 0;
		magic->bActive = false;
		magic->bActiveColor = false;
		magic->bSelectCheck = false;


		magic->Texture = pD3DTextureMagic;			// テクスチャへのMAGIC
		magic->size = D3DXVECTOR2(TEXTURE_MAGIC_SIZE_X, TEXTURE_MAGIC_SIZE_Y);

		// 頂点情報の作成
		MakeVertexMagic(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagic(void)
{
	MAGIC *magic = &magicWk[0];

	// メモリ解放
	if (pD3DTextureMagic != NULL)
	{
		pD3DTextureMagic->Release();
		pD3DTextureMagic = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagic(void)
{
	MAGIC *magic = &magicWk[0];

#ifdef _DEBUG
	PrintDebugProc("【 MAGIC 】\n");
#endif
	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		if (magic->bUse)
		{
			if (i == MAGIC_MAIN)	// メインマジック処理
			{
				SetMagicBgm();		// BGM制御
			}
			if (magic->bActive)
			{
				if (i == MAGIC_MAIN)	// メインマジック処理
				{
					ChangeMagic(i);		// スキル変更
					// 回転先と現在の角度を計算
				

					magic->fGoalAngle = (magic->nType * magic->fTypeAngle - D3DX_PI * 0.5f) - magic->rot.z;
					magic->fGoalAngle = PiCalculate180(magic->fGoalAngle);
					// 角度差を適用
					magic->rot.z += magic->fGoalAngle * MAGIC_ROT_INERTIA;
					magic->rot.z = PiCalculate180(magic->rot.z);
				}
				if (i == MAGIC_DUMMY)	// ダミーマジック処理
				{
					magic->colorMagic = magicWk[MAGIC_MAIN].colorMagic;
					magic->rot.z = magicWk[MAGIC_MAIN].rot.z + D3DX_PI * 0.5f;
				}


				SetChangeCollarMagic(i);					// カラー変更処理
				SetDiffuseMagic(i, magic->colorMagic);		// カラー適用処理
			}
			SetVertexMagic(i);							// 移動後の座標で頂点を設定
#ifdef _DEBUG
			PrintDebugProc("MagicType[%d]  RotZ:%f  Goal:%f\n", magic->nType, magic->rot.z, magic->fGoalAngle);
#endif
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		if (magic->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, magic->Texture);

			// MAGICの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_MAGIC, magic->vertexWk, sizeof(VERTEX_2D));
		}
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexMagic(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MAGIC *magic = &magicWk[no];

	// 角度と半径を求める
	magic->fBaseAngle = atan2f(TEXTURE_MAGIC_SIZE_Y, TEXTURE_MAGIC_SIZE_X);
	D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_MAGIC_SIZE_X, TEXTURE_MAGIC_SIZE_Y);
	magic->fRadius = D3DXVec2Length(&temp);

	// 頂点座標の設定
	magic->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	magic->vertexWk[1].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGIC_SIZE_X, 100.0f, 0.0f);
	magic->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + TEXTURE_MAGIC_SIZE_Y, 0.0f);
	magic->vertexWk[3].vtx = D3DXVECTOR3(100.0f + TEXTURE_MAGIC_SIZE_X, 100.0f + TEXTURE_MAGIC_SIZE_Y, 0.0f);
	//SetVertexMagic();

	// rhwの設定
	magic->vertexWk[0].rhw =
	magic->vertexWk[1].rhw =
	magic->vertexWk[2].rhw =
	magic->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	magic->vertexWk[0].diffuse =
	magic->vertexWk[1].diffuse =
	magic->vertexWk[2].diffuse =
	magic->vertexWk[3].diffuse = magic->colorMagic;

	// テクスチャ座標の設定
	magic->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	magic->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC, 0.0f);
	magic->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC);
	magic->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexMagic(int no)
{
	MAGIC *magic = &magicWk[no];
	// 頂点0:回転あり
	magic->vertexWk[0].vtx.x = magic->pos.x - cosf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[0].vtx.y = magic->pos.y - sinf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[0].vtx.z = 0.0f;

	// 頂点1:回転あり
	magic->vertexWk[1].vtx.x = magic->pos.x + cosf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[1].vtx.y = magic->pos.y - sinf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[1].vtx.z = 0.0f;

	// 頂点2:回転あり
	magic->vertexWk[2].vtx.x = magic->pos.x - cosf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[2].vtx.y = magic->pos.y + sinf(magic->fBaseAngle + magic->rot.z) * magic->fRadius;
	magic->vertexWk[2].vtx.z = 0.0f;

	// 頂点3:回転あり
	magic->vertexWk[3].vtx.x = magic->pos.x + cosf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[3].vtx.y = magic->pos.y + sinf(magic->fBaseAngle - magic->rot.z) * magic->fRadius;
	magic->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseMagic(int no, D3DXCOLOR color)
{
	MAGIC *magic = &magicWk[no];

	// 反射光の設定
	magic->vertexWk[0].diffuse =
	magic->vertexWk[1].diffuse =
	magic->vertexWk[2].diffuse =
	magic->vertexWk[3].diffuse = color;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureMagic(int no, int cntPattern)
{
	MAGIC *magic = &magicWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_MAGIC;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_MAGIC;
	magic->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	magic->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	magic->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	magic->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// カラー変更関数
//=============================================================================
void SetChangeCollarMagic(int no)
{
	MAGIC *magic = &magicWk[no];

	// 選択中のマジックのMPがあるか確認
	if (CheckMagicModel(0, magic->nType))
	{	// MPがあれば該当色にカラーチェンジ
		switch (magic->nType + 1)
		{
		case BULLET_SPECIAL_FIRE:
			magic->nColorFlag = COLOR_PALLET_ORANGE;
			break;
		case BULLET_SPECIAL_THUNDER:
			magic->nColorFlag = COLOR_PALLET_YELLOW;
			break;
		case BULLET_SPECIAL_ICE:
			magic->nColorFlag = COLOR_PALLET_CYAN;
			break;
		case BULLET_SPECIAL_WIND:
			magic->nColorFlag = COLOR_PALLET_LIGHTGREEN;
			break;
		case BULLET_SPECIAL_WATER:
			magic->nColorFlag = COLOR_PALLET_BLUE;
			break;
		case BULLET_SPECIAL_EARTH:
			magic->nColorFlag = COLOR_PALLET_BROWN;
			break;
		}
	}
	else
	{	// MPがなければグレーアウト
		magic->nColorFlag = COLOR_PALLET_GRAY;
	}
	
	// 赤色処理
	if (magic->colorMagic.r > SetColorPallet(magic->nColorFlag).r)
	{	// セットカラーより高い場合は減算
		magic->colorMagic.r -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.r < 0.0f)
		{
			magic->colorMagic.r = 0.0f;
		}
	}
	else if (magic->colorMagic.r < SetColorPallet(magic->nColorFlag).r)
	{	// セットカラーより低い場合は減算
		magic->colorMagic.r += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.r > 1.0f)
		{
			magic->colorMagic.r = 1.0f;
		}
	}
	// 緑色処理
	if (magic->colorMagic.g > SetColorPallet(magic->nColorFlag).g)
	{	// セットカラーより高い場合は減算
		magic->colorMagic.g -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.g < 0.0f)
		{
			magic->colorMagic.g = 0.0f;
		}
	}
	else if (magic->colorMagic.g < SetColorPallet(magic->nColorFlag).g)
	{	// セットカラーより低い場合は減算
		magic->colorMagic.g += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.g > 1.0f)
		{
			magic->colorMagic.g = 1.0f;
		}
	}
	// 青色処理
	if (magic->colorMagic.b > SetColorPallet(magic->nColorFlag).b)
	{	// セットカラーより高い場合は減算
		magic->colorMagic.b -= TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.b < 0.0f)
		{
			magic->colorMagic.b = 0.0f;
		}
	}
	else if (magic->colorMagic.b < SetColorPallet(magic->nColorFlag).b)
	{	// セットカラーより低い場合は減算
		magic->colorMagic.b += TEXTURE_MAGIC_CHANGE_COLLAR;
		if (magic->colorMagic.b > 1.0f)
		{
			magic->colorMagic.b = 1.0f;
		}
	}
}

//=============================================================================
// スキル変更関数
//=============================================================================
void ChangeMagic(int nMagic)
{
	MAGIC *magic = &magicWk[nMagic];

	// キーボード
	if (GetKeyboardTrigger(DIK_1) && magic->bActiveFire)
	{
		magic->nType = BULLET_SPECIAL_FIRE - 1;
	}
	else if (GetKeyboardTrigger(DIK_2) && magic->bActiveThunder)
	{
		magic->nType = BULLET_SPECIAL_THUNDER - 1;
	}
	else if (GetKeyboardTrigger(DIK_3) && magic->bActiveIce)
	{
		magic->nType = BULLET_SPECIAL_ICE - 1;
	}
	else if (GetKeyboardTrigger(DIK_4) && magic->bActiveWind)
	{
		magic->nType = BULLET_SPECIAL_WIND - 1;
	}
	else if (GetKeyboardTrigger(DIK_5) && magic->bActiveWater)
	{
		magic->nType = BULLET_SPECIAL_WATER - 1;
	}
	else if (GetKeyboardTrigger(DIK_6) && magic->bActiveEarth)
	{
		magic->nType = BULLET_SPECIAL_EARTH - 1;
	}

	if (GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(0, R_BUTTON_X))
	{
		magic->bSelectCheck = true;
		magic->nType--;							// タイプを減算
		if (magic->nType < 0)					// タイプ最小数を越えていたら
		{
			magic->nType = MAGIC_TYPE_MAX - 1;	// 最大数を渡す
		}
	}
	else if (GetKeyboardTrigger(DIK_E) || IsButtonTriggered(0, R_BUTTON_A))
	{
		magic->bSelectCheck = false;
		magic->nType++;							// タイプを加算
		if (magic->nType >= MAGIC_TYPE_MAX)		// タイプ最大数を越えていたら
		{
			magic->nType = 0;					// 最小数を渡す
		}
	}

	// マウスホイール操作
	if (magic->nCoolDown <= 0)							// 誤回転防止
	{
		float fTempRot = (float)GetMobUseZ();			// マウスホイールの回転取得
		if ((float)GetMobUseZ())
		{
			magic->nCoolDown = MAGIC_ROT_COOLDOWN;
			if (fTempRot > 0.0f)						// 戻り値がプラス
			{
				magic->bSelectCheck = true;
				magic->nType--;							// タイプを減算
				if (magic->nType < 0)					// タイプ最小数を越えていたら
				{
					magic->nType = MAGIC_TYPE_MAX - 1;	// 最大数を渡す
				}
			}
			else if (fTempRot < 0.0f)					// 戻り値がマイナス
			{
				magic->bSelectCheck = false;
				magic->nType++;							// タイプを加算
				if (magic->nType >= MAGIC_TYPE_MAX)		// タイプ最大数を越えていたら
				{
					magic->nType = 0;					// 最小数を渡す
				}
			}
		}
	}
	magic->nCoolDown--;

	if(!magic->bSelectCheck)
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			if (!magic->bActiveFire) magic->nType++;
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			if (!magic->bActiveThunder) magic->nType++;
			break;
		case BULLET_SPECIAL_ICE - 1:
			if (!magic->bActiveIce) magic->nType++;
			break;
		case BULLET_SPECIAL_WIND - 1:
			if (!magic->bActiveWind) magic->nType++;
			break;
		case BULLET_SPECIAL_WATER - 1:
			if (!magic->bActiveWater) magic->nType++;
			break;
		case BULLET_SPECIAL_EARTH - 1:
			if (!magic->bActiveEarth) magic->nType++;
			if (magic->nType >= MAGIC_TYPE_MAX) magic->nType = 0;
			break;
		}
	}
	else
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			if (!magic->bActiveFire) magic->nType--;
			if (magic->nType < 0) magic->nType = BULLET_SPECIAL_EARTH - 1;
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			if (!magic->bActiveThunder) magic->nType--;
			break;
		case BULLET_SPECIAL_ICE - 1:
			if (!magic->bActiveIce) magic->nType--;
			break;
		case BULLET_SPECIAL_WIND - 1:
			if (!magic->bActiveWind) magic->nType--;
			break;
		case BULLET_SPECIAL_WATER - 1:
			if (!magic->bActiveWater) magic->nType--;
			break;
		case BULLET_SPECIAL_EARTH - 1:
			if (!magic->bActiveEarth) magic->nType--;
			break;
		}
	}
}

//=============================================================================
// 有効設定
//=============================================================================
void SetMagicBgm(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];

	if (!magic->bActive)
	{
		SetSoundBgm(SOUND_BGM_GAME_NORMAL);
	}
	else
	{
		switch (magic->nType)
		{
		case BULLET_SPECIAL_FIRE - 1:
			SetSoundBgm(SOUND_BGM_GAME_FIRE);
			break;
		case BULLET_SPECIAL_THUNDER - 1:
			SetSoundBgm(SOUND_BGM_GAME_THUNDER);
			break;
		case BULLET_SPECIAL_ICE - 1:
			SetSoundBgm(SOUND_BGM_GAME_ICE);
			break;
		case BULLET_SPECIAL_WIND - 1:
			SetSoundBgm(SOUND_BGM_GAME_WIND);
			break;
		case BULLET_SPECIAL_WATER - 1:
			SetSoundBgm(SOUND_BGM_GAME_WATER);
			break;
		case BULLET_SPECIAL_EARTH - 1:
			SetSoundBgm(SOUND_BGM_GAME_EARTH);
			break;
		}
	}
}

//=============================================================================
// 有効設定
//=============================================================================
void SetMagic(void)
{
	MAGIC *magic = &magicWk[0];
	magic->bUse = true;
}

//=============================================================================
// タイプ設定関数
//=============================================================================
void SetMagicType(int nMagicType)
{
	MAGIC *magic = &magicWk[0];
	switch (nMagicType)
	{
	case BULLET_SPECIAL_FIRE:
		magic->bActiveFire = true;
		SetMagiceffect(MAGICEFFECT_FIRE);
		SetGage(GAGE_BULLET_SPECIAL_FIRE, 0);
		SetMagicModel(0, BULLET_SPECIAL_FIRE);
		break;
	case BULLET_SPECIAL_THUNDER:
		magic->bActiveThunder = true;
		SetMagiceffect(MAGICEFFECT_THUNDER);
		SetGage(GAGE_BULLET_SPECIAL_THUNDER, 0);
		SetMagicModel(0, BULLET_SPECIAL_THUNDER);
		break;
	case BULLET_SPECIAL_ICE:
		magic->bActiveIce = true;
		SetMagiceffect(MAGICEFFECT_ICE);
		SetGage(GAGE_BULLET_SPECIAL_ICE, 0);
		SetMagicModel(0, BULLET_SPECIAL_ICE);
		break;
	case BULLET_SPECIAL_WIND:
		magic->bActiveWind = true;
		SetMagiceffect(MAGICEFFECT_WIND);
		SetGage(GAGE_BULLET_SPECIAL_WIND, 0);
		SetMagicModel(0, BULLET_SPECIAL_WIND);
		break;
	case BULLET_SPECIAL_WATER:
		magic->bActiveWater = true;
		SetMagiceffect(MAGICEFFECT_WATER);
		SetGage(GAGE_BULLET_SPECIAL_WATER, 0);
		SetMagicModel(0, BULLET_SPECIAL_WATER);
		break;
	case BULLET_SPECIAL_EARTH:
		magic->bActiveEarth = true;
		SetMagiceffect(MAGICEFFECT_EARTH);
		SetGage(GAGE_BULLET_SPECIAL_EARTH, 0);
		SetMagicModel(0, BULLET_SPECIAL_EARTH);
		break;
	}

	for (int i = 0; i < MAGIC_MAX; i++, magic++)
	{
		magic->bActive = true;
	}
	SetGageselect();
}

//=============================================================================
// 無効設定
//=============================================================================
void ReleaseMagic(void)
{
	MAGIC *magic = &magicWk[0];
	magic->bUse = false;
}

//=============================================================================
// MAGIC取得関数
//=============================================================================
MAGIC *GetMagic(int no)
{
	return(&magicWk[no]);
}

//=============================================================================
// MAGICタイプ取得関数
//=============================================================================
int GetTypeMagic(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];
	return(magic->nType);
}

//=============================================================================
// カラー取得関数
//=============================================================================
D3DXCOLOR GetMagicColor(void)
{
	MAGIC *magic = &magicWk[MAGIC_MAIN];
	return(magic->colorMagic);
}

