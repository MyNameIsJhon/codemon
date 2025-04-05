#ifndef MAPS_H
# define MAPS_H

#include <stddef.h>

typedef struct	s_map {
	size_t width;
	size_t height;
	char ***stripe;
	char *content;
	char **map;
	size_t t_x;
	size_t t_y;
}				t_map;

char *get_map(char *path);
void ft_free_strsplit(char **split);
char	**ft_split(char *str, char *charset);
void stripe_parser(char *st_map, t_map *map);
void print_param(t_map *map);
t_map *init_map(char *path);
char *ft_strjoin(char const *s1, char const *s2);

#endif
