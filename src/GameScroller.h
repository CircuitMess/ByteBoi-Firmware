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

	void draw();
	void loop(uint micros);

	uint8_t prev();
	uint8_t next();

	bool scrolling();

	void splash(float f);
	void reset();

	GameInfo* const getSelectedGame() const;

private:
	const uint8_t gutter = 20;
	const uint8_t width = 64;
	const uint8_t origin;
	const float speed = 250;

	Sprite* canvas;
	std::vector<LauncherItem>& items;

	uint selectedGame = 0;
	enum Direction { PREV, NEXT } direction;
	float delta = 0;
	bool queued = false;
	uint8_t multiplier = 1;

	void repos();

	void selectNext();
	void selectPrev();

	GameImage* getCGame();
	GameImage* getLGame();
	GameImage* getRGame();
	GameImage* getLLGame();
	GameImage* getRRGame();
};


#endif //BYTEBOI_GAMESCROLLER_H
