#include "JigHWTest.h"
#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include "HWTestChecksums.hpp"
#include <SPIFFS.h>
#include <ByteBoi.h>
#include "../GameManagement/GameManager.h"
#include "../GameInfo.hpp"

JigHWTest *JigHWTest::test = nullptr;
static const char* preloadedGames[] = {"Bonk", "Invaderz", "SpaceRocks", "Snake", "Wheelson RC", "CastleJump"};
JigHWTest::JigHWTest(Display &_display) : canvas(_display.getBaseSprite()), display(&_display){

	test = this;

	tests.push_back({JigHWTest::psram, "PSRAM"});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS"});
	tests.push_back({JigHWTest::BatteryCheck,"Battery"});
	tests.push_back({JigHWTest::SDcheck,"SD"});

	Wire.begin(I2C_SDA, I2C_SCL);
}

void JigHWTest::start(){
	Serial.println();
	uint64_t mac = ESP.getEfuseMac();
	uint32_t upper = mac >> 32;
	uint32_t lower = mac & 0xffffffff;
	Serial.printf("TEST:begin:%x%x\n", upper, lower);

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setCursor(0, 0);

	canvas->printCenter("ByteBoi Hardware Test");
	canvas->setCursor(canvas->width() / 2, 10);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test &test : tests){

		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		bool result = test.test();

		canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
		canvas->printf("%s\n", result ? "PASSED" : "FAILED");
		display->commit();

		if(!(pass &= result)) break;
	}

	for(;;);
}

bool JigHWTest::psram(){
	if(!(psramFound() && psramInit())) return false;

	uint32_t free = ESP.getFreePsram();

	if(free != 4153272){
		test->log("free", free);
		return false;
	}

	return true;
}

bool JigHWTest::SPIFFSTest(){

	File file;

	/* SPIFFS begin test */
	if(!SPIFFS.begin()){
		test->log("Begin","Failed");
		return false;
	}

	for(const auto & check : SPIFFSChecksums){

		file = SPIFFS.open(check.name);

		if(!file){
			test->log("File open error", check.name);
			return false;
		}

		char buff;
		uint32_t fileBytesSum = 0;

		while(file.readBytes(&buff,1)){

			fileBytesSum+=buff;
		}

		if(fileBytesSum != check.sum){
			char logBuffer[100];
			sprintf(logBuffer, "%s - expected %d, got %d", check.name.c_str(), check.sum, fileBytesSum);
			test->log("Checksum mismatch", logBuffer);
			file.close();
			return false;
		}

		file.close();
	}

	return true;
}

bool JigHWTest::SDcheck(){
	Games.detectSD();
	if(!Games.SDinserted()){
		test->log("SD inserted", "false");
		return false;
	}
	Games.scanGames();

	for(auto name : preloadedGames){
		auto it = find_if(Games.getGames().begin(), Games.getGames().end(), [name](const GameInfo* obj) {return obj->name == name;});
		if(it == Games.getGames().end())
		{
			test->log("Missing game", name);
			return false;
		}
	}
	return true;
}

bool JigHWTest::BatteryCheck(){
	if(!(ByteBoi.getExpander()->getPortState() & (1 << CHARGE_DETECT_PIN))){
		test->log("Battery not charging", "false");
		return false;
	}
	return true;
}


void JigHWTest::log(const char *property, char *value){
	Serial.printf("\n%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, float value){
	Serial.printf("\n%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, uint32_t value){
	Serial.printf("\n%s:%s:%u\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, double value){
	Serial.printf("\n%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, bool value){
	Serial.printf("\n%s:%s:%d\n", currentTest, property, value ? 1 : 0);
}

void JigHWTest::log(const char* property, String value){
	Serial.printf("\n%s:%s:%s\n", currentTest, property, value.c_str());
}

