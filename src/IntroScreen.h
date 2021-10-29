#ifndef WHEELSON_FIRMWARE_INTROSCREEN_H
#define WHEELSON_FIRMWARE_INTROSCREEN_H

#include <Support/Context.h>
#include <UI/Screen.h>
#include <Loop/LoopListener.h>
#include <Display/AnimatedSprite.h>
#include <Playback/Sample.h>

class IntroScreen: public Context, public LoopListener {
public:

	IntroScreen(Display &display, Context* menu);

	virtual ~IntroScreen();

	void start();

	void stop();

	void draw();

	void loop(uint micros) override;

private:
	static IntroScreen *instance;
	AnimatedSprite* gif = nullptr;
	Context* menu = nullptr;
	Sample* intro = nullptr;
};

#endif //WHEELSON_FIRMWARE_INTROSCREEN_H
