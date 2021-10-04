#ifndef BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
#define BYTEBOI_FIRMWARE_LOADINGINDICATOR_H


#include <Display/Sprite.h>
#include <Loop/LoopListener.h>
#include "Elements/GameImage.h"
#include "GameManagement/GameLoader.h"

class Logo;
class GameScroller;
class GameTitle;
struct GameInfo;
class Launcher;

class LoadingIndicator : public LoopListener {
public:
	LoadingIndicator(Launcher* launcher, Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title);
	void setCanvas(Sprite* canvas);

	void start(GameInfo* game, GameImage* image);
	void abort(bool immediate = false);

	bool isActive() const;
	bool isBooting() const;

	void draw();

	void loop(uint micros) override;

private:
	Launcher* launcher;
	Sprite* canvas = nullptr;
	Logo* logo = nullptr;
	GameScroller* scroller = nullptr;
	GameTitle* title = nullptr;
	GameImage* image = nullptr;
	GameImage imageCopy;
	GameInfo* game;
	GameLoader::Job* loadJob = nullptr;

	String currentText;
	bool boot = false;

	enum { OUT, ENTER, IN, EXIT, FINISH } state = OUT;
	float f = 0;
	uint32_t finishTime = 0;

	uint32_t lastDraw = 0;

	void stop();
	void finish();
};


#endif //BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
