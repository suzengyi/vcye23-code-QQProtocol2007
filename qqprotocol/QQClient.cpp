//*************************************************************************
// QQClient.cpp: implementation of the CQQClient class.
// Version : 1.000
// Date : July 2007
// Author : Ye Feng
// Email :  vcye23@gmail.com
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQClient.h"

#include "QQ.h"
#include "BasicFamilyParser.h"
#include "LoginReplyPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQQClient::CQQClient() :
	m_pQQUser(NULL),
	m_uiSocket(INVALID_SOCKET),
	m_bLogin(false),
	m_hevLoginCancel(NULL),
	m_hLoginThread(NULL),
	m_dwLoginThreadID(0),
	m_sMessageID(0)
{
	InitializeCriticalSection(&m_csLogin);

}

CQQClient::~CQQClient()
{
	CancelLogin();
	DeleteCriticalSection(&m_csLogin);
	if( m_uiSocket != INVALID_SOCKET )
	{
		closesocket(m_uiSocket);
		m_uiSocket = INVALID_SOCKET;
	}

}

void CQQClient::SetQQUser(CQQUser *pQQUser)
{
	m_pQQUser = pQQUser;

}

int CQQClient::CreateSocket()
{
	if( m_uiSocket == INVALID_SOCKET )
	{
		m_uiSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if( m_uiSocket == INVALID_SOCKET )
			return -1;

		struct sockaddr_in saClient;

		saClient.sin_family = AF_INET;
		saClient.sin_addr.s_addr = htonl(INADDR_ANY);
		saClient.sin_port = htons(0);
		if( bind(m_uiSocket, (const sockaddr*)&saClient, sizeof(sockaddr_in)) == SOCKET_ERROR )
			return -1;
	}
	return 0;

}

int CQQClient::Login()
{
	if( IsLogin() )
		return 0;

	int iRetVal;

	if( m_pQQUser == NULL )
		return -1;
	iRetVal = CreateSocket();
	if( iRetVal < 0 )
		return iRetVal;

	m_hLoginThread = CreateThread(NULL, 0, s_LoginProc, this, CREATE_SUSPENDED, &m_dwLoginThreadID);

	SetThreadPriority(m_hLoginThread, THREAD_PRIORITY_BELOW_NORMAL);
	ResumeThread(m_hLoginThread);
	return 0;

}

void CQQClient::Logout()
{
	if( !IsLogin() )
		return;

	sockaddr_in saServer;
	int iServerLen = sizeof(saServer);
	BYTE pbBuf[MAX_SEND_PACKET_SIZE];
	size_t iBufLen;
	CBasicFamilyParser BasicFamilyParser;
	CBasicOutPacket* pBasicOutPacket;

	pBasicOutPacket = BasicFamilyParser.CreateOutPacket(QQ.QQ_CMD_LOGOUT, m_pQQUser);
	if( pBasicOutPacket == NULL )
		return;

	iBufLen = pBasicOutPacket->FillBuffer(pbBuf, sizeof(pbBuf));
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = m_pQQUser->GetServerIP();
	saServer.sin_port = m_pQQUser->GetServerPort();
	sendto(m_uiSocket, (const char*)pbBuf, iBufLen, 0, (const sockaddr*)&saServer, iServerLen);
	SAFE_DELETE(pBasicOutPacket);

	EnterCriticalSection(&m_csLogin);
	m_bLogin = false;
	LeaveCriticalSection(&m_csLogin);

}

void CQQClient::CancelLogin()
{
	if( !IsLogin() )
		return;

	if( m_hevLoginCancel != NULL )
	{
		SetEvent(m_hevLoginCancel);
		if( m_hLoginThread != NULL )
		{
			WaitForSingleObject(m_hLoginThread, INFINITE);
			CloseHandle(m_hLoginThread);
			m_hLoginThread = NULL;
		}
		CloseHandle(m_hevLoginCancel);
		m_hevLoginCancel = NULL;
	}

}

bool CQQClient::IsLogin() const
{
	bool bLogin;

	EnterCriticalSection(&m_csLogin);
	bLogin = m_bLogin;
	LeaveCriticalSection(&m_csLogin);
	return bLogin;

}

