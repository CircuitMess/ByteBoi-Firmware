#ifndef WHEELSON_FIRMWARE_TEXTELEMENT_H
#define WHEELSON_FIRMWARE_TEXTELEMENT_H

#include "SettingsElement.h"

namespace SettingsScreen {
class TextElement : public SettingsElement {
public:
	TextElement(ElementContainer* parent, String name, std::function<void(int)> onChange);

	void click() override;

private:
	void left() override final;
	void right() override final;

protected:
	void drawControl() override;
};
}

#endif //WHEELSON_FIRMWARE_TEXTELEMENT_H
