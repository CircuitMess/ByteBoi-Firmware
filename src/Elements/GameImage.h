#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Color.h>
#include <FS.h>

class Sprite;
class GameImageLoader;

class GameImage {
	friend GameImageLoader;
public:
	GameImage(Sprite* canvas, size_t gameIndex, uint8_t* genericIcon);

	virtual ~GameImage();

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;
	void releaseImage();
	void loadImage();

private:
	int16_t x;
	int16_t y;
	Color* appIconBuffer = nullptr;
	size_t index = 0;

	Sprite* canvas;
	Color* genericIcon = nullptr;
};


#endif //BYTEBOI_GAMEIMAGE_H
