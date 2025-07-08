#ifndef BYTEBOI_FIRMWARE_BOOLEANELEMENT_H
#define BYTEBOI_FIRMWARE_BOOLEANELEMENT_H

#include "SettingsElement.h"

namespace SettingsScreen {
class BooleanElement : public SettingsElement {
public:
	BooleanElement(ElementContainer* partent, String name, std::function<void(int)> onChange);

	void click() override;

	void left() override;
	void right() override;

	bool getBooleanSwitch() const;

	void setBooleanSwitch(bool booleanSwitch);

private:

	bool booleanSwitch = false;

protected:
	void drawControl() override;
};
}

#endif //BYTEBOI_FIRMWARE_BOOLEANELEMENT_H
