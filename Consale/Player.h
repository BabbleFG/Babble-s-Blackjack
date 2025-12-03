#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
#include <string>
#include "card.h"
#include "Deck.h"
using namespace std;
class Player{
    private:
    card hand[12];
    int wins;
    int currentCards;
    int balance;
    int currentBet = 0;
    
    
  public:
  
Player(int startingBalance=1000){
    currentCards=0;
    wins=0;
    balance=startingBalance;
}

int getBalance(){
    return balance;
}
void updateBalance(int amount){
    balance+=amount;
}
bool deductBalance(int amount){
    if(balance-amount<0){
        return false;
    }
    balance-=amount;
    return true;
}

void addCard(card c){
    hand[currentCards]=c;
    currentCards++;
}

int getScore () {
    int total = 0;
    int aceCount=0;
    // 1. Loop through the h-=nd
    for(int i=0;i<currentCards;  i++){
            total+=hand[i].getValue();
        if(hand[i].getRank()=="Ace"){
            aceCount++;
        }
    }
    while(total > 21 && aceCount > 0){
        total -= 10;
        aceCount--;
    }
   
    return total;
}


void showHand(){
    for(int i=0; i<currentCards; i++){
        //10 of spades 
        cout<<"card "<<i+1 << " :"<<hand[i].getRank()<<" of "<<hand[i].getSuit()<<endl;
    }
    cout<<"Total Score: "<<getScore()<<endl;
    cout<<endl;
}

void resetHand() {
    currentCards = 0;
}
void addWin() { 
    wins++; 
}


int getWins() {
    return wins; 
    
}

bool bust() {
    return getScore() > 21;
}

void showPartialHand() {
    if (currentCards > 0) {
        cout << "card 1 : " << hand[0].getRank() << " of " << hand[0].getSuit() << endl;
    } else {
        cout << "No cards to show." << endl;
    }
    cout << endl;
}

void setCurrentBet(int amount) {
    currentBet = amount;
}

int getCurrentBet() const {
    return currentBet;
}

void addToBet(int amount) {
    currentBet += amount;
}

void resetBet() {
    currentBet = 0;
}

const card* getHand() const {
    return hand;
}

int getCardCount() const {
    return currentCards;
}



};
#endif

