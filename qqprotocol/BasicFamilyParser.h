//*************************************************************************
// BasicFamilyParser.h: interface for the CBasicFamilyParser class.
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

#if !defined(AFX_BASICFAMILYPARSER_H__5D1CFB37_F6EC_49CC_AA5F_C05FB5DF52F6__INCLUDED_)
#define AFX_BASICFAMILYPARSER_H__5D1CFB37_F6EC_49CC_AA5F_C05FB5DF52F6__INCLUDED_

#include "QQUser.h"
#include "BasicInPacket.h"
#include "BasicOutPacket.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQQClient;

class CBasicFamilyParser  
{
public:
	CBasicInPacket* ParseInPacket(BYTE* pbBuf, size_t iBufLen, CQQUser* pQQUser) const;
	CBasicOutPacket* CreateOutPacket(short sCommand, CQQUser* pQQUser) const;

	CBasicFamilyParser();
	virtual ~CBasicFamilyParser();

protected:
	short GetCommand(BYTE* pbBuf, size_t iBufLen, CQQUser* pQQUser) const;

};

#endif // !defined(AFX_BASICFAMILYPARSER_H__5D1CFB37_F6EC_49CC_AA5F_C05FB5DF52F6__INCLUDED_)
