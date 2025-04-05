#include "raylib.h"
#include "maps.h"
#include <stdio.h>
#include <stdlib.h>

size_t map_size(FILE *file)
{
	char c;
	size_t i = 0;

	if (!file)
		return (0);
	while((c = fgetc(file)) != EOF)
		i++;
	return (i);
}

void read_map(FILE *file, char *map)
{
	char c;
	size_t i = 0;

	if (!file)
		return ;
	while ((c = fgetc(file)) != EOF)
		map[i++] = c;
	map[i] = '\n';
}

char *get_map(char *path)
{
	FILE *file = fopen(path, "r");
	size_t len = 0;
	char *str = NULL;

	if (!file)
		return (NULL);
	len = map_size(file);
	if (!(str = (char *)malloc(len + 1)))
		return (NULL);	
	read_map(file, str);
	return str;
}

t_map *init_map(char *path)
{
	t_map *map;
		
}
