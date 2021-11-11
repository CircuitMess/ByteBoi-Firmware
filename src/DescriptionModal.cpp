#include <Pins.hpp>
#include "DescriptionModal.h"
#include "Fonts/adobex11font.h"
#include <ByteBoi.h>
#include <lgfx/v1/lgfx_fonts.hpp>

using namespace std;

static const lgfx::U8g2font adobex11font(u8g2_font_helvB08_tr);

DescriptionModal::DescriptionModal(Context& context, GameInfo* gameInfo) : Modal(context, 124, calculateHeight(gameInfo)), gameInfo(gameInfo){

}

uint16_t DescriptionModal::calculateHeight(GameInfo* gameInfo){
	this->gameInfo = gameInfo;
	Sprite* canvas = new Sprite((Sprite*) nullptr, 124, 1);
	canvas->setTextSize(1);
	draw(canvas);
	uint16_t height = canvas->getCursorY() + 13;
	delete canvas;
	return height;
}

DescriptionModal::~DescriptionModal(){

}

void DescriptionModal::draw(){
	Sprite* canvas = screen.getSprite();
	canvas->clear(TFT_TRANSPARENT);
	canvas->fillRoundRect(screen.getTotalX(), screen.getTotalY(), screen.getSprite()->width(), screen.getSprite()->height(), 3, C_HEX(0x004194));
	canvas->fillRoundRect(screen.getTotalX() + 2, screen.getTotalY() + 2, screen.getSprite()->width() - 4, screen.getSprite()->height() - 4, 3, C_HEX(0x0041ff));
	draw(canvas);
	screen.draw();
}

void DescriptionModal::draw(Sprite* canvas){
	canvas->setTextColor(TFT_WHITE);
	canvas->setCursor(screen.getSprite()->width()/2 - screen.getSprite()->textWidth(gameInfo->name.c_str())/2, 3);
	canvas->setFont(&adobex11font);
	canvas->print(gameInfo->name.c_str());
	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(4, 17);
	canvas->print(gameInfo->author.c_str());
	canvas->setCursor(4, 31);
	splitPrintSentence(canvas, gameInfo->description.c_str());
}

void DescriptionModal::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void DescriptionModal::stop(){
	Input::getInstance()->removeListener(this);
}

void DescriptionModal::splitPrintSentence(Sprite* canvas, std::string sentence){
	uint8_t y_lenght = 32;
	std::string word;
	bool firstWord = true;
	int i = 0;
	for(auto x : sentence){
		i++;
		if(i == sentence.size() || x == ' ' || x == ('!') || x == ('.')){
			if(i == sentence.size()){
				word = word + x;
			}
			if(((canvas->getCursorX()) + canvas->textWidth(word.c_str())) > canvas->width()-4){
				y_lenght += 10;
				canvas->setCursor(4, y_lenght);
			}else{
				if(firstWord){
					canvas->setCursor(4, y_lenght);
					firstWord = false;
				}else{
					canvas->setCursor(canvas->getCursorX() + 3, y_lenght);
				}

			}
			canvas->print(word.c_str());
			word.erase();
		}else{
			word = word + x;
		}

	}
}

void DescriptionModal::buttonPressed(uint id){
	switch(id){
		case BTN_A:
		case BTN_B:
		case BTN_C:
			this->pop();
			break;

	}
}