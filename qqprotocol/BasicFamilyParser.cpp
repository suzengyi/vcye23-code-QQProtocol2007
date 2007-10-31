//*************************************************************************
// BasicFamilyParser.cpp: implementation of the CBasicFamilyParser class.
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
#include "BasicFamilyParser.h"
#include "QQ.h"

#include "RequestLoginTokenReplyPacket.h"
#include "LoginReplyPacket.h"

#include "RequestLoginTokenPacket.h"
#include "LoginPacket.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicFamilyParser::CBasicFamilyParser()
{

}

CBasicFamilyParser::~CBasicFamilyParser()
{

}

CBasicInPacket* CBasicFamilyParser::ParseInPacket(BYTE *pbBuf, size_t iBufLen, CQQUser *pQQUser) const
{
	CBasicInPacket* pBasicInPacket = NULL;
	short sCommand = GetCommand(pbBuf, iBufLen, pQQUser);

	if( sCommand == QQ.QQ_CMD_REQUEST_LOGIN_TOKEN )
	{
		pBasicInPacket = new CRequestLoginTokenReplyPacket(pQQUser);
	}
	else if( sCommand == QQ.QQ_CMD_LOGIN )
	{
		pBasicInPacket = new CLoginReplyPacket(pQQUser);
	}
	if( pBasicInPacket != NULL )
	{
		if( pBasicInPacket->ParseBuffer(pbBuf, iBufLen) )
			return pBasicInPacket;
		SAFE_DELETE(pBasicInPacket);
	}
	return NULL;

}

CBasicOutPacket* CBasicFamilyParser::CreateOutPacket(short sCommand, CQQUser *pQQUser) const
{
	if( sCommand == QQ.QQ_CMD_REQUEST_LOGIN_TOKEN )
	{
		return new CRequestLoginTokenPacket(pQQUser);
	}
	else if( sCommand == QQ.QQ_CMD_LOGIN )
	{
		return new CLoginPacket(pQQUser);
	}
	return NULL;

}

short CBasicFamilyParser::GetCommand(BYTE *pbBuf, size_t iBufLen, CQQUser *pQQUser) const
{
	if( !pQQUser->IsUDP() )
		OFFSET_BYTES(pbBuf, iBufLen, 5)
	else
		OFFSET_BYTES(pbBuf, iBufLen, 3)

	short sCommand;

	GET_WORD(pbBuf, iBufLen, sCommand);
	return sCommand;

}
