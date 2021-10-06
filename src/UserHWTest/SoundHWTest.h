#ifndef BYTEBOI_FIRMWARE_SOUNDHWTEST_H
#define BYTEBOI_FIRMWARE_SOUNDHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "HWTestPart.h"

class SoundHWTest : public HWTestPart, public LoopListener{
public:
	SoundHWTest(UserHWTest* userHwTest);

	virtual ~SoundHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void loop(uint micros) override;

	void buttonPressed(uint id) override;
private:
	uint8_t doneCounter = 0;
	bool soundIsDone = false;
	uint32_t previousTime = 0;

};
#endif //BYTEBOI_FIRMWARE_SOUNDHWTEST_H
