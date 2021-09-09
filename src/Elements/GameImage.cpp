#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>
#include <FS.h>
#include <SPIFFS.h>

GameImage::GameImage(Sprite* canvas, File icon) : canvas(canvas), icon(icon){
appIconBuffer = static_cast<Color*>(ps_malloc(64 * 64 * 2));
	if(appIconBuffer == nullptr){
		Serial.println("MainMenuApp picture unpack error");
		return;
	}
	icon.read(reinterpret_cast<uint8_t*>(appIconBuffer), 64 * 64 * 2);
	icon.close();
}

GameImage::~GameImage(){
	free(appIconBuffer);
}


void GameImage::draw() const {
//	canvas->pushImage(x, y, 64, 64, (lgfx::rgb565_t*)appIconBuffer, TFT_BLACK);

	canvas->drawIcon(appIconBuffer, x, y, 64, 64, 1, TFT_BLACK);
}

int16_t GameImage::getX() const{
	return x;
}

void GameImage::setX(int16_t x){
	GameImage::x = x;
}

int16_t GameImage::getY() const{
	return y;
}

void GameImage::setY(int16_t y){
	GameImage::y = y;
}


