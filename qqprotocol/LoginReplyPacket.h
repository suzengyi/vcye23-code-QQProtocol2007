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
 * QQ��½Ӧ���
 * 1. ͷ��
 * 2. �ظ���, 1�ֽ�
 * 2���������0x00
 * 3. session key, 16�ֽ�
 * 4. �û�QQ�ţ�4�ֽ�
 * 5. �ҵ��ⲿIP��4�ֽ�
 * 6. �ҵ��ⲿ�˿ڣ�2�ֽ�
 * 7. ������IP��4�ֽ�
 * 8. �������˿ڣ�2�ֽ�
 * 9. ���ε�¼ʱ�䣬4�ֽڣ�Ϊ��1970-1-1��ʼ�ĺ�������1000
 * 10. δ֪��2�ֽ�
 * 11. �û���֤����,24�ֽ�
 * 12. һ��δ֪������1��ip��4�ֽ�
 * 13. һ��δ֪������1�Ķ˿ڣ�2�ֽ�
 * 14. һ��δ֪������2��ip��4�ֽ�
 * 15. һ��δ֪������2�Ķ˿ڣ�2�ֽ�
 * 16. ����δ֪�ֽ�
 * 17. ����δ֪�ֽ�
 * 18. client key��32�ֽڣ����key���ڱ����¼QQ��԰֮��ĵط�
 * 19. 12��δ֪�ֽ�
 * 20. �ϴε�½��ip��4�ֽ�
 * 21. �ϴε�½��ʱ�䣬4�ֽ�
 * 22. 39��δ֪�ֽ�
 * 2���������0x01����ʾ�ض���
 * 3. �û�QQ�ţ�4�ֽ�
 * 4. �ض��򵽵ķ�����IP��4�ֽ�
 * 5. �ض��򵽵ķ������Ķ˿ڣ�2�ֽ�
 * 2���������0x05����ʾ��¼ʧ��
 * 3. һ��������Ϣ
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
