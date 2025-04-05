#include "chore/chore.h"
#include "codemon.h"
#include "raylib.h"
#include <string.h>
#include <stdlib.h>


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
	HRAL_DeclareLibrary(&ctx->hrContext, "./src/modules/graphics/libgraphics.dylib", " cd ./src/modules/graphics/ && clang -I./src/extern/raylib-5.5/src/ -undefined dynamic_lookup *.c -dynamiclib -o libgraphics.dylib ");
}

int main(void)
{
	AppContext *ctx = CreateAppContext();
	InitApp(ctx);
	SetTargetFPS(100);

	Player *player = CreatePlayer(ctx);
	double lastReloadTime = 0;
	double reloadTimeInterval = 1;
	void (*DrawGame)(AppContext*) = HRAL_GetFunctionFromContext(&ctx->hrContext, "DrawGame");

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
		{
			player->isMoving = true;
			if (IsKeyPressed(KEY_UP))
				player->dir = UP;
			else if (IsKeyPressed(KEY_DOWN))
				player->dir = DOWN;
			else if (IsKeyPressed(KEY_RIGHT))
				player->dir = RIGHT;
			else if (IsKeyPressed(KEY_LEFT))
				player->dir = LEFT;
		}
		else
			player->isMoving = false;
		UpdatePlayer(player);
		DrawGame(ctx);

		double currentTime = GetTime();
		if (currentTime - lastReloadTime > reloadTimeInterval)
		{
			HRAL_CheckForReload(&ctx->hrContext);
			DrawGame  = HRAL_GetFunctionFromContext(&ctx->hrContext, "DrawGame");
			lastReloadTime = currentTime;
		}
	}
	CloseWindow();

	return 0;
}
