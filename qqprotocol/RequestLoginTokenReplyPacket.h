//*************************************************************************
// RequestLoginTokenReplyPacket.h: interface for the CRequestLoginTokenReplyPacket class.
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

#if !defined(AFX_REQUESTLOGINTOKENREPLYPACKET_H__52493C05_1FDE_465B_AEBC_55463367D9CD__INCLUDED_)
#define AFX_REQUESTLOGINTOKENREPLYPACKET_H__52493C05_1FDE_465B_AEBC_55463367D9CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicInPacket.h"

/**
 * <pre>
 * 请求登录令牌的回复包，这个包的source字段和其他包不同，为QQ.QQ_SERVER_0000
 * 1. 头部
 * 2. 回复码，1字节，0x00表示成功
 * 3. 登录令牌长度，1字节
 * 4. 登录令牌
 * 5. 尾部
 * </pre>
 * 
 * @author luma
 */
class CRequestLoginTokenReplyPacket : public CBasicInPacket  
{
public:
	CRequestLoginTokenReplyPacket(CQQUser* pQQUser);
	virtual ~CRequestLoginTokenReplyPacket();

protected:
	virtual size_t DecryptBody(BYTE* pbBuf, size_t iBufLen, BYTE* pbBody, size_t iBodyLen) const;

	virtual bool ParseBody(BYTE* pbBuf, size_t iBufLen);
};

#endif // !defined(AFX_REQUESTLOGINTOKENREPLYPACKET_H__52493C05_1FDE_465B_AEBC_55463367D9CD__INCLUDED_)
