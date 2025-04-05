#ifndef CODEMON_H
#define CODEMON_H
#include "chore/chore.h"
#include "extern/raylib-5.5/src/raylib.h"

typedef enum Direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
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
}	Player;

typedef struct AppContext
{
	int				windowHeight;
	int				windowWidth;
	char			windowName[256];
	HRAL_Context	hrContext;
} AppContext;

Player	*CreatePlayer(AppContext *ctx);
void	DrawPlayer(Player *player);
void	UpdatePlayer(Player *player);


#endif // !CODEMON_H
