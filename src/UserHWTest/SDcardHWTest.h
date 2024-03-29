#ifndef BYTEBOI_FIRMWARE_SDCARDHWTEST_H
#define BYTEBOI_FIRMWARE_SDCARDHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "HWTestPart.h"

class SDcardHWTest : public HWTestPart, public LoopListener  {
public:
	SDcardHWTest(UserHWTest* userHwTest);

	virtual ~SDcardHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void buttonPressed(uint id) override;
	void loop(uint micros) override;
private:
	uint8_t doneCounter = 0;
	bool soundIsDone = false;
	std::vector<String> item;
	bool inserted = false;
};

#endif //BYTEBOI_FIRMWARE_SDCARDHWTEST_H
