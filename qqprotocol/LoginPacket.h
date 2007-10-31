//*************************************************************************
// LoginPacket.h: interface for the CLoginPacket class.
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

#if !defined(AFX_LOGINPACKET_H__44F448F8_1727_46B0_91D3_80C854BD2F40__INCLUDED_)
#define AFX_LOGINPACKET_H__44F448F8_1727_46B0_91D3_80C854BD2F40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicOutPacket.h"
#include "Crypter.h"

/**
 * <pre>
 * QQ登录请求包，格式为
 * 1. 头部
 * 2. 初始密钥，16字节
 * 3. 用户的密码密钥加密一个空串得到的16字节
 * 4. 36字节的固定内容，未知含义
 * 5. 登录状态，隐身登录还是什么，1字节
 * 6. 16字节固定内容，未知含义
 * 7. 登录令牌长度，1字节
 * 8. 登录令牌
 * 9. 登录模式，1字节，目前只支持普通模式
 * 10. 未知1字节，0x40
 * 11. 后面段的个数，1字节，1个段9字节(猜测)
 * 12. 段，每次基本都是固定内容，未知含义
 * 13. 长度不足则全部填0知道符合登录包长度
 * 14. 尾部
 * </pre>
 *
 * @author luma
 */
class CLoginPacket : public CBasicOutPacket  
{
public:
	CLoginPacket(CQQUser* pQQUser);
	virtual ~CLoginPacket();

protected:
	size_t EncryptBody(BYTE* pbBody, size_t iBodyLen, BYTE* pbBuf, size_t iBufLen) const;

	size_t PutBody(BYTE* pbBuf, size_t iBufLen) const;

private:
	size_t _PutBody(const CCrypter& Crypter, BYTE* pbBuf, size_t iBufLen) const;
};

#endif // !defined(AFX_LOGINPACKET_H__44F448F8_1727_46B0_91D3_80C854BD2F40__INCLUDED_)
