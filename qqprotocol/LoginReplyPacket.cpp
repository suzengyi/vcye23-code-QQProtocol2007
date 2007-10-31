//*************************************************************************
// LoginReplyPacket.cpp: implementation of the CLoginReplyPacket class.
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
#include "LoginReplyPacket.h"
#include "Crypter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginReplyPacket::CLoginReplyPacket(CQQUser *pQQUser) :
	CBasicInPacket(QQ.QQ_CMD_LOGIN, pQQUser)
{

}

CLoginReplyPacket::~CLoginReplyPacket()
{

}

bool CLoginReplyPacket::ParseBody(BYTE *pbBuf, size_t iBufLen)
{
	// 回复码
	GET_BYTE(pbBuf, iBufLen, m_bReplyCode);
	if( m_bReplyCode == QQ.QQ_REPLY_OK )
	{
		// 001-016字节是session key
		SAFE_FREE(m_pQQUser->m_pbSessionKey);
		m_pQQUser->m_pbSessionKey = (BYTE*)malloc(QQ.QQ_LENGTH_KEY);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbSessionKey, QQ.QQ_LENGTH_KEY);

		// 017-020字节是用户QQ号
		int iQQNum;

		GET_DWORD(pbBuf, iBufLen, iQQNum);
		if( iQQNum != m_pQQUser->m_iQQNum )
			return false;

		// 021-024字节是服务器探测到的用户IP
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwClientIP);

		// 025-026字节是服务器探测到的用户端口
		GET_PORT(pbBuf, iBufLen, m_pQQUser->m_iClientPort);

		// 027-032字节解释来自Luma，未验证，但如果设到m_pQQUser中会引起后面发不了包，待查
		//  好像不是服务器的IP和端口，有问题!
		// 027-030字节是服务器自己的IP
		DWORD dwServerIP;

		GET_IP(pbBuf, iBufLen, dwServerIP);

		// 031-032字节是服务器的端口
		int iServerPort;

		GET_PORT(pbBuf, iBufLen, iServerPort);

		// 033-036字节是本次登陆时间，为什么要乘1000？因为这个时间乘以1000才对，-_-!...
		GET_DWORD(pbBuf, iBufLen, m_pQQUser->m_lLoginTime);
		m_pQQUser->m_lLoginTime *= 1000;

		// 037-038, 未知的2字节
		WORD wUnknown;

		GET_WORD(pbBuf, iBufLen, wUnknown);

		// 039-062, 认证令牌
		SAFE_FREE(m_pQQUser->m_pbAuthToken);
		m_pQQUser->m_pbAuthToken = (BYTE*)malloc(24);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbAuthToken, 24);

		// 063-066字节是一个未知服务器1的ip
		// 067-068字节是一个未知服务器1的端口
		// 069-072是一个未知服务器2的ip
		// 073-074是一个未知服务器2的端口
		// 075-076是两个未知字节
		// 077-078是两个未知字节
		OFFSET_BYTES(pbBuf, iBufLen, 16);

		// 079-110是client key，这个key用在比如登录QQ家园之类的地方
		SAFE_FREE(m_pQQUser->m_pbClientKey);
		m_pQQUser->m_pbClientKey = (BYTE*)malloc(32);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbClientKey, 32);

		// 111-122是12个未知字节
		OFFSET_BYTES(pbBuf, iBufLen, 12);

		// 123-126是上次登陆的ip
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwLastLoginIP);

		// 127-130是上次登陆的时间
		GET_DWORD(pbBuf, iBufLen, m_pQQUser->m_lLastLoginTime);
		m_pQQUser->m_lLastLoginTime *= 1000;

		// 39个未知字节
		OFFSET_BYTES(pbBuf, iBufLen, 39);
	}
	else if( m_bReplyCode == QQ.QQ_REPLY_LOGIN_FAIL )
	{
		// 登录失败，我们得到服务器发回来的消息
//		GET_STRING(pbBuf, iBufLen, m_strReplyMessage, QQ.QQ_CHARSET_DEFAULT);
	}
	else if( m_bReplyCode == QQ.QQ_REPLY_LOGIN_REDIRECT )
	{
		// 登陆重定向，可能是为了负载平衡
		// 001-004字节是用户QQ号
		int iQQNum;

		GET_DWORD(pbBuf, iBufLen, iQQNum);
		if( iQQNum != m_pQQUser->m_iQQNum )
			return false;

		// 005-008字节是重定向到的服务器IP
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwServerIP);

		// 009-010字节是重定向到的服务器的端口
		GET_PORT(pbBuf, iBufLen, m_pQQUser->m_iServerPort);
	}
	return true;

}

size_t CLoginReplyPacket::DecryptBody(BYTE *pbBuf, size_t iBufLen, BYTE *pbBody, size_t iBodyLen) const
{
	CCrypter Crypter;
	size_t iRetVal;

	iRetVal = Crypter.Decrypt(pbBuf, 0, iBufLen, m_pQQUser->GetPasswordKey(),
		pbBody, iBodyLen);
	if( iRetVal == -1 )
	{
		iRetVal = Crypter.Decrypt(pbBuf, 0, iBufLen, m_pQQUser->GetInitKey(),
			pbBody, iBodyLen);
	}
	return iRetVal;

}
