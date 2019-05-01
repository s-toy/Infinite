#ifdef RENDER_ENGINE_EXPORTS
#define RENDER_ENGINE_DLL_EXPORT __declspec(dllexport)
#else 
#define RENDER_ENGINE_DLL_EXPORT __declspec(dllimport)
#endif