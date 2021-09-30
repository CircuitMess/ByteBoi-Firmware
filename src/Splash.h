#ifndef NIBBLE_SPLASH_H
#define NIBBLE_SPLASH_H

#include <Arduino.h>

class Sprite;
class GameScroller;
class Logo;

class Splash {
public:
	Splash(GameScroller* scroller, Logo* logo);

	void loop(uint micros);
	bool done();

private:
	float f = 0;

	Sprite* canvas;

	GameScroller* scroller;
	Logo* logo;
};


#endif //NIBBLE_SPLASH_H
