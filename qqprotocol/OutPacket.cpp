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
 * 创建一个基本输出包
 * 
 * @param command
 * 		包命令
 * @param ack
 * 		包是否需要回复
 * @param user
 * 		QQ用户对象
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
 * 从buf中构造一个OutPacket，用于调试。这个buf里面可能包含了抓包软件抓来的数据
 * 
 * @param buf
 * 			ByteBuffer
 * @throws PacketParseException
 * 			解析出错
 */
COutPacket::COutPacket(BYTE *pbBuf, size_t iBufLen) :
	m_pQQUser(NULL)
{

}

COutPacket::~COutPacket()
{

}

/**
 * 将整个包转化为字节流, 并写入指定的ByteBuffer对象.
 * 一般而言, 前后分别需要写入包头部和包尾部.
 * 
 * @param buf
 * 		将包写入的ByteBuffer对象.
 */
size_t COutPacket::FillBuffer(BYTE* pbBuf, size_t iBufLen) const
{
	// 保存当前pos
	BYTE* pbBufStart = pbBuf;
	size_t iHeaderLen;
	size_t iTailLen;

	// 填充头部
	iHeaderLen = PutHeader(pbBuf, iBufLen);
	pbBuf += iHeaderLen;
	iBufLen -= iHeaderLen;

	BYTE pbBody[MAX_SEND_PACKET_SIZE];
	size_t iBodyLen;
	BYTE pbEncryptedBody[MAX_SEND_PACKET_SIZE];
	size_t iEncryptedBodyLen;

	// 填充包体
	iBodyLen = PutBody(pbBody, sizeof(pbBody));
	// 加密包体
	iEncryptedBodyLen = EncryptBody(pbBody, iBodyLen, pbEncryptedBody, sizeof(pbEncryptedBody));
	if( iEncryptedBodyLen == -1 )
		return -1;
	// 加密内容写入最终buf
	memcpy(pbBuf, pbEncryptedBody, iEncryptedBodyLen);
	pbBuf += iEncryptedBodyLen;
	iBufLen -= iEncryptedBodyLen;

	// 填充尾部
	iTailLen = PutTail(pbBuf, iBufLen);
	pbBuf += iTailLen;
	iBufLen -= iTailLen;

	// 回填
	PostFill(pbBufStart, iHeaderLen + iEncryptedBodyLen + iTailLen);
	return iHeaderLen + iEncryptedBodyLen + iTailLen;

}
