/*=======================================================================

	ストリームユーティリティファンクション [stream_Utility_Functions.h]

												長岡佑樹

	ストリーム上またはストリーム上で動作するさまざまな便利な関数

=========================================================================*/
#ifndef STREAM_UTILITY_FUNCTIONS
#define STREAM_UTILITY_FUNCTIONS

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include <sstream>
#include <string>
#include <iomanip>

/////////////////////////////////////////////////////////
//
//	型を文字列に変換する
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
//	ブールを文字列に変換する
//
/////////////////////////////////////////////////////////
inline std::string btos(bool b)
{
  if (b) return "true";
  return "false";
}

/////////////////////////////////////////////////////////
//
//	GetValueFromStream関数
//
//	指定された型の値を入力ストリームから取得
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
//	WriteBitsToStream関数
//
//	その値をビットのバイナリ文字列として書き込む
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