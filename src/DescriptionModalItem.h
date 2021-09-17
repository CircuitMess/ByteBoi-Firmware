#ifndef BYTEBOI_FIRMWARE_DESCRIPTIONMODALITEM_H
#define BYTEBOI_FIRMWARE_DESCRIPTIONMODALITEM_H

#include <UI/CustomElement.h>

class DescriptionModalItem : public CustomElement {
public:
	DescriptionModalItem(ElementContainer* parent, const char* title, const char* description);

	virtual ~DescriptionModalItem();

	void draw();

private:
	const char* title;
	const char* description;
	void splitPrintSentence(std::string sentence);

};

#endif //BYTEBOI_FIRMWARE_DESCRIPTIONMODALITEM_H
