/*==============================================================================

    サウンド表示処理 [sound.cpp]
                                                       Author : Yuki Nagaoka
                                                       Date   : 2016/6/30

==============================================================================*/

///////////////////////////////////
//
//		ヘッダファイル
//
///////////////////////////////////
#include "sound.h"

///////////////////////////////////
//
//		定数定義
//
///////////////////////////////////

///////////////////////////////////
//
//		構造体
//
///////////////////////////////////

///////////////////////////////////
//
//		プロトタイプ宣言
//
///////////////////////////////////

///////////////////////////////////
//
//		静的メンバ変数
//
///////////////////////////////////
IXAudio2 *CSound::m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *CSound::m_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *CSound::m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *CSound::m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD CSound::m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

///////////////////////////////////
//
// 各音素材のパラメータ
//
///////////////////////////////////
CSound::SOUNDPARAM CSound::m_aParam[SOUND_LABEL_MAX] =
	{
	    {"data/BGM/bgm_maoudamashii_neorock54.wav", -1},          // BGM0
		{"data/BGM/bgm_maoudamashii_neorock63.wav", -1},		  // BGM1
	    {"data/BGM/覚醒の時.wav",-1},							  // BGM3
	    {"data/SE/laser2.wav", 0},								  // 弾発射音
	    {"data/SE/button81.wav", 0},				              // ヒット音
	    {"data/SE/explosion3.wav", 0},							  // 爆発音
	    {"data/SE/large_destruction1.wav", 0},					  // 爆発音
	    {"data/SE/car-bypass_mono.wav", 0},						  // 爆発音
	};
///////////////////////////////////
//
//		コンストラクタ
//
///////////////////////////////////
CSound::CSound()
{
	
}
///////////////////////////////////
//
//		デストラクタ
//
///////////////////////////////////
CSound::~CSound()
{
}

///////////////////////////////////
//
//		初期化処理
//
///////////////////////////////////
HRESULT CSound::InitSound(HWND hWnd)
{
    HRESULT hr;

    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    hr = XAudio2Create(&m_pXAudio2, 0);
    if(FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // マスターボイスの生成
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if(FAILED(hr))
    {
        MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

        if(m_pXAudio2)
        {
            // XAudio2オブジェクトの開放
            m_pXAudio2->Release();
            m_pXAudio2 = NULL;
        }

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // サウンドデータの初期化
    for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // バッファのクリア
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

        if(hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        if(dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
        if(FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // ソースボイスの生成
        hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
        if(FAILED(hr))
        {
            MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = m_aSizeAudio[nCntSound];
        buffer.pAudioData = m_apDataAudio[nCntSound];
        buffer.Flags      = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount  = m_aParam[nCntSound].nCntLoop;
        
        // オーディオバッファの登録
        m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
    }

    return S_OK;
}

///////////////////////////////////
//
//		終了処理
//
///////////////////////////////////
void CSound::UninitSound(void)
{
    // 一時停止
    for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if(m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);

            // ソースボイスの破棄
            m_apSourceVoice[nCntSound]->DestroyVoice();
            m_apSourceVoice[nCntSound] = NULL;

            // オーディオデータの開放
            free(m_apDataAudio[nCntSound]);
            m_apDataAudio[nCntSound] = NULL;
        }
    }

    // マスターボイスの破棄
    m_pMasteringVoice->DestroyVoice();
    m_pMasteringVoice = NULL;

    if(m_pXAudio2)
    {
        // XAudio2オブジェクトの解放
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    // COMライブラリの終了処理
    CoUninitialize();
}

///////////////////////////////////
//
//		再生処理
//
///////////////////////////////////
HRESULT CSound::PlaySound(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // バッファの値設定
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = m_aSizeAudio[label];
    buffer.pAudioData = m_apDataAudio[label];
    buffer.Flags      = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount  = m_aParam[label].nCntLoop;

    // 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

    if(xa2state.BuffersQueued != 0)
    {// 再生中
        // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

    // 再生
    m_apSourceVoice[label]->Start(0);

    return S_OK;
}


///////////////////////////////////
//
//		停止処理
//
///////////////////////////////////
void CSound::StopSound(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    m_apSourceVoice[label]->GetState(&xa2state);
    if(xa2state.BuffersQueued != 0)
    {// 再生中
        // 一時停止
        m_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        m_apSourceVoice[label]->FlushSourceBuffers();
    }
}

///////////////////////////////////
//
//		全て停止処理
//
///////////////////////////////////
void CSound::StopSound(void)
{
    // 一時停止
    for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if(m_apSourceVoice[nCntSound])
        {
            // 一時停止
            m_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

///////////////////////////////////
//
//		チェック処理
//
///////////////////////////////////
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
    HRESULT hr = S_OK;
    DWORD dwRead;
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwBytesRead = 0;
    DWORD dwOffset = 0;

    if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを先頭に移動
        return HRESULT_FROM_WIN32(GetLastError());
    }
    
    while(hr == S_OK)
    {
        if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクデータの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch(dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize  = dwChunkDataSize;
            dwChunkDataSize = 4;
            if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// ファイルタイプの読み込み
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// ファイルポインタをチャンクデータ分移動
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        dwOffset += sizeof(DWORD) * 2;
        if(dwChunkType == format)
        {
            *pChunkSize         = dwChunkDataSize;
            *pChunkDataPosition = dwOffset;

            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        if(dwBytesRead >= dwRIFFDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

///////////////////////////////////
//
//		読み込み処理
//
///////////////////////////////////
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// データの読み込み
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

