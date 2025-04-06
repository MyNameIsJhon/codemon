#include "graphics.h"


void	DrawPlayer(AppContext *ctx)
{
	Player *player = ctx->player;
	Rectangle dest = (Rectangle){ player->position.x, player->position.y, 35, 35 };
	Rectangle src;
	if (!player->isMoving)
		src = player->frames[player->dir];
	else
		src = player->frames[((((int)(GetTime() * PLAYER_SPEED) % 4)) * 4) + (player->dir)];
	DrawTexturePro(
		player->texture,
		src,
		dest,
		(Vector2){ 0, 0 },
		0,
		WHITE
	);
}

void DrawDialogBox(AppContext *ctx, Rectangle rec, const char* text, Color bgColor, Color textColor)
{
	static int letterCount = 0;
	static double timeElapsed = 0.0;
	const double letterInterval = 0.06;

	timeElapsed += GetFrameTime();
	if (timeElapsed >= letterInterval)
	{
		if (letterCount < (int)strlen(text)) letterCount++;
		timeElapsed = 0.0;
	}

	DrawRectangleRec(rec, bgColor);

	int textWidth = MeasureText(text, 20);
	int textHeight = 20;

	int textX = rec.x + (rec.width - textWidth) / 2;
	int textY = rec.y + (rec.height - textHeight) / 2;
	DrawText(TextSubtext(text, 0, letterCount), textX, textY, 20, textColor);
	DrawRectangleRoundedLinesEx(rec, 0.05, 1, 3, BLACK);
	if (letterCount >= (int)strlen(text)) 
	{
		letterCount = 0;
		timeElapsed = 0.0;
	}
}


void	DrawMap(AppContext *ctx)
{
	ClearBackground(WHITE);
}

void DrawPlayerDirections(AppContext *ctx, float centerX, float centerY)
{;
    const float ARROW_OFFSET = 60;
    const float ARROW_SIZE = 50;
    
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
        
        DrawTexturePro(
            ctx->playerDirections[i].texture,
            (Rectangle){ 0, 0, 32, 32 },
            (Rectangle){ x, y, ARROW_SIZE, ARROW_SIZE },
            (Vector2){ ARROW_SIZE/2, 0 },
            0.0,
			WHITE
        );
		Color color = { 0, 0, 0, 0 };
		if (ctx->playerDirections[i].selected)
			color = (Color){ 0, 0, 0, 40 };
		DrawRectanglePro(
			(Rectangle){ x, y, ARROW_SIZE, ARROW_SIZE },
			(Vector2){ ARROW_SIZE/2, 0 },
			0.0,
			color
		);
	}
}

void DrawArrows(AppContext* ctx, float centerX, float centerY) {
    const float ARROW_SIZE = 50;
    const float ARROW_OFFSET = 60;
    
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
        
        DrawTexturePro(
            ctx->arrowTexture,
            (Rectangle){ 0, 0, 1280, 1280 },
            (Rectangle){ x, y, ARROW_SIZE, ARROW_SIZE },
            (Vector2){ ARROW_SIZE/2, 0 },
            arrowPositions[i].rotation,
			WHITE
        );
		Color color = { 0, 0, 0, 0 };
		if (ctx->arrows[i].selected)
			color = (Color){ 0, 0, 0, 40 };
		DrawRectanglePro(
			(Rectangle){ x, y, ARROW_SIZE, ARROW_SIZE },
			(Vector2){ ARROW_SIZE/2, 0 },
			arrowPositions[i].rotation,
			color
		);
    }
}

void DrawGame(AppContext *ctx)
{
	BeginDrawing();
	DrawMap(ctx);
	DrawDialogBox(ctx, (Rectangle){.x = 0, .y = 0, .width = 500, .height = 500}, "Codemon\nProduit par :\n- Pierre\n- Lala\n- Po", LIGHTGRAY, BLACK);
	DrawPlayer(ctx);
	DrawArrows(ctx, 100, ctx->windowHeight - 100);
	DrawPlayerDirections(ctx, ctx->windowWidth - 150, ctx->windowHeight - 150);
	DrawConsole(ctx);
	EndDrawing();
}
