// DLL header file
#ifndef DICOMDLL_H
#define DICOMDLL_H

#ifdef DICOMDLL_EXPORTS
#define DICOMDLL_API __declspec(dllexport)
#else
#define DICOMDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


DICOMDLL_API int __stdcall createBinFile_ext(char *, char *, char *, char);


#ifdef __cplusplus
}
#endif

#endif

