#include <Input/Input.h>
#include <Pins.hpp>
#include "DescriptionModal.h"
#include <ByteBoi.h>

DescriptionModal* DescriptionModal::instance = nullptr;

DescriptionModal::DescriptionModal(Context& context, const char* title,const char* author, const char* description) : Modal(context, 80, 80),
																		 layout(new LinearLayout(&screen, VERTICAL)),
																		 descriptionText(new DescriptionModalItem(layout,title,author,description))
																		 {
	instance = this;
	buildUI();
}

DescriptionModal::~DescriptionModal(){

}

void DescriptionModal::draw(){
	screen.getSprite()->clear(C_HEX(0x53565c));
	screen.getSprite()->drawRect(screen.getTotalX(),screen.getTotalY(),80,80,C_HEX(0x9197a1));
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

void DescriptionModal::init(){
	Context::init();

}

void DescriptionModal::deinit(){
	Context::deinit();
}

void DescriptionModal::buildUI(){
	layout->setWHType(CHILDREN, CHILDREN);
	layout->addChild(descriptionText);
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