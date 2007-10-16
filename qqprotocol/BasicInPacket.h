//*************************************************************************
// BasicInPacket.h: interface for the CBasicInPacket class.
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

#if !defined(AFX_BASICINPACKET_H__7E9FC988_96A3_465D_BAD0_4587E185680B__INCLUDED_)
#define AFX_BASICINPACKET_H__7E9FC988_96A3_465D_BAD0_4587E185680B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InPacket.h"

class CBasicInPacket : public CInPacket  
{
public:
	CBasicInPacket(short sCommand, CQQUser* pQQUser);
	virtual ~CBasicInPacket();

protected:
	virtual size_t GetBodyBytes(BYTE*& pbBuf, size_t& iBufLen, BYTE* pbBody, size_t iBodyLen) const;

	virtual bool ParseTail(BYTE*& pbBuf, size_t& iBufLen);
	virtual bool ParseHeader(BYTE*& pbBuf, size_t& iBufLen);
};

#endif // !defined(AFX_BASICINPACKET_H__7E9FC988_96A3_465D_BAD0_4587E185680B__INCLUDED_)
