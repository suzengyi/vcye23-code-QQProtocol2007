//*************************************************************************
// QQUser.cpp: implementation of the CQQUser class.
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
#include "QQUser.h"
#include "md5.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQQUser::CQQUser(int iQQNum, LPCTSTR lpszPassword) :
	m_iQQNum(iQQNum),
	m_bLoginMode(QQ.QQ_LOGIN_MODE_NORMAL),
	m_bUDP(true),
	m_bStatus(QQ.QQ_STATUS_ONLINE),
	m_bShowFakeCam(false),
	m_bFakeIP(false)
{
	m_pbPasswordKey = (BYTE*)malloc(QQ.QQ_LENGTH_KEY);

	m_pbInitKey = (BYTE*)malloc(QQ.QQ_LENGTH_KEY);
	srand(time(NULL));
	m_sSequence = (short)rand();
	for( int n = 0; n < QQ.QQ_LENGTH_KEY; n++ )
	{
		m_pbInitKey[n] = (BYTE)(rand() % 256);
	}

	m_pbLoginToken = NULL;
	m_iLoginTokenLen = 0;
	m_pbSessionKey = NULL;
	m_pbAuthToken = NULL;
	m_pbClientKey = NULL;

	SetPassword(lpszPassword);

}

CQQUser::~CQQUser()
{
	SAFE_FREE(m_pbClientKey);
	SAFE_FREE(m_pbAuthToken);
	SAFE_FREE(m_pbSessionKey);
	SAFE_FREE(m_pbLoginToken);
	SAFE_FREE(m_pbInitKey);
	SAFE_FREE(m_pbPasswordKey);

}

void CQQUser::SetPassword(LPCSTR lpszPassword)
{
	m_strPassword = lpszPassword;

	BYTE abFirstTimeMD5[17] = { 0 };
	int iFirstTimeMD5Len = 16;
	BYTE abSecondTimeMD5[17] = { 0 };
	int iSecondTimeMD5Len = 16;

	_EncryptString(lpszPassword, abFirstTimeMD5, iFirstTimeMD5Len);
	abFirstTimeMD5[iFirstTimeMD5Len] = (BYTE)0;
	_EncryptString((const char*)abFirstTimeMD5, abSecondTimeMD5, iSecondTimeMD5Len);
	abSecondTimeMD5[iSecondTimeMD5Len] = (BYTE)0;
	memcpy(m_pbPasswordKey, abSecondTimeMD5, QQ.QQ_LENGTH_KEY);

}

short CQQUser::GetNextSequence()
{
	m_sSequence++;

	// 为了兼容iQQ
	// iQQ把序列号的高位都为0，如果为1，它可能会拒绝，wqfox称是因为TX是这样做的
	m_sSequence &= 0x7FFF;
	if( m_sSequence == 0 )
		m_sSequence++;
	return m_sSequence;

}

void CQQUser::SetServerIP(DWORD dwServerIP)
{
	m_dwServerIP = dwServerIP;

}

void CQQUser::SetServerPort(int iServerPort)
{
	m_iServerPort = iServerPort;

}

DWORD CQQUser::GetServerIP() const
{
	return m_dwServerIP;

}

int CQQUser::GetServerPort() const
{
	return m_iServerPort;

}

int CQQUser::GetQQNum() const
{
	return m_iQQNum;

}

short CQQUser::GetHead() const
{
	return m_sHead;

}

BYTE CQQUser::GetLoginMode() const
{
	return m_bLoginMode;

}

void CQQUser::SetHiddenLogin(bool bHidden)
{
	m_bLoginMode = bHidden ? QQ.QQ_LOGIN_MODE_HIDDEN : QQ.QQ_LOGIN_MODE_NORMAL;

}

bool CQQUser::IsUDP() const
{
	return m_bUDP;

}

void CQQUser::SetUDP(bool bUDP)
{
	m_bUDP = bUDP;

}

BYTE CQQUser::GetStatus() const
{
	return m_bStatus;

}

void CQQUser::SetStatus(BYTE bStatus)
{
	m_bStatus = bStatus;

}

bool CQQUser::IsShowFakeCam() const
{
	return m_bShowFakeCam;

}

void CQQUser::SetShowFakeCam(bool bShowFakeCam)
{
	m_bShowFakeCam = bShowFakeCam;

}

BYTE* CQQUser::GetPasswordKey() const
{
	return m_pbPasswordKey;

}

BYTE* CQQUser::GetInitKey() const
{
	return m_pbInitKey;

}

int CQQUser::GetLoginTokenLen() const
{
	return m_iLoginTokenLen;

}

BYTE* CQQUser::GetLoginToken() const
{
	return m_pbLoginToken;

}

BYTE* CQQUser::GetSessionKey() const
{
	return m_pbSessionKey;

}

bool CQQUser::GetFakeIP() const
{
	return m_bFakeIP;

}

void CQQUser::SetFakeIP(bool bFakeIP)
{
	m_bFakeIP = bFakeIP;

}
