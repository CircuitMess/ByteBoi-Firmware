#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Support/Context.h>

class Logo;
class GameTitle;
class GameScroller;
class Splash;
class Menu;


extern Context* runningContext;
extern bool exitingGame;

class Launcher : public LoopListener, public Context {
public:
	Launcher(Display* display);

	void loop(uint micros) override;
	void start() override;
	void stop() override;
	void draw() override;

private:
	static Launcher* instance;
	Display* display;
	Sprite* canvas;

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;
	Splash* splash;

	uint8_t selectedGame = 0;
	void prev();
	void next();

	void bindInput();

};

#endif //BYTEBOI_LAUNCHER_H
