// hebca_com_c_dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "hebca_com_c_dll.h"

#import "C:\Windows\SysWOW64\HebcaP11X.dll" raw_interfaces_only,no_namespace,named_guids\
	rename("value","cvalue") \
	rename("GetOpenFileName","cGetOpenFileName") \
	rename("GetSaveFileName","cGetSaveFileName")

HRESULT ret;
ICertMgrPtr pCertMgr;
ICert * pCert = NULL;
IDevice * pDevice = NULL;
IUtilPtr pUtil;
// 这是导出函数的一个示例。
HEBCA_COM_C_DLL_API int Add(int a,int b)
{
	return a+b;
}
HEBCA_COM_C_DLL_API int InitKey(void)
{
	long devcount = -1;
	ret = CoInitialize(0);
	if (FAILED(ret))
	{
		return -2;
	}

	ret = pCertMgr.CreateInstance(__uuidof(CertMgr));
	if (FAILED(ret))
	{
		return -3;
	}
	BSTR license = _bstr_t("amViY55oZWKcZmhlnXxhaGViY2GXGmNjYWh9YgsECgYDTykqNi9RIilJKgYjDQwwMCArJLM/+UWLrf1R3+2v1RD1WRWwwdCI");
	pCertMgr->put_Licence(license);
	pCertMgr->GetDeviceCount(&devcount);
	if (devcount < 1)
	{
		return -1;
	}
	return devcount;
}
HEBCA_COM_C_DLL_API int SelectCert(void)
{
	ret = pCertMgr->SelectSignCert(&pCert);
	if (ret == 0)
	{
		return 0;
	}
	return 1;
}
HEBCA_COM_C_DLL_API unsigned char* GetSignCert()
{
	BSTR CertB64;
	int c = 0;
	ret = pCert->GetCertB64(&CertB64);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	//int a = SysStringLen(CertB64);
	//unsigned char* b = new unsigned char[a];
	//for (int i = 0; i < a; i++)
	//{
	//	b[i] = (unsigned char)CertB64[i];
	//}
	_bstr_t _bstCertTmp = CertB64;
	char * _bstCert = _bstCertTmp;
	c = strlen(_bstCert);
	unsigned char tmpcert[2000] = "";
	memcpy(tmpcert, _bstCert, c);
	printf("cert:%s\n", tmpcert);
	return tmpcert;
}
HEBCA_COM_C_DLL_API unsigned char*  Sign(unsigned char* oraData)
{
	BSTR signdata,hash;
	int c = -2;
	BSTR ora = _com_util::ConvertStringToBSTR((const char*)oraData);
	//BSTR ora = _bstr_t("123");
	pCertMgr->get_Util(&pUtil);
	ret = pUtil->HashText(ora, HASH_SM3, &hash);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	ret = pCert->SignText(hash,SM3SM2,&signdata);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	_bstr_t _bstSignTmp = signdata;
	char * _bstSign = _bstSignTmp;
	c = strlen(_bstSign);
	unsigned char tmpSign[128] = "";
	memcpy(tmpSign, _bstSign, c);
	printf("sign:%s\n", tmpSign);
	return tmpSign;
}
HEBCA_COM_C_DLL_API unsigned char* GetPic()
{
	IDevice * pDevice = NULL;
	BSTR pic;
	int c = 0;
	ret = pCertMgr->GetDevice(0, &pDevice);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	ret = pDevice->ReadDataB64(_bstr_t("signpic"), &pic);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	_bstr_t _bstPicTmp = pic;
	char * _bstPic = _bstPicTmp;
	c = strlen(_bstPic);
	unsigned char tmpPic[10240] = "";
	memcpy(tmpPic, _bstPic, c);
	printf("pic:%s\n", tmpPic);
	return tmpPic;
}
HEBCA_COM_C_DLL_API unsigned char* Sm3Hash(unsigned char* oraData)
{
	BSTR hash;
	int c = -2;
	BSTR ora = _com_util::ConvertStringToBSTR((const char*)oraData);
	//BSTR ora = _bstr_t("123");
	pCertMgr->get_Util(&pUtil);
	ret = pUtil->HashText(ora, HASH_SM3, &hash);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	_bstr_t _bstHashTmp = hash;
	char * _bstHash = _bstHashTmp;
	c = strlen(_bstHash);
	unsigned char tmpHash[48] = "";
	memcpy(tmpHash, _bstHash, c);
	printf("sm3:%s\n", tmpHash);
	return tmpHash;
}
HEBCA_COM_C_DLL_API unsigned char* GetSubject(void)
{
	BSTR subject;
	int c = 0;
	ret = pCert->GetSubject(&subject);
	if (FAILED(ret))
	{
		return (unsigned char*)"null";
	}
	//int a = SysStringLen(CertB64);
	//unsigned char* b = new unsigned char[a];
	//for (int i = 0; i < a; i++)
	//{
	//	b[i] = (unsigned char)CertB64[i];
	//}
	_bstr_t _bstSubjectTmp = subject;
	char * _bstSubject = _bstSubjectTmp;
	c = strlen(_bstSubject);
	unsigned char tmpSubject[256] = "";
	memcpy(tmpSubject, _bstSubject, c);
	printf("subject:%s\n", tmpSubject);
	return tmpSubject;
}