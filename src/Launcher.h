#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Support/Context.h>
#include "GameInfo.hpp"
#include "GameManagement/GameListener.hpp"

class Logo;
class GameTitle;
class GameScroller;
class Splash;
class Menu;


class Launcher : public LoopListener, public Context, public GameListener {
public:
	Launcher(Display* display);

	virtual ~Launcher();

	void loop(uint micros) override;
	void start() override;
	void stop() override;
	void draw() override;
	void gamesChanged(bool inserted) override;

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

	Color* backgroundBuffer = nullptr;

protected:
	void init() override;

	void deinit() override;
};

#endif //BYTEBOI_LAUNCHER_H
