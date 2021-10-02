#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>

GameImage::GameImage(){}

GameImage::GameImage(Sprite* canvas) : canvas(canvas){
	buffer = static_cast<Color*>(malloc(64 * 64 * 2));
}

GameImage::GameImage(Sprite* canvas, const Color* buffer) : GameImage(canvas){
	if(buffer == nullptr) return;
	memcpy(this->buffer, buffer, 64 * 64 * 2);
}

GameImage::GameImage(const GameImage& other) : GameImage(other.canvas, other.buffer){

}

GameImage& GameImage::operator=(const GameImage& other){
	if(&other == this) return *this;

	this->canvas = other.canvas;
	free(buffer);
	buffer = static_cast<Color*>(malloc(64 * 64 * 2));
	if(other.buffer != nullptr){
		memcpy(buffer, other.buffer, 64 * 64 * 2);
	}
	return *this;
}

GameImage::~GameImage(){
	free(buffer);
	buffer = nullptr;
}

GameImage::operator bool() const{
	return canvas != nullptr && buffer != nullptr;
}

void GameImage::draw() const {
	if(!*this) return;
	canvas->drawIcon(buffer, x, y, 64, 64, 1, TFT_TRANSPARENT);
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

Color* GameImage::getBuffer() const{
	return buffer;
}

void GameImage::setCanvas(Sprite* canvas){
	GameImage::canvas = canvas;
}
