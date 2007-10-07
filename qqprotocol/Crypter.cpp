//*************************************************************************
// Crypter.cpp: implementation of the CCrypter class.
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
#include "Crypter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrypter::CCrypter()
{

}

CCrypter::~CCrypter()
{

}

/**
 * 加密
 * @param in 明文字节数组
 * @param offset 开始加密的偏移
 * @param len 加密长度
 * @param k 密钥
 * @return 密文字节数组
 */
int CCrypter::Encrypt(BYTE *pbIn, int iInOffset, int iInLen, BYTE *pbKey, BYTE *pbRet, int iRetLen) const
{
	// 计算头部填充字节数
	int iPos = (iInLen + 0x0A) % 8;

	if( iPos != 0 )
		iPos = 8 - iPos;

	// 计算输出的密文长度
	int iOutLen = iInLen + iPos + 10;

	if( iRetLen < iOutLen )
		return -1;

	BYTE* pbPlain = (BYTE*)malloc(8);
	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// 这里的操作把pos存到了plain的第一个字节里面
	// 0xF8后面三位是空的，正好留给pos，因为pos是0到7的值，表示文本开始的字节位置
	pbPlain[0] = (BYTE)((rand() & 0xF8) | iPos);

	// 这里用随机产生的数填充plain[1]到plain[pos]之间的内容
	for( int i = 1; i <= iPos; i++ )
		pbPlain[i] = (BYTE)(rand() & 0xFF);
	iPos++;

	// 这个就是prePlain，第一个8字节块当然没有prePlain，所以我们做一个全0的给第一个8字节块
	for( i = 0; i < 8; i++ )
		pbPrePlain[i] = 0x0;

	// 继续填充2个字节的随机数，这个过程中如果满了8字节就加密之
	bool bHeader = true;
	int iPadding = 1;

	while( iPadding <= 2 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = (BYTE)(rand() & 0xFF);
			iPadding++;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	// 头部填充完了，这里开始填真正的明文了，也是满了8字节就加密，一直到明文读完
	i = iInOffset;

	while( iInLen > 0 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = pbIn[i++];
			iInLen--;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	// 最后填上0，以保证是8字节的倍数
	iPadding = 1;
	while( iPadding <= 7 )
	{
		if( iPos < 8 )
		{
			pbPlain[iPos++] = 0x0;
			iPadding++;
		}
		if( iPos == 8 )
		{
			Encrypt8Bytes(bHeader, pbPlain, pbPrePlain, pbKey, iCrypt, iPreCrypt, pbOut);
			iPos = 0;
			bHeader = false;
		}
	}

	memcpy(pbRet, pbOut, iOutLen);
	free(pbOut);
	free(pbPrePlain);
	free(pbPlain);
	return iOutLen;

}

/**
 * 解密
 * @param in 密文
 * @param offset 密文开始的位置
 * @param len 密文长度
 * @param k 密钥
 * @return 明文
 */
int CCrypter::Decrypt(BYTE *pbIn, int iInOffset, int iInLen, BYTE *pbKey, BYTE *pbRet, int iRetLen) const
{
	// 因为QQ消息加密之后至少是16字节，并且肯定是8的倍数，这里检查这种情况
	if( (iInLen % 8 != 0) || (iInLen < 16) )
		return -1;

	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;
	int iCount;
	int iPos;

	// 得到消息的头部，关键是得到真正明文开始的位置，这个信息存在第一个字节里面，所以其用解密得到的第一个字节与7做与
	Decipher(pbIn, iInOffset, pbKey, pbPrePlain);
	iPos = pbPrePlain[0] & 0x7;
	// 得到真正明文的长度
	iCount = iInLen - iPos - 10;
	if( iCount < 0 )	// 如果明文长度小于0，那肯定是出错了，比如传输错误之类的，返回
	{
		free(pbPrePlain);
		return -1;
	}

	// 通过了上面的代码，密文应该是没有问题了，我们分配输出缓冲区
	int iOutLen = iCount;

	if( iRetLen < iCount )
	{
		free(pbPrePlain);
		return -1;
	}

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// 这个是临时的preCrypt，和加密时第一个8字节块没有prePlain一样，解密时
	// 第一个8字节块也没有preCrypt，所有这里建一个全0的
	BYTE* pbM = (BYTE*)malloc(iInOffset + 8);

	for( int i = 0; i < 8; i++ )
		pbM[iInOffset + i] = 0;

	// 设置preCrypt的位置等于0，注意目前的preCrypt位置是指向m的，因为java没有指针，所以我们在后面要控制当前密文buf的引用
	iPreCrypt = 0;
	// 当前的密文位置，为什么是8不是0呢？注意前面我们已经解密了头部信息了，现在当然该8了
	iCrypt = 8;

	// 自然这个也是8
	int iContextStart = 8;

	// 加1，和加密算法是对应的
	iPos++;
        
	// 开始跳过头部，如果在这个过程中满了8字节，则解密下一块
	// 因为是解密下一块，所以我们有一个语句 m = in，下一块当然有preCrypt了，我们不再用m了
	// 但是如果不满8，这说明了什么？说明了头8个字节的密文是包含了明文信息的，当然还是要用m把明文弄出来
	// 所以，很显然，满了8的话，说明了头8个字节的密文除了一个长度信息有用之外，其他都是无用的填充
	int iPadding = 1;

	while( iPadding <= 2 )
	{
		if( iPos < 8 )
		{
			iPos++;
			iPadding++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	// 这里是解密的重要阶段，这个时候头部的填充都已经跳过了，开始解密
	// 注意如果上面一个while没有满8，这里第一个if里面用的就是原始的m，否则这个m就是in了
	i = 0;

	while( iCount != 0 )
	{
		if( iPos < 8 )
		{
			pbOut[i] = (BYTE)(pbM[iInOffset + iPreCrypt + iPos] ^ pbPrePlain[iPos]);
			i++;
			iCount--;
			iPos++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			iPreCrypt = iCrypt - 8;
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	// 最后的解密部分，上面一个while已经把明文都解出来了，就剩下尾部的填充了，应该全是0
	// 所以这里有检查是否解密了之后是不是0，如果不是的话那肯定出错了，返回null
	for( iPadding = 1; iPadding < 8; iPadding++ )
	{
		if( iPos < 8 )
		{
			if( (pbM[iInOffset + iPreCrypt + iPos] ^ pbPrePlain[iPos]) != 0 )
			{
				free(pbOut);
				free(pbPrePlain);
				free(pbM);
				return -1;
			}
			iPos++;
		}
		if( iPos == 8 )
		{
			free(pbM);
			pbM = (BYTE*)malloc(iInLen);
			memcpy(pbM, pbIn, iInLen);
			iPreCrypt = iCrypt;
			Decrypt8Bytes(pbPrePlain, pbKey, iContextStart, iCrypt, iPreCrypt, pbIn, iInOffset, iInLen);
			iPos = 0;
		}
	}

	memcpy(pbRet, pbOut, iOutLen);
	free(pbOut);
	free(pbM);
	free(pbPrePlain);
	return iOutLen;

}

/**
 * 加密8字节
 */
void CCrypter::Encrypt8Bytes(bool bHeader, BYTE *pbPlain, BYTE *pbPrePlain, BYTE *pbKey, int &iCrypt, int &iPreCrypt, BYTE *pbOut) const
{
	// 这部分完成我上面所说的 pbPlain ^ pbPreCrypt，注意这里判断了是不是第一个8字节块，
	//  如果是的话，那个pbPrePlain就当作iPreCrypt用
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( bHeader )
			pbPlain[iPos] ^= pbPrePlain[iPos];
		else
			pbPlain[iPos] ^= pbOut[iPreCrypt + iPos];
	}
	// 这个完成我上面说的 f(pbPlain ^ iPreCrypt)
	BYTE* pbCrypted = (BYTE*)malloc(8);

	Encipher(pbPlain, pbKey, pbCrypted);
	memcpy(pbOut + iCrypt, pbCrypted, 8);
	free(pbCrypted);

	// 这个完成了 f(pbPlain ^ pbPreCrypt) ^ pbPrePlain，ok，下面拷贝一下就行了
	for( iPos = 0; iPos < 8; iPos++ )
		pbOut[iCrypt + iPos] ^= pbPrePlain[iPos];
	memcpy(pbPrePlain, pbPlain, 8);

	// 完成了加密，现在是调整crypt，preCrypt等等东西的时候了
	iPreCrypt = iCrypt;
	iCrypt += 8;

}

/**
 * 解密8个字节
 * 
 * @param in
 * 		密文字节数组
 * @param offset
 * 		从何处开始解密
 * @param len
 * 		密文的长度
 * @return
 * 		true表示解密成功
 */
void CCrypter::Decrypt8Bytes(BYTE *pbPrePlain, BYTE *pbKey, int &iContextStart, int &iCrypt, int &iPreCrypt, BYTE *pbIn, int iInOffset, int iInLen) const
{
	// 这里第一步就是判断后面还有没有数据，没有就返回，如果有，就执行 crypt ^ prePlain
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( iContextStart + iPos >= iInLen )
			return;
		pbPrePlain[iPos] ^= pbIn[iInOffset + iCrypt + iPos];
	}

	// 好，这里执行到了 d(crypt ^ prePlain)
	BYTE* pbDecrypted = (BYTE*)malloc(8);

	Decipher(pbPrePlain, 0, pbKey, pbDecrypted);
	memcpy(pbPrePlain, pbDecrypted, 8);
	free(pbDecrypted);

	// 解密完成，最后一步好像没做？
	// 这里最后一步放到decrypt里面去做了，因为解密的步骤有点不太一样
	// 调整这些变量的值先
	iContextStart += 8;
	iCrypt += 8;

}

/**
 * 加密一个8字节块
 * 
 * @param in
 * 		明文字节数组
 * @return
 * 		密文字节数组
 */
void CCrypter::Encipher(BYTE *pbIn, BYTE *pbKey, BYTE *pbRet) const
{
	// 得到明文和密钥的各个部分，注意java没有无符号类型，所以为了表示一个无符号的整数
	// 我们用了long，这个long的前32位是全0的，我们通过这种方式模拟无符号整数，后面用到的long也都是一样的
	// 而且为了保证前32位为0，需要和0xFFFFFFFF做一下位与
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// 这是算法的一些控制变量，为什么delta是0x9E3779B9呢？
	// 这个数是TEA算法的delta，实际是就是(sqr(5) - 1) * 2^31 (根号5，减1，再乘2的31次方)
	unsigned __int64 ui64Sum = 0;
	unsigned long ulDelta = 0x9E3779B9;

	ulDelta &= 0xFFFFFFFF;

	// 迭代次数，16次
	int iLoop = 0x10;

	// 开始迭代了，乱七八糟的，我也看不懂，反正和DES之类的差不多，都是这样倒来倒去
	while( iLoop-- > 0 )
	{
		ui64Sum += ulDelta;
		ui64Sum &= 0xFFFFFFFFF;

		unsigned __int64 y1, y2, y3, z1, z2, z3;

		y1 = (z << 4) + a;
		y2 = z + ui64Sum;
		y3 = (z / 32) + b;
		y += y1 ^ y2 ^ y3;
		y &= 0xFFFFFFFF;
		z1 = (y << 4) + c;
		z2 = y + ui64Sum;
		z3 = (y / 32) + d;
		z += z1 ^ z2 ^ z3;
		z &= 0xFFFFFFFF;
	}

	// 最后，我们输出密文，因为我用的long，所以需要强制转换一下变成int
	CRYPTER_PUT_DWORD(pbRet, (int)y);
	CRYPTER_PUT_DWORD(pbRet, (int)z);

}

/**
 * 解密从offset开始的8字节密文
 * 
 * @param in
 * 		密文字节数组
 * @param offset
 * 		密文开始位置
 * @return
 * 		明文
 */
void CCrypter::Decipher(BYTE *pbIn, int iInOffset, BYTE *pbKey, BYTE *pbRet) const
{
	pbIn += iInOffset;

	// 得到密文和密钥的各个部分，注意java没有无符号类型，所以为了表示一个无符号的整数
	// 我们用了long，这个long的前32位是全0的，我们通过这种方式模拟无符号整数，后面用到的long也都是一样的
	// 而且为了保证前32位为0，需要和0xFFFFFFFF做一下位与
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// 算法的一些控制变量，sum在这里也有数了，这个sum和迭代次数有关系
	// 因为delta是这么多，所以sum如果是这么多的话，迭代的时候减减减，减16次，最后
	// 得到0。反正这就是为了得到和加密时相反顺序的控制变量，这样才能解密呀～～
	unsigned __int64 ui64Sum = 0xE3779B90;
	unsigned long ulDelta = 0x9E3779B9;

	ui64Sum &= 0xFFFFFFFF;
	ulDelta &= 0xFFFFFFFF;

	// 迭代次数，16次
	int iLoop = 0x10;

	// 迭代开始了， @_@
	while( iLoop-- > 0 )
	{
		unsigned __int64 y1, y2, y3, z1, z2, z3;

		z1 = (y << 4) + c;
		z2 = y + ui64Sum;
		z3 = (y / 32) + d;
		z -= z1 ^ z2 ^ z3;
		z &= 0xFFFFFFFF;
		y1 = (z << 4) + a;
		y2 = z + ui64Sum;
		y3 = (z / 32) + b;
		y -= y1 ^ y2 ^ y3;
		y &= 0xFFFFFFFF;
		ui64Sum -= ulDelta;
		ui64Sum &= 0xFFFFFFFF;
	}

	CRYPTER_PUT_DWORD(pbRet, (int)y);
	CRYPTER_PUT_DWORD(pbRet, (int)z);

}
