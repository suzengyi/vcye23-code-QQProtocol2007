//*************************************************************************
// InPacket.cpp: implementation of the CInPacket class.
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
#include "InPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInPacket::CInPacket(short sCommand, CQQUser *pQQUser) :
	m_sCommand(sCommand),
	m_pQQUser(pQQUser)
{

}

CInPacket::~CInPacket()
{

}

bool CInPacket::ParseBuffer(BYTE *pbBuf, size_t iBufLen)
{
	if( !ParseHeader(pbBuf, iBufLen) )
		return false;

	BYTE pbBody[MAX_RECV_PACKET_SIZE];
	size_t iBodyLen;
	BYTE pbDecryptedBody[MAX_RECV_PACKET_SIZE];
	size_t iDecryptedBodyLen;

	iBodyLen = GetBodyBytes(pbBuf, iBufLen, pbBody, sizeof(pbBody));
	if( iBodyLen == -1 )
		return false;
	iDecryptedBodyLen = DecryptBody(pbBody, iBodyLen, pbDecryptedBody, sizeof(pbDecryptedBody));
	if( iDecryptedBodyLen == -1 )
		return false;
	if( !ParseBody(pbDecryptedBody, iDecryptedBodyLen) )
		return false;
	if( !ParseTail(pbBuf, iBufLen) )
		return false;
	return true;

}
