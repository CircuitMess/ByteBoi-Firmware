#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>
#include <FS.h>
#include <SPIFFS.h>
#include "../GameManagement/GameManager.h"

GameImage::GameImage(Sprite* canvas, uint8_t* icon) : canvas(canvas), appIconBuffer((Color*)icon){

}

GameImage::~GameImage(){
	free(appIconBuffer);
	appIconBuffer = nullptr;
}


void GameImage::draw() const {
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
