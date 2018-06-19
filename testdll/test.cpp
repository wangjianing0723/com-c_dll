#include <iostream>
#include "hebca_com_c_dll.h"
using namespace std;
//#pragma comment(lib,"hebca_com_c_dll")

int main()
{
	//int a, b, result;
	//while (cin >> a >> b)
	//{
	//	result = Add(a, b);
	//	cout << "Result : " << result << endl;
	//}
	int re,c = -214747;
	unsigned char *signcert;
	unsigned char *signdata;
	unsigned char *pic;
	unsigned char * hash;
	unsigned char *subject;
	unsigned char signdata1[128] = "";
	unsigned char hashdata[45] = "";
	unsigned char picdata[10240] = ""; 
	unsigned char ora[] = "Íõ¼ÑÄþ43.¡£}q";
	re = InitKey();
	re = SelectCert();
	signcert = GetSignCert();
	subject = GetSubject();
	//signdata = Sign(ora);
	//c = strlen((const char*)signdata);
	//memcpy(signdata1, signdata, c);
	//printf("%s\n", signdata1);
	hash = Sm3Hash(ora);
	c = strlen((const char*)hash);
	memcpy(hashdata, hash, c);
	printf("%s\n", hashdata);
	pic = GetPic();
	c = strlen((const char*)pic);
	memcpy(picdata, pic, c);
	printf("%s\n", picdata);
	return 0;
}