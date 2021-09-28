#ifndef BYTEBOI_FIRMWARE_GAMELISTENER_HPP
#define BYTEBOI_FIRMWARE_GAMELISTENER_HPP

class GameListener{
public:
	virtual void gamesChanged(bool inserted) = 0;
};

#endif //BYTEBOI_FIRMWARE_GAMELISTENER_HPP
