/*********************************************************************
* SEU_QQwry.h
*
* �����ռ�����
*
* ˵��:����IP���ݿ� QQWry.dat�Ĳ�����
* 
* ���ִ�����Դ������,���������ʹ��,�������޸�.���ǲ���������ҵ��;
*********************************************************************/

#pragma once

#define MAXBUF 50

typedef struct _tagEndInfo
{
	BYTE b0;
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE buf[MAXBUF];
	BYTE bMode;
	int offset1;
	int offset2;
}EndInfo,PEndInfo;

typedef struct _tagIPOFF
{
	BYTE b0;
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE off1;
	BYTE off2;
	BYTE off3;
}IPOFF,*PIPOFF;

typedef struct _tagBE
{
	int uBOff;
	int uEOff;
}BE,*PBE;

class SEU_QQwry  
{
public://��ͨ�û��ӿں���
	void SetPath(CString path);//����QQWry.dat��·��
    CString IPtoAdd(CString szIP);//������IP,����IP��Ӧ�ĵ�ַ   
	void SaveToFile(CString Name);//��QQWry.dat�ļ������ݵ���Ϊָ�����ļ�

public://�߼��û�����	
	DWORD m_dwLastIP;
	CString GetCountryLocal(int index);
	DWORD GetSIP(int index);
	DWORD IPtoDWORD(CString szIP);
	int GetIndex(CString szIP);
	CString GetStr(void);
	CString GetCountryLocal(BYTE bMode,int ioffset);
	CString GetStr(int ioffset);
	int GetRecordCount(void);
	int m_i;
	int GetStartIPInfo(int iIndex);
	CString m_buf;
	bool GetBE(void);
	bool OpenQQwry(CString szFileName);//��QQwry���ݿ�
	void CloseQQwry(void);//�ر�QQwry���ݿ�
	BE m_be;
	IPOFF m_ipoff;
	EndInfo m_ei;
	SEU_QQwry();
	virtual ~SEU_QQwry();
private:
	bool m_bOpen;
	CFile m_file;
};

