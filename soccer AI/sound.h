/*==============================================================================

    �T�E���h���� [sound.cpp]
                                                       Author : Yuki Nagaoka
                                                       Date   : 2016/6/30

==============================================================================*/
#ifndef _SOUND_H_
#define _SOUND_H_

///////////////////////////////////
//
//		�w�b�_�t�@�C��
//
///////////////////////////////////
#include "main.h"

///////////////////////////////////
//
//		�}�N����`
//
///////////////////////////////////

///////////////////////////////////
//
//		�\����
//
///////////////////////////////////

///////////////////////////////////
//
//		�񋓌^
//
///////////////////////////////////

///////////////////////////////////
//
//		�O���[�o���ϐ�
//
///////////////////////////////////

///////////////////////////////////
//
//		�v���g�^�C�v�錾
//
///////////////////////////////////
class CSound
{
public:
		// �p�����[�^�\���̒�`
		typedef struct
		{
		    char *pFilename;                            // �t�@�C����
		    int nCntLoop;                               // ���[�v�J�E���g
		} SOUNDPARAM;
		
		// �T�E���h�t�@�C��
		typedef enum
		{
		    SOUND_LABEL_BGM000 = 0,                     // BGM0
		    SOUND_LABEL_BGM001,                         // BGM1
			SOUND_LABEL_BGM002,							// BGM3
		    SOUND_LABEL_SE_SHOT,                        // �e���ˉ�
		    SOUND_LABEL_SE_HIT,                         // �q�b�g��
		    SOUND_LABEL_SE_EXPLOSION,                   // ������
			SOUND_LABEL_SE_EXPLOSION1,                  // ������
			SOUND_LABEL_SE_CAR,					        // �{�C�X
		    SOUND_LABEL_MAX								// �T�E���h�ő吔
		} SOUND_LABEL;									// ���O

		CSound();										// �R���X�g���N�^
		~CSound();										// �f�X�g���N�^
		HRESULT InitSound(HWND hWnd);					// ����������
		void UninitSound(void);							// �I������
		static HRESULT PlaySound(SOUND_LABEL label);	// �T�E���h�v���C
		static void StopSound(SOUND_LABEL label);		// �T�E���h�X�g�b�v
		static void StopSound(void);					// �T�E���h�X�g�b�v

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
protected:
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
