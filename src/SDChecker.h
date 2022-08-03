#ifndef BYTEBOI_FIRMWARE_SDCHECKER_H
#define BYTEBOI_FIRMWARE_SDCHECKER_H

#include <Loop/LoopListener.h>

class SDChecker : private LoopListener {
public:
	void start();
	void stop();

private:
	void loop(uint micros) override;

private:
	static constexpr uint32_t checkInterval = 5000; // [ms]

	uint32_t checkTime;
	void check();

};


#endif //BYTEBOI_FIRMWARE_SDCHECKER_H
