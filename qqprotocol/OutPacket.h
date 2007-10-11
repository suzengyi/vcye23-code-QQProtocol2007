//*************************************************************************
// OutPacket.h: interface for the COutPacket class.
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

#if !defined(AFX_OUTPACKET_H__8093B5C6_46AA_4DE9_9D7A_39F786FF3E29__INCLUDED_)
#define AFX_OUTPACKET_H__8093B5C6_46AA_4DE9_9D7A_39F786FF3E29__INCLUDED_

#include "QQUser.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SEND_PACKET_SIZE				800	// 最大发送包是800

#define PUT_BYTE(pbBuf, iBufLen, b)	\
{	\
	*pbBuf = b;	\
	pbBuf += sizeof(BYTE);	\
	iBufLen += sizeof(BYTE);	\
}

#define PUT_WORD(pbBuf, iBufLen, w)	\
{	\
	*pbBuf = (w & 0xFF00) >> 8;	\
	*(pbBuf + 1) = w & 0xFF;	\
	pbBuf += sizeof(WORD);	\
	iBufLen += sizeof(WORD);	\
}

#define PUT_DWORD(pbBuf, iBufLen, dw)	\
{	\
	*pbBuf = (dw & 0xFF000000) >> 24;	\
	*(pbBuf + 1) = (dw & 0xFF0000) >> 16;	\
	*(pbBuf + 2) = (dw & 0xFF00) >> 8;	\
	*(pbBuf + 3) = dw & 0xFF;	\
	pbBuf += sizeof(DWORD);	\
	iBufLen += sizeof(DWORD);	\
}

#define PUT_BYTES(pbBuf, iBufLen, pbBytes, cBytes)	\
{	\
	memcpy(pbBuf, pbBytes, cBytes);	\
	pbBuf += cBytes;	\
	iBufLen += cBytes;	\
}

#define FILL_BYTES(pbBuf, iBufLen, cTotalBytes)	\
{	\
	if( iBufLen < cTotalBytes )	\
	{	\
		memset(pbBuf, 0, cTotalBytes - iBufLen);	\
		pbBuf += cTotalBytes - iBufLen;	\
		iBufLen = cTotalBytes;	\
	}	\
}

class COutPacket  
{
public:
	COutPacket(BYTE bHeader, short sCommand, bool bAck, CQQUser* pQQUser);
	COutPacket(BYTE* pbBuf, size_t iBufLen);
	virtual ~COutPacket();

public:
	size_t FillBuffer(BYTE* pbBuf, size_t iBufLen) const;
	// 只有reply包有权调用SetSequence（和源包的sequence相同）
	void SetSequence(short sSequence) { m_sSequence = sSequence; }
	short GetSequence() const { return m_sSequence; }
	short GetCommand() const { return m_sCommand; }

protected:
	virtual size_t EncryptBody(BYTE* pbBody, size_t iBodyLen, BYTE* pbBuf, size_t iBufLen) const = 0;
	/**
	 * 回填，有些字段必须填完整个包才能确定其内容，比如长度字段，那么这个方法将在
	 * 尾部填充之后调用
	 * 
	 * @param buf
	 * 		ByteBuffer
	 * @param startPos
	 * 		包起始位置
	 */
	virtual size_t PostFill(BYTE* pbBufStart, size_t iBufLen) const = 0;

	virtual size_t PutHeader(BYTE* pbBuf, size_t iBufLen) const = 0;
	virtual size_t PutTail(BYTE* pbBuf, size_t iBufLen) const = 0;
	virtual size_t PutBody(BYTE* pbBuf, size_t iBufLen) const = 0;

	BYTE m_bHeader;
	short m_sSource;
	short m_sCommand;
	/** 包起始序列号 */
	short m_sSequence;
	/** 是否需要回应 */
	bool m_bAck;
	CQQUser* m_pQQUser;

};

#endif // !defined(AFX_OUTPACKET_H__8093B5C6_46AA_4DE9_9D7A_39F786FF3E29__INCLUDED_)
