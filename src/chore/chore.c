#include "chore.h"


HRAL_Lib* HRAL_LoadLib(const char* path)
{
	HRAL_Lib* lib = (HRAL_Lib*)malloc(sizeof(HRAL_Lib));
	if (!lib)
	{
		perror("Failed to allocate memory for HRAL_Lib");
		return NULL;
	}

	lib->path = strdup(path);
	if (!lib->path)
	{
		perror("Failed to allocate memory for path");
		free(lib);
		return NULL;
	}

	// Load the shared library
	lib->bin = dlopen(path, RTLD_LAZY);
	if (!lib->bin)
	{
		fprintf(stderr, "Error loading library %s: %s\n", path, dlerror());
		free(lib->path);
		free(lib);
		return NULL;
	}

	// Get the last modification time
	lib->lastModified = HRAL_GetLastModified(path);
	if (lib->lastModified == (time_t)-1)
	{
		fprintf(stderr, "Failed to get modification time for %s\n", path);
		dlclose(lib->bin);
		free(lib->path);
		free(lib);
		return NULL;
	}

	lib->isLoaded = 1;
	lib->functions = NULL;
	lib->functionCount = 0;

	printf("Library %s loaded successfully.\n", path);
	return lib;
}





#if defined(HRAL_PLATFORM_MACOS)
int HRAL_LoadAllFunctions(HRAL_Lib* lib)
{
	if (!lib || !lib->bin)
	{
		fprintf(stderr, "Invalid library handle.\n");
		return -1;
	}

	// Iterate through all loaded images to find the correct one
	uint32_t image_count = _dyld_image_count();
	int found = 0;

	for (uint32_t i = 0; i < image_count; i++)
	{
		const char* image_name = _dyld_get_image_name(i);
		if (strstr(image_name, lib->path) != NULL)
		{
			found = 1;
			printf("Inspecting library: %s\n", image_name);

			// Get the Mach-O header
			const struct mach_header_64* header = (const struct mach_header_64*)_dyld_get_image_header(i);
			const struct load_command* lc = (struct load_command*)((char*)header + sizeof(struct mach_header_64));

			for (uint32_t j = 0; j < header->ncmds; j++)
			{
				if (lc->cmd == LC_SYMTAB)
				{
					struct symtab_command* symtab = (struct symtab_command*)lc;
					char* string_table = (char*)header + symtab->stroff;
					struct nlist_64* symbol_table = (struct nlist_64*)((char*)header + symtab->symoff);

					printf("Found symbol table with %d symbols.\n", symtab->nsyms);

					for (uint32_t k = 0; k < symtab->nsyms; k++)
					{
						char* symbol_name = string_table + symbol_table[k].n_un.n_strx;

						// Skip symbols that do not start with '_' (usually not functions)
						if (symbol_name[0] != '_') continue;

						// Remove the leading underscore
						char* clean_name = symbol_name + 1;

						// Get the function pointer using dlsym
						void* function_ptr = dlsym(lib->bin, clean_name);
						if (function_ptr)
						{
							// Allocate or reallocate space for functions
							lib->functions = realloc(lib->functions, sizeof(HRAL_Function) * (lib->functionCount + 1));
							if (!lib->functions)
							{
								perror("Failed to allocate function list");
								return -1;
							}

							// Store the function name and pointer
							lib->functions[lib->functionCount].name = strdup(clean_name);
							lib->functions[lib->functionCount].functionPtr = function_ptr;
							lib->functionCount++;

							printf("Loaded function: %s at %p\n", clean_name, function_ptr);
						}
						else
						{
							fprintf(stderr, "Warning: Could not load symbol %s: %s\n", clean_name, dlerror());
						}
					}
					break;
				}
				lc = (struct load_command*)((char*)lc + lc->cmdsize);
			}
		}
	}

	if (!found)
	{
		printf("Library not found in loaded images.\n");
		return -1;
	}

	printf("Total loaded functions: %d\n", lib->functionCount);
	return lib->functionCount;
}
#endif

void* HRAL_GetFunction(HRAL_Lib* lib, const char* func_name)
{
	if (!lib || !lib->functions)
	{
		fprintf(stderr, "Library not loaded or no functions available.\n");
		return NULL;
	}

	for (int i = 0; i < lib->functionCount; i++)
	{
		if (strcmp(lib->functions[i].name, func_name) == 0)
		{
			return lib->functions[i].functionPtr;
		}
	}

	fprintf(stderr, "Function %s not found in library.\n", func_name);
	return NULL;
}

