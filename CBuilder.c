#include "src/extern/raylib-5.5/src/raylib.h"
#if 0
OS="$(uname -s 2>/dev/null || echo Windows)"
DIR="$(cd "$(dirname "$0")" && pwd)"
CBUILDER="$DIR/CBuilder"

if [ "$OS" = "Linux" ] || [ "$OS" = "Darwin" ]; then
    cc "$DIR/CBuilder.c" -o "$CBUILDER" && "$CBUILDER"
fi
exit
#endif


/*
 * CBuild V0.0.2
 */

#ifndef		_CBUILD_
	#define	_CBUILD_
	#define	BUILDER_SOURCE		"CBuilder.c"
	#define	BUILDER_BASE_NAME	"CBuilder"
	#if (defined(__MINGW64__) || defined(__MINGW32__)) && (defined(_WIN32) || defined(_WIN64))	// WARN: who cares about mingw?
		#define DEFAULT_COMPILER		"x86_64-w64-mingw32-gcc"
	#elif defined(__clang__)
		#define DEFAULT_COMPILER		"clang"
	#elif defined(__GNUC__)
		#define DEFAULT_COMPILER		"gcc"
	#elif defined(_MSC_VER)
		#define DEFAULT_COMPILER		"cl"
	#elif defined(__TINYC__)
		#define DEFAULT_COMPILER		"tcc"
	#endif
	#if defined(_WIN32) || defined(_WIN64)
		#include <windows.h>
		#include <direct.h>
		#include <sys/stat.h>
		#ifndef		PLATFORM_WINDOWS
			#	define	PLATFORM_WINDOWS
    		#	define mkdir _mkdir
		#endif
		#define	PATH_SEPARATOR			'\\'
		#define BUILDER_NAME			BUILDER_BASE_NAME ".exe"
		#define BUILDER_CC				DEFAULT_COMPILER
		#define BUILDER_CFLAGS			""
		#define BUILDER_LDFLAGS			""
		#define BUILDER_DEBUG_CFLAGS	"-g3"
		#define BUILDER_DEBUG_LDFLAGS	""
	#elif defined(__linux__) || defined(__APPLE__) && defined(__MACH__)
		#ifndef		PLATFORM_UNIX
			#define	PLATFORM_UNIX
		#endif
		#define	PATH_SEPARATOR	'/'
		#define BUILDER_NAME	"./" BUILDER_BASE_NAME
		#include <sys/types.h>
		#include <sys/wait.h>
		#include <sys/stat.h>
		#include <unistd.h>
		#include <time.h>
		#include <stdint.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h> 
		#include <unistd.h>
		#include <stdarg.h>
		#include <errno.h>
		#include <stddef.h>
		#include <pthread.h>
		#include <dirent.h>

		#if defined(__linux__)
			#ifndef		PLATFORM_LINUX
				#define	PLATFORM_LINUX
			#endif
			#define		BUILDER_CC				DEFAULT_COMPILER
			#define		BUILDER_CFLAGS			""
			#define		BUILDER_LDFLAGS			""
			#define		BUILDER_DEBUG_CFLAGS	"-g3"
			#define		BUILDER_DEBUG_LDFLAGS	""
		#elif defined(__APPLE__) && defined(__MACH__)
			#ifndef		PLATFORM_MACOS
				#define	PLATFORM_MACOS
			#endif
			#define		BUILDER_CC				DEFAULT_COMPILER
			#define		BUILDER_CFLAGS			""
			#define		BUILDER_LDFLAGS			""
			#define		BUILDER_DEBUG_CFLAGS	"-g3"
			#define		BUILDER_DEBUG_LDFLAGS	""
		#else
			#error "Unsupported platform"
		#endif
	#endif

#define		DEFAULT_STRING_ALLOCATION_SIZE	500
#define		MAX_RULES						100
#define		MAX_COMMAND						300












/*								This is a copy paste of my logger V2			*/

#ifndef LOGGER_H
#define LOGGER_H

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/* Platform-specific includes and definitions */
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define _STRDUP _strdup
#define _LOCALTIME(t, tm) localtime_s(&(tm), &(t))
#else
#include <unistd.h>
#define _STRDUP strdup
#define _LOCALTIME(t, tm) (tm = *localtime(&(t)))
#endif

/* Log levels */
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_FATAL 4

/* Default log level */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

/* Log file path and mode defaults */
#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "default_log.log"
#endif

#ifndef LOG_FILE_MODE
#define LOG_FILE_MODE "a"
#endif

#define MAX_LOG_MESSAGE_LENGTH 2048
#define LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

/* Logging modes */
typedef enum {
	LOG_MODE_FILE,
	LOG_MODE_STDERR,
	LOG_MODE_STDOUT
} LogMode;

/* Global settings */
static int _log_level = LOG_LEVEL;
static FILE* _log_output_stream = NULL;
static char _log_file_path[256] = LOG_FILE_PATH;
static char _log_file_mode[5] = LOG_FILE_MODE;
static LogMode _log_mode = LOG_MODE_STDERR;
static int _use_colors = 1;
static int _show_file_line = 1;
static int _show_timestamp = 1;
static int _show_log_level = 1;  /* Toggle log level visibility */

/* ANSI color macros */


#ifdef _WIN32
    #define ANSI_COLOR(code) ""
