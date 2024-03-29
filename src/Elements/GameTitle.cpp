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
	bool wasChanging = changeTo != "";

	state = DOWN;
	changeTo = newText;

	if(wasChanging) return;
	LoopManager::addListener(this);
}

String GameTitle::getCurrent(){
	return text;
}

void GameTitle::loop(uint micros){
	currentY += speed * (micros / 1000000.0f) * (state == DOWN ? 0.75f : -0.75f);

	if(state == DOWN && currentY >= canvas->height() - overHide + height){
		currentY = canvas->height() - overHide + height; // correction for first invoke, when micros is exceptionally yuge

		text = changeTo;
		state = UP;
	}else if(state == UP && currentY <= y){
		LoopManager::removeListener(this);
		changeTo = "";
		currentY = y;
	}
}

void GameTitle::draw(){
	if(text == "") return;
	canvas->setTextSize(text == "Loading..." ? 1 : 2);
	canvas->setTextFont(0);
	canvas->setTextWrap(false, false);
	canvas->setTextColor(TFT_WHITE);
	canvas->setTextDatum(textdatum_t::middle_center);
	uint32_t textWidth = canvas->textWidth(text.c_str());
	canvas->setCursor((canvas->width() - textWidth) / 2, currentY);
	canvas->print(text.c_str());
//	canvas->drawString(text, canvas->width()/2, currentY);
}

void GameTitle::setCanvas(Sprite* canvas){
	GameTitle::canvas = canvas;
}
