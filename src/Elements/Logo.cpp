#include "Logo.h"

#include <Display/Sprite.h>
#include <Display/LovyanGFX_setup.h>
#include <SPIFFS.h>
#include "../Bitmaps/logo.hpp"

Logo::Logo(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2), currentY(startY){
	logoBuffer = static_cast<Color*>(malloc(93 * 26 * 2));
	if(logoBuffer == nullptr){
		Serial.printf("Logo picture unpack error\n");
		return;
	}

	fs::File iconFile = SPIFFS.open("/launcher/ByteBoiLogo.raw");

	iconFile.read(reinterpret_cast<uint8_t*>(logoBuffer), 93 * 26 * 2);
	iconFile.close();
}

Logo::~Logo(){
	free(logoBuffer);
}

void Logo::loop(uint micros){
	f += (float) micros / 1000000.0f;
	if(f > 2 * M_PI) f -= 2 * M_PI;

	currentY = y + pow(cos(f * 1000.0f / speed), 2) * -amplitude;
}

void Logo::draw(){
	canvas->drawIcon(logoBuffer,x, currentY, width, height,1,TFT_BLACK);
}


