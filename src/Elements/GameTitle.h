#ifndef BYTEBOI_GAMETITLE_H
#define BYTEBOI_GAMETITLE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>

class Sprite;

class GameTitle : public LoopListener {
public:
	GameTitle(Sprite* canvas);

	void change(const char* newText);

	void loop(uint micros) override;
	void draw();

private:
	uint8_t width = 80;
	uint8_t height = 20;
	int8_t x;
	int8_t y;
	const float speed = 200;
	const uint8_t overHide = 5;

	Sprite* canvas;
	const char* text = nullptr;

	float currentY;
	enum State { DOWN, UP } state;
	const char* changeTo = nullptr;

};


#endif //BYTEBOI_GAMETITLE_H
