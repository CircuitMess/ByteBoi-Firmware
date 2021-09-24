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
	DescriptionModal(Context& context, const char* title,const char* author, const char* description);

	~DescriptionModal();

	void draw() override;

	void start() override;

	void stop() override;

protected:
	void init() override;

	void deinit() override;

private:
	static DescriptionModal* instance;

	void buildUI();

	void splitPrintSentence(std::string sentence);

	void buttonPressed(uint id) override;

	LinearLayout* layout;
	DescriptionModalItem* descriptionText;
};

#endif //BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H
