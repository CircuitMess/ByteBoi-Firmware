#ifndef BYTEBOI_LOGO_H
#define BYTEBOI_LOGO_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Display/Color.h>

class Sprite;

class Logo : public LoopListener {
public:
	Logo(Sprite* canvas);

	virtual ~Logo();

	void loop(uint micros) override;
	void draw();
	void start();
	void stop();
	void pause();

	void setCentered(float f);

private:
	const uint8_t width = 93;
	const uint8_t height = 26;
	const int8_t startY = 6;
	const int8_t startX;
	const float speed = 300;
	const float maxAmp = 12;
	const float ampSpeed = 5000;

	float ampf = 0;
	float amp = 0;

	float f = 0;
	float diffX = 0;

	float center = 0;
	uint8_t centerDiff;

	enum { OFF, ON, EXIT } state = OFF;

	Color* logoBuffer = nullptr;

	Sprite* canvas;
};


#endif //BYTEBOI_LOGO_H
