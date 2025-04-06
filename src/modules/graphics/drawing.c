#include "graphics.h"

void DrawMap(AppContext *ctx)
{
	Texture2D *textures = ctx->textures;
	t_map *map = ctx->map;
	Rectangle rec = texture_config(*map, 0, 0);
	Vector2 vec = {0, 0};

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

void DrawFloor(AppContext *ctx)
{
	Texture2D *textures = ctx->textures;
	t_map *map = ctx->map;
	Rectangle rec = texture_config(*map, 0, 0);
	Vector2 vec = {0, 0};

	for (size_t i = 0 ; map->map[i] ; i++)
	{
		for (size_t y = 0 ; map->map[i][y] ; y++)
		{
			Rectangle source = { 0, 0, (float)textures[0].width, (float)textures[0].height };
			Rectangle dest = { rec.x, rec.y, map->t_x, map->t_y };
			Vector2 origin = { 0, 0 };
			DrawTexturePro(textures[0], source, dest, origin, 0.0f, WHITE);
			rec.x += map->t_x;
		}
		rec.x = 0;
		rec.y += map->t_y;
	}
}
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

void DrawGame(AppContext *ctx)
{
	BeginDrawing();
	DrawFloor(ctx);
	DrawMap(ctx);
	DrawDialogBox(ctx, (Rectangle){.x = 0, .y = 0, .width = 500, .height = 500}, "Codemon\nProduit par :\n- Pierre\n- Lala\n- Po", LIGHTGRAY, BLACK);
	DrawPlayer(ctx);
	DrawConsole(ctx);
	EndDrawing();
}
