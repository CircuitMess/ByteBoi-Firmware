#ifndef BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H
#define BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H

#include <Support/Modal.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <Elements/ModalBackground.hpp>
#include "GameInfo.hpp"
#include <Input/InputListener.h>


class DescriptionModal : public Modal, private InputListener{
public:
	DescriptionModal(Context& context, GameInfo* gameInfo);

	~DescriptionModal();

	void draw() override;

	void start() override;

	void stop() override;

private:
	void splitPrintSentence(Sprite* canvas, std::string sentence);

	void buttonPressed(uint id) override;

	void draw(Sprite* canvas);

	uint16_t calculateHeight(GameInfo* gameInfo);

	GameInfo* gameInfo;
};

#endif //BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H