void* HRAL_GetFunctionFromContext(HRAL_Context* ctx, const char* func_name)
{
	for (int i = 0; i < ctx->libCount; i++)
	{
		HRAL_Lib* lib = &ctx->libs[i];
		void* funcPtr = HRAL_GetFunction(lib, func_name);
		if (funcPtr)
		{
			printf("Function %s found in library %s.\n", func_name, lib->path);
			return funcPtr;
		}
	}
	fprintf(stderr, "Function %s not found in any loaded library.\n", func_name);
	return NULL;
}

int HRAL_PrintAllFunctions(HRAL_Lib* lib)
{
	if (!lib || !lib->bin)
	{
		fprintf(stderr, "Invalid library handle.\n");
		return -1;
	}

	// Iterate through all loaded images to find the correct one
	uint32_t image_count = _dyld_image_count();
	int found = 0;

	for (uint32_t i = 0; i < image_count; i++)
	{
		const char* image_name = _dyld_get_image_name(i);
		if (strstr(image_name, lib->path) != NULL)
		{
			found = 1;
			printf("Inspecting library: %s\n", image_name);

			// Get the Mach-O header
			const struct mach_header_64* header = (const struct mach_header_64*)_dyld_get_image_header(i);
			const struct load_command* lc = (struct load_command*)((char*)header + sizeof(struct mach_header_64));

			for (uint32_t j = 0; j < header->ncmds; j++)
			{
				if (lc->cmd == LC_SYMTAB)
				{
					struct symtab_command* symtab = (struct symtab_command*)lc;
					char* string_table = (char*)header + symtab->stroff;
					struct nlist_64* symbol_table = (struct nlist_64*)((char*)header + symtab->symoff);

					printf("Found symbol table with %d symbols.\n", symtab->nsyms);

					for (uint32_t k = 0; k < symtab->nsyms; k++)
					{
						char* symbol_name = string_table + symbol_table[k].n_un.n_strx;
						// Print the raw symbol name as found
						printf("Symbol: %s\n", symbol_name);
					}
					break;
				}
				lc = (struct load_command*)((char*)lc + lc->cmdsize);
			}
		}
	}

	if (!found)
	{
		printf("Library not found in loaded images.\n");
	}
	int loadedFunctionCount = HRAL_LoadAllFunctions(lib);
	if (loadedFunctionCount <= 0)
	{
		fprintf(stderr, "No functions loaded after reload from %s.\n", lib->path);
		return -1;
	}
	return 0;
}


int HRAL_ReloadLib(HRAL_Lib* lib)
{
	if (!lib || !lib->isLoaded)
	{
		fprintf(stderr, "Library is not loaded or is already unloaded.\n");
		return -1;
	}
	if (!HRAL_HasFileChanged(lib))
	{
		printf("Library %s has not changed, no reload necessary.\n", lib->path);
		return 0;
	}

	// Unload the current library
	if (dlclose(lib->bin) != 0)
	{
		fprintf(stderr, "Error unloading library %s: %s\n", lib->path, dlerror());
		return -1;
	}

	// Free the function pointers and names
	for (int i = 0; i < lib->functionCount; i++)
	{
		free(lib->functions[i].name);
	}
	free(lib->functions);
	lib->functions = NULL;
	lib->functionCount = 0;

	// Resolve the absolute path again
	char* absLibPath = HRAL_GetAbsolutePath(lib->path);
	if (!absLibPath)
	{
		fprintf(stderr, "Failed to resolve absolute path for %s\n", lib->path);
		return -1;
	}

	// Reload the library using the absolute path
	lib->bin = dlopen(absLibPath, RTLD_LAZY);
	free(absLibPath);

	if (!lib->bin)
	{
		fprintf(stderr, "Error reloading library %s: %s\n", lib->path, dlerror());
		return -1;
	}

	// Update the last modification time
	lib->lastModified = HRAL_GetLastModified(lib->path);
	if (lib->lastModified == (time_t)-1)
	{
		fprintf(stderr, "Failed to update modification time for %s\n", lib->path);
		dlclose(lib->bin);
		return -1;
	}

	// Reload the functions
	int loadedFunctionCount = HRAL_LoadAllFunctions(lib);
	if (loadedFunctionCount <= 0)
	{
		fprintf(stderr, "No functions loaded after reload from %s.\n", lib->path);
		return -1;
	}

	printf("Library %s reloaded successfully with %d functions.\n", lib->path, loadedFunctionCount);
	return loadedFunctionCount;
}






