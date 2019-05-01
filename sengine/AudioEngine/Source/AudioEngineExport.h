#ifdef AUDIO_ENGINE_EXPORTS
#define AUDIO_ENGINE_DLL_EXPORT __declspec(dllexport)
#else 
#define AUDIO_ENGINE_DLL_EXPORT __declspec(dllimport)
#endif