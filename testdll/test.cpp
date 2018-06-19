#include <iostream>
#include "hebca_com_c_dll.h"
using namespace std;
//#pragma comment(lib,"hebca_com_c_dll")

int main()
{
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
