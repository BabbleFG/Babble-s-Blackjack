#ifndef DECK_H
#define DECK_H


#include <iostream>
#include <string>
#include "card.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
class Deck{
public:


Deck(){
    srand(int(time(0)));
}




card deal(){
    string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};
    string suit = suits[rand() % 4];
    int rankIndex = rand() % 13;
    string rank = ranks[rankIndex];
    int value = values[rankIndex];

    

    card c(suit, rank, value);
    return c;
}

   

    
};
#endif