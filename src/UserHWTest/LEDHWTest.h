#ifndef BYTEBOI_FIRMWARE_LEDHWTEST_H
#define BYTEBOI_FIRMWARE_LEDHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "HWTestPart.h"
#include <ByteBoiLED.h>

class LEDHWTest : public HWTestPart, public LoopListener {
public:
	LEDHWTest(UserHWTest* userHwTest);

	virtual ~LEDHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void loop(uint micros) override;

	void buttonPressed(uint id) override;

private:
	uint32_t previousTime = 0;
	ByteBoiLED rgbLED;

	const LEDColor ledArray[3] = {LEDColor::RED, LEDColor::GREEN, LEDColor::BLUE};
	uint8_t index = 0;

	uint8_t doneCounter = 0;
	bool soundIsDone = false;
};

#endif //BYTEBOI_FIRMWARE_LEDHWTEST_H
