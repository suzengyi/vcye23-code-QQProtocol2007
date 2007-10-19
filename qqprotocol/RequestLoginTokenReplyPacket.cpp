//*************************************************************************
// RequestLoginTokenReplyPacket.cpp: implementation of the CRequestLoginTokenReplyPacket class.
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
#include "RequestLoginTokenReplyPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRequestLoginTokenReplyPacket::CRequestLoginTokenReplyPacket(CQQUser *pQQUser) :
	CBasicInPacket(QQ.QQ_CMD_REQUEST_LOGIN_TOKEN, pQQUser)
{

}

CRequestLoginTokenReplyPacket::~CRequestLoginTokenReplyPacket()
{

}

bool CRequestLoginTokenReplyPacket::ParseBody(BYTE *pbBuf, size_t iBufLen)
{
	// »Ø¸´Âë
	BYTE bReplyCode;

	GET_BYTE(pbBuf, iBufLen, bReplyCode);
	if( bReplyCode == QQ.QQ_REPLY_OK )
	{
		// µÇÂ¼ÁîÅÆ³¤¶È
		GET_BYTE(pbBuf, iBufLen, m_pQQUser->m_iLoginTokenLen);

		// µÇÂ¼ÁîÅÆ
		SAFE_FREE(m_pQQUser->m_pbLoginToken);
		m_pQQUser->m_pbLoginToken = (BYTE*)malloc(m_pQQUser->m_iLoginTokenLen);
		GET_BYTES(pbBuf, iBufLen, m_pQQUser->m_pbLoginToken, m_pQQUser->m_iLoginTokenLen);
	}
	return true;

}

size_t CRequestLoginTokenReplyPacket::DecryptBody(BYTE *pbBuf, size_t iBufLen, BYTE *pbBody, size_t iBodyLen) const
{
	if( iBodyLen < iBufLen )
		return -1;
	memcpy(pbBody, pbBuf, iBufLen);
	return iBufLen;

}
