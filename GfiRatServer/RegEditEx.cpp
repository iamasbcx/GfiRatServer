
#include "pch.h"
#include "RegEditEx.h"


int mywcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	return CompareStringW(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, s1, n, s2, n);
}

wchar_t *mywcschr( wchar_t *str, wchar_t ch)
{
	while (*str)
	{
		if (*str == ch)
			return str;
		str++;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
__declspec(naked) wchar_t* __fastcall strcpyW(wchar_t *dest, const wchar_t *src)
{
	__asm
	{
		push edi
			push esi
			mov esi, ecx
			mov edi, edx
			or ecx, -1
			xor eax, eax
			repnz scasw
			not ecx
			add ecx, ecx
			mov edi, esi
			mov esi, edx
			mov eax, edi
			mov edx, ecx
			shr ecx, 2
			repnz movsd
			mov ecx, edx
			and ecx, 3
			repnz movsb
			pop esi
			pop edi
			retn
	}
}

__declspec(naked) wchar_t* __fastcall strendW(const wchar_t *s)
{
	__asm
	{
		mov edx, edi
			mov edi, ecx
			or ecx, -1
			xor eax, eax
			repnz scasw
			lea eax, [edi - 2]
			mov edi, edx
			retn
	}
}



wchar_t* __fastcall strcatW(wchar_t *dest, const wchar_t *src)
{
	strcpyW(strendW(dest), src);
	return dest;
}

//----------------------------------------------------------------------------------------------
//////////////////--------------------------------------------


//去除字符串类型前面的空格

TCHAR *DelSpacee(TCHAR *szData)
{
	int i=0 ;
	while(1)
	{
		#ifdef _UNICODE
		if(mywcsnicmp(szData+i,_T(" "),1))
		#else
		if(strnicmp(szData+i,_T(" "),1))
		#endif
			break;
		i++;			
	}
	return (szData+i);
} 

//设置注册表键读取的权限(KEY_READ||KEY_WRITE||KEY_ALL_ACCESS)
int SetKeySecurityEx(HKEY MainKey,LPCTSTR SubKey,DWORD security) 
{    
   HKEY  hKey; 
   SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY; 
   PSID pSystemSid              = NULL; 
   PSID pUserSid                = NULL; 
   SECURITY_DESCRIPTOR sd; 
   PACL    pDacl                = NULL; 
   DWORD   dwAclSize; 
   int     iResult              = 0;

   __try
   {
	   HINSTANCE advapi32 = LoadLibrary(_T("ADVAPI32.dll")); 

		typedef BOOL (WINAPI *GS)(PSID pSid); 
		GS myGetLengthSid; 
		myGetLengthSid= (GS)GetProcAddress(advapi32, "GetLengthSid"); 
	
	    typedef int (WINAPI *ROKE)(HKEY hKey,LPCTSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult); 
		ROKE myroke;  
#ifdef UNICODE
		myroke= (ROKE)GetProcAddress(advapi32,"RegOpenKeyExW");
#else
		myroke= (ROKE)GetProcAddress(advapi32,"RegOpenKeyExA");
#endif
			
		typedef BOOL (WINAPI *SSDD)(PSECURITY_DESCRIPTOR pSecurityDescriptor,BOOL bDaclPresent,PACL pDacl,BOOL bDaclDefaulted); 
		SSDD myssdd; 
		myssdd= (SSDD)GetProcAddress(advapi32,"SetSecurityDescriptorDacl");
	
		typedef BOOL (WINAPI *ISD)(PSECURITY_DESCRIPTOR pSecurityDescriptor,DWORD dwRevision); 
		ISD myisd; 
		myisd= (ISD)GetProcAddress(advapi32,"InitializeSecurityDescriptor");
	
		typedef BOOL (WINAPI *IA)(PACL pAcl,DWORD nAclLength,DWORD dwAclRevision); 
		IA myia; 
		myia= (IA)GetProcAddress(advapi32,"InitializeAcl");
		
	   	typedef BOOL (WINAPI *ADDA)(PACL pAcl,DWORD dwAceRevision,DWORD AccessMask,PSID pSid); 
		ADDA myadda;
		myadda= (ADDA)GetProcAddress(advapi32,"AddAccessAllowedAce");
		
		typedef BOOL (WINAPI *AAS)(PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,BYTE nSubAuthorityCount,DWORD dwSubAuthority0,DWORD dwSubAuthority1,DWORD dwSubAuthority2,DWORD dwSubAuthority3,DWORD dwSubAuthority4,DWORD dwSubAuthority5,DWORD dwSubAuthority6,DWORD dwSubAuthority7,PSID *pSid); 
		AAS myaas;
		myaas= (AAS)GetProcAddress(advapi32,"AllocateAndInitializeSid");

		typedef LONG (WINAPI *RSKS)(HKEY ,SECURITY_INFORMATION ,PSECURITY_DESCRIPTOR ); 
		RSKS myRegSetKeySecurity = (RSKS)GetProcAddress(advapi32,"RegSetKeySecurity");
		
	
	   if(myroke(MainKey, SubKey, 0, WRITE_DAC, &hKey)!= ERROR_SUCCESS) 
		   __leave; 
       if(!myaas(&sia,1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &pSystemSid )) 
           __leave;
       if(!myaas( &sia, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0, &pUserSid))  
           __leave; 
       dwAclSize = sizeof(ACL) + 2 * ( sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD) ) + myGetLengthSid(pSystemSid) + myGetLengthSid(pUserSid) ; 
       pDacl = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize); 
       if(pDacl == NULL) 
		   __leave; 
       if(!myia(pDacl, dwAclSize, ACL_REVISION)) 
           __leave; 
       if(!myadda( pDacl, ACL_REVISION, KEY_ALL_ACCESS, pSystemSid )) 
           __leave; 
       if(!myadda( pDacl, ACL_REVISION, (unsigned long)security, pUserSid )) 
           __leave; 
       if(!myisd(&sd, SECURITY_DESCRIPTOR_REVISION)) 
           __leave; 
       if(!myssdd(&sd, TRUE, pDacl, FALSE)) 
           __leave; 
       if(myRegSetKeySecurity(hKey, (SECURITY_INFORMATION)DACL_SECURITY_INFORMATION, &sd)!= ERROR_SUCCESS)
		   __leave;
	   iResult =1;
   }
   __finally
   {  
	   HINSTANCE advapi32 = LoadLibrary(_T("ADVAPI32.dll"));

		typedef BOOL (WINAPI *RCK)(HKEY hKey); 
		RCK myrck;
		myrck= (RCK)GetProcAddress(advapi32, "RegCloseKey");
	
		typedef BOOL (WINAPI *FREES )(PSID pSid); 
		FREES myFreeSid;
		myFreeSid= (FREES )GetProcAddress(advapi32, "FreeSid");  

	   myrck(MainKey); 
	   myrck(hKey); 
	   
	   if(pDacl !=NULL)         
		   HeapFree(GetProcessHeap(), 0, pDacl);  
       if(pSystemSid !=NULL)
	       myFreeSid(pSystemSid);
	   if(pUserSid !=NULL)
          myFreeSid(pUserSid); 
   }

   return iResult;
}
//读取注册表的指定键的数据(Mode:0-读键值数据 1-牧举子键 2-牧举指定键项 3-判断该键是否存在)
int  ReadRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,TCHAR *szData,LPBYTE szBytes,DWORD lbSize,int Mode)
{   
	HKEY   hKey;  
	int    ValueDWORD,iResult=0;
	TCHAR*  PointStr;  
	TCHAR   KeyName[32],ValueSz[MAX_PATH],ValueTemp[MAX_PATH];	
	DWORD  szSize,KnSize,dwIndex=0;	 

	memset(KeyName,0,sizeof(KeyName));
	memset(ValueSz,0,sizeof(ValueSz));
	memset(ValueTemp,0,sizeof(ValueTemp));

	HINSTANCE advapi32 = LoadLibrary(_T("ADVAPI32.dll"));
	
	typedef BOOL (WINAPI *RQVE)(HKEY hKey,LPCTSTR lpValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData);
	typedef int (WINAPI *ROKE)(HKEY hKey,LPCTSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
	typedef BOOL (WINAPI *REV)(HKEY hKey,DWORD dwIndex,LPTSTR lpValueName,LPDWORD lpcchValueName,LPDWORD lpReserved,LPDWORD lpType,LPBYTE lpData,LPDWORD lpcbData); 
	typedef BOOL (WINAPI *REKE)(HKEY hKey,DWORD dwIndex,LPTSTR lpName,LPDWORD lpcName,LPDWORD lpReserved,LPTSTR lpClass,LPDWORD lpcClass,PFILETIME lpftLastWriteTime); 

#ifdef UNICODE
	RQVE myRegQueryValueEx = (RQVE)GetProcAddress(advapi32, "RegQueryValueExW");
	ROKE myRegOpenKeyEx = (ROKE)GetProcAddress(advapi32, "RegOpenKeyExW");
	REV myRegEnumValue= (REV)GetProcAddress(advapi32, "RegEnumValueW");
	REKE myRegEnumKeyEx= (REKE)GetProcAddress(advapi32, "RegEnumKeyExW");
#else
	RQVE myRegQueryValueEx = (RQVE)GetProcAddress(advapi32, "RegQueryValueExA");
	ROKE myRegOpenKeyEx = (ROKE)GetProcAddress(advapi32, "RegOpenKeyExA");
	REV myRegEnumValue= (REV)GetProcAddress(advapi32, "RegEnumValueA");
	REKE myRegEnumKeyEx= (REKE)GetProcAddress(advapi32, "RegEnumKeyExA");
#endif
		
	typedef BOOL (WINAPI *RCK)(HKEY hKey); 
	RCK myRegCloseKey= (RCK)GetProcAddress(advapi32, "RegCloseKey");
		 
	__try
	{
		 SetKeySecurityEx(MainKey,SubKey,KEY_ALL_ACCESS);
	    	
		if(myRegOpenKeyEx(MainKey,SubKey,0,KEY_READ,&hKey) != ERROR_SUCCESS)
		{
            iResult = -1;
			__leave;
		}
		switch(Mode)		 
		{
		case 0:
			switch(Type)
			{
			case REG_SZ:
			case REG_EXPAND_SZ:				 
				szSize = sizeof(ValueSz);
				if(myRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)ValueSz,&szSize) == ERROR_SUCCESS)
				{
					lstrcpy(szData,DelSpacee(ValueSz));
					iResult =1;
				}
				break;
			case REG_MULTI_SZ:	
				szSize = sizeof(ValueSz);
				if(myRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)ValueSz,&szSize) == ERROR_SUCCESS)
				{
					#ifdef _UNICODE
					for(PointStr = ValueSz; *PointStr; PointStr = mywcschr(PointStr,0)+1)//strchr
					#else
					for(PointStr = ValueSz; *PointStr; PointStr = strchr(PointStr,0)+1)//strchr
					#endif
					{
						#ifdef _UNICODE
				//		wcsncat(ValueTemp,PointStr,sizeof(ValueTemp));
				//	    wcsncat(ValueTemp,_T(" "),sizeof(ValueTemp));
						strcatW(ValueTemp,PointStr);
					    strcatW(ValueTemp,_T(" "));
						#else
						strncat(ValueTemp,PointStr,sizeof(ValueTemp));
					    strncat(ValueTemp,_T(" "),sizeof(ValueTemp));
						#endif
					}
					lstrcpy(szData,ValueTemp);
					iResult =1;
				}
				break;				 			
			case REG_DWORD:
				szSize = sizeof(DWORD);
				if(myRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)&ValueDWORD,&szSize ) == ERROR_SUCCESS)						
				{
					wsprintf(szData,_T("%d"),ValueDWORD);
					iResult =1;
				}
				break;
            case REG_BINARY:
                szSize = lbSize;
				if(myRegQueryValueEx(hKey,Vname,NULL,&Type,szBytes,&szSize) == ERROR_SUCCESS)
					iResult =1;
				break;
			}
			break;
		case 1:
			while(1)
			{				 
				memset(ValueSz,0,sizeof(ValueSz));
				szSize   = sizeof(ValueSz);

                if(myRegEnumKeyEx(hKey,dwIndex++,ValueSz,&szSize,NULL,NULL,NULL,NULL) != ERROR_SUCCESS)
					break;
                wsprintf(ValueTemp,_T("[%s]\r\n"),ValueSz);
				lstrcat(szData,ValueTemp);
				iResult =1;
			}			 
			break;
		case 2:			  
			while(1)
			{				 
				memset(KeyName,0,sizeof(KeyName));
				memset(ValueSz,0,sizeof(ValueSz));
				memset(ValueTemp,0,sizeof(ValueTemp));
				KnSize = sizeof(KeyName);
                szSize = sizeof(ValueSz);

                if(myRegEnumValue(hKey,dwIndex++,KeyName,&KnSize,NULL,&Type,(LPBYTE)ValueSz,&szSize) != ERROR_SUCCESS)
					break;

				char szhide1[] = {'R','E','G','_','S','Z','\0'};
				char szhide2[] = {'R','E','G','_','E','X','P','A','N','D','_','S','Z','\0'};
				char szhide3[] = {'R','E','G','_','D','W','O','R','D','\0'};
				char szhide4[] = {'R','E','G','_','M','U','L','T','I','_','S','Z','\0'};
				char szhide5[] = {'R','E','G','_','B','I','N','A','R','Y','\0'};

				switch(Type)				 				
				{				     
				case REG_SZ:					 						 
					wsprintf(ValueTemp,_T("%-24s %-15s %s \r\n"),KeyName,szhide1,ValueSz);					     
					break;
				case REG_EXPAND_SZ:                   						 
					wsprintf(ValueTemp,_T("%-24s %-15s %s \r\n"),KeyName,szhide2,ValueSz);
					break;
				case REG_DWORD:
					wsprintf(ValueTemp,_T("%-24s %-15s 0x%x(%d) \r\n"),KeyName,szhide3,ValueSz,int(ValueSz));
					break;
				case REG_MULTI_SZ:
                    wsprintf(ValueTemp,_T("%-24s %-15s \r\n"),KeyName,szhide4);
					break;
			    case REG_BINARY:
					wsprintf(ValueTemp,_T("%-24s %-15s \r\n"),KeyName,szhide5);
					break;
				}
				lstrcat(szData,ValueTemp);
				iResult =1;
			}
			break;
		case 3:
            iResult =1;
			break;
		}
	}
	__finally
	{
        myRegCloseKey(MainKey);
		myRegCloseKey(hKey);
	}
	if(advapi32)
		FreeLibrary(advapi32);
     
	return iResult;
}
//写注册表的指定键的数据(Mode:0-新建键数据 1-设置键数据 2-删除指定键 3-删除指定键项)
int WriteRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,LPCTSTR szData,DWORD dwData,int Mode)
{
	HKEY  hKey; 
	DWORD dwDisposition;    
	int   iResult =0;

	HINSTANCE advapi32 = LoadLibrary(_T("ADVAPI32.dll"));

	typedef BOOL (WINAPI *RCKX)(HKEY hKey,LPCTSTR lpSubKey,DWORD Reserved,LPTSTR lpClass,DWORD dwOptions,REGSAM samDesired,LPSECURITY_ATTRIBUTES lpSecurityAttributes,PHKEY phkResult,LPDWORD lpdwDisposition); 
	RCKX myrckx;
#ifdef UNICODE
	myrckx= (RCKX)GetProcAddress(advapi32, "RegCreateKeyExW");
#else
	myrckx= (RCKX)GetProcAddress(advapi32, "RegCreateKeyExA");
#endif
			
	typedef BOOL (WINAPI *RSVE)(HKEY hKey,LPCTSTR lpValueName,DWORD Reserved,DWORD dwType,const BYTE *lpData,DWORD cbData); 
	RSVE myrsve; 
#ifdef UNICODE
	myrsve= (RSVE)GetProcAddress(advapi32, "RegSetValueExW");
#else
	myrsve= (RSVE)GetProcAddress(advapi32, "RegSetValueExA");
#endif
		
	typedef BOOL (WINAPI *RDK)(HKEY hKey,LPCTSTR lpSubKey); 
	RDK myrdk; 
#ifdef UNICODE
	myrdk= (RDK)GetProcAddress(advapi32, "RegDeleteKeyW");
#else
	myrdk= (RDK)GetProcAddress(advapi32, "RegDeleteKeyA");
#endif
			
	typedef BOOL (WINAPI *RDV)(HKEY hKey,LPCTSTR lpValueName); 
	RDV myrdv;
#ifdef UNICODE
	myrdv= (RDV)GetProcAddress(advapi32, "RegDeleteValueW");
#else
	myrdv= (RDV)GetProcAddress(advapi32, "RegDeleteValueA");
#endif
			
  	typedef int (WINAPI *ROKE)(HKEY hKey,LPCTSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult); 
	ROKE myroke; 
#ifdef UNICODE
	myroke= (ROKE)GetProcAddress(advapi32, "RegOpenKeyExW");
#else
	myroke= (ROKE)GetProcAddress(advapi32, "RegOpenKeyExA");
#endif
	
	typedef BOOL (WINAPI *RCK)(HKEY hKey); 
	RCK myrck;
	myrck= (RCK)GetProcAddress(advapi32, "RegCloseKey"); 
	
	__try
	{	
	//	SetKeySecurityEx(MainKey,Subkey,KEY_ALL_ACCESS);

		switch(Mode)		
		{			
		case 0:
			if(myrckx(MainKey,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dwDisposition) != ERROR_SUCCESS)
				__leave;		 
		case 1:	
			if(myroke(MainKey,SubKey,0,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)					 
				__leave;		 		 			 
			switch(Type)
			{		 
			case REG_SZ:		 
			case REG_EXPAND_SZ:	
#ifdef UNICODE
				if(myrsve(hKey,Vname,0,Type,(LPBYTE)szData,lstrlen(szData)*2+1) == ERROR_SUCCESS) 
#else
				if(myrsve(hKey,Vname,0,Type,(LPBYTE)szData,lstrlen(szData)+1) == ERROR_SUCCESS)
#endif
					iResult =1;				 			
				break;
		    case REG_DWORD:
                if(myrsve(hKey,Vname,0,Type,(LPBYTE)&dwData,sizeof(DWORD)) == ERROR_SUCCESS)  
		            iResult =1;				 			 
			    break;
		    case REG_BINARY:
			    break;
			}
			break;				
		case 2:
            if(myroke(MainKey,SubKey,NULL,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)				
				__leave;                
			if (myrdk(hKey,Vname) == ERROR_SUCCESS)		        
				iResult =1;
			break;		
		case 3:
            if(myroke(MainKey,SubKey,NULL,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)				
				__leave;                
			if (myrdv(hKey,Vname) == ERROR_SUCCESS)		        
				iResult =1;
			break;
		}
	}
	__finally 
	{
	   myrck(MainKey);		
	   myrck(hKey);
	}
	return iResult;
}