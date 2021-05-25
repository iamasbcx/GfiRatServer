#include "pch.h"
#include "LFileName.h"

#define  LANG_UNICODE    0
#define  LANG_ANSI       1
LFileName::LFileName(wchar_t* lpFileName)
{
	ZeroMemory(wstr_FullFileName, MAX_PATH);
	ZeroMemory(wstr_FileName, MAX_PATH);
	ZeroMemory(wstr_PathName, MAX_PATH);

	wcscpy(wstr_FullFileName, lpFileName);
	int i = wcslen(wstr_FullFileName);
	for (; i > 0; i--)
	{
		if (wstr_FullFileName[i - 1] == L'\\')
		{
			wcscpy(wstr_FileName, wstr_FullFileName + i);
			break;
		}
	}
	memcpy(wstr_PathName, wstr_FullFileName, i * sizeof(wchar_t));
	AnsiOrUnicode = LANG_UNICODE;

}
LFileName::LFileName(char* lpFileName)
{
	ZeroMemory(str_FullFileName, MAX_PATH);
	ZeroMemory(str_FileName, MAX_PATH);
	ZeroMemory(str_ExpandName, MAX_PATH);
	ZeroMemory(str_PathName, MAX_PATH);
	ZeroMemory(str_Name, MAX_PATH);
	strcpy(str_FullFileName, lpFileName);
	int i = strlen(str_FullFileName);
	//�õ��ļ���
	for (; i > 0; i--)
	{
		if (str_FullFileName[i - 1] == '\\')
		{
			strcpy(str_FileName, str_FullFileName + i);
			break;
		}
	}
	memcpy(str_PathName, str_FullFileName, i);
	//�õ���չ��
	i = strlen(str_FileName);
	for (; i > 0; i--)
	{
		if (str_FileName[i - 1] == '.')
		{
			strcpy(str_ExpandName, str_FileName + i);
			break;
		}
	}
	memcpy(str_Name, str_FileName, i);
	str_Name[i - 1] = 0;
	AnsiOrUnicode = LANG_ANSI;

}
LFileName::LFileName()
{
	ZeroMemory(wstr_FullFileName, MAX_PATH);
	ZeroMemory(wstr_FileName, MAX_PATH);
	ZeroMemory(wstr_PathName, MAX_PATH);
	ZeroMemory(str_FullFileName, MAX_PATH);
	ZeroMemory(str_FileName, MAX_PATH);
	ZeroMemory(str_ExpandName, MAX_PATH);
	ZeroMemory(str_PathName, MAX_PATH);
}
LFileName::~LFileName(void)
{

}

// �õ��ļ���
wchar_t* LFileName::getFileName(void)
{

	return wstr_FileName;
}

// �õ���ǰ·��
wchar_t* LFileName::getFilePath(void)
{
	return wstr_PathName;
}

// ��ʼ��
void LFileName::init(wchar_t* lpFileName)
{
	wcscpy(wstr_FullFileName, lpFileName);
	int i = wcslen(wstr_FullFileName);
	for (; i > 0; i--)
	{
		if (wstr_FullFileName[i - 1] == L'\\')
		{
			wcscpy(wstr_FileName, wstr_FullFileName + i);
			break;
		}
	}
	memcpy(wstr_PathName, wstr_FullFileName, i * sizeof(wchar_t));
}

void LFileName::init(char* lpFileName)
{
	strcpy(str_FullFileName, lpFileName);
	int i = strlen(str_FullFileName);
	//�õ��ļ���
	for (; i > 0; i--)
	{
		if (str_FullFileName[i - 1] == '\\')
		{
			strcpy(str_FileName, str_FullFileName + i);
			break;
		}
	}
	memcpy(str_PathName, str_FullFileName, i);
	//�õ���չ��
	i = strlen(str_FileName);
	for (; i > 0; i--)
	{
		if (str_FileName[i - 1] == '.')
		{
			strcpy(str_ExpandName, str_FileName + i);
			break;
		}
	}
	memcpy(str_Name, str_FileName, i);
	str_Name[i - 1] = 0;
	AnsiOrUnicode = LANG_ANSI;
}


// ��ʼ���ļ���
void LFileName::initFileName(char* lpFileName)
{
	strcpy(str_FileName, lpFileName);
	int i = strlen(str_FileName);
	for (; i > 0; i--)
	{
		if (str_FileName[i - 1] == '.')
		{
			strcpy(str_ExpandName, str_FileName + i);
			break;
		}
	}
	memcpy(str_Name, str_FileName, i);
	str_Name[i - 1] = 0;
	AnsiOrUnicode = LANG_ANSI;
}


char* LFileName::getFileNameA(void)
{
	return str_FileName;
}

char* LFileName::getFilePathA(void)
{
	return str_PathName;
}

// �õ��ļ���չ��
char* LFileName::getFileExpand(void)
{
	return str_ExpandName;
}

// �õ����ļ���
char* LFileName::getNameA(void)
{
	return str_Name;
}
