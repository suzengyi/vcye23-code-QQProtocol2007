//*************************************************************************
// BasicInPacket.cpp: implementation of the CBasicInPacket class.
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
#include "BasicInPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicInPacket::CBasicInPacket(short sCommand, CQQUser *pQQUser) :
	CInPacket(sCommand, pQQUser)
{

}

CBasicInPacket::~CBasicInPacket()
{

}

bool CBasicInPacket::ParseHeader(BYTE *&pbBuf, size_t &iBufLen)
{
	if( !m_pQQUser->IsUDP() )
	{
		WORD wBufLen;

		GET_WORD(pbBuf, iBufLen, wBufLen);
	}

	short sCommand = m_sCommand;

	GET_BYTE(pbBuf, iBufLen, m_bHeader);
	GET_WORD(pbBuf, iBufLen, m_sSource);
	GET_WORD(pbBuf, iBufLen, m_sCommand);
	GET_WORD(pbBuf, iBufLen, m_sSequence);

	return sCommand == m_sCommand;

}

bool CBasicInPacket::ParseTail(BYTE *&pbBuf, size_t &iBufLen)
{
	BYTE b;

	GET_BYTE(pbBuf, iBufLen, b);

	return true;

}

size_t CBasicInPacket::GetBodyBytes(BYTE *&pbBuf, size_t &iBufLen, BYTE *pbBody, size_t iBodyLen) const
{
	// 得到包体长度
	if( iBodyLen < iBufLen - QQ.QQ_LENGTH_BASIC_FAMILY_TAIL )
		return -1;

	if( !m_pQQUser->IsUDP() )
		iBodyLen -= 2;

	// 得到加密的包体内容
	iBodyLen = iBufLen - QQ.QQ_LENGTH_BASIC_FAMILY_TAIL;
	memcpy(pbBody, pbBuf, iBodyLen);
	pbBuf += iBodyLen;
	iBufLen -= iBodyLen;
	return iBodyLen;

}
