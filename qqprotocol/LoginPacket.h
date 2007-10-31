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
 * QQ��¼���������ʽΪ
 * 1. ͷ��
 * 2. ��ʼ��Կ��16�ֽ�
 * 3. �û���������Կ����һ���մ��õ���16�ֽ�
 * 4. 36�ֽڵĹ̶����ݣ�δ֪����
 * 5. ��¼״̬�������¼����ʲô��1�ֽ�
 * 6. 16�ֽڹ̶����ݣ�δ֪����
 * 7. ��¼���Ƴ��ȣ�1�ֽ�
 * 8. ��¼����
 * 9. ��¼ģʽ��1�ֽڣ�Ŀǰֻ֧����ͨģʽ
 * 10. δ֪1�ֽڣ�0x40
 * 11. ����εĸ�����1�ֽڣ�1����9�ֽ�(�²�)
 * 12. �Σ�ÿ�λ������ǹ̶����ݣ�δ֪����
 * 13. ���Ȳ�����ȫ����0֪�����ϵ�¼������
 * 14. β��
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
