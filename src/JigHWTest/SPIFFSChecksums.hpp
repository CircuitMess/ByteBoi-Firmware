#include <Arduino.h>

struct {
	String name;
	uint32_t sum;
} static const SPIFFSChecksums[] PROGMEM = {
		{ "/launcher/intro/intro.aac", 4730261 },
		{ "/launcher/intro/intro.g565.hs", 515217 },
		{ "/launcher/Logo/BBLogo.raw", 1004220 },
		{ "/launcher/Setts/SettsBg.raw.hs", 80264 },
		{ "/launcher/stock/emptySD.raw", 897143 },
		{ "/launcher/stock/load.raw", 216614 },
		{ "/launcher/stock/noIcon.raw", 816464 },
		{ "/launcher/stock/NoSD.raw", 892791 },
		{ "/launcher/stock/Setts.raw", 1331940 }
};
