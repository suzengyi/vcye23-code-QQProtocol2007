//*************************************************************************
// LoginPacket.cpp: implementation of the CLoginPacket class.
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
#include "LoginPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginPacket::CLoginPacket(CQQUser *pQQUser) :
	CBasicOutPacket(QQ.QQ_CMD_LOGIN, true, pQQUser)
{

}

CLoginPacket::~CLoginPacket()
{

}

size_t CLoginPacket::PutBody(BYTE *pbBuf, size_t iBufLen) const
{
	size_t iBodyLen = 0;

	// 初始密钥
	PUT_BYTES(pbBuf, iBodyLen, m_pQQUser->GetInitKey(), QQ.QQ_LENGTH_KEY);

	BYTE pbCryptedBody[MAX_SEND_PACKET_SIZE];
	size_t iCryptedBodyLen;

	CCrypter Crypter;
	BYTE pbOriginalBody[MAX_SEND_PACKET_SIZE];
	size_t iOriginalBodyLen = _PutBody(Crypter, pbOriginalBody, sizeof(pbOriginalBody));

	// 加密登录信息
	iCryptedBodyLen = Crypter.Encrypt(pbOriginalBody, 0, iOriginalBodyLen, m_pQQUser->GetInitKey(),
		pbCryptedBody, sizeof(pbCryptedBody));

	// 写入登录信息
	PUT_BYTES(pbBuf, iBodyLen, pbCryptedBody, iCryptedBodyLen);

	return iBodyLen;

}

size_t CLoginPacket::_PutBody(const CCrypter &Crypter, BYTE *pbBuf, size_t iBufLen) const
{
	size_t iBodyLen = 0;

	// 开始填充登陆信息
	// 头16个字节用md5处理的密码加密一个空字符串，这用来在服务器端校验密码
	// 其实不一定非要空串，任意均可，只要保证密文是16个字节就行，服务器端
	// 只是看看能不能用密码密钥解密，他不管解密出来的是什么
	BYTE pbCryptedBuf[MAX_SEND_PACKET_SIZE];
	size_t iCryptedBufLen;

	iCryptedBufLen = Crypter.Encrypt((BYTE*)"", 0, 0, m_pQQUser->GetPasswordKey(),
		pbCryptedBuf, sizeof(pbCryptedBuf));
	PUT_BYTES(pbBuf, iBodyLen, pbCryptedBuf, iCryptedBufLen);

	// 36字节的固定内容
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_16_51, QQ.QQ_LOGIN_16_51_LENGTH);

	// 登录状态，隐身登录还是什么，1字节
	PUT_BYTE(pbBuf, iBodyLen, m_pQQUser->GetLoginMode());

	// 16字节固定内容
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_53_68, QQ.QQ_LOGIN_53_68_LENGTH);

	// 登录令牌长度，1字节
	PUT_BYTE(pbBuf, iBodyLen, (BYTE)m_pQQUser->GetLoginTokenLen());

	// 登录令牌
	PUT_BYTES(pbBuf, iBodyLen, m_pQQUser->GetLoginToken(), m_pQQUser->GetLoginTokenLen());

	// 未知1字节，0x40
	PUT_BYTE(pbBuf, iBodyLen, 0x40);

	// 后面段的个数，1字节，1个段9字节(猜测)
	// 段，每次基本都是固定内容，未知含义
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_SEGMENTS, QQ.QQ_LOGIN_SEGMENTS_LENGTH);

	// 长度不足则全部填0直到符合登录包长度
	FILL_BYTES(pbBuf, iBodyLen, QQ.QQ_LENGTH_LOGIN_DATA);

	return QQ.QQ_LENGTH_LOGIN_DATA;

}

size_t CLoginPacket::EncryptBody(BYTE *pbBody, size_t iBodyLen, BYTE *pbBuf, size_t iBufLen) const
{
	if( iBufLen < iBodyLen )
		return -1;
	memcpy(pbBuf, pbBody, iBodyLen);
	return iBodyLen;

}
