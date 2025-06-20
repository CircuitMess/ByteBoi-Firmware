#include "JigHWTest.h"
#include "SDSizes.hpp"
#include "SPIFFSChecksums.hpp"
#include <ByteBoi.h>
#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include <SPIFFS.h>
#include <Audio/Notes.hpp>
#include <Loop/LoopManager.h>

JigHWTest *JigHWTest::test = nullptr;

JigHWTest::JigHWTest(Display &_display) : canvas(_display.getBaseSprite()), display(&_display){

	test = this;

	tests.push_back({JigHWTest::psram, "PSRAM"});
	tests.push_back({JigHWTest::BatteryCheck, "Battery"});
	tests.push_back({JigHWTest::SDtest, "SD"});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS"});
	tests.push_back({JigHWTest::buttons, "Buttons"});
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

		canvas->setTextColor(result ? TFT_SILVER : TFT_ORANGE);
		canvas->printf("%s\n", result ? "PASS" : "FAIL");
		display->commit();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)) break;
	}

	if(pass){
		Serial.println("TEST:passall");
	}else{
		Serial.printf("TEST:fail:%s\n", currentTest);
	}

	canvas->print("\n\n");
	canvas->setTextColor(pass ? TFT_BLUE : TFT_ORANGE);
	canvas->printCenter(pass ? "All OK!" : "FAIL!");
	display->commit();

	Settings.get().volume = 255;
	Playback.updateGain();

	bool painted = false;
	const auto color = pass ? TFT_GREEN : TFT_RED;
	auto flashTime = 0;
	bool tone = false;
	const auto note = NOTE_C6 + ((rand() * 20) % 400) - 200;
	for(;;){
		if(millis() - flashTime >= 500){
			for(int x = 0; x < canvas->width(); x++){
				for(int y = 0; y <  canvas->height(); y++){
					if(!painted && canvas->readPixel(x, y) == TFT_BLACK){
						canvas->drawPixel(x, y, color);
					}else if(painted && canvas->readPixel(x, y) == color){
						canvas->drawPixel(x, y, TFT_BLACK);
					}
				}
			}

			flashTime = millis();
			painted = !painted;
			display->commit();
		}

		LoopManager::loop();
		auto press = false;
		for(int i = 0; i < 6; i++){
			if(ByteBoi.getInput()->getButtonState(Pins.get((Pin) ((int) Pin::BtnUp + i)))){
				press = true;
				break;
			}
		}

		if(press && !tone){
			Playback.tone(note, 0);
			tone = true;
		}else if(!press && tone){
			Playback.noTone();
			tone = false;
		}
	}
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
	if(!Battery.isCharging()){
		test->log("charging", false);
		return false;
	}

	const uint32_t voltage = Battery.getVoltage(true);
	if(voltage < 4050){
		test->log("voltage", voltage);
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
	ByteBoi.checkSD();

	if(!ByteBoi.sdDetected()){
		LED.setRGB(LEDColor::RED);
		test->log("inserted", false);
		return false;
	}

	for(const auto& f : SDSizes){
		fs::File file = SD.open(f.name, "r");
		if(!file){
			LED.setRGB(LEDColor::GREEN);
			test->log("missing", f.name);

			file.close();
			SD.end();
			return false;
		}

		uint32_t size = file.size();
		if(size != f.size){
			LED.setRGB(LEDColor::YELLOW);
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

bool JigHWTest::buttons(){
	auto input = ByteBoi.getInput();

	const auto cX = test->canvas->getCursorX();
	const auto cY = test->canvas->getCursorY();
	bool flash = false;
	uint32_t flashTime = 0;

	static constexpr auto ButtonCount = 6;
	std::vector<bool> pressed(ButtonCount, false);
	std::vector<bool> released(ButtonCount, false);
	uint8_t pressCount = 0;
	uint8_t releaseCount = 0;
	for(;;){
		LoopManager::loop();

		for(int i = 0; i < ButtonCount; i++){
			if(input->getButtonState(Pins.get((Pin) ((int) Pin::BtnUp + i))) && !pressed[i]){
				pressed[i] = true;
				pressCount++;
			}else if(!input->getButtonState(Pins.get((Pin) ((int) Pin::BtnUp + i))) && pressed[i] && !released[i]){
				released[i] = true;
				releaseCount++;
			}
		}

		if(pressCount == ButtonCount && releaseCount == ButtonCount) break;

		if(millis() - flashTime > 500){
			if(flash){
				test->canvas->fillRect(cX, cY-4, 120, 8, TFT_BLACK);
			}else{
				test->canvas->setCursor(cX, cY);
				test->canvas->setTextColor(TFT_GOLD);
				test->canvas->printf("- PRESS BUTTONS -");
			}

			ByteBoi.getDisplay()->commit();
			flash = !flash;
			flashTime = millis();
		}

		test->canvas->fillRect(cX, cY+6, 120, 8, TFT_BLACK);
		test->canvas->setTextColor(TFT_LIGHTGRAY);
		test->canvas->setCursor(cX-3, cY+10);
		test->canvas->printf("[");
		for(int i = 0; i < ButtonCount; i++){
			if(input->getButtonState(i)){
				test->canvas->setTextColor(TFT_GOLD);
			}else if(pressed[i] && released[i]){
				test->canvas->setTextColor(TFT_BLUE);
			}else{
				test->canvas->setTextColor(TFT_DARKGRAY);
			}
			test->canvas->printf("-");
		}
		test->canvas->setTextColor(TFT_LIGHTGRAY);
		test->canvas->printf("]");

		ByteBoi.getDisplay()->commit();
	}

	test->canvas->fillRect(cX-3, cY-4, 120, 20, TFT_BLACK);
	test->canvas->setCursor(cX, cY);
	return pressCount == ButtonCount && releaseCount == ButtonCount;
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

