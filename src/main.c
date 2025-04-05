#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include "./maps/maps.h"
#include <stdio.h>

typedef struct AppContext
{
	int		windowHeight;
	int		windowWidth;
	char	windowName[256];
} AppContext;

AppContext	*CreateAppContext()
{
	AppContext *ctx = malloc(sizeof(AppContext));
	memset(ctx, 0, sizeof(AppContext));
	strcat(ctx->windowName, "Codemon");
	ctx->windowWidth	= 900;
	ctx->windowHeight	= 600;
	return (ctx);
}


void	InitApp(AppContext *ctx)
{
	InitWindow(ctx->windowWidth, ctx->windowHeight, ctx->windowName);
}


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


void draw_map(t_map *map, Texture2D *textures, AppContext ctx)
{
	Rectangle rec = texture_config(*map, 0, 0);

	for (size_t i = 0 ; map->map[i] ; i++)
	{
		for (size_t y = 0 ; map->map[i][y] ; y++)
		{
			for (size_t z = 0 ; map->stripe[z] ; z++)
			{
				if (map->stripe[z][0][0] == map->map[i][y])
				{
					Rectangle source = { 0, 0, (float)textures[z].width, (float)textures[z].height };
					Rectangle dest = { rec.x, rec.y, map->t_x, map->t_y };
					Vector2 origin = { 0, 0 };
					DrawTexturePro(textures[z], source, dest, origin, 0.0f, WHITE);
				}
			}
			rec.x += map->t_x;
		}
		rec.x = 0;
		rec.y += map->t_y;
	}
}

int main(void)
{
	t_map *map;
	AppContext *ctx;
	SetTargetFPS(100);
	ctx = CreateAppContext();

	InitApp(ctx);
	init_map(&map, "maps/code.map");
	prepare_map(map, ctx);
	Texture2D *textures = load_texture(map);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK); // Important
		draw_map(map, textures, *ctx);
		EndDrawing();
	}
	return 0;
}
