#pragma once
#include <windows.h>
#include <tchar.h>
//ȥ���ַ�������ǰ��Ŀո�
TCHAR *DelSpace(TCHAR *szData);

//����ע������ȡ��Ȩ��(KEY_READ||KEY_WRITE||KEY_ALL_ACCESS)
int SetKeySecurityEx(HKEY MainKey,LPCTSTR SubKey,DWORD security);
//��ȡע����ָ����������(Mode:0-����ֵ���� 1-�����Ӽ� 2-����ָ������ 3-�жϸü��Ƿ����)
int  ReadRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,TCHAR *szData,LPBYTE szBytes,DWORD lbSize,int Mode);
//дע����ָ����������(Mode:0-�½������� 1-���ü����� 2-ɾ��ָ���� 3-ɾ��ָ������)
int WriteRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,LPCTSTR szData,DWORD dwData,int Mode);