#else
    #define ANSI_COLOR(code) code
#endif

// Text colors
#define COLOR_WHITE			ANSI_COLOR("\033[37m")
#define COLOR_GREEN			ANSI_COLOR("\033[32m")
#define COLOR_YELLOW		ANSI_COLOR("\033[33m")
#define COLOR_RED			ANSI_COLOR("\033[31m")
#define COLOR_MAGENTA		ANSI_COLOR("\033[35m")
#define COLOR_CYAN			ANSI_COLOR("\033[36m")
#define COLOR_BLUE			ANSI_COLOR("\033[34m")
#define COLOR_BLACK			ANSI_COLOR("\033[30m")
#define COLOR_DARK_GRAY		ANSI_COLOR("\033[90m")
#define COLOR_LIGHT_RED		ANSI_COLOR("\033[91m")
#define COLOR_LIGHT_GREEN	ANSI_COLOR("\033[92m")
#define COLOR_LIGHT_YELLOW	ANSI_COLOR("\033[93m")
#define COLOR_LIGHT_BLUE	ANSI_COLOR("\033[94m")
#define COLOR_LIGHT_MAGENTA	ANSI_COLOR("\033[95m")
#define COLOR_LIGHT_CYAN	ANSI_COLOR("\033[96m")
#define COLOR_LIGHT_WHITE	ANSI_COLOR("\033[97m")

// Background colors
#define BG_BLACK			ANSI_COLOR("\033[40m")
#define BG_RED				ANSI_COLOR("\033[41m")
#define BG_GREEN			ANSI_COLOR("\033[42m")
#define BG_YELLOW			ANSI_COLOR("\033[43m")
#define BG_BLUE				ANSI_COLOR("\033[44m")
#define BG_MAGENTA			ANSI_COLOR("\033[45m")
#define BG_CYAN				ANSI_COLOR("\033[46m")
#define BG_WHITE			ANSI_COLOR("\033[47m")
#define BG_DARK_GRAY		ANSI_COLOR("\033[100m")
#define BG_LIGHT_RED		ANSI_COLOR("\033[101m")
#define BG_LIGHT_GREEN		ANSI_COLOR("\033[102m")
#define BG_LIGHT_YELLOW		ANSI_COLOR("\033[103m")
#define BG_LIGHT_BLUE		ANSI_COLOR("\033[104m")
#define BG_LIGHT_MAGENTA	ANSI_COLOR("\033[105m")
#define BG_LIGHT_CYAN		ANSI_COLOR("\033[106m")
#define BG_LIGHT_WHITE		ANSI_COLOR("\033[107m")

// Text styles
#define STYLE_BOLD			ANSI_COLOR("\033[1m")
#define STYLE_DIM			ANSI_COLOR("\033[2m")
#define STYLE_UNDERLINE		ANSI_COLOR("\033[4m")
#define STYLE_BLINK			ANSI_COLOR("\033[5m")
#define STYLE_REVERSE		ANSI_COLOR("\033[7m")
#define STYLE_HIDDEN		ANSI_COLOR("\033[8m")

// Reset
#define COLOR_RESET			ANSI_COLOR("\033[0m")

#ifdef _WIN32
static WORD _log_colors[] = {7, 10, 14, 12, 4};
#else
static const char* _log_colors[] = {
	COLOR_WHITE,   /* DEBUG */
	COLOR_GREEN,   /* INFO */
	COLOR_YELLOW,  /* WARN */
	COLOR_RED,     /* ERROR */
	COLOR_MAGENTA  /* FATAL */
};
#endif

/* Function to toggle log level visibility */
static inline void SetShowLogLevel(int enable) { _show_log_level = enable; }
static inline void SetUseColors(int enable) { _use_colors = enable; }
static inline void SetShowFileLine(int enable) { _show_file_line = enable; }
static inline void SetShowTimestamp(int enable) { _show_timestamp = enable; }

/* Change color for a specific log level */
static inline void SetLogColor(int level, const char* color_code)
{
#ifndef _WIN32
	if (level >= LOG_LEVEL_DEBUG && level <= LOG_LEVEL_FATAL)
	{
		_log_colors[level] = color_code;
	}
#endif
}

/* Convert log level to string */
static inline const char* _log_level_to_string(int level)
{
	switch (level)
	{
		case LOG_LEVEL_DEBUG: return "DEBUG";
		case LOG_LEVEL_INFO:  return "INFO";
		case LOG_LEVEL_WARN:  return "WARN";
		case LOG_LEVEL_ERROR: return "ERROR";
		case LOG_LEVEL_FATAL: return "FATAL";
		default:              return "UNKNOWN";
	}
}

/* Get current time string */
static inline void _current_time_string(char* buffer, size_t size)
{
	time_t now = time(NULL);
	struct tm tinfo;
	_LOCALTIME(now, tinfo);
	strftime(buffer, size, LOG_TIME_FORMAT, &tinfo);
}

/* Open the log file */
static inline void _open_log_file()
{
	if (_log_output_stream) fclose(_log_output_stream);
	_log_output_stream = fopen(_log_file_path, _log_file_mode);
}

/* Set log mode */
static inline void _set_log_mode(LogMode mode)
{
	_log_mode = mode;
	if (_log_mode == LOG_MODE_FILE) {
		_open_log_file();
	}
}

