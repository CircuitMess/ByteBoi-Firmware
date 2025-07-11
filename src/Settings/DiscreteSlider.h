#ifndef WHEELSON_FIRMWARE_DISCRETESLIDER_H
#define WHEELSON_FIRMWARE_DISCRETESLIDER_H

#include "SettingsElement.h"

namespace SettingsScreen {
class DiscreteSlider : public SettingsElement {
public:
	DiscreteSlider(ElementContainer* parent, String name, std::vector<uint8_t> shutDownTime, std::function<void(int)> onChange);

	void click() override;

	void right() override;

	void left() override;

	bool isSliderSelected() const;

	void setIndex(int index);

	int getIndex() const;

private:
	bool sliderIsSelected = false;

	int index = 4;

	std::vector<uint8_t> shutDownTime;
protected:
	void drawControl() override;
};
}


#endif //WHEELSON_FIRMWARE_DISCRETESLIDER_H
