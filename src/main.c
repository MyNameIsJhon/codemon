
#include "raylib.h"
#include <string.h>
#include <stdlib.h>


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

int main(void)
{
	AppContext *ctx = CreateAppContext();
	InitApp(ctx);
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
