//*************************************************************************
// Crypter.h: interface for the CCrypter class.
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

#if !defined(AFX_CRYPTER_H__0932A307_C276_4223_95E3_FCA3DB7F5F53__INCLUDED_)
#define AFX_CRYPTER_H__0932A307_C276_4223_95E3_FCA3DB7F5F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CRYPTER_PUT_DWORD(pbBuf, dw)	\
{	\
	*pbBuf = (dw & 0xFF000000) >> 24;	\
	*(pbBuf + 1) = (dw & 0xFF0000) >> 16;	\
	*(pbBuf + 2) = (dw & 0xFF00) >> 8;	\
	*(pbBuf + 3) = dw & 0xFF;	\
	pbBuf += sizeof(DWORD);	\
}

#define CRYPTER_GET_DWORD(pbBuf, dw)	\
{	\
	dw = *pbBuf << 24;	\
	dw += *(pbBuf + 1) << 16;	\
	dw += *(pbBuf + 2) << 8;	\
	dw += *(pbBuf + 3);	\
	pbBuf += sizeof(DWORD);	\
}

/**
 * ���ܽ���QQ��Ϣ�Ĺ�����. QQ��Ϣ�ļ����㷨��һ��16�εĵ������̣������Ƿ����ģ�ÿһ�����ܵ�Ԫ��8�ֽڣ����Ҳ��8�ֽڣ���Կ��16�ֽ�
 * ������prePlain��ʾǰһ�����Ŀ飬plain��ʾ��ǰ���Ŀ飬crypt��ʾ��ǰ���Ŀ���ܵõ������Ŀ飬preCrypt��ʾǰһ�����Ŀ�
 * f��ʾ�����㷨��d��ʾ�����㷨 ��ô��plain�õ�crypt�Ĺ�����: crypt = f(plain &circ; preCrypt) &circ;
 * prePlain ���ԣ���crypt�õ�plain�Ĺ�����Ȼ�� plain = d(crypt &circ; prePlain) &circ;
 * preCrypt ���⣬�㷨�����������ƣ����������ǰ�����ĺ�ֱ����һ�����ֽ������Ա�֤���ĳ�����8�ֽڵı���
 * �����ֽ�����ԭʼ���ĳ����йأ����ķ�����:
 * 
 * <pre>
 * <code>
 * 
 *      ------- ��Ϣ����㷨 ----------- 
 *      a = (���ĳ��� + 10) mod 8
 *      if(a ������ 0) a = 8 - a;
 *      b = ����� &amp; 0xF8 | a;              ����������ǰ�a��ֵ����������
 *      plain[0] = b;         	          Ȼ���b��Ϊ���ĵĵ�0���ֽڣ�������0���ֽھͱ�����a����Ϣ�������Ϣ�ڽ���ʱ��Ҫ�����ҵ��������ĵ���ʼλ��
 *      plain[1 �� a+2] = ����� &amp; 0xFF;    �����������������ĵĵ�1����a+2���ֽ�
 *      plain[a+3 �� a+3+���ĳ���-1] = ����; ��a+3�ֽڿ�ʼ��������������
 *      plain[a+3+���ĳ���, ���] = 0;       ��������0����䵽�ܳ���Ϊ8������Ϊֹ������Ϊֹ�������ˣ���������õ���Ҫ���ܵ���������
 *      ------- ��Ϣ����㷨 ------------
 *   
 * </code>
 * </pre>
 * 
 * @author luma
 * @author notXX
 */
class CCrypter  
{
public:
	CCrypter();
	virtual ~CCrypter();

public:
	int Decrypt(BYTE* pbIn, int iInOffset, int iInLen, BYTE* pbKey, BYTE* pbRet, int iRetLen) const;
	int Encrypt(BYTE* pbIn, int iInOffset, int iInLen, BYTE* pbKey, BYTE* pbRet, int iRetLen) const;

protected:
	void Decipher(BYTE* pbIn, int iInOffset, BYTE* pbKey, BYTE* pbRet) const;
	void Encipher(BYTE* pbIn, BYTE* pbKey, BYTE* pbRet) const;
	void Decrypt8Bytes(BYTE* pbPrePlain, BYTE* pbKey, int& iContextStart, int& iCrypt, int& iPreCrypt, BYTE* pbIn, int iInOffset, int iInLen) const;
	void Encrypt8Bytes(bool bHeader, BYTE* pbPlain, BYTE* pbPrePlain, BYTE* pbKey, int& iCrypt, int& iPreCrypt, BYTE* pbOut) const;
};

#endif // !defined(AFX_CRYPTER_H__0932A307_C276_4223_95E3_FCA3DB7F5F53__INCLUDED_)
