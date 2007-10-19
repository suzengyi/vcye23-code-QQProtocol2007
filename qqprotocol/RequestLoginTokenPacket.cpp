//*************************************************************************
// RequestLoginTokenPacket.cpp: implementation of the CRequestLoginTokenPacket class.
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
#include "RequestLoginTokenPacket.h"
#include "QQ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRequestLoginTokenPacket::CRequestLoginTokenPacket(CQQUser *pQQUser) :
	CBasicOutPacket(QQ.QQ_CMD_REQUEST_LOGIN_TOKEN, true, pQQUser)
{

}

CRequestLoginTokenPacket::~CRequestLoginTokenPacket()
{

}

size_t CRequestLoginTokenPacket::PutBody(BYTE *pbBuf, size_t iBufLen) const
{
	size_t iBodyLen = 0;

	PUT_BYTE(pbBuf, iBodyLen, 0);

	return iBodyLen;

}

size_t CRequestLoginTokenPacket::EncryptBody(BYTE *pbBody, size_t iBodyLen, BYTE *pbBuf, size_t iBufLen) const
{
	if( iBufLen < iBodyLen )
		return -1;
	memcpy(pbBuf, pbBody, iBodyLen);
	return iBodyLen;

}
