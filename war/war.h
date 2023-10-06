#pragma once

#include <queue>
#include <array>
#include <stdexcept>
#include <iostream>

enum class Winner { kFirst, kSecond, kNone };

struct GameResult {
    Winner winner;
    int turn;
};

bool Compare(int card1, int card2) {
    if ((card1 == 0) and (card2 == 11)) {
        return false;
    }
    if ((card1 == 11) and (card2 == 0)) {
        return true;
    }
    return card1 < card2;
}

GameResult SimulateWarGame(const std::array<int, 6>& first_deck,
                           const std::array<int, 6>& second_deck) {
    std::queue<int> q1;
    for (int card : first_deck) {
        q1.push(card);
    }

    std::queue<int> q2;
    for (int card : second_deck) {
        q2.push(card);
    }

    int counter = 0;

    while ((!q1.empty()) and (!q2.empty()) and (counter < 1e6)) {
        counter++;

        int card1 = q1.front();
        q1.pop();

        int card2 = q2.front();
        q2.pop();

        if (Compare(card1, card2)) {  // card1 < card2
            q2.push(card1);
            q2.push(card2);
        } else {
            q1.push(card1);
            q1.push(card2);
        }
    }
    if ((counter == 1e6) and (!q1.empty()) and (!q2.empty())) {
        return GameResult{Winner::kNone, counter};
    } else {
        if (q1.empty()) {
            return GameResult{Winner::kSecond, counter};
        } else {
            return GameResult{Winner::kFirst, counter};
        }
    }
}
