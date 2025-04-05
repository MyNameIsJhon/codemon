#ifndef CODEMON_H
#define CODEMON_H
#include "chore/chore.h"
#include "extern/raylib-5.5/src/raylib.h"
#define PLAYER_SPEED 5
#define PLAYER_SPRITE_LENGTH 24

typedef enum Direction
{
	LEFT,
	DOWN,
	RIGHT,
	UP
}	Direction;

typedef struct Point
{
	float x;
	float y;
}	Point;

typedef struct Player 
{
	Point position;
	bool isMoving;
	Direction dir;
	Texture2D texture;
	Rectangle frames[16];
}	Player;

typedef struct AppContext
{
	int				windowHeight;
	int				windowWidth;
	char			windowName[256];
	HRAL_Context	hrContext;
	Player			*player;
} AppContext;

Player	*CreatePlayer(AppContext *ctx);
void	DrawPlayer(AppContext *ctx);
void	UpdatePlayer(AppContext *ctx);


#endif // !CODEMON_H
