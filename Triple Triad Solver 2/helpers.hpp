#pragma once
#include "defs.hpp"
#include <iostream>

static unsigned char attributeToChar(int value) {
    switch (value) {
    case STRENGTH_MAX:
        return 'A';
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    default:
        std::cout << "attributeToChar() received an input value that isn't a valid attribute number: " << std::endl;
        return '%';
    }
}

static char colorToChar(Player color) {
    return color == PLAYER_RED ? 'R' :
        color == PLAYER_BLUE ? 'B' : 'N';
}

static Player otherPlayer(Player player) {
    if (player == PLAYER_RED)
        return PLAYER_BLUE;
    else
        return PLAYER_RED;
}

static int square(const int value) {
    return value * value;
}