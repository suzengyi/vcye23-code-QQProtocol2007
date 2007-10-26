// DemoQQClient.cpp: implementation of the CDemoQQClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DemoQQClient.h"

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDemoQQClient::CDemoQQClient()
{

}

CDemoQQClient::~CDemoQQClient()
{

}

void CDemoQQClient::LoginSuccess()
{
	std::cout << _T("Login Success") << std::endl;

}

void CDemoQQClient::LoginFail(int iError)
{
	std::cerr << _T("Login failed") << std::endl;

}
