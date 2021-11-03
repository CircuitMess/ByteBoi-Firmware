#include <Loop/LoopManager.h>
#include <Pins.hpp>
#include "InputHWTest.h"
#include "../Fonts/adobex11font.h"
#include <lgfx/v1/lgfx_fonts.hpp>

static const lgfx::U8g2font adobex11font(u8g2_font_helvB08_tr);

InputHWTest::InputHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest), screenLayout(new LinearLayout(&userHwTest->getScreen(), HORIZONTAL)), topLayout(new LinearLayout(screenLayout, VERTICAL)),
												   midLayout(new LinearLayout(screenLayout, HORIZONTAL)), bottomLayout(new LinearLayout(screenLayout, VERTICAL)), btnALayout(new LinearLayout(screenLayout, VERTICAL)),
												   btnBLayout(new LinearLayout(screenLayout, VERTICAL)), btnCLayout(new LinearLayout(screenLayout, VERTICAL)){

	leftBtnTest.push_back(new BtnTestElement(topLayout, false));
	for(int i = 0; i < 2; i++){
		midBtnTest.push_back(new BtnTestElement(midLayout, false));
	}
	rightBtnTest.push_back(new BtnTestElement(bottomLayout, false));
	btnATest.push_back(new BtnTestElement(btnALayout, false));
	btnBTest.push_back(new BtnTestElement(btnBLayout, false));
	btnCTest.push_back(new BtnTestElement(btnCLayout, true, "select"));
	buildUI();
}

InputHWTest::~InputHWTest(){

}

void InputHWTest::draw(){
	screenLayout->draw();
	if(!inputIsDone){
		userHwTest->getScreen().getSprite()->setTextFont(1);
		userHwTest->getScreen().getSprite()->setTextSize(1);
		userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("Press all buttons") / 2, 100);
		userHwTest->getScreen().getSprite()->print("Press all buttons");
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("to test them.") / 2, 110);
		userHwTest->getScreen().getSprite()->print("to test them.");
	}else{
		userHwTest->getScreen().getSprite()->setTextFont(1);
		userHwTest->getScreen().getSprite()->setTextSize(1);
		userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("All OK.") / 2, 100);
		userHwTest->getScreen().getSprite()->print("All OK.");
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("Press A to continue.") / 2, 110);
		userHwTest->getScreen().getSprite()->print("Press A to continue.");

	}
}

void InputHWTest::start(){
	Input::getInstance()->addListener(this);
	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void InputHWTest::stop(){
	Input::getInstance()->removeListener(this);
}

void InputHWTest::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->addChild(topLayout);
	screenLayout->addChild(midLayout);
	screenLayout->addChild(bottomLayout);
	screenLayout->addChild(btnALayout);
	screenLayout->addChild(btnBLayout);
	screenLayout->addChild(btnCLayout);

	topLayout->setWHType(PARENT, PARENT);
	topLayout->addChild(leftBtnTest[0]);
	topLayout->reflow();

	midLayout->setWHType(PARENT, PARENT);
	midLayout->setGutter(2);
	for(int i = 0; i < 2; i++){
		midLayout->addChild(midBtnTest[i]);
	}
	midLayout->reflow();

	bottomLayout->setWHType(PARENT, PARENT);
	bottomLayout->setGutter(2);
	bottomLayout->addChild(rightBtnTest[0]);
	bottomLayout->reflow();

	btnALayout->setWHType(PARENT, PARENT);
	btnALayout->addChild(btnATest[0]);
	btnALayout->reflow();

	btnBLayout->setWHType(PARENT, PARENT);
	btnBLayout->addChild(btnBTest[0]);
	btnBLayout->reflow();

	btnCLayout->setWHType(PARENT, PARENT);
	btnCLayout->addChild(btnCTest[0]);
	btnCLayout->reflow();

	userHwTest->getScreen().addChild(screenLayout);
	userHwTest->getScreen().repos();

	topLayout->setPos(33, 27);
	midLayout->setPos(15, 50);
	bottomLayout->setPos(33, 73);
	btnALayout->setPos(120, 60);
	btnBLayout->setPos(134, 40);
	btnCLayout->setPos(115, 2);
}

void InputHWTest::buttonPressed(uint id){
	switch(id){
		case BTN_UP:
			if(leftBtnTest[0]->isBtnPressed()) return;
			leftBtnTest[0]->btnPress();
			if(leftBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_DOWN:
			if(rightBtnTest[0]->isBtnPressed()) return;
			rightBtnTest[0]->btnPress();
			if(rightBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_LEFT:
			if(midBtnTest[0]->isBtnPressed()) return;
			midBtnTest[0]->btnPress();
			if(midBtnTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_RIGHT:
			if(midBtnTest[1]->isBtnPressed()) return;
			midBtnTest[1]->btnPress();
			if(midBtnTest[1]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_A:
			if(doneCounter >= 7){
				userHwTest->currentTestDone();
				return;
			}
			if(btnATest[0]->isBtnPressed()) return;
			btnATest[0]->btnPress();
			if(btnATest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				doneCounter++;
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_B:
			if(btnBTest[0]->isBtnPressed()) return;
			btnBTest[0]->btnPress();
			if(btnBTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
		case BTN_C:
			if(btnCTest[0]->isBtnPressed()) return;
			btnCTest[0]->btnPress();
			if(btnCTest[0]->isBtnPressed()){
				doneCounter++;
			}
			if(doneCounter >= 7){
				inputIsDone = true;
				userHwTest->draw();
				userHwTest->getScreen().commit();
				break;
			}
			userHwTest->draw();
			userHwTest->getScreen().commit();
			break;
	}
}

