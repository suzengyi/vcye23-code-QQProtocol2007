//*************************************************************************
// OutPacket.cpp: implementation of the COutPacket class.
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
#include "OutPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * ����һ�����������
 * 
 * @param command
 * 		������
 * @param ack
 * 		���Ƿ���Ҫ�ظ�
 * @param user
 * 		QQ�û�����
 */
COutPacket::COutPacket(BYTE bHeader, short sCommand, bool bAck, CQQUser *pQQUser) :
	m_bHeader(bHeader),
	m_sSource(QQ.QQ_CLIENT_VERSION),
	m_sSequence(pQQUser->GetNextSequence()),
	m_sCommand(sCommand),
	m_bAck(bAck),
	m_pQQUser(pQQUser)
{

}

/**
 * ��buf�й���һ��OutPacket�����ڵ��ԡ����buf������ܰ�����ץ�����ץ��������
 * 
 * @param buf
 * 			ByteBuffer
 * @throws PacketParseException
 * 			��������
 */
COutPacket::COutPacket(BYTE *pbBuf, size_t iBufLen) :
	m_pQQUser(NULL)
{

}

COutPacket::~COutPacket()
{

}

/**
 * ��������ת��Ϊ�ֽ���, ��д��ָ����ByteBuffer����.
 * һ�����, ǰ��ֱ���Ҫд���ͷ���Ͱ�β��.
 * 
 * @param buf
 * 		����д���ByteBuffer����.
 */
size_t COutPacket::FillBuffer(BYTE* pbBuf, size_t iBufLen) const
{
	// ���浱ǰpos
	BYTE* pbBufStart = pbBuf;
	size_t iHeaderLen;
	size_t iTailLen;

	// ���ͷ��
	iHeaderLen = PutHeader(pbBuf, iBufLen);
	pbBuf += iHeaderLen;
	iBufLen -= iHeaderLen;

	BYTE pbBody[MAX_SEND_PACKET_SIZE];
	size_t iBodyLen;
	BYTE pbEncryptedBody[MAX_SEND_PACKET_SIZE];
	size_t iEncryptedBodyLen;

	// ������
	iBodyLen = PutBody(pbBody, sizeof(pbBody));
	// ���ܰ���
	iEncryptedBodyLen = EncryptBody(pbBody, iBodyLen, pbEncryptedBody, sizeof(pbEncryptedBody));
	if( iEncryptedBodyLen == -1 )
		return -1;
	// ��������д������buf
	memcpy(pbBuf, pbEncryptedBody, iEncryptedBodyLen);
	pbBuf += iEncryptedBodyLen;
	iBufLen -= iEncryptedBodyLen;

	// ���β��
	iTailLen = PutTail(pbBuf, iBufLen);
	pbBuf += iTailLen;
	iBufLen -= iTailLen;

	// ����
	PostFill(pbBufStart, iHeaderLen + iEncryptedBodyLen + iTailLen);
	return iHeaderLen + iEncryptedBodyLen + iTailLen;

}
