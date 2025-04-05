#include "chore/chore.h"
#include "codemon.h"
#include "extern/raylib-5.5/src/raylib.h"
#include <stdio.h>



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
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(ctx->windowWidth, ctx->windowHeight, ctx->windowName);
	ctx->player = CreatePlayer(ctx);
	HRAL_DeclareLibrary(&ctx->hrContext, "./src/modules/graphics/libgraphics.dylib", " cd ./src/modules/graphics/ && clang -I./src/extern/raylib-5.5/src/ -undefined dynamic_lookup *.c -dynamiclib -o libgraphics.dylib ");
	ctx->player = CreatePlayer(ctx);
	ctx->map = malloc(sizeof(t_map));
	init_map(&ctx->map, "./maps/code.map");
	prepare_map(ctx->map, ctx); 
	ctx->textures = load_texture(ctx->map);
}




int main(void)
{
	AppContext *ctx;
	SetTargetFPS(100);
	ctx = CreateAppContext();

	InitApp(ctx);
	SetTargetFPS(100);

	double lastReloadTime = 0;
	double reloadTimeInterval = 1;
	void (*DrawGame)(AppContext*) = HRAL_GetFunctionFromContext(&ctx->hrContext, "DrawGame");

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
		{
			ctx->player->isMoving = true;
			if (IsKeyPressed(KEY_UP))
				ctx->player->dir = UP;
			else if (IsKeyPressed(KEY_DOWN))
				ctx->player->dir = DOWN;
			else if (IsKeyPressed(KEY_RIGHT))
				ctx->player->dir = RIGHT;
			else if (IsKeyPressed(KEY_LEFT))
				ctx->player->dir = LEFT;
		}
		else
			ctx->player->isMoving = false;
		UpdatePlayer(ctx);
		DrawGame(ctx);

		double currentTime = GetTime();
		if (currentTime - lastReloadTime > reloadTimeInterval)
		{
			HRAL_CheckForReload(&ctx->hrContext);
			DrawGame  = HRAL_GetFunctionFromContext(&ctx->hrContext, "DrawGame");
			lastReloadTime = currentTime;
		}
	}
	return 0;
}
