#include "JigHWTest.h"
#include "SDSizes.hpp"
#include "SPIFFSChecksums.hpp"
#include <ByteBoi.h>
#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include <SPIFFS.h>

JigHWTest *JigHWTest::test = nullptr;

JigHWTest::JigHWTest(Display &_display) : canvas(_display.getBaseSprite()), display(&_display){

	test = this;

	tests.push_back({JigHWTest::psram, "PSRAM", [](){ LED.setRGB(LEDColor::CYAN); }});
	tests.push_back({JigHWTest::BatteryCheck, "Battery", [](){ LED.setRGB(LEDColor::BLUE); }});
	tests.push_back({JigHWTest::SDtest, "SD", nullptr});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS", [](){ LED.setRGB(LEDColor::MAGENTA); }});
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

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("ByteBoi Hardware Test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test &test : tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
		canvas->printf("%s\n", result ? "PASSED" : "FAILED");
		display->commit();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

	Settings.get().volume = 255;
	Playback.updateGain();

	Playback.play(new Sample(SPIFFS.open("/launcher/intro/intro.aac")));

	for(int i = 0; i < 3; i++){
		LED.setRGB(LEDColor::RED);
		delay(500);

		LED.setRGB(LEDColor::GREEN);
		delay(500);

		LED.setRGB(LEDColor::BLUE);
		delay(500);
	}

	LED.setRGB(LEDColor::WHITE);

	for(;;);
}

bool JigHWTest::psram(){
	if(!(psramFound() && psramInit())) return false;

	uint32_t free = ESP.getFreePsram();

	if(free != 4155304){
		test->log("free", free);
		return false;
	}

	return true;
}

bool JigHWTest::BatteryCheck(){
	if(!Battery.chargePinDetected()){
		test->log("charging", false);
		return false;
	}
	return true;
}

uint32_t JigHWTest::calcChecksum(fs::File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}

bool JigHWTest::SDtest(){
	if(ByteBoi.getExpander()->getPortState() & (1 << SD_DETECT_PIN)){
		test->log("inserted", false);
		return false;
	}

	if(!SD.begin(SD_CS, SPI)){
		test->log("begin", false);

		SD.end();
		return false;
	}

	for(const auto& f : SDSizes){
		fs::File file = SD.open(f.name, "r");
		if(!file){
			test->log("missing", f.name);

			file.close();
			SD.end();
			return false;
		}

		uint32_t size = file.size();
		if(size != f.size){
			test->log("file", f.name);
			test->log("expected", f.size);
			test->log("got", size);

			file.close();
			SD.end();
			return false;
		}

		file.close();
	}

	SD.end();
	return true;
}

bool JigHWTest::SPIFFSTest(){
	for(const auto& f : SPIFFSChecksums){
		fs::File file = SPIFFS.open(f.name, "r");
		if(!file){
			test->log("missing", f.name);

			file.close();
			return false;
		}

		uint32_t sum = calcChecksum(file);
		if(sum != f.sum){
			test->log("expected", f.sum);
			test->log("got", sum);

			file.close();
			return false;
		}
	}

	return true;
}


void JigHWTest::log(const char *property, char *value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, uint32_t value){
	Serial.printf("%s:%s:%u\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char *property, bool value){
	Serial.printf("%s:%s:%d\n", currentTest, property, value ? 1 : 0);
}

void JigHWTest::log(const char* property, String value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

