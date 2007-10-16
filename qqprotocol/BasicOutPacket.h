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
 * ����Э��������������
 * ����Э����İ����������µĸ�ʽ:
 * 1. ��ͷ��־��1�ֽڣ�0x02
 * 2. �ͻ��˰汾���룬2�ֽ�
 * 3. ���2�ֽ�
 * 4. �����, 2�ֽ�
 * 5. �û�QQ�ţ�4�ֽ�
 * 6. ����
 * 7. ��β��־��1�ֽڣ�0x03
 * Note: 6���ֽ��ûỰ��Կ����, ��¼�����⣬6����Ҫ��������Կ���ܡ������¼���ư����⣬6���ֲ������
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
