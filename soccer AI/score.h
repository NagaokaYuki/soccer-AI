
//=============================================================================
//
// �X�R�A���� [score.h]
// Author : NAGAOKA YUKI
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

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
#define SCORE_MAX			(1)

///////////////////////////////////////////////
//
//		�\���̒�`
//
////////////////////////////////////////////////
class CScore : public CScene
{
public:
	CScore(int Priority);						// �R���X�g���N�^
	~CScore();						// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��	
	static CScore *Create(void);	// �I�u�W�F�N�g��������
	void AddScore(int nValue);		// �X�R�A���Z����  
	int GetScore(void);				// �X�R�A�擾����  

private:
	CNumber *m_apNumber[SCORE_MAX];	// 
	D3DXVECTOR3 m_pos;				// �\�����W
	D3DXVECTOR3 size;				// ���A����
	int m_nScore;					// �X�R�A�ϐ�

};

#endif