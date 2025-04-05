#ifndef CODEMON_H
#define CODEMON_H
#include "chore/chore.h"
#include "extern/raylib-5.5/src/raylib.h"
#include <stdio.h>
#include <ctype.h>
#define PLAYER_SPEED 5
#define PLAYER_SPRITE_LENGTH 24


// WARN: ======================  Shitstorm ===========================
typedef struct	s_map {
	size_t width;
	size_t height;
	char ***stripe;
	char *content;
	char **map;
	size_t t_x;
	size_t t_y;
}				t_map;
// WARN: ===========================================================

typedef enum Direction
{
	LEFT,
	DOWN,
	RIGHT,
	UP
}	Direction;

typedef struct Point
{
	float x;
	float y;
}	Point;

typedef struct Player 
{
	Point position;
	bool isMoving;
	Direction dir;
	Texture2D texture;
	Rectangle frames[16];
}	Player;

typedef struct AppContext
{
	int				windowHeight;
	int				windowWidth;
	char			windowName[256];
	HRAL_Context	hrContext;
	Player			*player;
	t_map			*map;
	Texture2D		*textures;
} AppContext;

Player	*CreatePlayer(AppContext *ctx);
void	DrawPlayer(AppContext *ctx);
void	UpdatePlayer(AppContext *ctx);


// WARN: ======================  Shitstorm ===========================
char *get_map(char *path);
void ft_free_strsplit(char **split);
char	**ft_split(char *str, char *charset);
void stripe_parser(char *st_map, t_map *map);
void print_param(t_map *map);
void init_map(t_map **imap, char *path);
char *ft_strjoin(char const *s1, char const *s2);
void prepare_map(t_map *map, AppContext *ctx);
Texture2D *load_texture(t_map *map);
Rectangle texture_config(t_map map, size_t actual_x, size_t actual_y);


size_t map_size(FILE *file);
void read_map(FILE *file, char *map);
char *get_map(char *path);
size_t map_striplen(char **splited);
void stripe_parser(char *st_map, t_map *map);
void load_map(t_map *map);
void init_map(t_map **imap, char *path);
void print_param(t_map *map);

// WARN: ===========================================================

#endif // !CODEMON_H
