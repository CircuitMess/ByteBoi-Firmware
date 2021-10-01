#include <Pins.hpp>
#include "DescriptionModal.h"
#include "Fonts/adobex11font.h"
#include <ByteBoi.h>
#include <lgfx/v1/lgfx_fonts.hpp>

using namespace std;

static const lgfx::U8g2font adobex11font(u8g2_font_helvB08_tr);

DescriptionModal::DescriptionModal(Context& context, GameInfo* gameInfo) : Modal(context, 100, 80),gameInfo(gameInfo){
}

DescriptionModal::~DescriptionModal(){

}

void DescriptionModal::draw(){
	screen.getSprite()->clear(TFT_TRANSPARENT);
	screen.getSprite()->fillRoundRect(screen.getTotalX(), screen.getTotalY(), screen.getSprite()->width(), screen.getSprite()->height(), 3, C_HEX(0x004194));
	screen.getSprite()->fillRoundRect(screen.getTotalX() + 2, screen.getTotalY() + 2, screen.getSprite()->width() - 4, screen.getSprite()->height() - 4, 3, C_HEX(0x0041ff));
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setCursor(screen.getSprite()->width()/2 - screen.getSprite()->textWidth(gameInfo->name.c_str())/2, 3);
	screen.getSprite()->setFont(&adobex11font);
	screen.getSprite()->print(gameInfo->name.c_str());
	screen.getSprite()->setTextFont(0);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setCursor(4, 17);
	screen.getSprite()->print(gameInfo->author.c_str());
	screen.getSprite()->setCursor(4, 32);
	splitPrintSentence(gameInfo->description.c_str());
	screen.draw();
}

void DescriptionModal::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void DescriptionModal::stop(){
	Input::getInstance()->removeListener(this);
}

void DescriptionModal::splitPrintSentence(std::string sentence){
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
			if(((screen.getSprite()->getCursorX()) + screen.getSprite()->textWidth(word.c_str())) > screen.getSprite()->width()-4){
				y_lenght += 10;
				screen.getSprite()->setCursor(4, y_lenght);
			}else{
				if(firstWord){
					screen.getSprite()->setCursor(4, y_lenght);
					firstWord = false;
				}else{
					screen.getSprite()->setCursor(screen.getSprite()->getCursorX() + 3, y_lenght);
				}

			}
			screen.getSprite()->print(word.c_str());
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