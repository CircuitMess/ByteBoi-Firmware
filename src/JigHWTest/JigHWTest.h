#ifndef WHEELSON_FIRMWARE_JIGHWTEST_H
#define WHEELSON_FIRMWARE_JIGHWTEST_H
#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Util/Task.h>

struct Test {
	bool (*test)();
	const char* name;
};

class JigHWTest
{
public:
	JigHWTest(Display& display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	static JigHWTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char *property, uint32_t value);
	void log(const char *property, String value);

	static bool psram();
	static bool SPIFFSTest();
	static bool SDcheck();
	static bool BatteryCheck();


};


#endif //WHEELSON_FIRMWARE_JIGHWTEST_H
