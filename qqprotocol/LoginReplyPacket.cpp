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
	// �ظ���
	GET_BYTE(pbBuf, iBufLen, m_bReplyCode);
	if( m_bReplyCode == QQ.QQ_REPLY_OK )
	{
		// 001-016�ֽ���session key
		SAFE_FREE(m_pQQUser->m_pbSessionKey);
		m_pQQUser->m_pbSessionKey = (BYTE*)malloc(QQ.QQ_LENGTH_KEY);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbSessionKey, QQ.QQ_LENGTH_KEY);

		// 017-020�ֽ����û�QQ��
		int iQQNum;

		GET_DWORD(pbBuf, iBufLen, iQQNum);
		if( iQQNum != m_pQQUser->m_iQQNum )
			return false;

		// 021-024�ֽ��Ƿ�����̽�⵽���û�IP
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwClientIP);

		// 025-026�ֽ��Ƿ�����̽�⵽���û��˿�
		GET_PORT(pbBuf, iBufLen, m_pQQUser->m_iClientPort);

		// 027-032�ֽڽ�������Luma��δ��֤��������赽m_pQQUser�л�������淢���˰�������
		//  �����Ƿ�������IP�Ͷ˿ڣ�������!
		// 027-030�ֽ��Ƿ������Լ���IP
		DWORD dwServerIP;

		GET_IP(pbBuf, iBufLen, dwServerIP);

		// 031-032�ֽ��Ƿ������Ķ˿�
		int iServerPort;

		GET_PORT(pbBuf, iBufLen, iServerPort);

		// 033-036�ֽ��Ǳ��ε�½ʱ�䣬ΪʲôҪ��1000����Ϊ���ʱ�����1000�Ŷԣ�-_-!...
		GET_DWORD(pbBuf, iBufLen, m_pQQUser->m_lLoginTime);
		m_pQQUser->m_lLoginTime *= 1000;

		// 037-038, δ֪��2�ֽ�
		WORD wUnknown;

		GET_WORD(pbBuf, iBufLen, wUnknown);

		// 039-062, ��֤����
		SAFE_FREE(m_pQQUser->m_pbAuthToken);
		m_pQQUser->m_pbAuthToken = (BYTE*)malloc(24);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbAuthToken, 24);

		// 063-066�ֽ���һ��δ֪������1��ip
		// 067-068�ֽ���һ��δ֪������1�Ķ˿�
		// 069-072��һ��δ֪������2��ip
		// 073-074��һ��δ֪������2�Ķ˿�
		// 075-076������δ֪�ֽ�
		// 077-078������δ֪�ֽ�
		OFFSET_BYTES(pbBuf, iBufLen, 16);

		// 079-110��client key�����key���ڱ����¼QQ��԰֮��ĵط�
		SAFE_FREE(m_pQQUser->m_pbClientKey);
		m_pQQUser->m_pbClientKey = (BYTE*)malloc(32);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbClientKey, 32);

		// 111-122��12��δ֪�ֽ�
		OFFSET_BYTES(pbBuf, iBufLen, 12);

		// 123-126���ϴε�½��ip
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwLastLoginIP);

		// 127-130���ϴε�½��ʱ��
		GET_DWORD(pbBuf, iBufLen, m_pQQUser->m_lLastLoginTime);
		m_pQQUser->m_lLastLoginTime *= 1000;

		// 39��δ֪�ֽ�
		OFFSET_BYTES(pbBuf, iBufLen, 39);
	}
	else if( m_bReplyCode == QQ.QQ_REPLY_LOGIN_FAIL )
	{
		// ��¼ʧ�ܣ����ǵõ�����������������Ϣ
//		GET_STRING(pbBuf, iBufLen, m_strReplyMessage, QQ.QQ_CHARSET_DEFAULT);
	}
	else if( m_bReplyCode == QQ.QQ_REPLY_LOGIN_REDIRECT )
	{
		// ��½�ض��򣬿�����Ϊ�˸���ƽ��
		// 001-004�ֽ����û�QQ��
		int iQQNum;

		GET_DWORD(pbBuf, iBufLen, iQQNum);
		if( iQQNum != m_pQQUser->m_iQQNum )
			return false;

		// 005-008�ֽ����ض��򵽵ķ�����IP
		GET_IP(pbBuf, iBufLen, m_pQQUser->m_dwServerIP);

		// 009-010�ֽ����ض��򵽵ķ������Ķ˿�
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
