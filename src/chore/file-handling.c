#include "chore.h"


char* HRAL_GetAbsolutePath(const char* path) {
    char* absPath = realpath(path, NULL);
    if (!absPath)
	{
        perror("Error getting absolute path");
    }
    return absPath;
}

time_t HRAL_GetLastModified(const char* path)
{
	struct stat fileInfo;
	if (stat(path, &fileInfo) != 0)
	{
		perror("Error getting file information");
		return (time_t)-1;
	}
	return fileInfo.st_mtime;
}

int HRAL_HasFileChanged(HRAL_Lib* lib)
{
	time_t currentModTime = HRAL_GetLastModified(lib->path);
	if (currentModTime == (time_t)-1)
	{
		return 0; // Error occurred, assume no change to avoid issues
	}

	if (lib->lastModified != currentModTime)
	{
		lib->lastModified = currentModTime;
		return 1; // File has changed
	}

	return 0; // No change
}


int HRAL_IsSourceNewer(const char* folder, const char* libPath)
{
	DIR* dir = opendir(folder);
	if (!dir)
	{
		perror("Failed to open folder");
		return 0;
	}

	time_t libModTime = HRAL_GetLastModified(libPath);
	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			char filePath[512];
			snprintf(filePath, sizeof(filePath), "%s/%s", folder, entry->d_name);
			time_t fileModTime = HRAL_GetLastModified(filePath);
			if (fileModTime > libModTime)
			{
				closedir(dir);
				return 1;
			}
		}
	}
	closedir(dir);
	return 0;
}


char* HRAL_GetParentDirectory(const char* path) {
    char* parentDir = strdup(path);
    if (!parentDir) return NULL;

    char* lastSlash = strrchr(parentDir, HRAL_PATH_SEPARATOR);
    if (lastSlash) *lastSlash = '\0'; // Remove the file name part

    return parentDir;
}
