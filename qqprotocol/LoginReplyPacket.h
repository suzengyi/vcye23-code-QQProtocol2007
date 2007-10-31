//*************************************************************************
// LoginReplyPacket.h: interface for the CLoginReplyPacket class.
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

#if !defined(AFX_LOGINREPLYPACKET_H__C30EAAA9_A635_48F1_BE1B_F3E462482CCF__INCLUDED_)
#define AFX_LOGINREPLYPACKET_H__C30EAAA9_A635_48F1_BE1B_F3E462482CCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicInPacket.h"

/**
 * <pre>
 * QQ登陆应答包
 * 1. 头部
 * 2. 回复码, 1字节
 * 2部分如果是0x00
 * 3. session key, 16字节
 * 4. 用户QQ号，4字节
 * 5. 我的外部IP，4字节
 * 6. 我的外部端口，2字节
 * 7. 服务器IP，4字节
 * 8. 服务器端口，2字节
 * 9. 本次登录时间，4字节，为从1970-1-1开始的毫秒数除1000
 * 10. 未知的2字节
 * 11. 用户认证令牌,24字节
 * 12. 一个未知服务器1的ip，4字节
 * 13. 一个未知服务器1的端口，2字节
 * 14. 一个未知服务器2的ip，4字节
 * 15. 一个未知服务器2的端口，2字节
 * 16. 两个未知字节
 * 17. 两个未知字节
 * 18. client key，32字节，这个key用在比如登录QQ家园之类的地方
 * 19. 12个未知字节
 * 20. 上次登陆的ip，4字节
 * 21. 上次登陆的时间，4字节
 * 22. 39个未知字节
 * 2部分如果是0x01，表示重定向
 * 3. 用户QQ号，4字节
 * 4. 重定向到的服务器IP，4字节
 * 5. 重定向到的服务器的端口，2字节
 * 2部分如果是0x05，表示登录失败
 * 3. 一个错误消息
 * </pre>
 *
 * @author luma
 */
class CLoginReplyPacket : public CBasicInPacket  
{
public:
	CLoginReplyPacket(CQQUser* pQQUser);
	virtual ~CLoginReplyPacket();

protected:
	virtual size_t DecryptBody(BYTE* pbBuf, size_t iBufLen, BYTE* pbBody, size_t iBodyLen) const;

	virtual bool ParseBody(BYTE* pbBuf, size_t iBufLen);

public:
	BYTE m_bReplyCode;
	std::string m_strReplyMessage;
};

#endif // !defined(AFX_LOGINREPLYPACKET_H__C30EAAA9_A635_48F1_BE1B_F3E462482CCF__INCLUDED_)
