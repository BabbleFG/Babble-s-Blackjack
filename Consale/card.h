#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <string>
using namespace std;
class card{
private:
string suit;
string rank;
int value;

public:
   card(string s="", string r="", int v=0){
         suit = s;
         rank = r;
         value = v;
   }

     int getValue() const { return value; }
     std::string getRank() const { return rank; }
     std::string getSuit() const { return suit; }



    string getAssetName() const {
    // Example: "2C", "AC", "10H"
    string shortRank;

    if (rank == "Jack") shortRank = "J";
    else if (rank == "Queen") shortRank = "Q";
    else if (rank == "King") shortRank = "K";
    else if (rank == "Ace") shortRank = "A";
    else shortRank = rank; 

    char suitChar;
    if (suit == "Clubs") suitChar = 'C';
    else if (suit == "Diamonds") suitChar = 'D';
    else if (suit == "Hearts") suitChar = 'H';
    else if (suit == "Spades") suitChar = 'S';

    return shortRank + suitChar; // e.g. "10H"
}


};

#endif
