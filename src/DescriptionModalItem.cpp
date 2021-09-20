#include <string>
#include "DescriptionModalItem.h"
#include "profont.h"

#define INF INT_MAX
using namespace std;

static const lgfx::U8g2font profont10(u8g2_font_profont10_tr);

DescriptionModalItem::DescriptionModalItem(ElementContainer* parent, const char* title,const char* author, const char* description) : CustomElement(parent, 80, 10), title(title),author(author), description(description){
}

DescriptionModalItem::~DescriptionModalItem(){

}

void DescriptionModalItem::draw(){
	getSprite()->setTextColor(TFT_WHITE);
	getSprite()->setCursor(40 - getSprite()->textWidth(title) / 2, 3);
	getSprite()->setFont(&profont10);
	getSprite()->print(title);
	getSprite()->setCursor(1, 20);
	getSprite()->print("Author:");
	getSprite()->setCursor(36, 20);
	getSprite()->print(author);
	getSprite()->setCursor(1, 40);
	splitPrintSentence(description);

}

void DescriptionModalItem::splitPrintSentence(std::string sentence){
	uint8_t y_lenght = 40;
	uint8_t x_lenght = 1;
	std::string word;
	for(auto x : sentence){
		if(x == ' ' || x == ('!') || x == ('.')){
			Serial.println(getSprite()->getCursorX());
			Serial.printf("width : %d\n", getSprite()->textWidth(word.c_str()));
			if(((getSprite()->getCursorX()) + getSprite()->textWidth(word.c_str())) > 80){
				y_lenght += 10;
				getSprite()->setCursor(x_lenght, y_lenght);
			}else{
				getSprite()->setCursor(getSprite()->getCursorX() + 3, y_lenght);
			}
			getSprite()->print(word.c_str());
			Serial.println(word.c_str());
			word.erase();
		}else{
			word = word + x;
		}
	}
}




