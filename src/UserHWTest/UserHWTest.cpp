#include <FS.h>
#include <FS/CompressedFile.h>
#include <SPIFFS.h>
#include "UserHWTest.h"
#include "InputHWTest.h"
#include "SoundHWTest.h"
#include "LEDHWTest.h"
#include "SDcardHWTest.h"


UserHWTest::UserHWTest(Display& display) : Context(display){
	hwTestPart = new InputHWTest(this);	Context::pack();

}

UserHWTest::~UserHWTest(){
	delete hwTestPart;

}

void UserHWTest::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 120, 1);
	hwTestPart->draw();
}

void UserHWTest::start(){
	hwTestPart->start();
}

void UserHWTest::stop(){
	hwTestPart->stop();
}

void UserHWTest::currentTestDone(){
	Serial.println("CurrentTestDone");
	Serial.printf("TestCounter : %d\n",testCounter);
	testCounter++;
	Serial.printf("TestCounter1 : %d\n",testCounter);
	if(testCounter > 3){
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}else{
		hwTestPart->stop();
		delete hwTestPart;
		hwTestPart = nullptr;
	}
	nextTest();
}

void UserHWTest::setDoneCallback(void (* doneCallback)(UserHWTest*)){
	UserHWTest::doneCallback = doneCallback;
}

void UserHWTest::init(){
	backgroundBuffer = static_cast<Color*>(ps_malloc(160 * 120 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("HWTest background unpack error");
		return;
	}

	fs::File bgFile = CompressedFile::open(SPIFFS.open("/launcher/SettingsBg.raw.hs"), 9, 8);
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 120 * 2);
	bgFile.close();
}

void UserHWTest::deinit(){
	free(backgroundBuffer);

}

void UserHWTest::nextTest(){
	if(testCounter == 1){
		hwTestPart = new SoundHWTest(this);
	}else if(testCounter == 2){
		hwTestPart = new LEDHWTest(this);
	}else if(testCounter == 3){
		hwTestPart = new SDcardHWTest(this);
	}else{
		if(doneCallback){
			doneCallback(this);
		}else{
			this->pop();
		}
		return;
	}
	hwTestPart->start();
}
