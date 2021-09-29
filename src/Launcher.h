#ifndef BYTEBOI_LAUNCHER_H
#define BYTEBOI_LAUNCHER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Support/Context.h>
#include <functional>
#include "GameInfo.hpp"
#include "Elements/GameImage.h"
#include "GameManagement/GameListener.hpp"
#include "LoadingIndicator.h"

class Logo;
class GameTitle;
class GameScroller;
class Splash;
class Menu;

struct LauncherItem {
	GameImage image;
	String text;
	std::function<void()> exec;

	LauncherItem(const GameImage& image, String text, std::function<void()> exec);
};

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
	LoadingIndicator* loading;

	GameImage genericIcon;
	std::vector<LauncherItem> items;
	void load();

	uint8_t selectedGame = 0;
	void prev();
	void next();

	void bindInput();

protected:
	void init() override;
	void deinit() override;

};

#endif //BYTEBOI_LAUNCHER_H