/* Set log level */
static inline void _set_log_level(int level) { _log_level = level; }

/* Set log file path */
static inline void _set_log_file_path(const char* filepath)
{
	strncpy(_log_file_path, filepath, sizeof(_log_file_path) - 1);
	_log_file_path[sizeof(_log_file_path) - 1] = '\0';
	if (_log_mode == LOG_MODE_FILE) _open_log_file();
}

/* Clear log file */
static inline void _clear_log_file()
{
	if (_log_mode == LOG_MODE_FILE && _log_output_stream)
	{
		freopen(_log_file_path, "w", _log_output_stream);
	}
}

/* Erase log file */
static inline void _erase_log_file()
{
	if (_log_mode == LOG_MODE_FILE)
	{
		fclose(_log_output_stream);
		remove(_log_file_path);
		_open_log_file();
	}
}

/* Log a message */
static inline void _log_message(int level, const char* file, int line, const char* format, ...) {
	if (level >= _log_level) {
		char log_message[MAX_LOG_MESSAGE_LENGTH];
		char time_buffer[64] = "";

		if (_show_timestamp) {
			_current_time_string(time_buffer, sizeof(time_buffer));
		}

		va_list args;
		va_start(args, format);
		vsnprintf(log_message, sizeof(log_message), format, args);
		va_end(args);

		FILE* output_stream = (_log_mode == LOG_MODE_FILE) ? _log_output_stream :
			(_log_mode == LOG_MODE_STDERR) ? stderr : stdout;

		if (_use_colors && _log_mode != LOG_MODE_FILE) {
#ifndef _WIN32
			fprintf(output_stream, "%s", _log_colors[level]);
#endif
		}

		if (_show_timestamp) fprintf(output_stream, "[%s] ", time_buffer);
		if (_show_file_line) fprintf(output_stream, "%s:%d ", file, line);
		if (_show_log_level) fprintf(output_stream, "[%s] ", _log_level_to_string(level));

		fprintf(output_stream, "%s\n", log_message);
		fflush(output_stream);

		if (_use_colors && _log_mode != LOG_MODE_FILE) {
#ifndef _WIN32
			fprintf(output_stream, "%s", COLOR_RESET);
#endif
		}
	}
}

/* Logging macros */
#define LOG_DEBUG(...)	_log_message(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) 	_log_message(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) 	_log_message(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)	_log_message(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...)	_log_message(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/* Fully restored macros */
#define SET_LOG_LEVEL(level) _set_log_level(level)
#define SET_LOG_FILE_PATH(filepath) _set_log_file_path(filepath)
#define SET_LOG_MODE(mode) _set_log_mode(mode)
#define CLEAR_LOG_FILE() _clear_log_file()
#define ERASE_LOG_FILE() _erase_log_file()
#define SET_LOG_SHOW_LEVEL(enable) SetShowLogLevel(enable)
#define SET_LOG_USE_COLORS(enable) SetUseColors(enable)
#define SET_LOG_SHOW_FILE_LINE(enable) SetShowFileLine(enable)
#define SET_LOG_SHOW_TIMESTAMP(enable) SetShowTimestamp(enable)

#endif /* LOGGER_H */











typedef struct String
{
	char *c_str;              // Pointer to the character array
	unsigned int length;      // Current length of the string
	unsigned int size;        // Allocated size of the string buffer
} String;
typedef String Command;


String *CreateStringEmpty(void)
{
	String *string = (String *)malloc(sizeof(String));
	if (!string) return NULL;

	string->c_str = (char *)malloc(DEFAULT_STRING_ALLOCATION_SIZE * sizeof(char));
	if (!string->c_str)
	{
		free(string);
		return NULL;
	}

	string->length = 0;
	string->size = DEFAULT_STRING_ALLOCATION_SIZE;
	string->c_str[0] = '\0';
	return string;
}

void AppendToString(String *str, const char *element)
{
	if (!str || !element) return;
	unsigned long elementLength = strlen(element);
	unsigned long requiredSize = str->length + elementLength + 1;
	if (requiredSize > str->size)
	{
		unsigned int newSize = str->size * 2;
		while (newSize < requiredSize)
			newSize *= 2;
		char *newBuffer = (char *)realloc(str->c_str, newSize);
		if (!newBuffer) return;
		str->c_str = newBuffer;
		str->size = newSize;
	}
	memcpy(str->c_str + str->length, element, elementLength);
	str->length += elementLength;
	str->c_str[str->length] = '\0';
}

void AppendToStringVA(String *str, va_list args)
{
	if (!str) return;

	const char *arg = va_arg(args, const char *);
	while (arg != NULL)
	{
		if (str->length > 0)
		{
			AppendToString(str, " ");
		}
		AppendToString(str, arg);
		arg = va_arg(args, const char *);
	}
}

void AppendToStringNULL(String *str, ...)
{
	if (!str) return;
	va_list args;
	va_start(args, str);
	AppendToStringVA(str, args);
	va_end(args);
}

// Implementation function that handles variadic arguments
String *CreateStringImpl(int dummy, ...)
{
	String *string = CreateStringEmpty();
	if (!string) return NULL;

	// Process strings, if any
	va_list args;
	va_start(args, dummy);
	AppendToStringVA(string, args);
	va_end(args);

	return string;
}

