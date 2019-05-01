#ifdef DIRECTX_RENDERER_EXPORTS
#define DIRECTX_RENDERER_DLL_EXPORT __declspec(dllexport)
#else 
#define DIRECTX_RENDERER_DLL_EXPORT __declspec(dllimport)
#endif