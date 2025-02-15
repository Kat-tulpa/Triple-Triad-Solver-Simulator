#pragma once
#include "Defs.hpp"

class Card {
private:
    uint8_t myAttributes[4] = { 0, 0, 0, 0 };
    ID myID = 0;
    uint8_t myStarCount = 0;
    Player myControllingPlayer = PLAYER_NONE;

public:
    Card() = default;

    Card(ID id, uint8_t stars, uint8_t top, uint8_t right, uint8_t bottom, uint8_t left, Player controllingPlayer)
        : myID(id), myStarCount(stars), myControllingPlayer(controllingPlayer), myAttributes{ top, right, bottom, left } {
    }

    // Getters
    const bool& isEnemy(const Card& other) const {
        return other.controllingPlayer() != PLAYER_NONE && controllingPlayer() != PLAYER_NONE && other.controllingPlayer() != controllingPlayer();
    }

    const int& stars() const {
        return myStarCount;
    }

    const Player& controllingPlayer() const {
        return myControllingPlayer;
    }

    const int& attribute(const int& index) const {
        return myAttributes[index];
    }

    const ID& id() const {
        return myID;
    }

    // Setters
    void setControllingPlayer(const Player& player) {
        myControllingPlayer = player;
    }
};