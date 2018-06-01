/*=======================================================================

	�C���t�@�C�����[�_�[�x�[�X [inFileLoaderBase.cpp]

												
												�����C��

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	�w�b�_�t�@�C��
//
/////////////////////////////////////////////////////////
#include "iniFileLoaderBase.h"
using std::string;


/////////////////////////////////////////////////////////
//
//	RemoveCommentingFromLine�֐�
//	(�e�L�X�g�s����R�����g���폜����)
//
/////////////////////////////////////////////////////////
void RemoveCommentingFromLine(string& line)
{
   // �R�����g���������č폜����
   string::size_type idx = line.find('//');

   if (idx != string::npos)
   {
     // �R�����g��؂���
     line = line.substr(0, idx);
   }
}

/////////////////////////////////////////////////////////
//
//	GetNextParameter�֐�
//
//		���̗L���ȃp�����[�^�̃e�L�X�g�t�@�C��������
//		�R�����g��j�����A���̒l�𕶎���Ƃ��ĕԂ�
//
/////////////////////////////////////////////////////////
string iniFileLoaderBase::GetNextParameter()
{
 
  // ����͎��̃p�����[�^��ێ����镶����ɂȂ�܂�
  std::string line;
  
  std::getline(file, line);
   
  RemoveCommentingFromLine(line);

  // �s�̒������[���̏ꍇ�́A�t�@�C�����玟�̍s���擾����
  if (line.length() == 0)
  {
    return GetNextParameter();
  }

  GetParameterValueAsString(line);  
    
  return line;
}

/////////////////////////////////////////////////////////
//
//	GetParameterValueAsString�֐�
//
//	�^����ꂽ�e�L�X�g�s��n���ƁA
//	���̊֐��̓p�����[�^�L�q���폜���A
//	�p�����[�^��std :: string�Ƃ��ĕԂ�
//
/////////////////////////////////////////////////////////
void iniFileLoaderBase::GetParameterValueAsString(string& line)
{
  // �p�����[�^�L�q�̎n�߂�������
  string::size_type begIdx;
  string::size_type endIdx;

  // �������̃f���~�^���`����
  const string delims(" \;=,");

  begIdx = line.find_first_not_of(delims);

  // �p�����[�^�L�q�̏I����������
  if (begIdx != string::npos)
  {
    endIdx = line.find_first_of(delims, begIdx);

    // �P��̏I���͍s�̏I���
    if (endIdx == string::npos)
    {
      endIdx = line.length();
    }
  }   

  // �p�����[�^�l�̐擪��������
  begIdx = line.find_first_not_of(delims, endIdx);

  // �p�����[�^�l�̏I����������
  if(begIdx != string::npos)
  {
    endIdx = line.find_first_of(delims, begIdx);

    // �P��̏I���͍s�̏I���
    if (endIdx == string::npos)
    {
      endIdx = line.length();
    }
  }
    
  line = line.substr(begIdx, endIdx);
}

/////////////////////////////////////////////////////////
//
//	GetNextToken�֐�
//
//  �R�����g�𖳎����Ď��̕�������擾
//
/////////////////////////////////////////////////////////
std::string iniFileLoaderBase::GetNextToken()
{ 
	// �C�ӂ̃R�����g�s���폜
	while (CurrentLine.length() == 0)
	{
	  std::getline(file, CurrentLine);
	 
	  RemoveCommentingFromLine(CurrentLine);
	}

	// �p�����[�^�L�q�̎n�߂�������
	string::size_type begIdx; 
	string::size_type endIdx;
	
	// �������̃f���~�^���`����
	const string delims(" \;=,");
	
	begIdx = CurrentLine.find_first_not_of(delims);
	
	// �p�����[�^�L�q�̏I����������
	if (begIdx != string::npos)
	{
		endIdx = CurrentLine.find_first_of(delims, begIdx);
		
		// �P��̏I���͍s�̏I���
		if (endIdx == string::npos)
		{
		  endIdx = CurrentLine.length();
		}
	}
    
	string s = CurrentLine.substr(begIdx, endIdx);
	
	if (endIdx != CurrentLine.length())
	{
		// �s����g�[�N������菜��
		CurrentLine = CurrentLine.substr(endIdx+1, CurrentLine.length());
	}
	else { CurrentLine = "";}
	
	return s;
}