// WARN: It does not look like the proper way to handle that.
#define CreateString(...) CreateStringImpl(0, ##__VA_ARGS__, NULL)

#define CreateCommand CreateString
#define AppendToCommandNULL AppendToStringNULL
#define AppendToCommand(command, ...) AppendToCommandNULL(command, __VA_ARGS__, NULL)

void AppendStringsToCommand(Command *cmd, char **args, int count)
{
	if (!cmd || !args) return;
	for (int i = 0; i < count; ++i)
	{
		if (args[i])
			AppendToCommand(cmd, args[i]);
	}
}


/* typedef void(*RuleFunction)(void); */
typedef void(*RuleFunctionWithArgs)(int argc, char **argv);
typedef struct Rule
{
	char					*name;
	RuleFunctionWithArgs	function;
} Rule;

Rule gRules[MAX_RULES];
int	gRuleCount;
int	gDryRun	= 0;

void CreateRule(const char *ruleName, RuleFunctionWithArgs function) {
	if (gRuleCount >= MAX_RULES)
		return;
	gRules[gRuleCount].name = strdup(ruleName);
	gRules[gRuleCount].function = function;
	gRuleCount++;
}

int IsRuleName(const char *name)
{
	for (int i = 0; i < gRuleCount; ++i)
		if (strcmp(gRules[i].name, name) == 0)
			return 1;
	return 0;
}

void ParseGlobalFlags(int *argc, char ***argv)
{
	int writeIndex = 0;
	for (int readIndex = 0; readIndex < *argc; readIndex++)
	{
		const char *arg = (*argv)[readIndex];
		if (strcmp(arg, "--dry-run") == 0)
		{
			LOG_INFO(COLOR_BLUE"[DRY-RUN] "COLOR_GREEN"enabled"COLOR_RESET);
			gDryRun = 1;
		}
		else
		{
			(*argv)[writeIndex++] = (*argv)[readIndex];
		}
	}
	*argc = writeIndex;
}

void HelpRule()
{
	LOG_INFO(COLOR_YELLOW"This CBuilder has been compiled with "COLOR_CYAN""DEFAULT_COMPILER""COLOR_YELLOW" and will fallback on it if needed"COLOR_RESET);
	LOG_INFO(COLOR_BLUE"Available rules:"COLOR_RESET);
	for (int i = 0; i < gRuleCount; i++)
		LOG_INFO("- "COLOR_CYAN"%s"COLOR_RESET, gRules[i].name);
}

int ExecuteRule(const char *rule, int argc, char **argv)
{
	for (int i = 0; i < gRuleCount; i++)
	{
		if (strcmp(gRules[i].name, rule) == 0)
		{
			LOG_INFO(COLOR_YELLOW"Executing rule "COLOR_CYAN"%s"COLOR_RESET, gRules[i].name);
			gRules[i].function(argc, argv);
			return 1;
		}
	}
	LOG_WARN(COLOR_RED"Rule not found: "COLOR_CYAN"%s"COLOR_RESET, rule);
	HelpRule();
	return 0;
}


int ExecuteCommandGeneric(const Command *command, int runInThread)
{
	if (!command || !command->c_str) {
		LOG_ERROR("Invalid command: NULL pointer");
		return 0;
	}
	if (!gDryRun)
	{
		LOG_INFO(COLOR_YELLOW"--cmd--"COLOR_RESET" %s", command->c_str);
	
#ifdef PLATFORM_WINDOWS
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char *cmdBuffer = strdup(command->c_str);
	if (!cmdBuffer) {
		fprintf(stderr, "Memory allocation failed for command buffer\n");
		return 0;
	}

	if (!CreateProcess(NULL, cmdBuffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		fprintf(stderr, "Command execution failed: %lu\n", GetLastError());
		free(cmdBuffer);
		return 0;
	}

	if (runInThread) {
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForSingleObject, pi.hProcess, 0, NULL);
		if (thread == NULL) {
			fprintf(stderr, "Failed to create thread: %lu\n", GetLastError());
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			free(cmdBuffer);
			return 0;
		}
		WaitForSingleObject(thread, INFINITE);
		CloseHandle(thread);
	} else {
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	free(cmdBuffer);
#elif defined(PLATFORM_UNIX)
	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork failed");
		return 0;
	}

	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", command->c_str, (char *)NULL);
		perror("execl failed");
		exit(EXIT_FAILURE);
	} else {
		if (runInThread) {
			pthread_t waitThread;
			pthread_create(&waitThread, NULL, (void *(*)(void *))waitpid, (void *)&pid);
			pthread_join(waitThread, NULL);
		} else {
			int status;
			waitpid(pid, &status, 0);
			return WIFEXITED(status) ? (WEXITSTATUS(status) == 0 ? 1 : 0) : 0;
		}
	}
#endif

	return 1;
	}
	else
	{
		LOG_INFO(COLOR_YELLOW"--cmd--"COLOR_BLUE"[DRY-RUN]"COLOR_RESET" %s", command->c_str);
		return 1;
	}
}
#define ExecuteCommand(command) ExecuteCommandGeneric(command, 0)
#define ExecuteCommandWithThreading(command) ExecuteCommandGeneric(command, 1)


