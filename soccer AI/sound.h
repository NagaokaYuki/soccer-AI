/*==============================================================================

    サウンド処理 [sound.cpp]
                                                       Author : Yuki Nagaoka
                                                       Date   : 2016/6/30

==============================================================================*/
#ifndef _SOUND_H_
#define _SOUND_H_

///////////////////////////////////
//
//		ヘッダファイル
//
///////////////////////////////////
#include "main.h"

///////////////////////////////////
//
//		マクロ定義
//
///////////////////////////////////

///////////////////////////////////
//
//		構造体
//
///////////////////////////////////

///////////////////////////////////
//
//		列挙型
//
///////////////////////////////////

///////////////////////////////////
//
//		グローバル変数
//
///////////////////////////////////

///////////////////////////////////
//
//		プロトタイプ宣言
//
///////////////////////////////////
class CSound
{
public:
		// パラメータ構造体定義
		typedef struct
		{
		    char *pFilename;                            // ファイル名
		    int nCntLoop;                               // ループカウント
		} SOUNDPARAM;
		
		// サウンドファイル
		typedef enum
		{
		    SOUND_LABEL_BGM000 = 0,                     // BGM0
		    SOUND_LABEL_BGM001,                         // BGM1
			SOUND_LABEL_BGM002,							// BGM3
		    SOUND_LABEL_SE_SHOT,                        // 弾発射音
		    SOUND_LABEL_SE_HIT,                         // ヒット音
		    SOUND_LABEL_SE_EXPLOSION,                   // 爆発音
			SOUND_LABEL_SE_EXPLOSION1,                  // 爆発音
			SOUND_LABEL_SE_CAR,					        // ボイス
		    SOUND_LABEL_MAX								// サウンド最大数
		} SOUND_LABEL;									// 名前

		CSound();										// コンストラクタ
		~CSound();										// デストラクタ
		HRESULT InitSound(HWND hWnd);					// 初期化処理
		void UninitSound(void);							// 終了処理
		static HRESULT PlaySound(SOUND_LABEL label);	// サウンドプレイ
		static void StopSound(SOUND_LABEL label);		// サウンドストップ
		static void StopSound(void);					// サウンドストップ

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
protected:
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
