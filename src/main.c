#include "chore/chore.h"
#include "codemon.h"
#include "raylib.h"
#include <string.h>
#include <stdlib.h>

void CreatePlayerDirection(AppContext *ctx)
{
    const float ARROW_SIZE = 50;
    const float ARROW_OFFSET = 60;
    const float centerX = ctx->windowWidth - 150;
    const float centerY = ctx->windowHeight - 150;
    const struct {
        float xOffset;
        float yOffset;
        float rotation;
    } arrowPositions[] = {
        {-ARROW_OFFSET, 0, -90.0f},
        {0, -ARROW_OFFSET, 0.0f},
        {ARROW_OFFSET, 0, 90.0f},
        {0, ARROW_OFFSET, 180.0f}
    };
    
    for (int i = 0; i < 4; i++) {
        float x = centerX + arrowPositions[i].xOffset;
        float y = centerY + arrowPositions[i].yOffset;
        
        Rectangle hitbox = (Rectangle){ 
            x - ARROW_SIZE/2,
            y - ARROW_SIZE/2,
            ARROW_SIZE, 
            ARROW_SIZE 
        };
		char tmp[256];
		sprintf(tmp, "./sprites/icon%d.png", i + 1);
        ctx->playerDirections[i] = (PlayerDirection){ 
            false,
            i,
            0,
            hitbox,
			LoadTexture(tmp)
        };
    }
}

void CreateArrows(AppContext *ctx)
{
    const float ARROW_SIZE = 50;
    const float ARROW_OFFSET = 60;
    const float centerX = 100;
    const float centerY = ctx->windowHeight - 100;
    const struct {
        float xOffset;
        float yOffset;
        float rotation;
    } arrowPositions[] = {
        {-ARROW_OFFSET, 0, -90.0f},
        {0, -ARROW_OFFSET, 0.0f},
        {ARROW_OFFSET, 0, 90.0f},
        {0, ARROW_OFFSET, 180.0f}
    };
    
    for (int i = 0; i < 4; i++) {
        float x = centerX + arrowPositions[i].xOffset;
        float y = centerY + arrowPositions[i].yOffset;
        
        Rectangle hitbox = (Rectangle){ 
            x - ARROW_SIZE/2,
            y - ARROW_SIZE/2,
            ARROW_SIZE, 
            ARROW_SIZE 
        };

        ctx->arrows[i] = (Arrow){ 
            false,
            i,
            arrowPositions[i].rotation,
            hitbox
        };
    }
}

AppContext	*CreateAppContext()
{
	AppContext *ctx = malloc(sizeof(AppContext));
	memset(ctx, 0, sizeof(AppContext));
	strcat(ctx->windowName, "Codemon");
	ctx->windowWidth	= 900;
	ctx->windowHeight	= 600;
	CreateArrows(ctx);
	return (ctx);
}

void	InitApp(AppContext *ctx)
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(ctx->windowWidth, ctx->windowHeight, ctx->windowName);
	ctx->player = CreatePlayer(ctx);
	ctx->arrowTexture = LoadTexture("./sprites/arrow.png");
	CreatePlayerDirection(ctx);
	HRAL_DeclareLibrary(&ctx->hrContext, "./src/modules/graphics/libgraphics.dylib", " cd ./src/modules/graphics/ && clang -I./src/extern/raylib-5.5/src/ -undefined dynamic_lookup *.c -dynamiclib -o libgraphics.dylib ");
}

int main(void)
{
	AppContext *ctx = CreateAppContext();
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
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			for (int i = 0; i < 4; i++)
			{
				if (CheckCollisionPointRec(GetMousePosition(), ctx->arrows[i].hitbox))
				{
					for (int j = 0; j < 4; j++)
						ctx->arrows[j].selected = false;

					ctx->arrows[i].selected = true;
					break;
				}
				if (CheckCollisionPointRec(GetMousePosition(), ctx->playerDirections[i].hitbox))
				{
					for (int j = 0; j < 4; j++)
					{
						ctx->playerDirections[j].selected = false;
					}

					ctx->playerDirections[i].selected = true;
					break;
				}
			}
		}
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
	CloseWindow();

	return 0;
}