//	FIXME: Treads only works on posix for now
#if defined(PLATFORM_UNIX)
void ExecuteCommands(int runInThread, ...)
{
	va_list args;
	va_start(args, runInThread);

	pthread_t *threads = NULL;
	int threadCount = 0;
	if (runInThread)
		threads = (pthread_t *)malloc(sizeof(pthread_t) * MAX_COMMAND); // Arbitrary maximum command count

	Command *cmd;
	while ((cmd = va_arg(args, Command *)) != NULL)
	{
		if (runInThread)
		{
			pthread_create(&threads[threadCount], NULL, (void *(*)(void *))ExecuteCommandGeneric, (void *)cmd);
			threadCount++;
		}
		else
		ExecuteCommand(cmd);
	}

	if (runInThread) {
		for (int i = 0; i < threadCount; i++) {
			pthread_join(threads[i], NULL);
		}
		free(threads);
	}

	va_end(args);
}
#define ExecuteCommands(...) ExecuteCommands(__VA_ARGS__, NULL)
#endif


// FIXME: ===================	Problematic	====================
char *GetCommandOutput(const char *cmd)
{
#ifdef PLATFORM_WINDOWS
	FILE *pipe = _popen(cmd, "r");
#else
	FILE *pipe = popen(cmd, "r");
#endif

	if (!pipe) return NULL;

	char buffer[512];
	char *result = NULL;

	if (fgets(buffer, sizeof(buffer), pipe))
	{
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';  // Remove newline

		result = _STRDUP(buffer);  // strdup or _strdup
	}

#ifdef PLATFORM_WINDOWS
	_pclose(pipe);
#else
	pclose(pipe);
#endif

	return result; // Must be freed by caller
}




char *GetCompiler(const char *compilerName)
{
	if (!compilerName || !*compilerName) return NULL;

	char command[256];

#ifdef PLATFORM_WINDOWS
	snprintf(command, sizeof(command), "where %s 2>nul", compilerName); // redirect stderr
#else
	snprintf(command, sizeof(command), "which %s 2>/dev/null", compilerName);
#endif

#ifdef PLATFORM_WINDOWS
	FILE *pipe = _popen(command, "r");
#else
	FILE *pipe = popen(command, "r");
#endif

	if (!pipe) return NULL;

	char buffer[512] = {0};
	char *path = NULL;

	if (fgets(buffer, sizeof(buffer), pipe))
	{
		size_t len = strlen(buffer);
		while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
			buffer[--len] = '\0'; // strip newline(s)
		}

		path = _STRDUP(buffer); // _strdup on Windows, strdup on Unix
	}

#ifdef PLATFORM_WINDOWS
	_pclose(pipe);
#else
	pclose(pipe);
#endif

	return path; // caller must free
}
// FIXME: ========================================================================================================


time_t GetFileLastEditTime(const char *filePath)
{
	struct stat fileStat;
	if (stat(filePath, &fileStat) != 0)
	{
		perror("Failed to get file stats");
		return 0;
	}
	return fileStat.st_mtime;
}


void RebuildSelf()
{
	Command *selfBuildCmd = CreateCommand();
	AppendToCommand(selfBuildCmd, BUILDER_CC, BUILDER_CFLAGS, BUILDER_SOURCE, "-o", BUILDER_NAME);
	LOG_INFO(COLOR_YELLOW"Rebuilding CBuilder..."COLOR_RESET);
	if (!ExecuteCommand(selfBuildCmd))
	{
		LOG_FATAL("Failed to rebuild CBuilder");
		exit(EXIT_FAILURE);
	}
	LOG_INFO(COLOR_GREEN"CBuilder rebuilt successfully."COLOR_RESET);
	free(selfBuildCmd->c_str);
	free(selfBuildCmd);
}

void RunSelf(String *args)
{
	Command *cmd = CreateCommand();
#ifdef PLATFORM_WINDOWS
	AppendToCommand(cmd, BUILDER_NAME);
#else
	AppendToCommand(cmd, BUILDER_NAME);
#endif
	AppendToCommand(cmd, args->c_str);
	if (!ExecuteCommand(cmd))
		LOG_FATAL("Failed to execute command %s", cmd->c_str);
}

void ManageRebuild(int argc, char **argv)
{
	time_t sourceEditTime = GetFileLastEditTime(BUILDER_SOURCE);
	time_t execEditTime = GetFileLastEditTime(BUILDER_NAME);

	if (sourceEditTime > execEditTime)
	{
		LOG_INFO(COLOR_YELLOW"Source file is newer than executable. Rebuilding..."COLOR_RESET);
		String *args = CreateString();
		for (int i = 1; i < argc; i++)
			AppendToCommand(args, argv[i]);
		RebuildSelf();
		RunSelf(args);
		exit(0);
	}
}


void ManageRules(int argc, char **argv)
{
	int retValue;
	ManageRebuild(argc, argv);
	if (argc == 1)
	{
		LOG_INFO(COLOR_BLUE"No rule provided -- using the first rule declared"COLOR_RESET);
		LOG_INFO(COLOR_YELLOW"Executing rule "COLOR_CYAN"%s"COLOR_RESET, gRules[0].name);
		gRules[0].function(argc, argv);
	}
	for (int i = 1; i < argc; i++)
	{
		char *ruleName = argv[i];
		int nextRuleIndex = i + 1;
		while (nextRuleIndex < argc && !IsRuleName(argv[nextRuleIndex]))
			nextRuleIndex++;
		int argCount = nextRuleIndex - i - 1;
		retValue = ExecuteRule(ruleName, argCount,  &argv[i + 1]);
		i = nextRuleIndex - 1;
	}

}

