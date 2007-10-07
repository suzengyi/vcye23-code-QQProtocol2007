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
 * 加密解密QQ消息的工具类. QQ消息的加密算法是一个16次的迭代过程，并且是反馈的，每一个加密单元是8字节，输出也是8字节，密钥是16字节
 * 我们以prePlain表示前一个明文块，plain表示当前明文块，crypt表示当前明文块加密得到的密文块，preCrypt表示前一个密文块
 * f表示加密算法，d表示解密算法 那么从plain得到crypt的过程是: crypt = f(plain &circ; preCrypt) &circ;
 * prePlain 所以，从crypt得到plain的过程自然是 plain = d(crypt &circ; prePlain) &circ;
 * preCrypt 此外，算法有它的填充机制，其会在明文前和明文后分别填充一定的字节数，以保证明文长度是8字节的倍数
 * 填充的字节数与原始明文长度有关，填充的方法是:
 * 
 * <pre>
 * <code>
 * 
 *      ------- 消息填充算法 ----------- 
 *      a = (明文长度 + 10) mod 8
 *      if(a 不等于 0) a = 8 - a;
 *      b = 随机数 &amp; 0xF8 | a;              这个的作用是把a的值保存了下来
 *      plain[0] = b;         	          然后把b做为明文的第0个字节，这样第0个字节就保存了a的信息，这个信息在解密时就要用来找到真正明文的起始位置
 *      plain[1 至 a+2] = 随机数 &amp; 0xFF;    这里用随机数填充明文的第1到第a+2个字节
 *      plain[a+3 至 a+3+明文长度-1] = 明文; 从a+3字节开始才是真正的明文
 *      plain[a+3+明文长度, 最后] = 0;       在最后，填充0，填充到总长度为8的整数为止。到此为止，结束了，这就是最后得到的要加密的明文内容
 *      ------- 消息填充算法 ------------
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
