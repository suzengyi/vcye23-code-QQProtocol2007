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
 * ����
 * @param in �����ֽ�����
 * @param offset ��ʼ���ܵ�ƫ��
 * @param len ���ܳ���
 * @param k ��Կ
 * @return �����ֽ�����
 */
int CCrypter::Encrypt(BYTE *pbIn, int iInOffset, int iInLen, BYTE *pbKey, BYTE *pbRet, int iRetLen) const
{
	// ����ͷ������ֽ���
	int iPos = (iInLen + 0x0A) % 8;

	if( iPos != 0 )
		iPos = 8 - iPos;

	// ������������ĳ���
	int iOutLen = iInLen + iPos + 10;

	if( iRetLen < iOutLen )
		return -1;

	BYTE* pbPlain = (BYTE*)malloc(8);
	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// ����Ĳ�����pos�浽��plain�ĵ�һ���ֽ�����
	// 0xF8������λ�ǿյģ���������pos����Ϊpos��0��7��ֵ����ʾ�ı���ʼ���ֽ�λ��
	pbPlain[0] = (BYTE)((rand() & 0xF8) | iPos);

	// ��������������������plain[1]��plain[pos]֮�������
	for( int i = 1; i <= iPos; i++ )
		pbPlain[i] = (BYTE)(rand() & 0xFF);
	iPos++;

	// �������prePlain����һ��8�ֽڿ鵱Ȼû��prePlain������������һ��ȫ0�ĸ���һ��8�ֽڿ�
	for( i = 0; i < 8; i++ )
		pbPrePlain[i] = 0x0;

	// �������2���ֽڵ������������������������8�ֽھͼ���֮
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

	// ͷ��������ˣ����￪ʼ�������������ˣ�Ҳ������8�ֽھͼ��ܣ�һֱ�����Ķ���
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

	// �������0���Ա�֤��8�ֽڵı���
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
 * ����
 * @param in ����
 * @param offset ���Ŀ�ʼ��λ��
 * @param len ���ĳ���
 * @param k ��Կ
 * @return ����
 */
int CCrypter::Decrypt(BYTE *pbIn, int iInOffset, int iInLen, BYTE *pbKey, BYTE *pbRet, int iRetLen) const
{
	// ��ΪQQ��Ϣ����֮��������16�ֽڣ����ҿ϶���8�ı������������������
	if( (iInLen % 8 != 0) || (iInLen < 16) )
		return -1;

	BYTE* pbPrePlain = (BYTE*)malloc(8);
	int iCrypt = 0;
	int iPreCrypt = 0;
	int iCount;
	int iPos;

	// �õ���Ϣ��ͷ�����ؼ��ǵõ��������Ŀ�ʼ��λ�ã������Ϣ���ڵ�һ���ֽ����棬�������ý��ܵõ��ĵ�һ���ֽ���7����
	Decipher(pbIn, iInOffset, pbKey, pbPrePlain);
	iPos = pbPrePlain[0] & 0x7;
	// �õ��������ĵĳ���
	iCount = iInLen - iPos - 10;
	if( iCount < 0 )	// ������ĳ���С��0���ǿ϶��ǳ����ˣ����紫�����֮��ģ�����
	{
		free(pbPrePlain);
		return -1;
	}

	// ͨ��������Ĵ��룬����Ӧ����û�������ˣ����Ƿ������������
	int iOutLen = iCount;

	if( iRetLen < iCount )
	{
		free(pbPrePlain);
		return -1;
	}

	BYTE* pbOut = (BYTE*)malloc(iOutLen);

	// �������ʱ��preCrypt���ͼ���ʱ��һ��8�ֽڿ�û��prePlainһ��������ʱ
	// ��һ��8�ֽڿ�Ҳû��preCrypt���������ｨһ��ȫ0��
	BYTE* pbM = (BYTE*)malloc(iInOffset + 8);

	for( int i = 0; i < 8; i++ )
		pbM[iInOffset + i] = 0;

	// ����preCrypt��λ�õ���0��ע��Ŀǰ��preCryptλ����ָ��m�ģ���Ϊjavaû��ָ�룬���������ں���Ҫ���Ƶ�ǰ����buf������
	iPreCrypt = 0;
	// ��ǰ������λ�ã�Ϊʲô��8����0�أ�ע��ǰ�������Ѿ�������ͷ����Ϣ�ˣ����ڵ�Ȼ��8��
	iCrypt = 8;

	// ��Ȼ���Ҳ��8
	int iContextStart = 8;

	// ��1���ͼ����㷨�Ƕ�Ӧ��
	iPos++;
        
	// ��ʼ����ͷ����������������������8�ֽڣ��������һ��
	// ��Ϊ�ǽ�����һ�飬����������һ����� m = in����һ�鵱Ȼ��preCrypt�ˣ����ǲ�����m��
	// �����������8����˵����ʲô��˵����ͷ8���ֽڵ������ǰ�����������Ϣ�ģ���Ȼ����Ҫ��m������Ū����
	// ���ԣ�����Ȼ������8�Ļ���˵����ͷ8���ֽڵ����ĳ���һ��������Ϣ����֮�⣬�����������õ����
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

	// �����ǽ��ܵ���Ҫ�׶Σ����ʱ��ͷ������䶼�Ѿ������ˣ���ʼ����
	// ע���������һ��whileû����8�������һ��if�����õľ���ԭʼ��m���������m����in��
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

	// ���Ľ��ܲ��֣�����һ��while�Ѿ������Ķ�������ˣ���ʣ��β��������ˣ�Ӧ��ȫ��0
	// ���������м���Ƿ������֮���ǲ���0��������ǵĻ��ǿ϶������ˣ�����null
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
 * ����8�ֽ�
 */
void CCrypter::Encrypt8Bytes(bool bHeader, BYTE *pbPlain, BYTE *pbPrePlain, BYTE *pbKey, int &iCrypt, int &iPreCrypt, BYTE *pbOut) const
{
	// �ⲿ�������������˵�� pbPlain ^ pbPreCrypt��ע�������ж����ǲ��ǵ�һ��8�ֽڿ飬
	//  ����ǵĻ����Ǹ�pbPrePlain�͵���iPreCrypt��
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( bHeader )
			pbPlain[iPos] ^= pbPrePlain[iPos];
		else
			pbPlain[iPos] ^= pbOut[iPreCrypt + iPos];
	}
	// ������������˵�� f(pbPlain ^ iPreCrypt)
	BYTE* pbCrypted = (BYTE*)malloc(8);

	Encipher(pbPlain, pbKey, pbCrypted);
	memcpy(pbOut + iCrypt, pbCrypted, 8);
	free(pbCrypted);

	// �������� f(pbPlain ^ pbPreCrypt) ^ pbPrePlain��ok�����濽��һ�¾�����
	for( iPos = 0; iPos < 8; iPos++ )
		pbOut[iCrypt + iPos] ^= pbPrePlain[iPos];
	memcpy(pbPrePlain, pbPlain, 8);

	// ����˼��ܣ������ǵ���crypt��preCrypt�ȵȶ�����ʱ����
	iPreCrypt = iCrypt;
	iCrypt += 8;

}

/**
 * ����8���ֽ�
 * 
 * @param in
 * 		�����ֽ�����
 * @param offset
 * 		�Ӻδ���ʼ����
 * @param len
 * 		���ĵĳ���
 * @return
 * 		true��ʾ���ܳɹ�
 */
void CCrypter::Decrypt8Bytes(BYTE *pbPrePlain, BYTE *pbKey, int &iContextStart, int &iCrypt, int &iPreCrypt, BYTE *pbIn, int iInOffset, int iInLen) const
{
	// �����һ�������жϺ��滹��û�����ݣ�û�оͷ��أ�����У���ִ�� crypt ^ prePlain
	for( int iPos = 0; iPos < 8; iPos++ )
	{
		if( iContextStart + iPos >= iInLen )
			return;
		pbPrePlain[iPos] ^= pbIn[iInOffset + iCrypt + iPos];
	}

	// �ã�����ִ�е��� d(crypt ^ prePlain)
	BYTE* pbDecrypted = (BYTE*)malloc(8);

	Decipher(pbPrePlain, 0, pbKey, pbDecrypted);
	memcpy(pbPrePlain, pbDecrypted, 8);
	free(pbDecrypted);

	// ������ɣ����һ������û����
	// �������һ���ŵ�decrypt����ȥ���ˣ���Ϊ���ܵĲ����е㲻̫һ��
	// ������Щ������ֵ��
	iContextStart += 8;
	iCrypt += 8;

}

/**
 * ����һ��8�ֽڿ�
 * 
 * @param in
 * 		�����ֽ�����
 * @return
 * 		�����ֽ�����
 */
void CCrypter::Encipher(BYTE *pbIn, BYTE *pbKey, BYTE *pbRet) const
{
	// �õ����ĺ���Կ�ĸ������֣�ע��javaû���޷������ͣ�����Ϊ�˱�ʾһ���޷��ŵ�����
	// ��������long�����long��ǰ32λ��ȫ0�ģ�����ͨ�����ַ�ʽģ���޷��������������õ���longҲ����һ����
	// ����Ϊ�˱�֤ǰ32λΪ0����Ҫ��0xFFFFFFFF��һ��λ��
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// �����㷨��һЩ���Ʊ�����Ϊʲôdelta��0x9E3779B9�أ�
	// �������TEA�㷨��delta��ʵ���Ǿ���(sqr(5) - 1) * 2^31 (����5����1���ٳ�2��31�η�)
	unsigned __int64 ui64Sum = 0;
	unsigned long ulDelta = 0x9E3779B9;

	ulDelta &= 0xFFFFFFFF;

	// ����������16��
	int iLoop = 0x10;

	// ��ʼ�����ˣ����߰���ģ���Ҳ��������������DES֮��Ĳ�࣬��������������ȥ
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

	// �������������ģ���Ϊ���õ�long��������Ҫǿ��ת��һ�±��int
	CRYPTER_PUT_DWORD(pbRet, (int)y);
	CRYPTER_PUT_DWORD(pbRet, (int)z);

}

/**
 * ���ܴ�offset��ʼ��8�ֽ�����
 * 
 * @param in
 * 		�����ֽ�����
 * @param offset
 * 		���Ŀ�ʼλ��
 * @return
 * 		����
 */
void CCrypter::Decipher(BYTE *pbIn, int iInOffset, BYTE *pbKey, BYTE *pbRet) const
{
	pbIn += iInOffset;

	// �õ����ĺ���Կ�ĸ������֣�ע��javaû���޷������ͣ�����Ϊ�˱�ʾһ���޷��ŵ�����
	// ��������long�����long��ǰ32λ��ȫ0�ģ�����ͨ�����ַ�ʽģ���޷��������������õ���longҲ����һ����
	// ����Ϊ�˱�֤ǰ32λΪ0����Ҫ��0xFFFFFFFF��һ��λ��
	unsigned long y, z;
	unsigned long a, b, c, d;

	CRYPTER_GET_DWORD(pbIn, y);
	CRYPTER_GET_DWORD(pbIn, z);
	CRYPTER_GET_DWORD(pbKey, a);
	CRYPTER_GET_DWORD(pbKey, b);
	CRYPTER_GET_DWORD(pbKey, c);
	CRYPTER_GET_DWORD(pbKey, d);

	// �㷨��һЩ���Ʊ�����sum������Ҳ�����ˣ����sum�͵��������й�ϵ
	// ��Ϊdelta����ô�࣬����sum�������ô��Ļ���������ʱ�����������16�Σ����
	// �õ�0�����������Ϊ�˵õ��ͼ���ʱ�෴˳��Ŀ��Ʊ������������ܽ���ѽ����
	unsigned __int64 ui64Sum = 0xE3779B90;
	unsigned long ulDelta = 0x9E3779B9;

	ui64Sum &= 0xFFFFFFFF;
	ulDelta &= 0xFFFFFFFF;

	// ����������16��
	int iLoop = 0x10;

	// ������ʼ�ˣ� @_@
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
