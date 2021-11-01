#ifndef WHEELSON_FIRMWARE_INPUTHWTEST_H
#define WHEELSON_FIRMWARE_INPUTHWTEST_H

#include <Arduino.h>
#include <Support/Context.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>
#include "BtnTestElement.h"
#include "HWTestPart.h"
#include "UserHWTest.h"

class InputHWTest : public HWTestPart{
public:
	InputHWTest(UserHWTest* userHwTest);

	virtual ~InputHWTest();

	void draw() override;

	void start() override;

	void stop() override;

	void buttonPressed(uint id) override;

private:
	LinearLayout* screenLayout;
	LinearLayout* topLayout;
	LinearLayout* midLayout;
	LinearLayout* bottomLayout;
	LinearLayout* btnALayout;
	LinearLayout* btnBLayout;
	LinearLayout* btnCLayout;

	std::vector<BtnTestElement*> leftBtnTest;
	std::vector<BtnTestElement*> midBtnTest;
	std::vector<BtnTestElement*> rightBtnTest;
	std::vector<BtnTestElement*> btnATest;
	std::vector<BtnTestElement*> btnBTest;
	std::vector<BtnTestElement*> btnCTest;

	uint8_t doneCounter = 0;
	bool inputIsDone = false;

	void buildUI();

};

#endif //WHEELSON_FIRMWARE_INPUTHWTEST_H
