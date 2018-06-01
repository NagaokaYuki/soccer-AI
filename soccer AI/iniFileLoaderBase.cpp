/*=======================================================================

	インファイルローダーベース [inFileLoaderBase.cpp]

												
												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#include "iniFileLoaderBase.h"
using std::string;


/////////////////////////////////////////////////////////
//
//	RemoveCommentingFromLine関数
//	(テキスト行からコメントを削除する)
//
/////////////////////////////////////////////////////////
void RemoveCommentingFromLine(string& line)
{
   // コメントを検索して削除する
   string::size_type idx = line.find('//');

   if (idx != string::npos)
   {
     // コメントを切り取る
     line = line.substr(0, idx);
   }
}

/////////////////////////////////////////////////////////
//
//	GetNextParameter関数
//
//		次の有効なパラメータのテキストファイルを検索
//		コメントを破棄し、その値を文字列として返す
//
/////////////////////////////////////////////////////////
string iniFileLoaderBase::GetNextParameter()
{
 
  // これは次のパラメータを保持する文字列になります
  std::string line;
  
  std::getline(file, line);
   
  RemoveCommentingFromLine(line);

  // 行の長さがゼロの場合は、ファイルから次の行を取得する
  if (line.length() == 0)
  {
    return GetNextParameter();
  }

  GetParameterValueAsString(line);  
    
  return line;
}

/////////////////////////////////////////////////////////
//
//	GetParameterValueAsString関数
//
//	与えられたテキスト行を渡すと、
//	この関数はパラメータ記述を削除し、
//	パラメータをstd :: stringとして返す
//
/////////////////////////////////////////////////////////
void iniFileLoaderBase::GetParameterValueAsString(string& line)
{
  // パラメータ記述の始めを見つける
  string::size_type begIdx;
  string::size_type endIdx;

  // いくつかのデリミタを定義する
  const string delims(" \;=,");

  begIdx = line.find_first_not_of(delims);

  // パラメータ記述の終わりを見つける
  if (begIdx != string::npos)
  {
    endIdx = line.find_first_of(delims, begIdx);

    // 単語の終わりは行の終わり
    if (endIdx == string::npos)
    {
      endIdx = line.length();
    }
  }   

  // パラメータ値の先頭を見つける
  begIdx = line.find_first_not_of(delims, endIdx);

  // パラメータ値の終わりを見つける
  if(begIdx != string::npos)
  {
    endIdx = line.find_first_of(delims, begIdx);

    // 単語の終わりは行の終わり
    if (endIdx == string::npos)
    {
      endIdx = line.length();
    }
  }
    
  line = line.substr(begIdx, endIdx);
}

/////////////////////////////////////////////////////////
//
//	GetNextToken関数
//
//  コメントを無視して次の文字列を取得
//
/////////////////////////////////////////////////////////
std::string iniFileLoaderBase::GetNextToken()
{ 
	// 任意のコメント行を削除
	while (CurrentLine.length() == 0)
	{
	  std::getline(file, CurrentLine);
	 
	  RemoveCommentingFromLine(CurrentLine);
	}

	// パラメータ記述の始めを見つける
	string::size_type begIdx; 
	string::size_type endIdx;
	
	// いくつかのデリミタを定義する
	const string delims(" \;=,");
	
	begIdx = CurrentLine.find_first_not_of(delims);
	
	// パラメータ記述の終わりを見つける
	if (begIdx != string::npos)
	{
		endIdx = CurrentLine.find_first_of(delims, begIdx);
		
		// 単語の終わりは行の終わり
		if (endIdx == string::npos)
		{
		  endIdx = CurrentLine.length();
		}
	}
    
	string s = CurrentLine.substr(begIdx, endIdx);
	
	if (endIdx != CurrentLine.length())
	{
		// 行からトークンを取り除く
		CurrentLine = CurrentLine.substr(endIdx+1, CurrentLine.length());
	}
	else { CurrentLine = "";}
	
	return s;
}

