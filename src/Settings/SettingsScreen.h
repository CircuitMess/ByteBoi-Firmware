#ifndef BYTEBOI_FIRMWARE_SETTINGSSCREEN_H
#define BYTEBOI_FIRMWARE_SETTINGSSCREEN_H

#include <Support/Context.h>
#include <UI/LinearLayout.h>
#include "SliderElement.h"
#include "DiscreteSlider.h"
#include "BooleanElement.h"
#include <Input/InputListener.h>
#include "TextElement.h"
#include <FS.h>

namespace SettingsScreen {
class SettingsScreen : public Context, private InputListener, public LoopListener {
public:

	SettingsScreen(Display& display);

	virtual ~SettingsScreen();

	void start();

	void stop();

	void draw();

	void loop(uint micros) override;

protected:
	void deinit() override;

	void init() override;


private:
	static SettingsScreen* instance;

	LinearLayout* screenLayout;
	DiscreteSlider* shutDownSlider;
	SliderElement* volumeSlider;
	BooleanElement* enableLED;
	TextElement* inputTest;
	TextElement* save;

	std::array<SettingsElement*, 5> elements;
	int NumElements = 5;

	int selectedSetting = 0;
	bool disableMainSelector = false;

	void buildUI();
	void buttonPressed(uint id) override;
	void selectApp(int8_t num);

	Color* backgroundBuffer = nullptr;
	fs::File introSong;
	uint32_t blinkTime = 0;

	void exitAndSave();
};
}

#endif //BYTEBOI_FIRMWARE_SETTINGSSCREEN_H
