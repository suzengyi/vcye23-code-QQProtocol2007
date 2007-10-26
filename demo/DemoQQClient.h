// DemoQQClient.h: interface for the CDemoQQClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOQQCLIENT_H__E970D916_8B73_43E6_8C78_7C61989C04D8__INCLUDED_)
#define AFX_DEMOQQCLIENT_H__E970D916_8B73_43E6_8C78_7C61989C04D8__INCLUDED_

#include "../QQProtocol/QQClient.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemoQQClient : public CQQClient  
{
public:
	CDemoQQClient();
	virtual ~CDemoQQClient();

protected:
	void LoginFail(int iError);
	void LoginSuccess();

};

#endif // !defined(AFX_DEMOQQCLIENT_H__E970D916_8B73_43E6_8C78_7C61989C04D8__INCLUDED_)
