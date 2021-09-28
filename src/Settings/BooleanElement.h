#ifndef BYTEBOI_FIRMWARE_BOOLEANELEMENT_H
#define BYTEBOI_FIRMWARE_BOOLEANELEMENT_H

#include "SettingsElement.h"

namespace SettingsScreen {
	class BooleanElement : public SettingsElement {
	public:
		BooleanElement(ElementContainer *partent, String name);

		void toggle() override;

	private:

		bool booleanSwitch = false;

	protected:
		void drawControl() override;
	};
}

#endif //BYTEBOI_FIRMWARE_BOOLEANELEMENT_H
