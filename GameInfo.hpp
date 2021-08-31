#ifndef GAME_INFO
#define GAME_INFO
#include <CircuitOS.h>
#include <Display/Display.h>
#include <FS.h>

class Context;
struct GameInfo {
	const char* title;
	const char* description;
    fs::File (*icon)();
	Context* (*launch)(Display& display);
};


#endif
