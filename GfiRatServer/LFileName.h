#pragma once



class LFileName
{
public:
	LFileName(wchar_t* lpFileName);
	LFileName(char* lpFileName);
	LFileName();
	~LFileName(void);
protected:
	wchar_t  wstr_FullFileName[MAX_PATH];   //�ļ�ȫ��
	wchar_t  wstr_FileName[MAX_PATH]; //�ļ���
	wchar_t  wstr_PathName[MAX_PATH];   //·��

	char str_FullFileName[MAX_PATH];     //ȫ��
	char str_FileName[MAX_PATH];        //�ļ���
	char str_PathName[MAX_PATH];         //·����
	char str_ExpandName[MAX_PATH];       //��չ��  
	char str_Name[MAX_PATH];              //���ļ���

	BOOL  AnsiOrUnicode;     // 0  unicode  1 ansi 


public:
	// �õ��ļ���
	wchar_t* getFileName(void);
	// �õ���ǰ·��
	wchar_t* getFilePath(void);
	// ��ʼ��
	void init(wchar_t* lpFileName);
	void init(char* lpFileName);
	char* getFileNameA(void);
	char* getFilePathA(void);
	// �õ��ļ���չ��
	char* getFileExpand(void);
	// �õ����ļ���
	char* getNameA(void);
	// ��ʼ���ļ���
	void initFileName(char* lpFileName);
};