DWORD CALLBACK CQQClient::s_LoginProc(LPVOID lpvData)
{
	int iRetVal;
	CQQClient* pQQClient = reinterpret_cast<CQQClient*>(lpvData);
	SOCKET uiSocket = pQQClient->m_uiSocket;
	CQQUser* pQQUser = pQQClient->m_pQQUser;
	sockaddr_in saServer;
	int iServerLen = sizeof(saServer);
	fd_set fset;
	timeval timeout;

	saServer.sin_family = AF_INET;
	FD_ZERO(&fset);
	FD_SET(uiSocket, &fset);
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	BYTE pbBuf[MAX_SEND_PACKET_SIZE];
	size_t iBufLen;
	CBasicFamilyParser BasicFamilyParser;
	CBasicOutPacket* pBasicOutPacket;
	CBasicInPacket* pBasicInPacket;

	pBasicOutPacket = BasicFamilyParser.CreateOutPacket(QQ.QQ_CMD_REQUEST_LOGIN_TOKEN, pQQClient->m_pQQUser);
	if( pBasicOutPacket != NULL )
	{
		iBufLen = pBasicOutPacket->FillBuffer(pbBuf, sizeof(pbBuf));
		saServer.sin_addr.s_addr = pQQUser->GetServerIP();
		saServer.sin_port = pQQUser->GetServerPort();
		sendto(uiSocket, (const char*)pbBuf, iBufLen, 0, (const sockaddr*)&saServer, iServerLen);
		SAFE_DELETE(pBasicOutPacket);

		iRetVal = select(0, &fset, NULL, NULL, &timeout);
		if( iRetVal == SOCKET_ERROR ||	// 出错
			iRetVal == 0 )	// 超时
		{
			pQQClient->LoginFail(-1);
			return 0;
		}

		iBufLen = recvfrom(uiSocket, (char*)pbBuf, sizeof(pbBuf), 0, (sockaddr*)&saServer, &iServerLen);
		if( iBufLen == SOCKET_ERROR )
		{
			pQQClient->LoginFail(-2);
			return 0;
		}

		pBasicInPacket = BasicFamilyParser.ParseInPacket(pbBuf, iBufLen, pQQUser);
		if( pBasicInPacket == NULL )
		{
			pQQClient->LoginFail(-2);
			return 0;
		}

		short sCommand = pBasicInPacket->GetCommand();

		SAFE_DELETE(pBasicInPacket);
		if( sCommand != QQ.QQ_CMD_REQUEST_LOGIN_TOKEN )
		{
			pQQClient->LoginFail(-2);
			return 0;
		}

		BYTE bReplyCode;

		for( int iLogin = 0; iLogin < 5; iLogin++ )	// 至多登录5次
		{
			pBasicOutPacket = BasicFamilyParser.CreateOutPacket(QQ.QQ_CMD_LOGIN, pQQUser);
			if( pBasicOutPacket != NULL )
			{
				iBufLen = pBasicOutPacket->FillBuffer(pbBuf, sizeof(pbBuf));
				saServer.sin_addr.s_addr = pQQUser->GetServerIP();
				saServer.sin_port = pQQUser->GetServerPort();
				sendto(uiSocket, (const char*)pbBuf, iBufLen, 0, (const sockaddr*)&saServer, iServerLen);
				SAFE_DELETE(pBasicOutPacket);

				do {
					iRetVal = select(0, &fset, NULL, NULL, &timeout);
					if( iRetVal == SOCKET_ERROR ||	// 出错
						iRetVal == 0 )	// 超时
					{
						pQQClient->LoginFail(-3);
						return 0;
					}

					iBufLen = recvfrom(uiSocket, (char*)pbBuf, sizeof(pbBuf), 0, (sockaddr*)&saServer, &iServerLen);
					if( iBufLen == SOCKET_ERROR )
					{
						pQQClient->LoginFail(-4);
						return 0;
					}

					pBasicInPacket = BasicFamilyParser.ParseInPacket(pbBuf, iBufLen, pQQUser);
					// QQ 成功登录后收到的第一个包不一定是登录回复包！碰到过收到好友改变状态包的情况！
					if( pBasicInPacket == NULL )
					{
						continue;	//a
/*d						pQQClient->LoginFail(-4);
						return 0;
*/					}

					short sCommand = pBasicInPacket->GetCommand();

					if( sCommand == QQ.QQ_CMD_LOGIN )
						bReplyCode = ((CLoginReplyPacket*)pBasicInPacket)->m_bReplyCode;
					SAFE_DELETE(pBasicInPacket);
					if( sCommand != QQ.QQ_CMD_LOGIN )
					{
						continue;	//a
/*d						pQQClient->LoginFail(-4);
						return 0;
*/					}
					break;
				}while( sCommand != QQ.QQ_CMD_LOGIN );

				if( bReplyCode == QQ.QQ_REPLY_OK )
					break;
				else if( bReplyCode == QQ.QQ_REPLY_LOGIN_FAIL )
					break;
			}
			else
			{
				pQQClient->LoginFail(-6);
				return 0;
			}
		}
		if( bReplyCode == QQ.QQ_REPLY_OK )
		{
			EnterCriticalSection(&pQQClient->m_csLogin);
			pQQClient->m_bLogin = true;
			LeaveCriticalSection(&pQQClient->m_csLogin);

			pQQClient->LoginSuccess();
		}
		else if( bReplyCode == QQ.QQ_REPLY_LOGIN_FAIL )
		{
			pQQClient->LoginFail(0);
		}
		else if( iLogin >= 5 )
		{
			pQQClient->LoginFail(-5);
		}
		else
		{
			pQQClient->LoginFail(-6);
		}
	}
	else
		pQQClient->LoginFail(-6);
	return 0;

}

void CQQClient::LoginSuccess()
{

}

void CQQClient::LoginFail(int iError)
{

}
