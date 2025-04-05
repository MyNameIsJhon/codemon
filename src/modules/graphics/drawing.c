#include "../../codemon.h"
#include "../../extern//raylib-5.5/src/raylib.h"





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
	EndDrawing();
}
