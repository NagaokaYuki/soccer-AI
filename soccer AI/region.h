/*=======================================================================

	���W���� [region.h]

												�����C��
		��`�̈���`����
		���̗̈�ɂ͎��ʔԍ���4�̃R�[�i�[��ݒ�

=========================================================================*/
#ifndef REGION_H
#define REGION_H

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <math.h>
#include "vector2D.h"
#include "cgdi.h"
#include "utils.h"
#include "stream_Utility_Functions.h"

/////////////////////////////////////////////////////////
//
//	Region�N���X
//
/////////////////////////////////////////////////////////
class Region
{
public:
	enum region_modifier{halfsize, normal};
  
protected:
	double        m_dTop;		// ��
	double        m_dLeft;		// ��
	double        m_dRight;		// �E
	double        m_dBottom;	// ��
	
	double        m_dWidth;		// ��
	double        m_dHeight;	// ����
	
	Vector2D     m_vCenter;		// ���S
	
	int          m_iID;			// ID

public:
	Region():m_dTop(0),m_dBottom(0),m_dLeft(0),m_dRight(0){}
	
	Region(double left,
	       double top,
	       double right,
	       double bottom,
	       int id = -1):m_dTop(top),
	                      m_dRight(right),
	                      m_dLeft(left),
	                      m_dBottom(bottom),
	                      m_iID(id)
	{
	  // ���S���v�Z���� 
	  m_vCenter = Vector2D( (left+right)*0.5, (top+bottom)*0.5 );
	
	  m_dWidth  = fabs(right-left);
	  m_dHeight = fabs(bottom-top);
	}
	
	virtual ~Region(){}
	
	virtual inline void     Render(bool ShowID)const;
	
	// �w�肳�ꂽ�ʒu���̈���ɂ���ꍇ��true��Ԃ�
	// �̈�C���q���g�p���ė̈拫�E���k�����邱�Ƃ��\
	inline bool     Inside(Vector2D pos, region_modifier r)const;
	
	// �̈���̃����_���Ȉʒu��\���x�N�g����Ԃ�
	inline Vector2D GetRandomPosition()const;
	
	//-------------------------------
	double     Top()const{return m_dTop;}
	double     Bottom()const{return m_dBottom;}
	double     Left()const{return m_dLeft;}
	double     Right()const{return m_dRight;}
	double     Width()const{return fabs(m_dRight - m_dLeft);}
	double     Height()const{return fabs(m_dTop - m_dBottom);}
	double     Length()const{return max(Width(), Height());}
	double     Breadth()const{return min(Width(), Height());}
	
	Vector2D  Center()const{return m_vCenter;}
	int       ID()const{return m_iID;}
	
};

////////////////////////////////////////////////////
//
// GetRandomPosition�֐�
//  
////////////////////////////////////////////////////
inline Vector2D Region::GetRandomPosition()const
{
  return Vector2D(RandInRange(m_dLeft, m_dRight),
                   RandInRange(m_dTop, m_dBottom));
}

////////////////////////////////////////////////////
//
// Inside�֐�
//  
////////////////////////////////////////////////////
inline bool Region::Inside(Vector2D pos, region_modifier r=normal)const
{
  if (r == normal)
  {
    return ((pos.x > m_dLeft) && (pos.x < m_dRight) &&
         (pos.y > m_dTop) && (pos.y < m_dBottom));
  }
  else
  {
    const double marginX = m_dWidth * 0.25;
    const double marginY = m_dHeight * 0.25;

    return ((pos.x > (m_dLeft+marginX)) && (pos.x < (m_dRight-marginX)) &&
         (pos.y > (m_dTop+marginY)) && (pos.y < (m_dBottom-marginY)));
  }

}

////////////////////////////////////////////////////
//
// Render�֐�
//  
////////////////////////////////////////////////////
inline void Region::Render(bool ShowID = 0)const
{
  gdi->HollowBrush();
  gdi->GreenPen();
  gdi->Rect(m_dLeft, m_dTop, m_dRight, m_dBottom);

  if (ShowID)
  { 
    gdi->TextColor(Cgdi::green);
    gdi->TextAtPos(Center(), ttos(ID()));
  }
}

#endif
