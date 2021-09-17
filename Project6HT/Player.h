#pragma once
#include "Portable.h"

class Player final:
    public Portable
{
public:
    Player(); // ¬вод значений с консоли
    Player(string name, unsigned int price, string color, unsigned int batteryLife, unsigned int totalTracks) : IElectronics(GoodsType::PLAYER, name, price, color), Portable(batteryLife), _totalTracks(totalTracks) {};
    ~Player();

    unsigned int ShowTotalTracks() const;
    string ShowSpec() const override;

protected:
    unsigned int _totalTracks;

};

