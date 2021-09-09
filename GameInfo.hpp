#ifndef GAME_INFO
#define GAME_INFO
#include <CircuitOS.h>
#include <Display/Display.h>
#include <FS.h>
#include <SPIFFS.h>

class Context;
extern fs::File globalFile;



struct GameInfo {
	const char* title;
	const char* description;
    void (*icon)();
	Context* (*launch)(Display& display);
};


#endif
