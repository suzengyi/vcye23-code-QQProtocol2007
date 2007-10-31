//*************************************************************************
// QQClient.h: interface for the CQQClient class.
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

#if !defined(AFX_QQCLIENT_H__AA3BEAE9_8CE6_4E3D_9AF8_09E716733750__INCLUDED_)
#define AFX_QQCLIENT_H__AA3BEAE9_8CE6_4E3D_9AF8_09E716733750__INCLUDED_

#include "QQUser.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQQClient  
{
public:
	int CreateSocket();
	void SetQQUser(CQQUser* pQQUser);

	int Login();
	void Logout();
	void CancelLogin();
	bool IsLogin() const;

	CQQClient();
	virtual ~CQQClient();

protected:
	virtual void LoginFail(int iError);
	// iError : 0 服务器返回错误
	//			-1 收RequestLoginTokenPacket反馈包超时
	//			-2 收RequestLoginTokenPacket反馈包错误
	//			-3 收LoginPacket反馈包超时
	//			-4 收LoginPacket反馈包错误
	//			-5 重定位太多次
	//			-6 未知错误
	virtual void LoginSuccess();

	CQQUser* m_pQQUser;
	SOCKET m_uiSocket;
	bool m_bLogin;
	mutable CRITICAL_SECTION m_csLogin;

	HANDLE m_hevLoginCancel;

	HANDLE m_hLoginThread;
	DWORD m_dwLoginThreadID;

	short m_sMessageID;

private:
	static DWORD CALLBACK s_LoginProc(LPVOID lpvData);
};

#endif // !defined(AFX_QQCLIENT_H__AA3BEAE9_8CE6_4E3D_9AF8_09E716733750__INCLUDED_)
