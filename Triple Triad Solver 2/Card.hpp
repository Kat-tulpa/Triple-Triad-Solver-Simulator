#pragma once
#include "Defs.hpp"

class Card {
private:
    int myAttributes[4] = { 0, 0, 0, 0 };
    ID myID = 0;
    int myStarCount = 0;
    Player myControllingPlayer = PLAYER_NONE;

public:
    Card() = default;

    Card(ID id, int stars, int top, int right, int bottom, int left, Player controllingPlayer)
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

    const ID id() const {
        return myID;
    }

    const int* attributes() const {
        return myAttributes;
    }

    const bool hasOwner() const {
        return myControllingPlayer != PLAYER_NONE;
    }

    // Setters
    void setControllingPlayer(const Player& player) {
        myControllingPlayer = player;
    }
};