// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HEBCA_COM_C_DLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HEBCA_COM_C_DLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#include <string.h>
#ifdef HEBCA_COM_C_DLL_EXPORTS
#define HEBCA_COM_C_DLL_API __declspec(dllexport)
#else
#define HEBCA_COM_C_DLL_API __declspec(dllimport)
#endif


HEBCA_COM_C_DLL_API int Add(int a,int b);
HEBCA_COM_C_DLL_API int InitKey(void);
HEBCA_COM_C_DLL_API int SelectCert(void);
HEBCA_COM_C_DLL_API unsigned char* GetSignCert(void);
HEBCA_COM_C_DLL_API unsigned char* Sign(unsigned char* oraData);
HEBCA_COM_C_DLL_API unsigned char* GetPic(void);
HEBCA_COM_C_DLL_API unsigned char* Sm3Hash(unsigned char* oraData);
HEBCA_COM_C_DLL_API unsigned char* GetSubject();

