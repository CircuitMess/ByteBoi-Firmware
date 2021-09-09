#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Color.h>
#include <FS.h>

class Sprite;

class GameImage {
public:
	GameImage(Sprite* canvas,fs::File icon);

	virtual ~GameImage();

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;

private:
	int16_t x;
	int16_t y;
	fs::File icon;
	Color* appIconBuffer = nullptr;

	Sprite* canvas;
};


#endif //BYTEBOI_GAMEIMAGE_H