HRAL_Lib* HRAL_DeclareLibrary(HRAL_Context* ctx, const char* libPath, const char* buildCommand)
{
	// WARN: Reallocating each time a new library is added -- use DA?
	ctx->libs = realloc(ctx->libs, sizeof(HRAL_Lib) * (ctx->libCount + 1));
	if (!ctx->libs)
	{
		perror("Failed to allocate library context");
		return NULL;
	}

	char* absLibPath = HRAL_GetAbsolutePath(libPath);
	if (!absLibPath)
	{
		fprintf(stderr, "Failed to resolve absolute path for %s\n", libPath);
		return NULL;
	}

	HRAL_Lib* lib = HRAL_LoadLib(absLibPath);
	if (!lib)
	{
		fprintf(stderr, "Failed to load library at path: %s\n", absLibPath);
		free(absLibPath);
		return NULL;
	}

	lib->buildCommand = strdup(buildCommand);
	if (!lib->buildCommand)
	{
		perror("Failed to allocate build command");
		free(lib->path);
		free(absLibPath);
		free(lib);
		return NULL;
	}
	HRAL_LoadAllFunctions(lib);

	// Add the library to the context and increment the count
	ctx->libs[ctx->libCount++] = *lib;

	printf("Declared library: %s (Build Command: %s)\n", absLibPath, buildCommand);
	free(absLibPath);
	return lib;
}




void HRAL_CheckForReload(HRAL_Context* ctx)
{
	for (int i = 0; i < ctx->libCount; i++)
	{
		HRAL_Lib* lib = &ctx->libs[i];

		// Get the parent folder of the library path
		char* folder = HRAL_GetParentDirectory(lib->path);
		if (!folder)
		{
			fprintf(stderr, "Error: Could not determine parent folder for library: %s\n", lib->path);
			continue;
		}

		// Check if any source file in the folder is newer than the library file
		if (HRAL_IsSourceNewer(folder, lib->path))
		{
			printf("Source file in folder %s is newer. Recompiling library: %s\n", folder, lib->path);
			// Run the build command
			int result = system(lib->buildCommand);
			if (result == 0)
			{
				// Attempt to reload the library after successful compilation
				if (HRAL_ReloadLib(lib) > 0)
					printf("Library %s reloaded successfully after recompilation.\n", lib->path);
				else
					fprintf(stderr, "Error: Failed to reload library %s after recompilation.\n", lib->path);
			}
			else
				fprintf(stderr, "Error: Compilation failed for library %s using command: %s\n", lib->path, lib->buildCommand);
		}
		// Free the folder string
		free(folder);
	}
}



int HRAL_CallFunction(HRAL_Context* ctx, const char* func_name, void* ret_value, const char* ret_type, ...)
{
    void* funcPtr = HRAL_GetFunctionFromContext(ctx, func_name);
    if (!funcPtr)
    {
        fprintf(stderr, "Function %s not found in any loaded library.\n", func_name);
        return -1;
    }

    va_list args;
    va_start(args, ret_type);

    if (strcmp(ret_type, "int") == 0)
    {
        int (*typed_func)(int, ...) = (int (*)(int, ...))funcPtr;
        int res = typed_func(va_arg(args, int), va_arg(args, int));
        *((int*)ret_value) = res;
    }
    else if (strcmp(ret_type, "float") == 0)
    {
        float (*typed_func)(double, ...) = (float (*)(double, ...))funcPtr;
        float res = typed_func(va_arg(args, double), va_arg(args, double));
        *((float*)ret_value) = res;
    }
    else if (strcmp(ret_type, "double") == 0)
    {
        double (*typed_func)(double, ...) = (double (*)(double, ...))funcPtr;
        double res = typed_func(va_arg(args, double), va_arg(args, double));
        *((double*)ret_value) = res;
    }
    else if (strcmp(ret_type, "char*") == 0)
    {
        char* (*typed_func)(char*, ...) = (char* (*)(char*, ...))funcPtr;
        char* res = typed_func(va_arg(args, char*));
        *((char**)ret_value) = res;
    }
    else
    {
        void* (*typed_func)(void*, ...) = (void* (*)(void*, ...))funcPtr;
        void* res = typed_func(va_arg(args, void*));
        *((void**)ret_value) = res;
    }

    va_end(args);
    return 0;
}


