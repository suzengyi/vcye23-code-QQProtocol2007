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

	// ��ʼ��Կ
	PUT_BYTES(pbBuf, iBodyLen, m_pQQUser->GetInitKey(), QQ.QQ_LENGTH_KEY);

	BYTE pbCryptedBody[MAX_SEND_PACKET_SIZE];
	size_t iCryptedBodyLen;

	CCrypter Crypter;
	BYTE pbOriginalBody[MAX_SEND_PACKET_SIZE];
	size_t iOriginalBodyLen = _PutBody(Crypter, pbOriginalBody, sizeof(pbOriginalBody));

	// ���ܵ�¼��Ϣ
	iCryptedBodyLen = Crypter.Encrypt(pbOriginalBody, 0, iOriginalBodyLen, m_pQQUser->GetInitKey(),
		pbCryptedBody, sizeof(pbCryptedBody));

	// д���¼��Ϣ
	PUT_BYTES(pbBuf, iBodyLen, pbCryptedBody, iCryptedBodyLen);

	return iBodyLen;

}

size_t CLoginPacket::_PutBody(const CCrypter &Crypter, BYTE *pbBuf, size_t iBufLen) const
{
	size_t iBodyLen = 0;

	// ��ʼ����½��Ϣ
	// ͷ16���ֽ���md5������������һ�����ַ������������ڷ�������У������
	// ��ʵ��һ����Ҫ�մ���������ɣ�ֻҪ��֤������16���ֽھ��У���������
	// ֻ�ǿ����ܲ�����������Կ���ܣ������ܽ��ܳ�������ʲô
	BYTE pbCryptedBuf[MAX_SEND_PACKET_SIZE];
	size_t iCryptedBufLen;

	iCryptedBufLen = Crypter.Encrypt((BYTE*)"", 0, 0, m_pQQUser->GetPasswordKey(),
		pbCryptedBuf, sizeof(pbCryptedBuf));
	PUT_BYTES(pbBuf, iBodyLen, pbCryptedBuf, iCryptedBufLen);

	// 36�ֽڵĹ̶�����
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_16_51, QQ.QQ_LOGIN_16_51_LENGTH);

	// ��¼״̬�������¼����ʲô��1�ֽ�
	PUT_BYTE(pbBuf, iBodyLen, m_pQQUser->GetLoginMode());

	// 16�ֽڹ̶�����
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_53_68, QQ.QQ_LOGIN_53_68_LENGTH);

	// ��¼���Ƴ��ȣ�1�ֽ�
	PUT_BYTE(pbBuf, iBodyLen, (BYTE)m_pQQUser->GetLoginTokenLen());

	// ��¼����
	PUT_BYTES(pbBuf, iBodyLen, m_pQQUser->GetLoginToken(), m_pQQUser->GetLoginTokenLen());

	// δ֪1�ֽڣ�0x40
	PUT_BYTE(pbBuf, iBodyLen, 0x40);

	// ����εĸ�����1�ֽڣ�1����9�ֽ�(�²�)
	// �Σ�ÿ�λ������ǹ̶����ݣ�δ֪����
	PUT_BYTES(pbBuf, iBodyLen, QQ.QQ_LOGIN_SEGMENTS, QQ.QQ_LOGIN_SEGMENTS_LENGTH);

	// ���Ȳ�����ȫ����0ֱ�����ϵ�¼������
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
