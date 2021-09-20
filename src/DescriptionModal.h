#ifndef BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H
#define BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H

#include <Support/Modal.h>
#include <Loop/LoopListener.h>
#include <UI/LinearLayout.h>
#include <Elements/ModalBackground.hpp>
#include "DescriptionModalItem.h"

class DescriptionModal : public Modal{
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

	LinearLayout* layout;
	DescriptionModalItem* descriptionText;
};

#endif //BYTEBOI_FIRMWARE_DESCRIPTIONMODAL_H
