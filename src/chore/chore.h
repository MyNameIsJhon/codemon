
#ifndef HOTRELOAD_ABSTRACTION_LAYER
#define HOTRELOAD_ABSTRACTION_LAYER

#if defined(_WIN32) || defined(_WIN64)
    #define HRAL_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #define HRAL_PLATFORM_MACOS
	#define HRAL_PLATFORM_POSIX
#elif defined(__linux__)
    #define HRAL_PLATFORM_LINUX
	#define HRAL_PLATFORM_POSIX
#else
    #error "Unsupported platform for hot-reloading"
#endif
#ifdef HRAL_PLATFORM_WINDOWS
    #include <windows.h>
#elif defined(HRAL_PLATFORM_POSIX)
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <limits.h>
	#include <string.h>
	#include <stdarg.h>
	#include <dirent.h>
	#include <sys/stat.h>
	#include <time.h>
	#include <dlfcn.h>
	#if defined(HRAL_PLATFORM_MACOS)
		#include <mach-o/dyld.h>
		#include <mach-o/nlist.h>
	#elif defined(HRAL_PLATFORM_LINUX)
		#include <dlfcn.h>
		#include <elf.h>
		#include <link.h>
	#endif
#endif

#ifdef HRAL_PLATFORM_WINDOWS
    #define HRAL_LIB_EXTENSION ".dll"
    #define HRAL_LIB_PREFIX ""
	#define HRAL_PATH_SEPARATOR '\\'
#elif defined(HRAL_PLATFORM_MACOS)
    #define HRAL_LIB_EXTENSION ".dylib"
    #define HRAL_LIB_PREFIX "lib"
	#define HRAL_PATH_SEPARATOR '/'
#elif defined(HRAL_PLATFORM_LINUX)
    #define HRAL_LIB_EXTENSION ".so"
    #define HRAL_LIB_PREFIX "lib"
	#define HRAL_PATH_SEPARATOR '/'
#endif

typedef union HRAL_ReturnValue
{
    int i;
    float f;
    double d;
    char* s;
    void* p;
} HRAL_ReturnValue;


typedef enum HRAL_ERROR
{
	SUCCESS,
	FAIL,
	COMPILATION_ERROR,
	RELOAD_ERROR,
	LOAD_ERROR
} HRAL_ERROR;

typedef struct HRAL_Function
{
	char*	name;
	void*	functionPtr;
}	HRAL_Function;

typedef struct HRAL_Lib
{
	char*			path;
	char*			buildCommand;
	void			*bin;
	int				isLoaded;
	time_t			lastModified;
	HRAL_Function*	functions;
	int				functionCount;
}	HRAL_Lib;

typedef struct HRAL_Context
{
	HRAL_Lib*	libs;
	int			libCount;
	int			libCapacity;
}	HRAL_Context;

#define HRAL_CALL(lib, return_type, func_name, ...) ((return_type (*)(typeof(__VA_ARGS__)))HRAL_GetFunction(lib, #func_name))(__VA_ARGS__)
/* #define HRAL_CALL_CTX(ctx, return_type, func_name, ...) ((return_type (*)(typeof(__VA_ARGS__)))HRAL_GetFunctionFromContext(ctx, #func_name))(__VA_ARGS__) */
/* #define HRAL_CALL_CTX(context, return_type, function_name, ...)											\ */
/* 	do																									\ */
/* 	{																									\ */
/* 		void *func_ptr = HRAL_GetFunctionFromContext(context, #function_name);							\ */
/* 		if (func_ptr)																					\ */
/* 		{																								\ */
/* 			return_type (*typed_func_ptr)() = (return_type (*)())func_ptr;								\ */
/* 			typed_func_ptr(__VA_ARGS__);																\ */
/* 		}																								\ */
/* 		else																							\ */
/* 		{																								\ */
/* 			fprintf(stderr, "Function %s not found in any loaded library\n", #function_name);			\ */
/* 		}																								\ */
/* 	} while(0) */
/**/

time_t HRAL_GetLastModified(const char* path);
int HRAL_HasFileChanged(HRAL_Lib* lib);
int HRAL_IsSourceNewer(const char* folder, const char* libPath);
char* HRAL_GetParentDirectory(const char* path);
char* HRAL_GetAbsolutePath(const char* path);
HRAL_Lib* HRAL_DeclareLibrary(HRAL_Context* ctx, const char* libPath, const char* buildCommand);
void HRAL_CheckForReload(HRAL_Context* ctx);

int HRAL_CallFunction(HRAL_Context* ctx, const char* func_name, void* ret_value, const char* ret_type, ...);

void* HRAL_GetFunctionFromContext(HRAL_Context* ctx, const char* func_name);
#endif
