//*************************************************************************
// RequestLoginTokenPacket.h: interface for the CRequestLoginTokenPacket class.
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

#if !defined(AFX_REQUESTLOGINTOKENPACKET_H__AD2449BA_A992_4528_9227_FC180FFFA048__INCLUDED_)
#define AFX_REQUESTLOGINTOKENPACKET_H__AD2449BA_A992_4528_9227_FC180FFFA048__INCLUDED_

#include "BasicOutPacket.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * <pre>
 * 请求登录令牌的包，格式为：
 * 1. 头部
 * 2. 未知的1字节，0x00
 * 3. 尾部
 * </pre>
 * 
 * @author luma
 */
class CRequestLoginTokenPacket : public CBasicOutPacket 
{
public:
	CRequestLoginTokenPacket(CQQUser* pUser);
	virtual ~CRequestLoginTokenPacket();

protected:
	size_t EncryptBody(BYTE* pbBody, size_t iBodyLen, BYTE* pbBuf, size_t iBufLen) const;

	size_t PutBody(BYTE* pbBuf, size_t iBufLen) const;
};

#endif // !defined(AFX_REQUESTLOGINTOKENPACKET_H__AD2449BA_A992_4528_9227_FC180FFFA048__INCLUDED_)
