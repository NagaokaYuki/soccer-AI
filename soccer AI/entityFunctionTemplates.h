/*=======================================================================

	�G���e�B�e�B�̃e���v���[�g [entityFunctionTemplates.h]

												�����C��

=========================================================================*/
#ifndef GAME_ENTITY_FUNCTION_TEMPLATES
#define GAME_ENTITY_FUNCTION_TEMPLATES

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "baseGameEntity.h"
#include "geometry.h"

/////////////////////////////////////////////////////////
//
//	Overlapped�֐�
//
//	�G���e�B�e�B���W���R���e�i�Ɋi�[����Ă���
//	�������̃G���e�B�e�B�̂����ꂩ�Əd�����Ă��邩
//	�ǂ����𒲂ׂ�
//
/////////////////////////////////////////////////////////
template <class T, class conT>
bool Overlapped(const T* ob, const conT& conOb, double MinDistBetweenObstacles = 40.0)
{
  typename conT::const_iterator it;

  for (it=conOb.begin(); it != conOb.end(); ++it)
  {
    if (TwoCirclesOverlapped(ob->Pos(),
                             ob->BRadius()+MinDistBetweenObstacles,                             
                             (*it)->Pos(),
                             (*it)->BRadius()))
    {
      return true;
    }
  }

  return false;
}

/////////////////////////////////////////////////////////
//
//	TagNeighbors�֐�
//
//	�P��̃G���e�B�e�B�p�����[�^�̔��a���ɂ���std
//	�R���e�i�Ɋ܂܂��C�ӂ̃G���e�B�e�B���^�O�t��
//
/////////////////////////////////////////////////////////
template <class T, class conT>
void TagNeighbors(T* entity, conT& others, const double radius)
{
  typename conT::iterator it;

  // �͈͂��`�F�b�N���邷�ׂẴG���e�B�e�B�𔽕���������
  for (it=others.begin(); it != others.end(); ++it)
  {
    // ���݂̃^�O�����ׂď���
    (*it)->UnTag();

    // sqrts������邽�߂ɋ��������ō��
    Vector2D to = (*it)->Pos() - entity->Pos();

    // �͈͂ɉ����邱�Ƃɂ���āA�����̋��E���a���l���ɓ����
    double range = radius + (*it)->BRadius();

    // �͈͓��̃G���e�B�e�B�̏ꍇ�A�������邽�߂̃^�O
    if ( ((*it) != entity) && (to.LengthSq() < range*range))
    {
      (*it)->Tag();
    } 
  }
}

/////////////////////////////////////////////////////////
//
//	EnforceNonPenetrationContraint�֐�
//
//	�G���e�B�e�B�Ԃ̏d�������邩�ǂ������`�F�b�N���A
//	���݂���ꍇ�A�G���e�B�e�B�݂͌��ɗ���Ĉړ�����
//
/////////////////////////////////////////////////////////
template <class T, class conT>
void EnforceNonPenetrationContraint(T entity, const conT& others)
{
  typename conT::const_iterator it;

  // ���E�̔��a�̃I�[�o�[���b�v���`�F�b�N����
  // ���ׂẴG���e�B�e�B�𔽕���������
  for (it=others.begin(); it != others.end(); ++it)
  {
    if (*it == entity) continue;

    // �G���e�B�e�B�̈ʒu�Ԃ̋������v�Z����
    Vector2D ToEntity = entity->Pos() - (*it)->Pos();

    double DistFromEachOther = ToEntity.Length();

    // ���̋��������a�̍��v�����������ꍇ�A
    // ���̃G���e�B�e�B��ToEntity�x�N�g����
    // ���s�ȕ����Ɉړ�����
    double AmountOfOverLap = (*it)->BRadius() + entity->BRadius() -
                             DistFromEachOther;

    if (AmountOfOverLap >= 0)
    {
      // �G���e�B�e�B���I�[�o�[���b�v�̗ʂɑ������鋗�������ړ�������
      entity->SetPos(entity->Pos() + (ToEntity/DistFromEachOther) *
                     AmountOfOverLap);
    }
  }
}

#endif