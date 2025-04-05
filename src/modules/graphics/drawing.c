#include "../../codemon.h"
#include "../../extern//raylib-5.5/src/raylib.h"

static Rectangle	GetStillSprite(Direction dir)
{
	switch (dir)
	{
		case UP:
			return (Rectangle){ 16, 16, 16, 16 };
		case DOWN:
			return (Rectangle){ 16, 0, 16, 16 };
		case RIGHT:
			return (Rectangle){ 0, 16, 16, 16 };
		case LEFT:
			return (Rectangle){ 0, 0, 16, 16 };
	}
}

void	DrawPlayer(AppContext *ctx)
{
	Player *player = ctx->player;
	Rectangle dest = (Rectangle){ player->position.x, player->position.y, 35, 35 };
	Rectangle src;
	if (!player->isMoving)
		src = GetStillSprite(player->dir);
	else
		src = player->frames[((((int)(GetTime() * FPS) % 4)) * 4) + (player->dir)];
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

void DrawGame(AppContext *ctx)
{
	BeginDrawing();
	DrawMap(ctx);
	DrawDialogBox(ctx, (Rectangle){.x = 0, .y = 0, .width = 500, .height = 500}, "Coucou les musulmans\n moi je mange la glace\net toi tu manges rien", LIGHTGRAY, BLACK);
	DrawPlayer(ctx);
	EndDrawing();
}
