#ifndef CODEMON_H
#define CODEMON_H
#include "chore/chore.h"
#include "raylib.h"


typedef struct AppContext
{
	int				windowHeight;
	int				windowWidth;
	char			windowName[256];
	HRAL_Context	hrContext;
} AppContext;


#endif // !CODEMON_H
