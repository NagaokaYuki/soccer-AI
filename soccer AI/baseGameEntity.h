/*=======================================================================

	�x�[�X�Q�[���G���e�B�e�B [baseGameEntity.h]

												�����C��

	���ׂẴQ�[���G���e�B�e�B�̋��ʃC���^�[�t�F�C�X���`�����{�N���X

=========================================================================*/
#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H
#pragma warning (disable:4786)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <iosfwd>
#include "vector2D.h"
#include "geometry.h"
#include "utils.h"

/////////////////////////////////////////////////////////
//
//	�\����`
//
/////////////////////////////////////////////////////////
struct Telegram;

/////////////////////////////////////////////////////////
//
//	BaseGameEntity�N���X
//
/////////////////////////////////////////////////////////
class BaseGameEntity
{
public:
	enum {default_entity_type = -1};

private:
	int         m_ID;			// �e�G���e�B�e�B��ID
	int         m_iType;		// �e�G���e�B�e�B�̃^�C�v
	bool        m_bTag;		// �^�O�t���O
	
	// BaseGameEntity���C���X�^���X������邽�т�
	// ���̒l�̓^�O�t���O�ɍX�V�����
	static int  m_iNextValidID;
	
	// ID���������ݒ肳��Ă��邱�Ƃ��m�F���邽�߂ɁA
	// �e�R���X�g���N�^�[���ŌĂяo��
	// ID��ݒ肵�Ď��̗L����ID���C���N�������g����O�ɁA
	// ���\�b�h�ɓn���ꂽ�l�����̗L����ID�ȏ�ł��邱�Ƃ�����
	void SetID(int val);

protected:
	Vector2D m_vPosition;		// ���W�ʒu
	Vector2D m_vScale;			// �g��k��

	// �I�u�W�F�N�g�̋��E���a�̑傫��
	double    m_dBoundingRadius;
	BaseGameEntity(int ID);

public:
	virtual ~BaseGameEntity(){}
	virtual void Update(){}; 
	virtual void Render()=0;
	virtual bool HandleMessage(const Telegram& msg){return false;}
	virtual void Write(std::ostream&  os)const{}
	virtual void Read (std::ifstream& is){}
	
	// ���̗L����ID���擾
	static int   GetNextValidID(){return m_iNextValidID;}
	
	// ����ID�����Z�b�g
	static void  ResetNextValidID(){m_iNextValidID = 0;}

	Vector2D     Pos()const{return m_vPosition;}
	void         SetPos(Vector2D new_pos){m_vPosition = new_pos;}
	
	double       BRadius()const{return m_dBoundingRadius;}
	void         SetBRadius(double r){m_dBoundingRadius = r;}
	int          ID()const{return m_ID;}
	
	bool         IsTagged()const{return m_bTag;}
	void         Tag(){m_bTag = true;}
	void         UnTag(){m_bTag = false;}
	
	Vector2D     Scale()const{return m_vScale;}
	void         SetScale(Vector2D val){m_dBoundingRadius *= MaxOf(val.x, val.y)/MaxOf(m_vScale.x, m_vScale.y); m_vScale = val;}
	void         SetScale(double val){m_dBoundingRadius *= (val/MaxOf(m_vScale.x, m_vScale.y)); m_vScale = Vector2D(val, val);} 
	
	int          EntityType()const{return m_iType;}
	void         SetEntityType(int new_type){m_iType = new_type;}
	
};
      
#endif




