#ifndef BYTEBOI_GAMESCROLLER_H
#define BYTEBOI_GAMESCROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Util/Vector.h>
#include <Properties.h>
#include "GameInfo.hpp"
#include "Elements/GameImage.h"
#include "Launcher.h"

class Sprite;
class Launcher;

class GameScroller : public LoopListener {
public:
	GameScroller(Sprite* canvas, std::vector<LauncherItem>& items);
	~GameScroller();
	void setCanvas(Sprite* canvas);

	void draw();
	void loop(uint micros);

	uint8_t prev();
	uint8_t next();

	bool scrolling();

	void splash(float f);
	void load(float f);
	void finish(float f);
	void reset();
	void repos();
	uint getSelectedIndex() const;

private:
	const uint8_t gutter = 25;
	const uint8_t width = 64;
	const uint8_t origin;
	const uint8_t originY = 35;
	const float speed = 250;

	Sprite* canvas;
	std::vector<LauncherItem>& items;

	uint selectedGame = 0;
	enum Direction { PREV, NEXT } direction;
	float delta = 0;
	bool queued = false;
	uint8_t multiplier = 1;

	void selectNext();
	void selectPrev();

	GameImage* getCGame();
	GameImage* getLGame();
	GameImage* getRGame();
	GameImage* getLLGame();
	GameImage* getRRGame();
};


#endif //BYTEBOI_GAMESCROLLER_H
