//*************************************************************************
// QQUser.h: interface for the CQQUser class.
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

#if !defined(AFX_QQUSER_H__933FF93B_1B19_4240_93F2_CCAF465B7A87__INCLUDED_)
#define AFX_QQUSER_H__933FF93B_1B19_4240_93F2_CCAF465B7A87__INCLUDED_

#include "QQ.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRequestLoginTokenReplyPacket;
class CLoginReplyPacket;

#define SAFE_FREE(p)	\
if( p != NULL )	\
{	\
	free(p);	\
	p = NULL;	\
}

#define SAFE_DELETE(p)	\
if( p != NULL )	\
{	\
	delete p;	\
	p = NULL;	\
}

class CQQUser  
{
public:
	friend class CRequestLoginTokenReplyPacket;
	friend class CLoginReplyPacket;

	CQQUser(int iQQNum, LPCTSTR lpszPassword);
	virtual ~CQQUser();

	void SetPassword(LPCSTR lpszPassword);
	short GetNextSequence();

	void SetServerIP(DWORD dwServerIP);
	void SetServerPort(int iServerPort);
	DWORD GetServerIP() const;
	int GetServerPort() const;

	int GetQQNum() const;
	short GetHead() const;

	BYTE GetLoginMode() const;
	void SetHiddenLogin(bool bHidden);

	bool IsUDP() const;
	void SetUDP(bool bUDP);

	BYTE GetStatus() const;
	void SetStatus(BYTE bStatus);

	bool IsShowFakeCam() const;
	void SetShowFakeCam(bool bShowFakeCam);

	BYTE* GetPasswordKey() const;
	BYTE* GetInitKey() const;
	int GetLoginTokenLen() const;
	BYTE* GetLoginToken() const;
	BYTE* GetSessionKey() const;
	bool GetFakeIP() const;
	void SetFakeIP(bool bFakeIP);

protected:
	int m_iQQNum;
	short m_sHead;	// 由 CGetUserInfoReplyPacket 填充
	std::string m_strPassword;
	// 登陆模式，隐身还是非隐身
	BYTE m_bLoginMode;
	// 设置登陆服务器的方式是UDP还是TCP
	bool m_bUDP;
	// 当前的状态，比如在线，隐身等等
	BYTE m_bStatus;
	// 是否显示虚拟摄像头
	bool m_bShowFakeCam;

	short m_sSequence;

	BYTE* m_pbPasswordKey;
	BYTE* m_pbInitKey;

	// 由 CRequestLoginTokenReplyPacket 填充
	int m_iLoginTokenLen;
	BYTE* m_pbLoginToken;

	// 由 CLoginReplyPacket 填充
	BYTE* m_pbSessionKey;

	bool m_bFakeIP;

	DWORD m_dwClientIP;
	int m_iClientPort;
	DWORD m_dwServerIP;
	int m_iServerPort;
	long m_lLoginTime;
	BYTE* m_pbAuthToken;
	BYTE* m_pbClientKey;

	DWORD m_dwLastLoginIP;
	long m_lLastLoginTime;

};

#endif // !defined(AFX_QQUSER_H__933FF93B_1B19_4240_93F2_CCAF465B7A87__INCLUDED_)
