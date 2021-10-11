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
	String text;
	GameImage image;
	std::function<void()> primary;
	std::function<void()> secondary;

	LauncherItem(String text, const GameImage& image = GameImage(), std::function<void()> primary = {}, std::function<void()> secondary = {});
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

	void checkLoaded();

private:
	static Launcher* instance;
	Display* display;
	Sprite* canvas;
	void setCanvas(Sprite* canvas);

	Logo* logo;
	GameTitle* title;
	GameScroller* scroller;
	Splash* splash;
	LoadingIndicator* loader;

	GameImage genericIcon;
	GameImage settingsIcon;
	std::vector<LauncherItem> items;
	void load();
	GameInfo* loaded = nullptr;

	uint8_t selectedGame = 0;
	void prev();
	void next();

	void openGameImageSPIFFS(String path, GameImage& gameImage);
	void bindInput();

protected:
	void init() override;
	void deinit() override;

};

#endif //BYTEBOI_LAUNCHER_H
