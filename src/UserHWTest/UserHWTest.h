#ifndef BYTEBOI_FIRMWARE_USERHWTEST_H
#define BYTEBOI_FIRMWARE_USERHWTEST_H

#include <Support/Context.h>
#include <Arduino.h>
#include <Loop/LoopListener.h>
#include <Input/Input.h>
#include <UI/LinearLayout.h>

class HWTestPart;

class UserHWTest : public Context {
public:
	UserHWTest(Display& display);
	virtual ~UserHWTest();
	void draw() override;

	void start() override;

	void stop() override;

	void currentTestDone();

	void setDoneCallback(void (* doneCallback)(UserHWTest*));
protected:
	void init() override;

	void deinit() override;
private:
	HWTestPart* hwTestPart;

	uint8_t testCounter = 0;
	void nextTest();
	Color* backgroundBuffer= nullptr;
	void (* doneCallback)(UserHWTest* test) = nullptr;
};

#endif //BYTEBOI_FIRMWARE_USERHWTEST_H
