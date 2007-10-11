//*************************************************************************
// InPacket.h: interface for the CInPacket class.
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

#if !defined(AFX_INPACKET_H__25610848_0FEE_4BD6_946A_5552DC8E393A__INCLUDED_)
#define AFX_INPACKET_H__25610848_0FEE_4BD6_946A_5552DC8E393A__INCLUDED_

#include "QQUser.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_RECV_PACKET_SIZE				10240	// 最大接受包是800

#define GET_BYTE(pbBuf, iBufLen, b)	\
{	\
	b = *pbBuf;	\
	pbBuf += sizeof(BYTE);	\
	iBufLen -= sizeof(BYTE);	\
}

#define GET_WORD(pbBuf, iBufLen, w)	\
{	\
	w = *pbBuf << 8;	\
	w += *(pbBuf + 1);	\
	pbBuf += sizeof(WORD);	\
	iBufLen -= sizeof(WORD);	\
}

#define GET_DWORD(pbBuf, iBufLen, dw)	\
{	\
	dw = *pbBuf << 24;	\
	dw += *(pbBuf + 1) << 16;	\
	dw += *(pbBuf + 2) << 8;	\
	dw += *(pbBuf + 3);	\
	pbBuf += sizeof(DWORD);	\
	iBufLen -= sizeof(DWORD);	\
}

#define GET_IP(pbBuf, iBufLen, dw)	\
{	\
	dw = *(DWORD*)pbBuf;	\
	pbBuf += sizeof(DWORD);	\
	iBufLen -= sizeof(DWORD);	\
}

#define GET_PORT(pbBuf, iBufLen, w)	\
{	\
	w = *(WORD*)pbBuf;	\
	pbBuf += sizeof(WORD);	\
	iBufLen -= sizeof(WORD);	\
}

#define GET_BYTES(pbBuf, iBufLen, pbBytes, cBytes)	\
{	\
	memcpy(pbBytes, pbBuf, cBytes);	\
	pbBuf += cBytes;	\
	iBufLen -= cBytes;	\
}

#define GET_STRING(pbBuf, iBufLen, str, strlen)	\
{	\
	str.assign((char*)pbBuf, strlen);	\
	pbBuf += sizeof(char) * strlen;	\
	iBufLen -= sizeof(char) * strlen; \
	if( str.length() != strlen )	\
		assert(false);	\
}

#define OFFSET_BYTES(pbBuf, iBufLen, cOffsetBytes)	\
{	\
	pbBuf += cOffsetBytes;	\
	iBufLen -= cOffsetBytes;	\
}

class CInPacket  
{
public:
	CInPacket(short sCommand, CQQUser* pQQUser);
	virtual ~CInPacket();

public:
	bool ParseBuffer(BYTE* pbBuf, size_t iBufLen);

	short GetCommand() const { return m_sCommand; }
	short GetSequence() const { return m_sSequence; }

protected:
	virtual size_t DecryptBody(BYTE* pbBuf, size_t iBufLen, BYTE* pbBody, size_t iBodyLen) const = 0;
	virtual size_t GetBodyBytes(BYTE*& pbBuf, size_t& iBufLen, BYTE* pbBody, size_t iBodyLen) const = 0;

	virtual bool ParseHeader(BYTE*& pbBuf, size_t& iBufLen) = 0;
	virtual bool ParseTail(BYTE*& pbBuf, size_t& iBufLen) = 0;
	virtual bool ParseBody(BYTE* pbBuf, size_t iBufLen) = 0;

	BYTE m_bHeader;
	short m_sSource;
	short m_sCommand;
	short m_sSequence;
	CQQUser* m_pQQUser;

};

#endif // !defined(AFX_INPACKET_H__25610848_0FEE_4BD6_946A_5552DC8E393A__INCLUDED_)
