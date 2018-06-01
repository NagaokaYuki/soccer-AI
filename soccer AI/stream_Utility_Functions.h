/*=======================================================================

	�X�g���[�����[�e�B���e�B�t�@���N�V���� [stream_Utility_Functions.h]

												�����C��

	�X�g���[����܂��̓X�g���[����œ��삷�邳�܂��܂ȕ֗��Ȋ֐�

=========================================================================*/
#ifndef STREAM_UTILITY_FUNCTIONS
#define STREAM_UTILITY_FUNCTIONS

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <sstream>
#include <string>
#include <iomanip>

/////////////////////////////////////////////////////////
//
//	�^�𕶎���ɕϊ�����
//
/////////////////////////////////////////////////////////
template <class T>
inline std::string ttos(const T& t, int precision = 2)
{
  std::ostringstream buffer;

  buffer << std::fixed << std::setprecision(precision) << t;

  return buffer.str();
}

/////////////////////////////////////////////////////////
//
//	�u�[���𕶎���ɕϊ�����
//
/////////////////////////////////////////////////////////
inline std::string btos(bool b)
{
  if (b) return "true";
  return "false";
}

/////////////////////////////////////////////////////////
//
//	GetValueFromStream�֐�
//
//	�w�肳�ꂽ�^�̒l����̓X�g���[������擾
//
/////////////////////////////////////////////////////////
template <typename T>
inline T GetValueFromStream(std::ifstream& stream)
{
  T val;

  stream >> val;

  if (!stream)
  {
    throw std::runtime_error("Attempting to retrieve wrong type from stream");
  }

  return val;
}

/////////////////////////////////////////////////////////
//
//	WriteBitsToStream�֐�
//
//	���̒l���r�b�g�̃o�C�i��������Ƃ��ď�������
//
/////////////////////////////////////////////////////////
template <typename T>
void WriteBitsToStream(std::ostream& stream, const T& val)
{
  int iNumBits = sizeof(T) * 8;

  while (--iNumBits >= 0)
  {
    if ((iNumBits+1) % 8 == 0) stream << " ";
    unsigned long mask = 1 << iNumBits;
    if (val & mask) stream << "1";
    else stream << "0";
  }
}

#endif