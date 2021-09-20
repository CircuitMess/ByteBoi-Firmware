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
	Input::getInstance()->setBtnPressCallback(BTN_B, [](){
		if(instance == nullptr) return;
		instance->pop();
	});
	draw();
	screen.commit();
}

void DescriptionModal::stop(){
	Input::getInstance()->removeBtnPressCallback(BTN_B);
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
