#include "../codemon.h"

void prepare_map(t_map *map, AppContext *ctx)
{
	map->t_x = ctx->windowWidth / map->width;
	map->t_y = ctx->windowHeight / map->height;
}

Texture2D *load_texture(t_map *map)
{
	Texture2D *textures;
	size_t i = 0;
	char str[255];

	while (map->stripe[i])
		i++;
	if (!(textures = (Texture2D *)malloc(sizeof(Texture2D) * (i + 1))))
		return (NULL);
	i = 0;
	while (map->stripe[i])
	{
		strcpy(str, "./textures/");
		strcat(str, map->stripe[i][1]);
		textures[i++] = LoadTexture(str);
	}
	return textures;
}

Rectangle texture_config(t_map map, size_t actual_x, size_t actual_y)
{
	Rectangle rec;

	rec.x = actual_x;
	rec.y = actual_y;
	rec.width = map.t_x;
	rec.height = map.t_y;
	return rec;
}

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
	map[i] = '\0'; 
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
	fclose(file);
	return str;
}

size_t map_striplen(char **splited)
{
	size_t i = 0;
	size_t y = 0;

	while (splited[i] && strcmp(splited[i], "#END"))
	{
		if (!(splited[i][0] == '#' && splited[i][0] == '%'))
			y++;
		i++;
	}
	return (y);
}

void stripe_parser(char *st_map, t_map *map)
{
	char **splited = ft_split(st_map, "\n");
	size_t i = 0;
	size_t y = 0;
	
	while (splited[i][0] == '%' || splited[i][0] == '#')
		i++;
	if(!(map->stripe = (char ***)malloc(sizeof(char **) * (map_striplen(splited) + 1))))
		return ;
	while (splited[i] && strcmp(splited[i], "#END") != 0)
	{
		if(splited[i][0] && splited[i][0] != '#' && splited[i][0] != '%')
			map->stripe[y++] = ft_split(splited[i], " =");
		i++;
	}
	map->stripe[y] = NULL;
	ft_free_strsplit(splited);
}

void load_map(t_map *map)
{
	size_t i = 0;
	size_t y = 0;
	char **splited = ft_split(map->content, "\n");
	
	while (!isdigit(splited[i][y]))
		y++;
	map->height = atoi((splited[i] + y));

	while (splited[i] && strcmp(splited[i], "#END"))
		i++;
	i++;
	y = 0;
	if (!(map->map = (char **) malloc(sizeof(char *) * (map->height + 1))))	
		return ;
	while (splited[i])
		map->map[y++] = strdup(splited[i++]);
	map->map[y] = NULL;
	ft_free_strsplit(splited);
}

void init_map(t_map **imap, char *path)
{
	t_map *map;

	if (!(map = (t_map *) malloc(sizeof(t_map))))
		return ;
	*imap = map;
	map->content = get_map(path);
	stripe_parser(map->content, map);
	load_map(map); 
	map->width = strlen(map->map[0]); 
}

void print_param(t_map *map)
{
	printf("INFOS: Image et texture dedies\n\n");
	for (int i = 0; map->stripe[i]; i++ )
		printf("%s = %s\n", map->stripe[i][0] ,map->stripe[i][1]);
	printf("\n\nINFOS: Map format ascii\n\n");
	for (int i = 0; map->map[i]; i++ )
		printf("%s = %s\n", map->map[i] ,map->map[i]);
	printf("\n\nINFOS: Map format ascii\n\n");
}
