#ifndef BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
#define BYTEBOI_FIRMWARE_LOADINGINDICATOR_H


#include <Display/Sprite.h>
#include <Loop/LoopListener.h>
class Logo;
class GameScroller;
class GameTitle;
struct GameInfo;

class LoadingIndicator : public LoopListener {
public:
	LoadingIndicator(Sprite* canvas, Logo* logo, GameScroller* scroller, GameTitle* title);

	void start(GameInfo* game);
	void stop();
	void finish();

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
	bool boot = false;
	GameInfo* game = nullptr;

	enum { OUT, ENTER, IN, EXIT, FINISH } state = OUT;
	float f = 0;
	float exitf = 0;
	float ballf = 0;
	bool ballGrown = false;
	bool ballGrown2 = false;
};


#endif //BYTEBOI_FIRMWARE_LOADINGINDICATOR_H
