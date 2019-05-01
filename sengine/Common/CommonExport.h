#ifdef COMMON_EXPORTS
#define COMMON_DLL_EXPORT __declspec(dllexport)
#else 
#define COMMON_DLL_EXPORT __declspec(dllimport)
#endif