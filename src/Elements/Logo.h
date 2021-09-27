#ifndef BYTEBOI_LOGO_H
#define BYTEBOI_LOGO_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Display/Color.h>

class Sprite;

class Logo {
public:
	Logo(Sprite* canvas);

	void loop(uint micros);
	void splash(float f);
	void draw();

private:
	const uint8_t width = 93;
	const uint8_t height = 26;
	const int8_t x;
	const int8_t y = 8;
	const int8_t startY = 50;
	const float speed = 300;
	const float amplitude = 3;

	float currentY;
	float f = 0;

	Color* logoBuffer = nullptr;

	Sprite* canvas;
};


#endif //BYTEBOI_LOGO_H
