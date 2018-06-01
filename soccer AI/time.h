
//=============================================================================
//
// �X�R�A���� [time.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

///////////////////////////////////////////////
//
//		�w�b�_�t�@�C��
//
////////////////////////////////////////////////
#include "scene.h"


class CNumber;
///////////////////////////////////////////////
//
//		�}�N����`
//
////////////////////////////////////////////////
#define TIME_NUM			(1)

///////////////////////////////////////////////
//
//		�\���̒�`
//
////////////////////////////////////////////////
class CTime : public CScene
{
public:
	CTime(int Priority);			// �R���X�g���N�^
	~CTime();						// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��	
	static CTime *Create(void);	// �I�u�W�F�N�g��������

private:
	CNumber *m_apNumber[TIME_NUM];	// �i���o�[�N���X�̐ÓI�����o�ϐ�[�\�����鐔���̍ő吔] 
	D3DXVECTOR3 m_pos;				// �\�����W
	D3DXVECTOR3 size;				// ���A����
	int m_nTime;					// �^�C���ϐ�
	int m_FrameTime;				// �t���C���^�C��
};

#endif