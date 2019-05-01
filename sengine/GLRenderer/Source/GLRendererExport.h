#ifdef OPENGL_RENDERER_EXPORTS
#define OPENGL_RENDERER_DLL_EXPORT __declspec(dllexport)
#else 
#define OPENGL_RENDERER_DLL_EXPORT __declspec(dllimport)
#endif