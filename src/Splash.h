#ifndef NIBBLE_SPLASH_H
#define NIBBLE_SPLASH_H

#include <Arduino.h>

class Sprite;
class GameTitle;
class GameScroller;

class Splash {
public:
	Splash(GameScroller* scroller);

	void loop(uint micros);
	bool done();

private:
	const float hold = 0.5;

	float f = 0;

	Sprite* canvas;

	GameTitle* title;
	GameScroller* scroller;
};


#endif //NIBBLE_SPLASH_H