/* ----------------------------------------- SCRIPT UTILS ----------------------------------------- */

int HasArg(int argc, char **argv, const char *target)
{
	for (int i = 0; i < argc; ++i)
		if (strcmp(argv[i], target) == 0)
			return 1;
	return 0;
}



int MatchPattern(const char *pattern, const char *text)
{
	while (*pattern && *text)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern) return 1;
			while (*text)
			{
				if (MatchPattern(pattern, text)) return 1;
				text++;
			}
			return 0;
		}
		else if (*pattern == *text)
		{
			pattern++;
			text++;
		}
		else
		{
			return 0;
		}
	}
	while (*pattern == '*') pattern++;
	return *pattern == '\0' && *text == '\0';
}




Command *CommandFromArgs(int argc, char **argv)
{
	Command *cmd = CreateCommand(); for (int i = 0; i < argc; i++) AppendToCommand(cmd, argv[i]);
	return cmd;
}


int CB_FileExists(const char *path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

int CB_CreateFile(const char *path, const char *content)
{
	FILE *file	= fopen(path, "w");
	if (!file) return 0;

	if (content && *content)
		fprintf(file, "%s", content);

	fclose(file);
	return 1;
}

int CB_CreateFileWithMode(const char *path, const char *content, const char *mode)
{
	FILE *file	= fopen(path, mode ? mode : "w");
	if (!file) return 0;

	if (content && *content)
		fprintf(file, "%s", content);

	fclose(file);
	return 1;
}

int CB_CopyFile(const char *src, const char *dest)
{
	FILE *srcFile	= fopen(src, "rb");
	if (!srcFile) return 0;

	FILE *destFile	= fopen(dest, "wb");
	if (!destFile)
	{
		fclose(srcFile);
		return 0;
	}

	char buffer[4096];
	size_t bytes;
	while ((bytes = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
		fwrite(buffer, 1, bytes, destFile);

	fclose(srcFile);
	fclose(destFile);
	return 1;
}

int CB_RemoveFile(const char *path)
{
	return remove(path) == 0;
}


int CB_CreateDirectory(const char *path)
{
#ifdef PLATFORM_WINDOWS
    return _mkdir(path) == 0 || errno == EEXIST;
#else
    return mkdir(path, 0777) == 0 || errno == EEXIST;
#endif
}

int CB_IsDirectoryEmpty(const char *path)
{
	DIR *dir = opendir(path);
	if (!dir) return 0;

	struct dirent *entry;
	while ((entry = readdir(dir)))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			closedir(dir);
			return 0;
		}
	}
	closedir(dir);
	return 1;
}

int CB_RemoveDirectory(const char *path)
{
	DIR *dir = opendir(path);
	if (!dir) return 0;

	struct dirent *entry;
	char fullPath[4096];

	while ((entry = readdir(dir)))
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(fullPath, sizeof(fullPath), "%s%c%s", path, PATH_SEPARATOR, entry->d_name);

		struct stat st;
		if (stat(fullPath, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				if (!CB_RemoveDirectory(fullPath))
				{
					closedir(dir);
					return 0;
				}
			}
			else
			{
				if (remove(fullPath) != 0)
				{
					closedir(dir);
					return 0;
				}
			}
		}
	}

	closedir(dir);

#ifdef _WIN32
	return _rmdir(path) == 0;
#else
	return rmdir(path) == 0;
#endif
}


int CB_DirectoryExists(const char *path) {
	struct stat buffer;
	if (stat(path, &buffer) != 0)
		return 0;

	#ifdef _WIN32
		return (buffer.st_mode & _S_IFDIR) != 0;
	#else
		return S_ISDIR(buffer.st_mode);
	#endif
}






void RecursiveCollectFiles(const char *pattern, String **results, int *count, int max) 
{
    if (!pattern || !results || !count) return;

    const char *lastSlash = strrchr(pattern, '/');
    char baseDir[1024] = ".";
    const char *matchPattern = pattern;

    if (lastSlash) {
        size_t len = lastSlash - pattern;
        if (len >= sizeof(baseDir)) len = sizeof(baseDir) - 1;
        snprintf(baseDir, sizeof(baseDir), "%.*s", (int)len, pattern);
        matchPattern = lastSlash + 1;
    }

    // Check if the matchPattern itself contains a '/' indicating deeper levels
    int isRecursive = strchr(matchPattern, '/') != NULL;

    DIR *dir = opendir(baseDir);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", baseDir, entry->d_name);

        struct stat st;
        if (stat(fullPath, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            if (isRecursive) {
                char subPattern[1024];
                snprintf(subPattern, sizeof(subPattern), "%s/%s", baseDir, entry->d_name);
                char nextPattern[1024];
                snprintf(nextPattern, sizeof(nextPattern), "%s/%s", subPattern, matchPattern);
                RecursiveCollectFiles(nextPattern, results, count, max);
            }
        } else {
            if (MatchPattern(matchPattern, entry->d_name)) {
                if (*count < max) {
                    results[*count] = CreateString(fullPath);
                    (*count)++;
                }
            }
        }
    }

    closedir(dir);
}











