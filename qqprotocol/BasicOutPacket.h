//*************************************************************************
// BasicOutPacket.h: interface for the CBasicOutPacket class.
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

#if !defined(AFX_BASICOUTPACKET_H__71BEFBE6_3199_457E_8824_0EFD7BB8A0C0__INCLUDED_)
#define AFX_BASICOUTPACKET_H__71BEFBE6_3199_457E_8824_0EFD7BB8A0C0__INCLUDED_

#include "OutPacket.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * <pre>
 * 基本协议族的输出包基类
 * 基本协议族的包都具有以下的格式:
 * 1. 包头标志，1字节，0x02
 * 2. 客户端版本代码，2字节
 * 3. 命令，2字节
 * 4. 包序号, 2字节
 * 5. 用户QQ号，4字节
 * 6. 包体
 * 7. 包尾标志，1字节，0x03
 * Note: 6部分将用会话密钥加密, 登录包例外，6部分要用密码密钥加密。请求登录令牌包例外，6部分不需加密
 * </pre>
 * 
 * @author notxx
 * @author luma
 */
class CBasicOutPacket : public COutPacket 
{
public:
	CBasicOutPacket(short sCommand, bool bAck, CQQUser* pQQUser);
	CBasicOutPacket(BYTE* pbBuf, size_t iBufLen);
	virtual ~CBasicOutPacket();

protected:
	size_t PostFill(BYTE* pbBufStart, size_t iBufLen) const;

	size_t PutHeader(BYTE* pbBuf, size_t iBufLen) const;
	size_t PutTail(BYTE* pbBuf, size_t iBufLen) const;

};

#endif // !defined(AFX_BASICOUTPACKET_H__71BEFBE6_3199_457E_8824_0EFD7BB8A0C0__INCLUDED_)
