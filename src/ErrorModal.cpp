#include <Pins.hpp>
#include "ErrorModal.h"
#include "Fonts/adobex11font.h"
#include <ByteBoi.h>
#include <lgfx/v1/lgfx_fonts.hpp>

using namespace std;

static const lgfx::U8g2font adobex11font(u8g2_font_helvB08_tr);

ErrorModal::ErrorModal(Context& context, const String& error) : Modal(context, 100, calculateHeight(error)),error(error){

}

uint16_t ErrorModal::calculateHeight(const String& text){
	Sprite* canvas = new Sprite((Sprite*) nullptr, 100, 1);
	canvas->setTextSize(1);
	canvas->setTextFont(0);
	canvas->setCursor(4, 4);
	uint16_t height = splitPrintSentence(canvas, text.c_str()) * 10 + 6;
	delete canvas;
	return height;
}

ErrorModal::~ErrorModal(){

}

void ErrorModal::draw(){
	Sprite* canvas = screen.getSprite();

	canvas->clear(TFT_TRANSPARENT);
	canvas->fillRoundRect(screen.getTotalX(), screen.getTotalY(), canvas->width(), canvas->height(), 3, C_HEX(0x004194));
	canvas->fillRoundRect(screen.getTotalX() + 2, screen.getTotalY() + 2, canvas->width() - 4, canvas->height() - 4, 3, C_HEX(0x0041ff));

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setTextColor(TFT_WHITE);
	canvas->setCursor(4, 4);
	splitPrintSentence(canvas, error.c_str());
}

void ErrorModal::start(){
	Input::getInstance()->addListener(this);
	draw();
	screen.commit();
}

void ErrorModal::stop(){
	Input::getInstance()->removeListener(this);
}

uint8_t ErrorModal::splitPrintSentence(Sprite* canvas, std::string sentence){
	uint8_t y_lenght = 4;
	std::string word;
	bool firstWord = true;
	int lines = 1;
	for(auto x : sentence){
		if(x == ' ' || x == ('!') || x == ('.')){
			if(((canvas->getCursorX()) + canvas->textWidth(word.c_str())) > canvas->width()-4){
				y_lenght += 10;
				canvas->setCursor(4, y_lenght);
				lines++;
			}else{
				if(firstWord){
					canvas->setCursor(4, y_lenght);
					firstWord = false;
				}else{
					canvas->setCursor(canvas->getCursorX() + 3, y_lenght);
				}

			}
			canvas->print(word.c_str());
			word.erase();
		}else{
			word = word + x;
		}

	}

	return lines;
}

void ErrorModal::buttonPressed(uint id){
	switch(id){
		case BTN_A:
		case BTN_B:
		case BTN_C:
			this->pop();
			break;

	}
}