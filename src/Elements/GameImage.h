#ifndef BYTEBOI_GAMEIMAGE_H
#define BYTEBOI_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Color.h>
#include <FS.h>

class Sprite;

class GameImage {
public:
	GameImage();
	explicit GameImage(Sprite* canvas);
	GameImage(Sprite* canvas, const Color* buffer);
	GameImage(const GameImage& other);
	GameImage& operator=(const GameImage& other);
	virtual ~GameImage();

	operator bool() const;
	Color* getBuffer() const;

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;

private:
	int16_t x;
	int16_t y;
	Color* buffer = nullptr;

	Sprite* canvas = nullptr;
};


#endif //BYTEBOI_GAMEIMAGE_H
