#ifndef NIBBLE_MENU_H
#define NIBBLE_MENU_H

#include <Display/Display.h>
#include <UI/Screen.h>
#include <UI/LinearLayout.h>
#include <UI/TextElement.h>
#include <Loop/LoopListener.h>
#include "Elements/Switch.h"
#include <Support/Modal.h>
class Sprite;
class Context;

class Menu : public LoopListener, public Modal {
public:
	Menu(Context* currentContext);
	virtual ~Menu();
	void loop(uint micros) override;
	void draw() override;

	void start() override;
	void stop() override;

private:
	static Menu* instance;

	Sprite* canvas;
	Context* currentContext = nullptr;

	LinearLayout layout;
	TextElement exit;
	LinearLayout audioLayout;
	TextElement muteText;
	Switch audioSwitch;

	float showProgress = 0;
	bool shown = false;
	bool shownDone = false;

	bool exiting = false;
	bool stopping = false;

	float selectAccum = 0;
	uint8_t selectedX;
	uint8_t selectedElement = 0;
	void selectElement(uint8_t index);

	void buildUI();
	void bindInput();
	void releaseInput();
};


#endif //NIBBLE_MENU_H
