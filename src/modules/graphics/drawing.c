#include "../../codemon.h"
#include "../../extern//raylib-5.5/src/raylib.h"




void	DrawDialogBox(AppContext *ctx, Rectangle rec, const char* text, Color bgColor, Color textColor)
{
	DrawRectangleRec(rec, bgColor);
	int textWidth = MeasureText(text, 10);
	int textHeight = 10;

	int textX	= (rec.width - textWidth)/2;
	int textY	= (rec.height - textHeight)/2;
	DrawText(text, textX, textY, 20, textColor);
	DrawRectangleLinesEx(rec, 3, BLACK);
}

void	DrawMap(AppContext *ctx)
{
	ClearBackground(WHITE);
}