void CompileFilesNULL(const char *pattern, const char *dest, const char *compiler, ...)
{
	String *files[1000];
	int fileCount = 0;
	memset(files, 0, sizeof(files));
	RecursiveCollectFiles(pattern, files, &fileCount, 1000);
	if (!CB_DirectoryExists(dest))
		CB_CreateDirectory(dest);

	va_list originalArgs;
	va_start(originalArgs, compiler);

	for (int i = 0; i < fileCount; ++i)
	{
		String *source = files[i];
		if (!source || !source->c_str)
			continue;

		const char *filename = strrchr(source->c_str, '/');
		filename = filename ? filename + 1 : source->c_str;

		char objname[512];
		snprintf(objname, sizeof(objname), "%s/%s.o", dest, filename);

		Command *cmd = CreateCommand(compiler);
		va_list args;
		va_copy(args, originalArgs);
		const char *arg = NULL;
		while ((arg = va_arg(args, const char *)) != NULL)
		{
			AppendToCommand(cmd, arg);
		}
		va_end(args);

		AppendToCommand(cmd, "-c", source->c_str, "-o", objname);
		if (!ExecuteCommand(cmd))
		{
			LOG_ERROR("Failed to compile: %s", source->c_str);
		}

		free(cmd->c_str);
		free(cmd);
	}

	va_end(originalArgs);

	for (int i = 0; i < fileCount; ++i)
	{
		if (files[i])
		{
			free(files[i]->c_str);
			free(files[i]);
		}
	}
}
#define CompileFiles(pattern, dest, compiler, ...) CompileFilesNULL(pattern, dest, compiler, __VA_ARGS__, NULL)



void LinkFilesNULL(const char *pattern, const char *outputBinary, const char *linker, ...)
{
	if (!pattern || !outputBinary || !linker) {
		LOG_FATAL("LinkFiles received null argument(s)");
		exit(1);
	}

	String *files[1000];
	int fileCount = 0;
	memset(files, 0, sizeof(files));

	RecursiveCollectFiles(pattern, files, &fileCount, 1000);

	if (fileCount == 0) {
		LOG_WARN("No object files matched pattern %s", pattern);
		return;
	}

	Command *cmd = CreateCommand(linker);
	if (!cmd) {
		LOG_FATAL("Failed to create link command");
		exit(1);
	}

	va_list args;
	va_start(args, linker);
	const char *arg;
	while ((arg = va_arg(args, const char *)) != NULL) {
		AppendToCommand(cmd, arg);
	}
	va_end(args);

	for (int i = 0; i < fileCount; ++i) {
		AppendToCommand(cmd, files[i]->c_str);
	}

	AppendToCommand(cmd, "-o", outputBinary);

	if (!ExecuteCommand(cmd)) {
		LOG_ERROR("Linking failed");
	}

	// Cleanup
	for (int i = 0; i < fileCount; ++i) {
		if (files[i]) {
			free(files[i]->c_str);
			free(files[i]);
		}
	}
	free(cmd->c_str);
	free(cmd);
}
#define LinkFiles(pattern, outputBinary, linker, ...) LinkFilesNULL(pattern, outputBinary, linker, __VA_ARGS__, NULL)


#define NO_PREFIX
#if defined(NO_PREFIX) && defined(PLATFORM_UNIX)
	#define	CreateFile			CB_CreateFile
	#define	CreateFileWithMode	CB_CreateFileWithMode
	#define	CopyFile			CB_CopyFile
	#define	FileExists			CB_FileExists
	#define	RemoveFile			CB_RemoveFile
	#define	DirectoryExists		CB_DirectoryExists
	#define CreateDirectory		CB_CreateDirectory
	#define	RemoveDirectory		CB_RemoveDirectory
#endif











/*
 * CBuilder v0.0.2
 * 
 * Description:
 * ------------
 * CBuilder is a lightweight, single-file build system written in C, designed for flexibility
 * and direct scriptability. It does not check for memory allocation failures, does not free memory,
 * and remains entirely contained within a single source file.
 *
 * How to Use:
 * -----------
 * - Modify the `main` function at the bottom of the file to add your custom rules.
 * - A rule is a function `void MyRule(void)` registered with `CreateRule("name", MyRule);`.
 * - Use `ExecuteCommand(CreateCommand("your_command_here"));` to execute shell commands.
 * - Use `ExecuteCommands(cmd1, cmd2);` to execute multiple commands.
 * - Run `./CBuilder help` to see the available rules.
 *
 * ----------------------------------------- USER SPACE ----------------------------------------- */

