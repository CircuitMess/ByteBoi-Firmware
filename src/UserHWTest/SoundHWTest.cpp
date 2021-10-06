#include <Pins.hpp>
#include <Audio/Piezo.h>
#include <Audio/Notes.hpp>
#include "SoundHWTest.h"

SoundHWTest::SoundHWTest(UserHWTest* userHwTest) : HWTestPart(userHwTest){
}

SoundHWTest::~SoundHWTest(){

}

void SoundHWTest::draw(){
	userHwTest->getScreen().getSprite()->setTextFont(1);
	userHwTest->getScreen().getSprite()->setTextSize(1);
	userHwTest->getScreen().getSprite()->setTextColor(TFT_WHITE);
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("You should hear") / 2, 40);
	userHwTest->getScreen().getSprite()->print("You should hear");
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("a melody playing.") / 2, 50);
	userHwTest->getScreen().getSprite()->print("a melody playing.");
	userHwTest->getScreen().getSprite()->setCursor(userHwTest->getScreen().getSprite()->width() / 2 - userHwTest->getScreen().getSprite()->textWidth("Press A to continue") / 2, 110);
	userHwTest->getScreen().getSprite()->print("Press A to continue.");
}

void SoundHWTest::start(){
	Input::getInstance()->addListener(this);
	userHwTest->draw();
	userHwTest->getScreen().commit();
}

void SoundHWTest::stop(){
	Input::getInstance()->removeListener(this);
}

void SoundHWTest::buttonPressed(uint id){
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

void SoundHWTest::loop(uint micros){
	if(millis() - previousTime >= 500){
		previousTime = millis();
		Piezo.tone(notesArray[index % 10], 100);
		index++;
	}

}
