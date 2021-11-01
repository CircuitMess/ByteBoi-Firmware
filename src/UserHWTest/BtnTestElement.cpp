#include "BtnTestElement.h"

BtnTestElement::BtnTestElement(ElementContainer* parent, bool squareBtn, String name) : CustomElement(parent, 35, 25), squareBtn(squareBtn),name(name){

}

void BtnTestElement::btnPress(){
	if(!btnPressed){
		btnPressed = true;

	}
}

void BtnTestElement::draw(){
	if(!squareBtn){
		getSprite()->fillCircle(getTotalX() + 2, getTotalY() + 10, 10, !btnPressed ? TFT_DARKGREY : TFT_GREEN);
		getSprite()->drawCircle(getTotalX() + 2, getTotalY() + 10, 10, TFT_WHITE);
	}else{
		getSprite()->fillRect(getTotalX(), getTotalY() + 2, 39, 13, !btnPressed ? TFT_DARKGREY : TFT_GREEN);
		getSprite()->drawRect(getTotalX(), getTotalY() + 2, 39, 13, TFT_WHITE);
		getSprite()->setTextFont(1);
		getSprite()->setTextSize(1);
		getSprite()->setTextColor(TFT_WHITE);
		getSprite()->setCursor(getTotalX() + 2, getTotalY() + 5);
		getSprite()->print(name);
	}


}

bool BtnTestElement::isBtnPressed() const{
	return btnPressed;
}
