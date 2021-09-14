#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Color.h>
#include <FS.h>

class Sprite;

class GameImage {
public:
	GameImage(Sprite* canvas, size_t gameIndex);

	virtual ~GameImage();

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;
	void loadImage();
	void releaseImage();

private:
	int16_t x;
	int16_t y;
	Color* appIconBuffer = nullptr;
	size_t index = 0;

	Sprite* canvas;
};


#endif //BYTEBOI_GAMEIMAGE_H
