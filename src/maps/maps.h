#ifndef MAPS_H
# define MAPS_H

#include <stddef.h>

typedef struct	s_map {
	size_t width;
	size_t height;
	char **stripe;
	char *content;
	char **map;
}				t_map;

char *get_map(char *path);
char	**ft_split(char *str, char *charset);

#endif
