#ifndef BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
#define BYTEBOI_FIRMWARE_LOADINGINDICATOR_H


#include <Display/Sprite.h>
#include <Loop/LoopListener.h>
class Logo;
class GameScroller;
class GameTitle;

class LoadingIndicator : public LoopListener {
public:
	LoadingIndicator(Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title);

	void start();
	void stop();

	bool isActive() const;
	void draw();

	void loop(uint micros) override;

private:
	Sprite* canvas = nullptr;
	Logo* logo = nullptr;
	GameScroller* scroller = nullptr;
	GameTitle* title = nullptr;

	bool active = false;
	String currentText;

	enum { OUT, ENTER, IN, EXIT } state = OUT;
	float f = 0;
	float ballf = 0;
};


#endif //BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
