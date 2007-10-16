//*************************************************************************
// BasicOutPacket.cpp: implementation of the CBasicOutPacket class.
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
#include "BasicOutPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicOutPacket::CBasicOutPacket(short sCommand, bool bAck, CQQUser *pQQUser) :
	COutPacket(QQ.QQ_HEADER_BASIC_FAMILY, sCommand, bAck, pQQUser)
{

}

CBasicOutPacket::CBasicOutPacket(BYTE *pbBuf, size_t iBufLen) :
	COutPacket(pbBuf, iBufLen)
{

}

CBasicOutPacket::~CBasicOutPacket()
{

}

size_t CBasicOutPacket::PutHeader(BYTE *pbBuf, size_t iBufLen) const
{
	size_t iHeaderLen = 0;

	if( !m_pQQUser->IsUDP() )
		PUT_WORD(pbBuf, iHeaderLen, 0);
	PUT_BYTE(pbBuf, iHeaderLen, m_bHeader);
	PUT_WORD(pbBuf, iHeaderLen, m_sSource);
	PUT_WORD(pbBuf, iHeaderLen, m_sCommand);
	PUT_WORD(pbBuf, iHeaderLen, m_sSequence);
	PUT_DWORD(pbBuf, iHeaderLen, m_pQQUser->GetQQNum());

	return iHeaderLen;

}

size_t CBasicOutPacket::PutTail(BYTE *pbBuf, size_t iBufLen) const
{
	size_t iTailLen = 0;

	PUT_BYTE(pbBuf, iTailLen, QQ.QQ_TAIL_BASIC_FAMILY);

	return iTailLen;

}

size_t CBasicOutPacket::PostFill(BYTE *pbBufStart, size_t iBufLen) const
{
	size_t iBufStartLen = 0;

	// 如果是tcp包，到包的开头处填上包长度，然后回到目前的pos
	if( !m_pQQUser->IsUDP() )
	{
		PUT_WORD(pbBufStart, iBufStartLen, iBufLen);
	}
	return iBufStartLen;

}
