#include <Pins.hpp>
#include "SDcardHWTest.h"
#include "../GameManagement/GameManager.h"
#include "../GameInfo.hpp"
#include <Loop/LoopManager.h>

SDcardHWTest::SDcardHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){

}

SDcardHWTest::~SDcardHWTest(){

}

void SDcardHWTest::draw(){
	if(!inserted){
		userHwTest->getScreen().getSprite()->setTextFont(1);
		userHwTest->getScreen().getSprite()->setTextSize(1);
		userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width()/2 - userHwTest->getScreen().getSprite()->textWidth("SD card not detected")/2, 40);
		userHwTest->getScreen().getSprite()->print("SD card not detected");
	}else if(inserted && Games.getGames().empty()){
		userHwTest->getScreen().getSprite()->setTextFont(1);
		userHwTest->getScreen().getSprite()->setTextSize(1);
		userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width()/2 - userHwTest->getScreen().getSprite()->textWidth("SD card detected")/2, 40);
		userHwTest->getScreen().getSprite()->print("SD card detected");
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width()/2 - userHwTest->getScreen().getSprite()->textWidth("and empty")/2, 50);
		userHwTest->getScreen().getSprite()->print("and empty");
	}else if(inserted && !Games.getGames().empty()){
		userHwTest->getScreen().getSprite()->setTextFont(1);
		userHwTest->getScreen().getSprite()->setTextSize(1);
		userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
		userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("Games on SD card:") / 2,20);
		userHwTest->getScreen().getSprite()->print("Games on SD card:");
		uint8_t i = 0;
		for(const auto& game : Games.getGames()){
			userHwTest->getScreen().getSprite()->setTextFont(1);
			userHwTest->getScreen().getSprite()->setTextSize(1);
			userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
			userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth(game->name.c_str()) / 2,33+(i*10) );
			userHwTest->getScreen().getSprite()->print(game->name.c_str());
			Serial.printf("Games : %s\n", game->name.c_str());
			i++;
		}

	}

}

void SDcardHWTest::start(){
	Input::getInstance()->addListener(this);
	userHwTest->draw();
	userHwTest->getScreen().getSprite()->setTextFont(1);
	userHwTest->getScreen().getSprite()->setTextSize(1);
	userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width()/2 - userHwTest->getScreen().getSprite()->textWidth("Checking SD card")/2, 10);
	userHwTest->getScreen().getSprite()->print("Checking SD card");
	userHwTest->getScreen().commit();
	Games.detectSD();
	userHwTest->draw();
	userHwTest->getScreen().commit();
	LoopManager::addListener(this);
}

void SDcardHWTest::stop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void SDcardHWTest::buttonPressed(uint id){
	switch(id){
		case BTN_A:
		case BTN_B:
		case BTN_C:
		case BTN_UP:
		case BTN_DOWN:
		case BTN_LEFT:
		case BTN_RIGHT:
			userHwTest->currentTestDone();
			break;
	}
}

void SDcardHWTest::loop(uint micros){
	if(inserted != Games.SDinserted()){
		inserted = Games.SDinserted();
		userHwTest->draw();
		userHwTest->getScreen().commit();
	}
}
