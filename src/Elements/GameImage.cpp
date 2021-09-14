#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ByteBoi.h>

GameImage::GameImage(Sprite* canvas, size_t gameIndex) : canvas(canvas), index(gameIndex){

}

GameImage::~GameImage(){
	releaseImage();
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

void GameImage::loadImage(){
	if(appIconBuffer == nullptr){
		appIconBuffer = static_cast<Color*>(ps_malloc(64 * 64 * 2));
	}else return;

	if(appIconBuffer == nullptr){
		Serial.println("MainMenuApp picture unpack error");
		return;
	}
	fs::File icon = ByteBoi.getIcon(index);
	icon.read(reinterpret_cast<uint8_t*>(appIconBuffer), 64 * 64 * 2);
	icon.close();
}

void GameImage::releaseImage(){
	if(appIconBuffer != nullptr){
		free(appIconBuffer);
		appIconBuffer = nullptr;
	}
}
