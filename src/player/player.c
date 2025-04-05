#include "../codemon.h"

Player	*CreatePlayer(AppContext *ctx)
{
	Player	*player;

	player = (Player *)malloc(sizeof(Player));
	player->position = (Point){ ctx->windowWidth / 2.0, ctx->windowHeight / 2.0 };
	player->dir = DOWN;
	player->isMoving = false;
	player->texture = LoadTexture("./sprites/player_sprite.png");
	for (int i = 0; i < 16; i ++)
		player->frames[i] = (Rectangle){ 16 * ((i % 4)), (i / 4) * 16, 16, 16 };
	return (player);
}

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

void	UpdatePlayer(AppContext *ctx)
{
	Player *player = ctx->player;
	if (!player->isMoving)
		return ;
	switch(player->dir)
	{
		case UP:
			player->position.y -= 2;
			break ;
		case DOWN:
			player->position.y += 2;
			break ;
		case RIGHT:
			player->position.x += 2;
			break ;
		case LEFT:
			player->position.x -= 2;
			break ;
	}
}
