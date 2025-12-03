#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "Player.h"

using namespace std;
class Game {
private:
    Player &player;
    Player &dealer;
    Deck deck;
    bool roundActive;
    
    
public:
Game(Player &p, Player &d) : player(p), dealer(d) {
    roundActive = true;
}


    void resetRound() {
        player.resetHand();
        dealer.resetHand();
        roundActive = true;
    }

    void showWins() {
        cout << "Player Wins: " << player.getWins()<< " - Dealer Wins: " << dealer.getWins() << endl;
        
    }
void start() {
    resetRound();
    int bet = 0;

    cout << "Your current balance is: $" << player.getBalance() << endl;
    cout << "Enter your bet amount: $";
    cin >> bet;

    while (bet > player.getBalance() || bet <= 0) {
        cout << "Invalid bet amount. Please enter a valid bet: $";
        cin >> bet;
    }

    player.setCurrentBet(bet);
    player.deductBalance(bet);
    cout << "Your new balance is: $" << player.getBalance() << endl;

    // 1. Deal initial cards
    player.addCard(deck.deal());
    player.addCard(deck.deal());
    dealer.addCard(deck.deal());
    dealer.addCard(deck.deal());

    // 2. Show hands
    cout << "Dealer's hand:" << endl;
    dealer.showPartialHand();
    cout << "Your hand:" << endl;
    player.showHand();

    // 3. Let player hit/stand
    string userInput = "";
    bool playerTurn = true;
    while (playerTurn && player.getScore() <= 21) {
        cout << "Your turn: Hit or Stand" << endl;
        cin >> userInput;

        if (userInput == "Hit" || userInput == "hit") {
            if (!player.bust()) {
                player.addCard(deck.deal());
                player.showHand();
                if (player.bust()) {
                    cout << "Bust! You went over 21." << endl;
                    playerTurn = false;
                }
            }
        } else if (userInput == "Stand" || userInput == "stand") {
            playerTurn = false;
        }
    }

    // 4. Dealer logic
    if (!player.bust()) {
        cout << "Dealer's full hand:" << endl;
        dealer.showHand();
        while (dealer.getScore() < 17 && !dealer.bust()) {
            dealer.addCard(deck.deal());
            dealer.showHand();
        }
    }

    // 5. Determine winner
    int betAmount = player.getCurrentBet(); // ✅ Use stored bet

    if (player.bust()) {
        dealer.addWin();
        showWins();
    } else if (dealer.bust()) {
        cout << "Dealer busted — you WIN!" << endl;
        player.addWin();
        showWins();
        player.updateBalance(betAmount * 2);
    } else if (dealer.getScore() > player.getScore()) {
        cout << "Dealer slammed you. You lose." << endl;
        dealer.addWin();
        showWins();
    } else if (dealer.getScore() < player.getScore()) {
        cout << "You slammed the dealer. You win!" << endl;
        player.addWin();
        showWins();
        player.updateBalance(betAmount * 2);
    } else {
        cout << "It's a tie." << endl;
        showWins();
        player.updateBalance(betAmount); // Return bet
    }

    player.resetBet(); // ✅ Clear bet for next round
}

card dealCard() {
    return deck.deal();
}

void playerHit(Player& player) {
    player.addCard(deck.deal());
}

void dealerPlay(Player& dealer) {
    while (dealer.getScore() < 17 && !dealer.bust()) {
        dealer.addCard(deck.deal());
    }
}

void resolveRound(Player& player, Player& dealer) {
    int betAmount = player.getCurrentBet();

    if (player.bust()) {
        dealer.addWin();
    } else if (dealer.bust()) {
        player.addWin();
        player.updateBalance(betAmount * 2);
    } else if (dealer.getScore() > player.getScore()) {
        dealer.addWin();
    } else if (dealer.getScore() < player.getScore()) {
        player.addWin();
        player.updateBalance(betAmount * 2);
    } else {
        player.updateBalance(betAmount); // tie
    }

    player.resetBet();
}
};


#endif