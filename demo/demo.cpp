// demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "demo.h"
#include "../QQProtocol/QQUser.h"
#include "../QQProtocol/QQClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		if( argc < 3 )
			cerr << _T("Format: demo qqnumber password") << endl;

		WSADATA wsaData;

		if( WSAStartup(MAKEWORD(2,2),&wsaData) != 0 )
			return FALSE;

		CString strQQNum(argv[1]);
		CString strPassword(argv[2]);
		int iQQNum = _ttoi(strQQNum);
		CQQUser* pQQUser = new CQQUser(iQQNum, strPassword);
		CQQClient QQClient;

		char szServerIP[] = "219.133.49.171";
		DWORD dwServerIP = inet_addr(szServerIP);
		u_short usServerPort = htons(8000);

		pQQUser->SetServerIP(dwServerIP);
		pQQUser->SetServerPort(usServerPort);
		pQQUser->SetUDP(true);
		QQClient.SetQQUser(pQQUser);
		if( QQClient.Login() < 0 )
		{
		}
		Sleep(10 * 60 * 1000);
	}

	return nRetCode;
}


