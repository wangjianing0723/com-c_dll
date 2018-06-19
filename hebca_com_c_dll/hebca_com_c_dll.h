// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HEBCA_COM_C_DLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HEBCA_COM_C_DLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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

