#ifndef BYTEBOI_FIRMWARE_ERRORMODAL_H
#define BYTEBOI_FIRMWARE_ERRORMODAL_H

#include <Support/Modal.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <Elements/ModalBackground.hpp>
#include "GameInfo.hpp"
#include <Input/InputListener.h>


class ErrorModal : public Modal, private InputListener{
public:
	ErrorModal(Context& context, const String& error);
	~ErrorModal();

	void draw() override;
	void start() override;
	void stop() override;

private:
	static uint8_t splitPrintSentence(Sprite* canvas, std::string sentence);
	static uint16_t calculateHeight(const String& text);
	void buttonPressed(uint id) override;

	String error;
};

#endif //BYTEBOI_FIRMWARE_ERRORMODAL_H
