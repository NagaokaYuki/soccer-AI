/*=======================================================================

	�C���t�@�C�����[�_�[�x�[�X [infileLoaderBase.h]

												�����C��

=========================================================================*/
#ifndef INIFILELOADERBASE
#define INIFILELOADERBASE
#pragma warning(disable:4800)

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	iniFileLoaderBase�N���X
//
/////////////////////////////////////////////////////////
class iniFileLoaderBase
{

private:

  // �p�����[�^���i�[����Ă���t�@�C��
  std::ifstream file;

  std::string   CurrentLine;

  void        GetParameterValueAsString(std::string& line);

  std::string GetNextParameter();

  // �R�����g�𖳎����A���̋�؂蕶�����������
  std::string GetNextToken();

  // ���[�U�[���w�肵���t�@�C�����L���ȏꍇ��true
  bool        m_bGoodFile;

public:

  // ���������p�����[�^�l���֘A����^�ɕϊ�
  double      GetNextParameterDouble(){if (m_bGoodFile) return atof(GetNextParameter().c_str());throw std::runtime_error("bad file");}
  float       GetNextParameterFloat(){if (m_bGoodFile) return (float)atof(GetNextParameter().c_str());throw std::runtime_error("bad file");}
  int         GetNextParameterInt(){if (m_bGoodFile) return atoi(GetNextParameter().c_str());throw std::runtime_error("bad file");}
  bool        GetNextParameterBool(){return (bool)(atoi(GetNextParameter().c_str()));throw std::runtime_error("bad file");}

  double      GetNextTokenAsDouble(){if (m_bGoodFile) return atof(GetNextToken().c_str()); throw std::runtime_error("bad file");}
  float       GetNextTokenAsFloat(){if (m_bGoodFile) return (float)atof(GetNextToken().c_str()); throw std::runtime_error("bad file");}
  int         GetNextTokenAsInt(){if (m_bGoodFile) return atoi(GetNextToken().c_str()); throw std::runtime_error("bad file");}
  std::string GetNextTokenAsString(){if (m_bGoodFile) return GetNextToken(); throw std::runtime_error("bad file");}

  bool        eof()const{if (m_bGoodFile) return file.eof(); throw std::runtime_error("bad file");}
  bool        FileIsGood()const{return m_bGoodFile;}

  iniFileLoaderBase(char* filename):CurrentLine(""), m_bGoodFile(true)
  {
    file.open(filename);

    if (!file){m_bGoodFile = false;}
  }

};

#endif



