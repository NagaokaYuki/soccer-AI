/*=======================================================================

	インファイルローダーベース [infileLoaderBase.h]

												長岡佑樹

=========================================================================*/
#ifndef INIFILELOADERBASE
#define INIFILELOADERBASE
#pragma warning(disable:4800)

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <cassert>

/////////////////////////////////////////////////////////
//
//	iniFileLoaderBaseクラス
//
/////////////////////////////////////////////////////////
class iniFileLoaderBase
{

private:

  // パラメータが格納されているファイル
  std::ifstream file;

  std::string   CurrentLine;

  void        GetParameterValueAsString(std::string& line);

  std::string GetNextParameter();

  // コメントを無視し、次の区切り文字列を見つける
  std::string GetNextToken();

  // ユーザーが指定したファイルが有効な場合はtrue
  bool        m_bGoodFile;

public:

  // 見つかったパラメータ値を関連する型に変換
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



