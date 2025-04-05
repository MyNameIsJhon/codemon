#include "chore/chore.h"
#include "codemon.h"
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
	InitWindow(ctx->windowWidth, ctx->windowHeight, ctx->windowName);
	HRAL_DeclareLibrary(&ctx->hrContext, "./src/modules/graphics/libgraphics.dylib", " cd ./src/modules/graphics/ && clang -I./src/extern/raylib-5.5/src/ -undefined dynamic_lookup *.c -dynamiclib -o libgraphics.dylib ");
}

int main(void)
{
	AppContext *ctx = CreateAppContext();
	InitApp(ctx);
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		HRAL_CheckForReload(&ctx->hrContext);
		void (*DrawMap)(AppContext*) = HRAL_GetFunctionFromContext(&ctx->hrContext, "DrawMap");
		BeginDrawing();
		DrawMap(ctx);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}
