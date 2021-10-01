#include "Logo.h"

#include <Display/Sprite.h>
#include <Display/LovyanGFX_setup.h>
#include <SPIFFS.h>
#include <Loop/LoopManager.h>

Logo::Logo(Sprite* canvas) : canvas(canvas), startX((canvas->width() - width) / 2), centerDiff((canvas->height() - height) / 2 - startY){
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
	if(state == OFF){
		LoopManager::removeListener(this);
		return;
	}

	if(state == EXIT){
		ampf -= ((float) micros / 1000000.0f);

		if(ampf <= 0 || f == 0){
			reset();
			state = OFF;
			LoopManager::removeListener(this);
			return;
		}

		float diff = 1000.0f * ((float) micros / 1000000.0f) / speed;
		if(f >= M_PI) f += diff;
		else f -= diff;

		if(f < 0 || f >= 2.0f * M_PI){
			f = 0;
		}
	}else{
		f += 1000.0f * ((float) micros / 1000000.0f) / speed;
		if(f >= 2 * M_PI) f -= 2.0f * M_PI;

		ampf += 1000.0f * ((float) micros / 1000000.0f) / ampSpeed;
		if(ampf >= 2 * M_PI) ampf -= 2.0f * M_PI;
	}

	amp = maxAmp * std::abs(sin(ampf));

	diffX = sin(f) * amp;
}

void Logo::start(){
	state = ON;
	LoopManager::addListener(this);
}

void Logo::stop(){
	state = EXIT;
}

void Logo::pause(){
	LoopManager::removeListener(this);
}

void Logo::reset(){
	diffX = 0;
	ampf = 0;
	f = 0;
}

void Logo::draw(){
	canvas->drawIcon(logoBuffer, startX + diffX, ((float) startY + (float) centerDiff * center), width, height,1,TFT_BLACK);
}

void Logo::setCentered(float f){
	center = f;
}
