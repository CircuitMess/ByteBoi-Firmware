#include "GameTitle.h"

#include <Display/Sprite.h>
#include <Loop/LoopManager.h>

GameTitle::GameTitle(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2){
	currentY = canvas->height() + overHide;
	canvas->setTextSize(2);
	canvas->setFont(&fonts::Font0);
	height = canvas->fontHeight();
	y = currentY = canvas->height() - height + 7;
}

void GameTitle::change(const String& newText){
	bool wasChanging = changeTo != nullptr;

	state = DOWN;
	changeTo = newText;

	if(wasChanging) return;
	LoopManager::addListener(this);
}

void GameTitle::loop(uint micros){
	currentY += speed * (micros / 1000000.0f) * (state == DOWN ? 0.75f : -0.75f);

	if(state == DOWN && currentY >= canvas->height() - overHide + height){
		currentY = canvas->height() - overHide + height; // correction for first invoke, when micros is exceptionally yuge

		text = changeTo;
		state = UP;
	}else if(state == UP && currentY <= y){
		LoopManager::removeListener(this);
		changeTo = nullptr;
		currentY = y;
	}
}

void GameTitle::draw(){
	if(text == nullptr) return;
	canvas->setTextSize(2);
	canvas->setTextFont(0);
	canvas->setTextWrap(false, false);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::middle_center);
	uint32_t textWidth = canvas->textWidth(text);
	canvas->setCursor((canvas->width() - textWidth) / 2, currentY);
	canvas->print(text);
//	canvas->drawString(text, canvas->width()/2, currentY);
}
