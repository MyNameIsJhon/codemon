#include "raylib.h"
#include "maps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t map_size(FILE *file)
{
	char c;
	size_t i = 0;

	if (!file)
		return (0);
	while((c = fgetc(file)) != EOF)
		i++;
	fseek(file, 0, SEEK_SET);
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
	fseek(file, 0, SEEK_SET);
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

size_t map_striplen(char **splited)
{
	size_t i = 0;
	size_t y = 0;

	while (strcmp(splited[i], "#END"))
	{
		if (!(splited[i][0] == '#' && splited[i][0] == '%'))
			y++;
		i++;
	}
	return (y);
}

void map_parser(char *st_map, t_map *map)
{
	char **splited = ft_split(st_map, "\n");
	size_t i = 0;
	
	while (splited[i][0] != '%' && splited[i][0] == '#')
		i++;
	if(!(map->stripe = (char **)malloc(sizeof(char *) * (map_striplen(splited) + 1))))
		return ;
	while (strcmp(splited[i], "#END"))
	{
		
	}
}