#if defined(PLATFORM_MACOS)
	#define EXECNAME			"./Codemon"
	#define CC			DEFAULT_COMPILER
	#define LD			"gcc"
	#define SRC_DIR			"./src/"
	#define RAYLIB_DIR		SRC_DIR"extern/raylib-5.5/"
	#define RAYLIB_SRC		RAYLIB_DIR"src/"
	#define RAYLIB_BIN		RAYLIB_SRC"libraylib.a"
	#define BUILD_RAYLIB_CMD	"cd " RAYLIB_SRC" && make PLATFORM=PLATFORM_DESKTOP"

	#define	MACOS_FRAMEWORKS	"-framework CoreVideo		\
								-framework CoreAudio		\
								-framework Cocoa,			\
								-framework IOKit,			\
								-framework GLUT,			\
								-framework OpenGL,			\
								-framework GameController,	\
								-framework AudioToolbox,	\
								-framework Metal,			\
								-framework CoreHaptics,		\
								-framework ForceFeedback,	\
								-framework Carbon,			\
								-framework AVFoundation"


	#define CFLAGS		"-Wall -Wextra -I"RAYLIB_SRC
	#define LDFLAGS		""MACOS_FRAMEWORKS
	#define DEBUG_CFLAGS	"-fsanitize=address -g3"
	#define DEBUG_LDGLAGS	""

	#define MODULES_PATH		SRC_DIR"modules/"
	
	#define GRAPHICS_DIR		MODULES_PATH"graphics/"
	#define GRAPHICS_BIN		GRAPHICS_DIR"libgraphics.dylib"
	#define BUILD_GRAPHICS_CMD	"cd "GRAPHICS_DIR" && "CC" -I./src/extern/raylib-5.5/src/ -undefined dynamic_lookup *.c -dynamiclib -o libgraphics.dylib "LDFLAGS

#elif defined(PLATFORM_LINUX)
	#define EXECNAME	"lambdac"
	#define CC			DEFAULT_COMPILER
	#define LD			"gcc"
	#define CFLAGS		"-Wall -Wextra"
	#define DEBUG_CFLAGS	"-fsanitize=address"
	#define DEBUG_LDGLAGS	""
#define LDFLAGS		""
#elif defined(PLATFORM_WINDOWS)
	#define EXECNAME	"lambdac.exe"
	#define CC			DEFAULT_COMPILER
	#define LD			"gcc"
	#define CFLAGS		""
	#define LDFLAGS		""
	#define DEBUG_CFLAGS	"-fsanitize=address"
	#define DEBUG_LDGLAGS	""

#endif

void BuildDebugSelf(int argc, char **argv)
{
	Command *cmd = CreateCommand(BUILDER_CC);
	if (HasArg(argc, argv, "asan"))
		AppendToCommand(cmd, "-fsanitize=address");
	if (HasArg(argc, argv, "gdb"))
		AppendToCommand(cmd, "-g3");
	AppendToCommand(cmd, BUILDER_SOURCE, "-o", BUILDER_NAME);
	ExecuteCommand(cmd);
}

void CleanRule(int argc, char **argv)
{
	if (CB_FileExists(EXECNAME))
		CB_RemoveFile(EXECNAME);
	if (CB_FileExists(BUILDER_NAME) && HasArg(argc, argv, "all"))
		CB_RemoveFile(BUILDER_NAME);
	if (CB_DirectoryExists("build"))
	{
		CB_RemoveDirectory("build");
	}
}


void BuildModules(int ac, char **av)
{
	ExecuteCommand(CreateCommand(BUILD_GRAPHICS_CMD));
}


void BuildRaylib(int argc, char **argv)
{
	if (!FileExists(RAYLIB_BIN))
		ExecuteCommand(CreateCommand(BUILD_RAYLIB_CMD));
}

void BuildRule(int argc, char **argv)
{
	BuildModules(argc, argv);
	BuildRaylib(argc, argv);
	char cflags[512];
	char ldflags[512];
	strcpy(cflags, CFLAGS);
	strcpy(ldflags, LDFLAGS);
	if (HasArg(argc, argv, "debug"))
	{
		strcat(cflags, " -fsanitize=address");
		strcat(cflags, " -g3");
	}
	CompileFiles("src/*.c", "build", CC, cflags);
	CompileFiles("src/player/*.c", "build", CC, cflags);
	CompileFiles("src/chore/*.c", "build", CC, cflags);
	CompileFiles("src/maps/*.c", "build", CC, cflags);
	LinkFiles("build/*.o", EXECNAME, LD, RAYLIB_BIN, cflags, ldflags);
	if (HasArg(argc, argv, "no-obj"))
		CB_RemoveDirectory("build");
}

void ExecRule(int argc, char **argv)
{
	Command *execCommand = CreateCommand();
	AppendToCommand(execCommand, EXECNAME);
	AppendStringsToCommand(execCommand, argv, argc);
	ExecuteCommand(execCommand);
}


int main(int argc, char **argv)
{
	SET_LOG_SHOW_FILE_LINE(0);	// NOTE: Set to 1 to display the line number when logging.
	SET_LOG_SHOW_TIMESTAMP(0);	// NOTE: Set to 1 to display the timestamp when logging.
	SET_LOG_SHOW_LEVEL(0);		// NOTE: Set to 1 to display the log level when logging.
	LOG_INFO(COLOR_YELLOW"======== CBuilder v0.0.2 ========"COLOR_RESET);

	ParseGlobalFlags(&argc, &argv);

	CreateRule("build",				BuildRule);
	CreateRule("build-raylib",		BuildRaylib);
	CreateRule("build-modules",		BuildModules);
	CreateRule("exec",				ExecRule);
	CreateRule("clean",				CleanRule);

	CreateRule("help",				HelpRule);
	CreateRule("self-rebuild",		RebuildSelf);
	CreateRule("self-build-debug",	BuildDebugSelf);

	ManageRules(argc, argv);
	LOG_INFO(COLOR_YELLOW"================================"COLOR_RESET);
	return 0;
}

#endif
