#include <Pins.hpp>
#include "DescriptionModal.h"
#include "profont.h"
#include "adobex11font.h"
#include <ByteBoi.h>
#include <lgfx/v1/lgfx_fonts.hpp>

using namespace std;

static const lgfx::U8g2font adobex11font(u8g2_font_helvB08_tr);
DescriptionModal* DescriptionModal::instance = nullptr;

DescriptionModal::DescriptionModal(Context& context, GameInfo* gameInfo) : Modal(context, 80, 80),gameInfo(gameInfo),
																		   layout(new LinearLayout(&screen, VERTICAL)){
	instance = this;
	buildUI();
}

DescriptionModal::~DescriptionModal(){

}

void DescriptionModal::draw(){
	screen.getSprite()->clear(C_HEX(0x53565c));
	screen.getSprite()->drawRect(screen.getTotalX(), screen.getTotalY(), 80, 80, C_HEX(0x9197a1));
	screen.getSprite()->setTextColor(TFT_WHITE);
	screen.getSprite()->setCursor(40 - screen.getSprite()->textWidth(gameInfo->name.c_str()) / 2, 3);
	screen.getSprite()->setFont(&adobex11font);
	screen.getSprite()->print(gameInfo->name.c_str());
	screen.getSprite()->setTextFont(0);
	screen.getSprite()->setTextSize(1);
	screen.getSprite()->setCursor(1, 18);
	screen.getSprite()->print(gameInfo->author.c_str());
	screen.getSprite()->setCursor(1, 30);
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

void DescriptionModal::buildUI(){
	layout->setWHType(CHILDREN, CHILDREN);
	layout->reflow();
	screen.addChild(layout);
	screen.repos();
}

void DescriptionModal::splitPrintSentence(std::string sentence){
	uint8_t y_lenght = 30;
	std::string word;
	bool firstWord = true;
	for(auto x : sentence){
		if(x == ' ' || x == ('!') || x == ('.')){
			Serial.println(screen.getSprite()->getCursorX());
			//Serial.printf("width : %d\n", screen.getSprite()->textWidth(word.c_str()));
			if(((screen.getSprite()->getCursorX()) + screen.getSprite()->textWidth(word.c_str())) > 80){
				y_lenght += 10;
				screen.getSprite()->setCursor(1, y_lenght);
			}else{
				if(firstWord){
					screen.getSprite()->setCursor(1, y_lenght);
					firstWord = false;
				}else{
					screen.getSprite()->setCursor(screen.getSprite()->getCursorX() + 3, y_lenght);
				}

			}
			screen.getSprite()->print(word.c_str());
			Serial.println(word.c_str());
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