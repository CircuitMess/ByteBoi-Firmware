#ifndef BYTEBOI_FIRMWARE_SOUNDHWTEST_H
#define BYTEBOI_FIRMWARE_SOUNDHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "HWTestPart.h"
#include <Audio/Notes.hpp>

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
	const uint16_t notesArray[10] = {NOTE_G3,NOTE_A3,NOTE_C3,NOTE_D3,NOTE_D3,NOTE_C3,NOTE_A3,NOTE_G3,NOTE_F3,NOTE_F3};
	uint8_t index = 0;
	uint32_t previousTime = 0;
	uint8_t volume;

};
#endif //BYTEBOI_FIRMWARE_SOUNDHWTEST_H
